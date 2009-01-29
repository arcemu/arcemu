/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
  * SocketWin32.cpp - Windows-specific functions of Socket class are located here.
 *
 * Modded by Cebernic
 */


#include "Network.h"
#ifdef CONFIG_USE_IOCP

void Socket::WriteCallback()
{
	m_writeMutex.Acquire();

	if(m_deleted){
		m_writeMutex.Release();
		return;
	}

	DWORD w_length = 0;
	DWORD flags = 0;

	if(writeBuffer.GetContiguiousBytes())
	{
		// attempt to push all the data out in a non-blocking fashion.
		WSABUF buf;
		buf.len = (u_long)writeBuffer.GetContiguiousBytes();
		buf.buf = (char*)writeBuffer.GetBufferStart();

		m_writeEvent.Reset(SOCKET_IO_EVENT_WRITE_END);

		if ( !m_writeEvent.Mark() ) {
			m_writeEvent.Unmark();
			DecSendLock();
			Disconnect();
			m_writeMutex.Release();
			return;
		}

		int r = WSASend(m_fd, &buf, 1, &w_length, flags, &m_writeEvent.m_overlap, 0);
		if(r == SOCKET_ERROR)
		{
			if(WSAGetLastError() != WSA_IO_PENDING)
			{
				m_writeEvent.Unmark();
				DecSendLock();
				Disconnect();
				m_writeMutex.Release();
				return;
			}
		}
	}
	else
	{
		// Write operation is completed.
		DecSendLock();
	}
	m_writeEvent.Unmark(); // unmark imediatelly!
	m_writeMutex.Release();
}

void Socket::SetupReadEvent(uint32 len)
{

	m_readMutex.Acquire();
	
	if(m_deleted){
		m_readMutex.Release();
		return;
	}

	if (len>0) { // cebernic: correct packet received.
		readBuffer.IncrementWritten(len);
		OnRead();
		readBuffer.Remove(len);
	}

	m_readEvent.Reset(SOCKET_IO_EVENT_READ_COMPLETE);

	if ( !m_readEvent.Mark() ){
		m_readEvent.Unmark();
		Disconnect();
		m_readMutex.Release();
		return;
	}

	DWORD r_length = 0;
	DWORD flags = 0;
	WSABUF buf;
	buf.len = (u_long)readBuffer.GetSpace();
	buf.buf = (char*)readBuffer.GetBuffer();

	memset(buf.buf,0,buf.len);
	if(WSARecv(m_fd, &buf, 1, &r_length, &flags, &m_readEvent.m_overlap, 0) == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSA_IO_PENDING)
		{
			m_readEvent.Unmark();
			Disconnect();
			m_readMutex.Release();
			return;
		}
	}

	m_readEvent.Unmark();
	m_readMutex.Release();
}

void Socket::ReadCallback(uint32 len)
{
	SetupReadEvent(len);
}

void Socket::AssignToCompletionPort()
{
	if ( m_deleted ) return;

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	long portthreads = (si.dwNumberOfProcessors*2)+2;

	if ( CreateIoCompletionPort((HANDLE)m_fd, m_completionPort, (ULONG_PTR)this,portthreads)==0 )
		Disconnect();

	//__asm int 3;
}

void Socket::CompletionPortIOComplete()
{
	OverlappedStruct * ov = new OverlappedStruct(SOCKET_IO_THREAD_SHUTDOWN);
	PostQueuedCompletionStatus(m_completionPort, 0, (ULONG_PTR)0, &ov->m_overlap);
}

void Socket::BurstPush()
{
	if(AcquireSendLock()){
		WriteCallback();
	}
}

#endif
