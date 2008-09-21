/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketMgr - epoll manager for Linux.
 *
 */

#include "Network.h"
#ifdef CONFIG_USE_EPOLL

//#define ENABLE_ANTI_DOS

initialiseSingleton(SocketMgr);
void SocketMgr::AddSocket(Socket * s)
{
#ifdef ENABLE_ANTI_DOS
	uint32 saddr;
	int i, count;

	// Check how many connections we already have from that ip
	saddr = s->GetRemoteAddress().s_addr;
	for (i=0, count=0; i<max_fd; i++)
	{
		if (fds[i])
		{
			if (fds[i]->GetRemoteAddress().s_addr == saddr) count++;
		}
	}

	// More than 16 connections from the same ip? enough! xD
	if (count > 16)
	{
		s->Disconnect(false);
		return;
	}
#endif

	if(fds[s->GetFd()] != NULL)
	{
		//fds[s->GetFd()]->Delete();
		//fds[s->GetFd()] = NULL;
		s->Delete();
		return;
	}

#ifdef ENABLE_ANTI_DOS
	if (max_fd < s->GetFd()) max_fd = s->GetFd();
#endif
    fds[s->GetFd()] = s;

    // Add epoll event based on socket activity.
    struct epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));
    ev.events = (s->GetWriteBuffer().GetSize()) ? EPOLLOUT : EPOLLIN;
    ev.events |= EPOLLET;			/* use edge-triggered instead of level-triggered because we're using nonblocking sockets */
    ev.data.fd = s->GetFd();
    
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev))
		Log.Warning("epoll", "Could not add event to epoll set on fd %u", ev.data.fd);
}

void SocketMgr::AddListenSocket(ListenSocketBase * s)
{
	assert(listenfds[s->GetFd()] == 0);
	listenfds[s->GetFd()] = s;

	// Add epoll event based on socket activity.
	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	ev.events = EPOLLIN;
	ev.events |= EPOLLET;			/* use edge-triggered instead of level-triggered because we're using nonblocking sockets */
	ev.data.fd = s->GetFd();

	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev))
		Log.Warning("epoll", "Could not add event to epoll set on fd %u", ev.data.fd);
}

void SocketMgr::RemoveSocket(Socket * s)
{
    if(fds[s->GetFd()] != s)
	{
		Log.Warning("epoll", "Could not remove fd %u from the set due to it not existing?", s->GetFd());
        return;
	}

	fds[s->GetFd()] = 0;
    // Remove from epoll list.
    struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
    ev.data.fd = s->GetFd();
    ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLONESHOT;

    if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, ev.data.fd, &ev))
		Log.Warning("epoll", "Could not remove fd %u from epoll set, errno %u", s->GetFd(), errno);
}

void SocketMgr::CloseAll()
{
    for(uint32 i = 0; i < SOCKET_HOLDER_SIZE; ++i)
        if(fds[i] != NULL)
            fds[i]->Delete();
}

void SocketMgr::SpawnWorkerThreads()
{
    uint32 count = 1;
    for(uint32 i = 0; i < count; ++i)
        ThreadPool.ExecuteTask(new SocketWorkerThread());
}

bool SocketWorkerThread::run()
{
    int fd_count;
    Socket * ptr;
    int i;
    running = true;
    SocketMgr * mgr = SocketMgr::getSingletonPtr();

    while(running)
    {
        fd_count = epoll_wait(mgr->epoll_fd, events, THREAD_EVENT_SIZE, 5000);
        for(i = 0; i < fd_count; ++i)
        {
            if(events[i].data.fd >= SOCKET_HOLDER_SIZE)
            {
                Log.Warning("epoll", "Requested FD that is too high (%u)", events[i].data.fd);
                continue;
            }

            ptr = mgr->fds[events[i].data.fd];

            if(ptr == NULL)
            {
				if( (ptr = ((Socket*)mgr->listenfds[events[i].data.fd])) != NULL )
					((ListenSocketBase*)ptr)->OnAccept();
				else
					Log.Warning("epoll", "Returned invalid fd (no pointer) of FD %u", events[i].data.fd);

                continue;
            }

            if(events[i].events & EPOLLHUP || events[i].events & EPOLLERR)
            {
				ptr->Disconnect();
                continue;
            }
			else if(events[i].events & EPOLLIN)
            {
                ptr->ReadCallback(0);               // Len is unknown at this point.

				/* changing to written state? */
				if(ptr->GetWriteBuffer().GetSize() && !ptr->HasSendLock() && ptr->IsConnected())
					ptr->PostEvent(EPOLLOUT);
            }
			else if(events[i].events & EPOLLOUT)
            {
                ptr->BurstBegin();          // Lock receive mutex
                ptr->WriteCallback();       // Perform actual send()
                if(ptr->GetWriteBuffer().GetSize() > 0)
                {
                    /* we don't have to do anything here. no more oneshots :) */
                }
                else
                {
					/* change back to a read event */
                    ptr->DecSendLock();
                    ptr->PostEvent(EPOLLIN);
                }
                ptr->BurstEnd();            // Unlock
            }
        }       
    }
    return true;
}

#endif
