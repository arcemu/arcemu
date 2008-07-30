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

#ifdef NETLIB_EPOLL

epollEngine::epollEngine()
{
	new SocketDeleter();
	epoll_fd = epoll_create(MAX_DESCRIPTORS);
	assert(epoll_fd != -1);
	memset(this->fds, 0, sizeof(void*) * MAX_DESCRIPTORS);
	m_running = true;
}

epollEngine::~epollEngine()
{
	close(epoll_fd);
}

void epollEngine::AddSocket(BaseSocket * s)
{
	assert(fds[s->GetFd()] == 0);
	fds[s->GetFd()] = s;

	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	ev.data.fd = s->GetFd();
	ev.events = (s->Writable()) ? EPOLLOUT : EPOLLIN;
	ev.events |= EPOLLET;

	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, s->GetFd(), &ev);
}

void epollEngine::RemoveSocket(BaseSocket * s)
{
	assert(fds[s->GetFd()] == s);
	fds[s->GetFd()] = 0;

	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	ev.data.fd = s->GetFd();
	ev.events = (s->Writable()) ? EPOLLOUT : EPOLLIN;
	ev.events |= EPOLLET;

	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, s->GetFd(), &ev);
}

void epollEngine::WantWrite(BaseSocket * s)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	ev.data.fd = s->GetFd();
	ev.events = EPOLLOUT | EPOLLET;

	epoll_ctl(epoll_fd, EPOLL_CTL_MOD, s->GetFd(), &ev);
}

void epollEngine::MessageLoop()
{
	const static int maxevents = 1024;
	struct epoll_event events[1024];
	int nfds, i;
	BaseSocket * s;
	while(m_running)
	{
        	nfds = epoll_wait(epoll_fd, events, maxevents, 1000);
		for(i = 0; i < nfds; ++i)
		{
	            s = fds[events[i].data.fd];
			if(s == 0)
			{
				printf("epoll returned invalid fd %u\n", events[i].data.fd);
				continue;
			}

			if(events[i].events & EPOLLHUP || events[i].events & EPOLLERR)
			{
				s->OnError(errno);
			}
			else if(events[i].events & EPOLLIN)
			{
				s->OnRead(0);
				if(s->Writable() && !s->m_writeLock)
				{
					++s->m_writeLock;
					WantWrite(s);
				}
			}
			else if(events[i].events & EPOLLOUT)
			{
				s->OnWrite(0);
				if(!s->Writable())
				{
					/* change back to read state */
					struct epoll_event ev;
					memset(&ev, 0, sizeof(epoll_event));
					ev.data.fd = s->GetFd();
					ev.events = EPOLLIN | EPOLLET;

					epoll_ctl(epoll_fd, EPOLL_CTL_MOD, s->GetFd(), &ev);
					--s->m_writeLock;
				}
			}
		}
	}
}

void epollEngine::Shutdown()
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

void epollEngine::SpawnThreads()
{
	launch_thread(new SocketEngineThread(this));
}

#endif
