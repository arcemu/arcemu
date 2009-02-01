/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketMgr - iocp-based SocketMgr for windows.
 * Modded by 2009 Cebernic
 */

#include "Network.h"
#ifdef CONFIG_USE_IOCP

#include "../CrashHandler.h"

initialiseSingleton(SocketMgr);
SocketMgr::SocketMgr()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
	m_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (ULONG_PTR)0, 0);

}

SocketMgr::~SocketMgr()
{
	
}

void SocketMgr::SpawnWorkerThreads()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	threadcount = (si.dwNumberOfProcessors*2)+2; // don'tworry it performance increased.

	Log.Notice("SocketMgr","IOCP Spawning [-%u-] worker threads.", threadcount);
	for(long x = 0; x < threadcount; ++x)
		ThreadPool.ExecuteTask(new SocketWorkerThread());
}

bool SocketWorkerThread::run()
{
	THREAD_TRY_EXECUTION2
	HANDLE cp = sSocketMgr.GetCompletionPort();
	DWORD len;
	Socket * s;
	LPOVERLAPPED ol_ptr;
	BOOL _ret=0;
	OverlappedStruct * _gov =NULL;
	shutdown = false;
	while(!shutdown)
	{
#ifndef _WIN64
		_ret = GetQueuedCompletionStatus(cp, &len,  (LPDWORD)&s, &ol_ptr, 5000);
#else
		_ret = GetQueuedCompletionStatus(cp, &len, (PULONG_PTR)&s, &ol_ptr,5000);
#endif

		if ( !_ret ) continue;
			
		_gov = CONTAINING_RECORD(ol_ptr, OverlappedStruct, m_overlap);

		if ( _gov==NULL ) continue;

		if ( _gov->m_event == SOCKET_IO_THREAD_SHUTDOWN )
		{
			ophandlers[_gov->m_event](s, len);
			if ( _gov ) delete _gov;
			_gov = NULL;
			break;
		}
		
		DWORD   dwLastError=GetLastError();

		if   (ERROR_INVALID_HANDLE==dwLastError || ERROR_OPERATION_ABORTED==dwLastError || FALSE==_ret || NULL==s)
		{
			if ( _ret && s && !s->IsDeleted() ) {
				if ( s->IsConnected() ) s->Disconnect(); // really need to be end.
			}
			continue; // we just continue ,if thread was terminated we all died so :D
		}

		if ( _gov->m_event == SOCKET_IO_EVENT_READ_COMPLETE )
		{
			if ( s && len > 0 ){
				s->LockReader();
				s->total_read_bytes +=len;
				if ( !s->IsConnected() && !s->IsDeleted() ) s->markConnected();
				s->ReleaseReader();
			}
		}
		else
		if ( _gov->m_event == SOCKET_IO_EVENT_WRITE_END )
		{
			if ( s && len > 0 ){
				s->BurstBegin();
				s->total_send_bytes +=len;
				s->BurstEnd();
			}
		}

		if( _gov->m_event < NUM_SOCKET_IO_EVENTS )
			ophandlers[_gov->m_event](s, len);

	}
	--sSocketMgr.threadcount;
	Log.Notice("SocketMgr","IOCP thread was shut down,current (%d) remaining.",sSocketMgr.threadcount);
	THREAD_HANDLE_CRASH2
	return true;
}

void HandleReadComplete(Socket * s, uint32 len)
{
	if ( s==NULL) return;
	if(!s->IsDeleted())
	{
		s->m_readEvent.Unmark();
		if(len)
		{
			s->ReadCallback(len);
		}
		//else
		//	s->Disconnect();
	}
}

void HandleWriteComplete(Socket * s, uint32 len)
{
	if ( s==NULL) return;
	if(!s->IsDeleted())
	{
		s->m_writeEvent.Unmark();
		s->BurstBegin();					// Lock
		s->GetWriteBuffer().Remove(len);
		if( s->GetWriteBuffer().GetContiguiousBytes() > 0 ){
			s->BurstEnd();
			s->WriteCallback();
			return;
		}
		else{
			s->DecSendLock();
		}
		s->BurstEnd();					  // Unlock
	}
}

void HandleShutdown(Socket * s, uint32 len)
{
	Log.Notice("SocketMgr","IOCP single thread shutdown requesting...");
}

void SocketMgr::CloseAll()
{
	list<Socket*> tokill;

	socketLock.Acquire();
	for(set<Socket*>::iterator itr = _sockets.begin(); itr != _sockets.end(); ++itr)
		tokill.push_back(*itr);
	socketLock.Release();
	
	for(list<Socket*>::iterator itr = tokill.begin(); itr != tokill.end(); ++itr)
		(*itr)->Disconnect();

	size_t size;
	do
	{
		socketLock.Acquire();
		size = _sockets.size();
		socketLock.Release();
	}while(size);
}

void SocketMgr::ShutdownThreads()
{
	Log.Notice("SocketMgr","Going to shutdown all spawned threads.");
	for(int i = 0; i < threadcount; ++i)
	{
		OverlappedStruct * ov = new OverlappedStruct(SOCKET_IO_THREAD_SHUTDOWN);
		PostQueuedCompletionStatus(sSocketMgr.GetCompletionPort(), 0, (ULONG_PTR)0, &ov->m_overlap);
	}
}

void SocketMgr::ShowStatus()
{
	socketLock.Acquire();
	sLog.outString("current _sockets: %u garbage sockets: %u", _sockets.size(),sSocketGarbageCollector.GetSocketSize());
	socketLock.Release();
}

#endif