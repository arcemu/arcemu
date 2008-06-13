/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketLinux.cpp - Linux-specific functions of Socket class are located here.
 *
 */

#include "Network.h"
#include <errno.h>
#ifdef CONFIG_USE_KQUEUE

void Socket::PostEvent(int events, bool oneshot)
{
    int kq = sSocketMgr.GetKq();

    struct kevent ev;
    if(oneshot)
        EV_SET(&ev, m_fd, events, EV_ADD | EV_ONESHOT, 0, 0, NULL);
    else
        EV_SET(&ev, m_fd, events, EV_ADD, 0, 0, NULL);

    if(kevent(kq, &ev, 1, 0, 0, NULL) < 0)
		Log.Warning("kqueue", "Could not modify event for fd %u", GetFd());
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
        PostEvent(EVFILT_WRITE, true);
}

#endif
