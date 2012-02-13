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

#ifndef SOCKET_DEFINES_H
#define SOCKET_DEFINES_H

#include "../Common.h"
using namespace std;

/* Implementation Selection */
#ifdef WIN32		// Easy
#define CONFIG_USE_IOCP
//#define CONFIG_USE_SELECT
#else

// unix defines
#define SOCKET int
#define SD_BOTH SHUT_RDWR

#if UNIX_FLAVOUR == UNIX_FLAVOUR_LINUX

// select: epoll
#include <sys/epoll.h>
#define CONFIG_USE_EPOLL

#elif UNIX_FLAVOUR == UNIX_FLAVOUR_BSD

// select: kqueue
#include <sys/event.h>
#define CONFIG_USE_KQUEUE

#elif UNIX_FLAVOUR == UNIX_FLAVOUR_OSX
// select: kqueue
#include <sys/event.h>
#define CONFIG_USE_KQUEUE


#endif

#endif

/* IOCP Defines */

#ifdef CONFIG_USE_IOCP
enum SocketIOEvent
{
    SOCKET_IO_EVENT_READ_COMPLETE   = 0,
    SOCKET_IO_EVENT_WRITE_END		= 1,
    SOCKET_IO_THREAD_SHUTDOWN		= 2,
    NUM_SOCKET_IO_EVENTS			= 3,
};

class OverlappedStruct
{
	public:
		OVERLAPPED m_overlap;
		SocketIOEvent m_event;
		volatile long m_inUse;
		OverlappedStruct(SocketIOEvent ev) : m_event(ev)
		{
			memset(&m_overlap, 0, sizeof(OVERLAPPED));
			m_inUse = 0;
		};

		OverlappedStruct()
		{
			memset(&m_overlap, 0, sizeof(OVERLAPPED));
			m_inUse = 0;
		}

		__forceinline void Reset(SocketIOEvent ev)
		{
			memset(&m_overlap, 0, sizeof(OVERLAPPED));
			m_event = ev;
		}

		void Mark()
		{
			long val = InterlockedCompareExchange(&m_inUse, 1, 0);
			if(val != 0)
				sLog.outError("!!!! Network: Detected double use of read/write event! Previous event was %u.", m_event);
		}

		void Unmark()
		{
			InterlockedExchange(&m_inUse, 0);
		}
};

#endif

#endif
