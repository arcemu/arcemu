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

	threadcount = si.dwNumberOfProcessors*2;

	Log.Notice("SocketMgr","IOCP Spawning [-%u-] worker threads.", threadcount);
	for(long x = 0; x < threadcount; ++x)
		ThreadPool.ExecuteTask(new SocketWorkerThread());
}

bool SocketWorkerThread::run()
{
	THREAD_TRY_EXECUTION2
	HANDLE cp = sSocketMgr.GetCompletionPort();
	DWORD len = 0;
	Socket * s=NULL;
	LPOVERLAPPED ol_ptr;
	BOOL _ret=0;
	OverlappedStruct * _gov =NULL;
	shutdown = false;
	while(!shutdown)
	{
#ifndef _WIN64
		_ret = GetQueuedCompletionStatus(cp, &len,  (LPDWORD)&s, &ol_ptr, INFINITE);
#else
		_ret = GetQueuedCompletionStatus(cp, &len, (PULONG_PTR)&s, &ol_ptr,INFINITE);
#endif

		if ( NULL == s ) continue; // null?

		if ( FALSE == _ret ) { // still get this with INFINITE? i can't believe:D
			s->Disconnect();
			continue;
		}

		if ( s->IsDeleted() ) {
			continue; // do not operate the socket which in garbagesystem
		}

		_gov = CONTAINING_RECORD(ol_ptr, OverlappedStruct, m_overlap); // Calc
		if ( _gov==NULL ) {
			s->Disconnect();
			continue;
		}

		if ( _gov->m_event == SOCKET_IO_THREAD_SHUTDOWN ) // workthread going down
		{
			ophandlers[_gov->m_event](s, len);
			if ( _gov ) delete _gov;
			_gov = NULL;
			break;
		}
		
		if ( 0 == len ) // No posting status to us,just go ahead.
		{
			s->Disconnect();
			continue;
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
	s->m_readEvent.Unmark();
	if ( len > s->_recvbuffsize ) {
		s->Disconnect();
		return;	
	}
	if ( s->GetReadBuffer().GetSpace() < len ) {
		s->Disconnect();
		return;
	}
	if ( !s->IsConnected() ) s->markConnected();

	s->LockReader();
	if ( s->GetReadBuffer().GetSize()==0 ) s->GetReadBuffer().IncrementWritten(len);
	s->ReleaseReader();

	if ( s->IsConnected() ) s->OnRead();

	s->LockReader();
	if ( s->GetReadBuffer().GetSize()!=0 ) s->GetReadBuffer().Remove(s->GetReadBuffer().GetSize()); // cleanup
	s->ReleaseReader();

	s->ReadCallback(len);
}

void HandleWriteComplete(Socket * s, uint32 len)
{
	s->m_writeEvent.Unmark();
	if ( len > s->_sendbuffsize || s->GetWriteBuffer().GetSize() > s->_sendbuffsize ) {
		s->Disconnect();
		return;
	}
	s->BurstBegin();				// Lock
	s->GetWriteBuffer().Remove(len);
	if( s->GetWriteBuffer().GetContiguiousBytes() > 0 ){
		s->BurstEnd();
		s->WriteCallback(len);
	return;
	}
	else{
		s->DecSendLock();
	}
	s->BurstEnd();					  // Unlock
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