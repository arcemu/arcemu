/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketMgr - epoll manager for Linux.
 *
 */

#include "Network.h"
#ifdef CONFIG_USE_KQUEUE

initialiseSingleton(SocketMgr);
void SocketMgr::AddSocket(Socket * s)
{
    assert(fds[s->GetFd()] == 0);
    fds[s->GetFd()] = s;

    struct kevent ev;
    if(s->GetWriteBuffer().GetSize())
		EV_SET(&ev, s->GetFd(), EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL);
	else
		EV_SET(&ev, s->GetFd(), EVFILT_READ, EV_ADD, 0, 0, NULL);

    if(kevent(kq, &ev, 1, 0, 0, NULL) < 0)
    {
        Log.Error("kqueue", "Could not add initial kevent for fd %u!", s->GetFd());
        return;
    }
}

void SocketMgr::AddListenSocket(ListenSocketBase * s)
{
	assert(listenfds[s->GetFd()] == 0);
	listenfds[s->GetFd()] = s;

	struct kevent ev;
	EV_SET(&ev, s->GetFd(), EVFILT_READ, EV_ADD, 0, 0, NULL);

	if(kevent(kq, &ev, 1, 0, 0, NULL) < 0)
	{
		Log.Error("kqueue", "Could not add initial kevent for fd %u!", s->GetFd());
		return;
	}
}

void SocketMgr::RemoveSocket(Socket * s)
{
    if(fds[s->GetFd()] != s)
	{
		/* already removed */
		Log.Warning("kqueue", "Duplicate removal of fd %u!", s->GetFd());
        return;
	}
    fds[s->GetFd()] = 0;

    // remove kevent
    struct kevent ev, ev2;
    EV_SET(&ev, s->GetFd(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    EV_SET(&ev2, s->GetFd(), EVFILT_READ, EV_DELETE, 0, 0, NULL);
	if(kevent(kq, &ev, 1, 0, 0, NULL) && kevent(kq, &ev2, 1, 0, 0, NULL))
		Log.Warning("kqueue", "Could not remove from kqueue: fd %u", s->GetFd());
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
    //printf("Worker thread started.\n");
    int fd_count;
    running=true;
    Socket * ptr;
    int i;
    struct kevent ev;
    struct timespec ts;
    ts.tv_nsec = 0;
    ts.tv_sec = 5;
	struct kevent ev2;

    SocketMgr * mgr = SocketMgr::getSingletonPtr();
    int kq = mgr->GetKq();

    while(running)
    {
        fd_count = kevent(kq, NULL, 0, &events[0], THREAD_EVENT_SIZE, &ts);
        for(i = 0; i < fd_count; ++i)
        {
            if(events[i].ident >= SOCKET_HOLDER_SIZE)
            {
                Log.Warning("kqueue", "Requested FD that is too high (%u)", events[i].ident);
                continue;
            }

            ptr = mgr->fds[events[i].ident];

            if(ptr == NULL)
            {
				if( (ptr = ((Socket*)mgr->listenfds[events[i].ident])) != NULL )
				{
					((ListenSocketBase*)ptr)->OnAccept();
				}
				else
				{
					Log.Warning("kqueue", "Returned invalid fd (no pointer) of FD %u", events[i].ident);

					/* make sure it removes so we don't go chasing it again */
					EV_SET(&ev, events[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
					EV_SET(&ev2, events[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
					kevent(kq, &ev, 1, 0, 0, NULL);
					kevent(kq, &ev2, 1, 0, 0, NULL);
				}
                continue;
            }

            if(events[i].flags & EV_EOF || events[i].flags & EV_ERROR)
            {
                ptr->Disconnect();
                continue;
            }
			else if(events[i].filter == EVFILT_WRITE)
            {
                ptr->BurstBegin();          // Lock receive mutex
                ptr->WriteCallback();       // Perform actual send()
                if(ptr->GetWriteBuffer().GetSize() > 0)
                    ptr->PostEvent(EVFILT_WRITE, true);   // Still remaining data.
                else
				{
                    ptr->DecSendLock();
                    ptr->PostEvent(EVFILT_READ, false);
                }
                ptr->BurstEnd();            // Unlock
            }
			else if(events[i].filter == EVFILT_READ)
            {
                ptr->ReadCallback(0);               // Len is unknown at this point.
				if(ptr->GetWriteBuffer().GetSize() > 0 && ptr->IsConnected() && !ptr->HasSendLock())
				{
					ptr->PostEvent(EVFILT_WRITE, true);
					ptr->IncSendLock();
				}
            }
        }       
    }
    return true;
}

#endif
