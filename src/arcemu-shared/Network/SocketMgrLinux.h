/*
 * ArcEmu, a MMORPG Server
 * 
 * Copyright (C) 2008-2012 ArcEmu Developers <http://arcemu.org/>
 *
 * See former copyright holders in CREDITS
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef SOCKETMGR_LINUX_H
#define SOCKETMGR_LINUX_H

#include "SocketDefines.h"
#ifdef CONFIG_USE_EPOLL

#define SOCKET_HOLDER_SIZE 30000    // You don't want this number to be too big, otherwise you're gonna be eating
// memory. 65536 = 256KB, so thats no big issue for now, and I really can't
// see anyone wanting to have more than 65536 concurrent connections.

#define THREAD_EVENT_SIZE 4096      // This is the number of socket events each thread can receieve at once.
// This default value should be more than enough.

class Socket;
class SocketWorkerThread;
class ListenSocketBase;

class SocketMgr : public Singleton<SocketMgr>
{
		/// /dev/epoll instance handle
		int epoll_fd;

		// fd -> pointer binding.
		Socket* fds[SOCKET_HOLDER_SIZE];
		ListenSocketBase* listenfds[SOCKET_HOLDER_SIZE];

		/// socket counter
		Arcemu::Threading::AtomicCounter socket_count;

		int max_fd;

	public:

		/// friend class of the worker thread -> it has to access our private resources
		friend class SocketWorkerThread;

		/// constructor > create epoll device handle + initialize event set
		SocketMgr()
		{
			epoll_fd = epoll_create(SOCKET_HOLDER_SIZE);
			if(epoll_fd == -1)
			{
				sLog.outError("Could not create epoll fd (/dev/epoll).");
				exit(-1);
			}

			// null out the pointer array
			memset(fds, 0, sizeof(void*) * SOCKET_HOLDER_SIZE);
			memset(listenfds, 0, sizeof(void*) * SOCKET_HOLDER_SIZE);
			max_fd = 0;
		}

		/// destructor > destroy epoll handle
		~SocketMgr()
		{
			// close epoll handle
			close(epoll_fd);
		}

		/// add a new socket to the epoll set and to the fd mapping
		void AddSocket(Socket* s);
		void AddListenSocket(ListenSocketBase* s);

		/// remove a socket from epoll set/fd mapping
		void RemoveSocket(Socket* s);

		/// returns epoll fd
		inline int GetEpollFd() { return epoll_fd; }

		/// closes all sockets
		void CloseAll();

		/// spawns worker threads
		void SpawnWorkerThreads();

		/// show status
		void ShowStatus();
};

class SocketWorkerThread : public ThreadBase
{
		/// epoll event struct
		struct epoll_event events[THREAD_EVENT_SIZE];
		bool running;
	public:
		bool run();
		void OnShutdown()
		{
			running = false;
		}
};

#define sSocketMgr SocketMgr::getSingleton()

#endif

#endif
