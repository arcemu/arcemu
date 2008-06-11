/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketLinux.cpp - Linux-specific functions of Socket class are located here.
 *
 */

#include "Network.h"
#ifdef CONFIG_USE_EPOLL

void Socket::PostEvent(uint32 events)
{
    int epoll_fd = sSocketMgr.GetEpollFd();

    struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
    ev.data.fd = m_fd;
    ev.events = events | EPOLLET;			/* use edge-triggered instead of level-triggered because we're using nonblocking sockets */

    // post actual event
    if(epoll_ctl(epoll_fd, EPOLL_CTL_MOD, ev.data.fd, &ev))
		Log.Warning("epoll", "Could not post event on fd %u", m_fd);
}

void Socket::ReadCallback(uint32 len)
{
    // We have to lock here.
    m_readMutex.Acquire();

    size_t space = readBuffer.GetSpace();
    int bytes = recv(m_fd, readBuffer.GetBuffer(), space, 0);
    if(bytes <= 0)
    {
        m_readMutex.Release();
        Disconnect();
        return;
    }    
    else if(bytes > 0)
    {
        //m_readByteCount += bytes;
        readBuffer.IncrementWritten(bytes);
        // call virtual onread()
        OnRead();
    }

    m_readMutex.Release();
}

void Socket::WriteCallback()
{
    // We should already be locked at this point, so try to push everything out.
    int bytes_written = send(m_fd, writeBuffer.GetBufferStart(), writeBuffer.GetContiguiousBytes(), 0);
    if(bytes_written < 0)
    {
        // error.
        Disconnect();
        return;
    }

    //RemoveWriteBufferBytes(bytes_written, false);
    writeBuffer.Remove(bytes_written);
}

void Socket::BurstPush()
{
    if(AcquireSendLock())
        PostEvent(EPOLLOUT);
}

#endif
