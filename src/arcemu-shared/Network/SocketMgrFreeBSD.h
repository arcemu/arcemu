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
#ifdef CONFIG_USE_KQUEUE

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
		// kqueue handle
		int kq;

		// fd -> pointer binding.
		Socket* fds[SOCKET_HOLDER_SIZE];
		ListenSocketBase* listenfds[SOCKET_HOLDER_SIZE];		// shouldnt be more than 1024

		/// socket counter
		int socket_count;

	public:

		/// friend class of the worker thread -> it has to access our private resources
		friend class SocketWorkerThread;

		/// constructor > create epoll device handle + initialize event set
		SocketMgr()
		{
			kq = kqueue();
			if(kq == -1)
			{
				sLog.outError("Could not create a kqueue fd.");
				exit(-1);
			}

			// null out the pointer array
			memset(fds, 0, sizeof(Socket*) * SOCKET_HOLDER_SIZE);
			memset(listenfds, 0, sizeof(Socket*) * SOCKET_HOLDER_SIZE);
		}

		/// destructor > destroy epoll handle
		~SocketMgr()
		{
			// close epoll handle
			close(kq);
		}

		/// add a new socket to the set and to the fd mapping
		void AddSocket(Socket* s);
		void AddListenSocket(ListenSocketBase* s);

		void ShowStatus();	//TODO: Script it

		/// remove a socket from set/fd mapping
		void RemoveSocket(Socket* s);

		/// returns kqueue fd
		inline int GetKq() { return kq; }

		/// returns number of sockets in array
		inline int Count() { return socket_count; }

		/// closes all sockets
		void CloseAll();

		/// spawns worker threads
		void SpawnWorkerThreads();
};

class SocketWorkerThread : public ThreadBase
{
		/// epoll event struct
		struct kevent events[THREAD_EVENT_SIZE];
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
