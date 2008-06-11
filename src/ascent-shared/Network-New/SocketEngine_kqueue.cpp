/****************************************************************************
 *
 * Multiplatform High-Performance Async Network Library
 * Implemented epoll Socket Engine
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

#ifdef NETLIB_KQUEUE

kqueueEngine::kqueueEngine()
{
	new SocketDeleter();
	kq = kqueue();
	assert(kq != -1);
	memset(this->fds, 0, sizeof(void*) * MAX_DESCRIPTORS);
	m_running = true;
}

kqueueEngine::~kqueueEngine()
{
	close(kq);
}

void kqueueEngine::AddSocket(BaseSocket * s)
{
	assert(fds[s->GetFd()] == 0);
	fds[s->GetFd()] = s;

	struct kevent ev;
	EV_SET(&ev, s->GetFd(), s->Writable() ? EVFILT_WRITE : EVFILT_READ, EV_ADD, 0, 0, NULL);
	if(kevent(kq, &ev, 1, NULL, 0, NULL) < 0)
		printf("!! could not add kevent for fd %u\n", s->GetFd());
}

void kqueueEngine::RemoveSocket(BaseSocket * s)
{
	int a, b;
	assert(fds[s->GetFd()] == s);
	fds[s->GetFd()] = 0;

	struct kevent ke, k2;
	EV_SET(&ke, s->GetFd(), EVFILT_READ, EV_DELETE, 0, 0, NULL);
	EV_SET(&k2, s->GetFd(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
	if((kevent(kq, &ke, 1, NULL, 0, NULL) < 0) && (kevent(kq, &k2, 1, NULL, 0, NULL) < 0))
		printf("!! both kevent removals returned -1 for fd %u\n", s->GetFd());	
}

void kqueueEngine::WantWrite(BaseSocket * s)
{
	struct kevent ev;
	EV_SET(&ev, s->GetFd(), EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL);
	if(kevent(kq, &ev, 1, NULL, 0, NULL) < 0)
		printf("!! could not modify kevent for fd %u\n", s->GetFd());
}

void kqueueEngine::MessageLoop()
{
	const static int maxevents = MAX_DESCRIPTORS;
	timespec timeout;
	timeout.tv_sec = 1;
	timeout.tv_nsec = 0;
	struct kevent events[MAX_DESCRIPTORS];
	struct kevent ev;
	int nfds, i;
	BaseSocket * s;
	while(m_running)
	{
		nfds = kevent(kq, 0, 0, events, maxevents, &timeout);
		for(i = 0; i < nfds; ++i)
		{
	        s = fds[events[i].ident];
			if(s == 0)
			{
				printf("kqueue returned invalid fd %u\n", events[i].ident);
				continue;
			}

			if(events[i].flags & EV_EOF || events[i].flags & EV_ERROR)
			{
				s->OnError(events[i].fflags);
				continue;
			}

			if(events[i].filter == EVFILT_READ)
			{
				s->OnRead(0);
                if(s->Writable() && !s->m_writeLock)
				{
					++s->m_writeLock;
					WantWrite(s);
				}
			}
			else if(events[i].filter == EVFILT_WRITE)
			{
				s->OnWrite(0);
				if(!s->Writable())
				{
					--s->m_writeLock;
					EV_SET(&ev, s->GetFd(), EVFILT_READ, EV_ADD, 0, 0, NULL);
					if(kevent(kq, &ev, 1, NULL, 0, NULL) < 0)
						printf("!! could not modify kevent (to read) for fd %u\n", s->GetFd());
				}
				else
				{
					EV_SET(&ev, s->GetFd(), EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL);
                    if(kevent(kq, &ev, 1, NULL, 0, NULL) < 0)
	                    printf("!! could not modify kevent (to write) for fd %u\n", s->GetFd());
				}

			}
			else
			{
				printf("Unknwon filter: %u Fflags: %u, fd: %u, flags: %u\n", events[i].filter, events[i].fflags, events[i].ident, events[i].flags);
			}
		}
	}
}

void kqueueEngine::Shutdown()
{
	m_running = false;
	for(int i = 0; i < MAX_DESCRIPTORS; ++i)
	{
		if(fds[i] != 0)
		{
			fds[i]->Delete();
		}
	}

	sSocketDeleter.Kill();
	delete SocketDeleter::getSingletonPtr();
	delete this;
}

void kqueueEngine::SpawnThreads()
{
	launch_thread(new SocketEngineThread(this));
}

#endif
