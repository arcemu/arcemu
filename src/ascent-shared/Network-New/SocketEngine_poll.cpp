/****************************************************************************
 *
 * Multiplatform High-Performance Async Network Library
 * Implemented Poll Socket Engine
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

#ifdef NETLIB_POLL

pollEngine::pollEngine()
{
	new SocketDeleter();
	memset(this->fds, 0, sizeof(void*) * MAX_DESCRIPTORS);
	m_running = true;
	highest_fd = 0;
	for(int i = 0; i < MAX_DESCRIPTORS; ++i)
	{
		poll_events[i].fd = -1;
		poll_events[i].events = 0;
		poll_events[i].revents = 0;
	}
}

pollEngine::~pollEngine()
{
	
}

void pollEngine::AddSocket(BaseSocket * s)
{
	assert(fds[s->GetFd()] == 0);
	fds[s->GetFd()] = s;

	/* check the highest fd */
	if(s->GetFd() > highest_fd)
	{
		highest_fd = s->GetFd();
	}

	poll_events[s->GetFd()].fd = s->GetFd();
	poll_events[s->GetFd()].events = (s->Writable() ? POLLOUT : POLLIN);
	poll_events[s->GetFd()].revents = 0;
}

void pollEngine::RemoveSocket(BaseSocket * s)
{
	assert(fds[s->GetFd()] == s);
	fds[s->GetFd()] = 0;

	poll_events[s->GetFd()].fd = s->GetFd();
	poll_events[s->GetFd()].events = 0;
	poll_events[s->GetFd()].revents = 0;

	if(highest_fd == s->GetFd())
	{
		/* go down until we find the new highest fd */
		int new_highest = 0;
		for(int i = highest_fd; i > 0; --i)
		{
			if(fds[i] != 0)
			{
				new_highest = i;
				break;
			}
		}

		highest_fd = new_highest;
	}
}

void pollEngine::WantWrite(BaseSocket * s)
{
	poll_events[s->GetFd()].events = POLLOUT;
}

void pollEngine::MessageLoop()
{
	const static int maxevents = 1024;
	int nfds, i;
	BaseSocket * s;
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 50000;

	while(m_running)
	{
        nfds = poll(poll_events, highest_fd + 1, 50);
		if(!m_running)
			return;

		if(nfds > 0)
		{
			for(i = 0; i <= highest_fd; ++i)
			{
                if(poll_events[i].fd > 0)
				{
					s = fds[i];
					if(s == 0)
					{
						printf("poll returned invalid fd %u\n", i);
						poll_events[i].fd = -1;
						poll_events[i].events = 0;
						poll_events[i].revents = 0;
						continue;
					}

					if(poll_events[i].revents & POLLERR || poll_events[i].revents & POLLHUP)
					{
						s->Disconnect();
						continue;
					}

					if(poll_events[i].revents & POLLIN)
					{
						s->OnRead(0);

						/* are we writable now? */
						if(s->Writable() && !s->m_writeLock)
						{
							++s->m_writeLock;
							poll_events[i].events = POLLOUT;
						}
					}
					
					if(poll_events[i].revents & POLLOUT)
					{
						s->OnWrite(0);

						/* are we readable now? */
						if(!s->Writable())
						{
							--s->m_writeLock;
							poll_events[i].events = POLLIN;
						}
					}
				}
			}
		}
	}
}

void pollEngine::Shutdown()
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

void pollEngine::SpawnThreads()
{
	launch_thread(new SocketEngineThread(this));
}

#endif
