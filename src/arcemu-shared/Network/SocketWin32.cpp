/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketWin32.cpp - Windows-specific functions of Socket class are located here.
 *
 */


#include "Network.h"
#ifdef CONFIG_USE_IOCP

bool Socket::WriteCallback()
{
	if(m_deleted || !m_connected)
		return true;

	//printf("\nSocket::Writecallback(): sendsize : %u\n", this->m_writeByteCount);
	// We don't want any writes going on while this is happening.
	m_writeMutex.Acquire();
	if(writeBuffer.GetContiguiousBytes())
	{
		DWORD w_length = 0;
		DWORD flags = 0;

		// attempt to push all the data out in a non-blocking fashion.
		WSABUF buf;
		buf.len = (u_long)writeBuffer.GetContiguiousBytes();
		buf.buf = (char*)writeBuffer.GetBufferStart();

		m_writeEvent.Mark();
		m_writeEvent.Reset(SOCKET_IO_EVENT_WRITE_END);
		int r = WSASend(m_fd, &buf, 1, &w_length, flags, &m_writeEvent.m_overlap, 0);
		if(r == SOCKET_ERROR)
		{
			int wsaerror = WSAGetLastError();

			if( wsaerror != WSA_IO_PENDING)
			{
				sLog.outError( "WSAGetLastError() = %d on socket %u",wsaerror, m_fd );

				m_writeEvent.Unmark();
				DecSendLock();

				m_writeMutex.Release();

				BurstEnd();
				Disconnect();

				return false;
			}
		}
        m_BytesSent += w_length;
	}
	else
	{
        // Write operation is completed.
		DecSendLock();
	}
	m_writeMutex.Release();

	return true;
}

bool Socket::SetupReadEvent()
{
	if(m_deleted || !m_connected)
		return true;

	m_readMutex.Acquire();
	DWORD r_length = 0;
	DWORD flags = 0;
	WSABUF buf;
	buf.len = (u_long)readBuffer.GetSpace();
	buf.buf = (char*)readBuffer.GetBuffer();	

	m_readEvent.Mark();
	m_readEvent.Reset(SOCKET_IO_EVENT_READ_COMPLETE);
	if(WSARecv(m_fd, &buf, 1, &r_length, &flags, &m_readEvent.m_overlap, 0) == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSA_IO_PENDING)
		{
			m_readEvent.Unmark();
			m_readMutex.Release();

			Disconnect();

			return false;
		}
	}
    m_BytesRecieved += r_length;
	m_readMutex.Release();

	return true;
}

bool Socket::ReadCallback(uint32 len)
{
	readBuffer.IncrementWritten(len);
	OnRead();
	
	bool ret = SetupReadEvent();

	return ret;
}

void Socket::AssignToCompletionPort()
{
	/*HANDLE h = */CreateIoCompletionPort((HANDLE)m_fd, m_completionPort, (ULONG_PTR)this, 0);
	//__asm int 3;
}

void Socket::BurstPush()
{
	if(AcquireSendLock())
		WriteCallback();
}

#endif
