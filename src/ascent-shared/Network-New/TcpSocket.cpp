/****************************************************************************
 *
 * Multiplatform High-Performance Async Network Library
 * Implementation of TCP Socket
 * Copyright (c) 2007 Burlex
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech ASA of Norway and appearing in the file
 * COPYING included in the packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "Network.h"

TcpSocket::TcpSocket(int fd, size_t readbuffersize, size_t writebuffersize, bool use_circular_buffer, const sockaddr_in * peer)
{
	SetFd(fd);
	if(use_circular_buffer)
	{
		// m_writeBuffer = new CircularBuffer;
		// m_readBuffer = new CircularBuffer;
		m_writeBuffer = new StraightBuffer;
		m_readBuffer = new StraightBuffer;
	}
	else
	{
		m_writeBuffer = new StraightBuffer;
		m_readBuffer = new StraightBuffer;
	}

	m_writeBuffer->Allocate(writebuffersize);
	m_readBuffer->Allocate(readbuffersize);
	memcpy(&m_peer, peer, sizeof(sockaddr));

	/* switch the socket to nonblocking mode */
	u_long arg = 1;
	ioctlsocket(fd, FIONBIO, &arg);
	m_writeLock = 0;
	m_deleted = false;
	m_connected = true;

	/* disable nagle buffering by default */
	int arg2 = 1;
	setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, (char*)&arg2, sizeof(int));
}

TcpSocket::~TcpSocket()
{
	delete m_writeBuffer;
	delete m_readBuffer;
}

void TcpSocket::OnRead(size_t len)
{
	/* This is called when the socket engine gets an event on the socket */

#ifdef NETLIB_IOCP
	if(!len)
	{
		/* socket disconnected */
		Disconnect();
		return;
	}

	/* IOCP is easy. */
	if(len != 0xFFFFFFFF)
		m_readBuffer->IncrementWritten(len);

	/* Wewt, we read again! */
	OnRecvData();

	if(!IsConnected())
		return;

	/* Setup another read event */
	WSABUF buf;
	buf.buf = (char*)m_readBuffer->GetBuffer();
	buf.len = m_readBuffer->GetSpace();

	DWORD recved;
	DWORD flags = 0;
	Overlapped * ov = new Overlapped;
	memset(ov, 0, sizeof(Overlapped));
	ov->m_op = IO_EVENT_READ;

	if(WSARecv(m_fd, &buf, 1, &recved, &flags, &ov->m_ov, 0) == SOCKET_ERROR)
	{
        if(WSAGetLastError() != WSA_IO_PENDING)
			Disconnect();
	}

#else

	/* Any other platform, we have to call recv() to actually get the data. */
	int bytes = recv(m_fd, (char*)m_readBuffer->GetBuffer(), m_readBuffer->GetSpace(), 0);

	/* Under some socket engines, if this returns 0, we're in deep poo poo. */
	/* Check if recv() failed. */
	if(bytes <= 0)
		Disconnect();			// whoopes. :P
	else
	{
		m_readBuffer->IncrementWritten(bytes);
		OnRecvData();
	}

#endif
}

void TcpSocket::OnWrite(size_t len)
{
	/* Lock the buffer so nothing can modify it */
	LockWriteBuffer();

	/* This is called when the socket engine gets an event on the socket */
#ifdef NETLIB_IOCP

	/* Data has already been output to the socket, we can clear the buffer */
	if(!len)
	{
		Disconnect();
		return;
	}

	m_writeBuffer->Remove(len);

	/* Do we still have data to write? */
	if(m_writeBuffer->GetSize())
		sSocketEngine.WantWrite(this);
	else
		InterlockedDecrement(&m_writeLock);

#else
	/* Push as much data out as we can in a nonblocking fashion. */
	int bytes = send(m_fd, (const char*)m_writeBuffer->GetBufferOffset(), m_writeBuffer->GetSize(), 0);

	if(bytes < 0)
		Disconnect();
	else
		m_writeBuffer->Remove(bytes);
#endif

	/* Unlock the write buffer, we're finished */
	UnlockWriteBuffer();
}

void TcpSocket::Finalize()
{
	sSocketEngine.AddSocket(this);
	OnConnect();
}

bool TcpSocket::Write(const void * data, size_t bytes)
{
	if(!m_connected)
		return false;

	bool rv = m_writeBuffer->Write(data, bytes);
	if(rv)
	{
#ifdef NETLIB_IOCP
		/* On windows since you have multiple threads this has to be guarded. */
		if(InterlockedCompareExchange(&m_writeLock, 1, 0) == 0)
			sSocketEngine.WantWrite(this);
#else
		if(!m_writeLock)
		{
			++m_writeLock;
			sSocketEngine.WantWrite(this);
		}
#endif
	}

    return rv;
}

void TcpSocket::Disconnect()
{
	if(!m_connected) return;
	m_connected = false;

	OnDisconnect();
	sSocketEngine.RemoveSocket(this);
	shutdown(m_fd, SD_BOTH);
	closesocket(m_fd);

	if(!m_deleted)
		Delete();
}

void TcpSocket::Delete()
{
	if(m_deleted) return;
	m_deleted = true;

	sSocketDeleter.Add(this);
	if(m_connected) Disconnect();
}

void TcpSocket::OnError(int errcode)
{
	/* Bleh.. :p */
	Disconnect();
}

bool TcpSocket::Writable()
{
	return (m_writeBuffer->GetSize() > 0) ? true : false;
}
