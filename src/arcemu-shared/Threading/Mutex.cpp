/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../Common.h"
#include "Mutex.h"

#ifdef WIN32

/* Windows Critical Section Implementation */
Mutex::Mutex() { InitializeCriticalSection(&cs); }
Mutex::~Mutex() { DeleteCriticalSection(&cs); }

#else

/* this is done slightly differently on bsd-variants */
#if defined(__FreeBSD__) ||  defined(__APPLE_CC__) || defined(__OpenBSD__)
#define recursive_mutex_flag PTHREAD_MUTEX_RECURSIVE
#else
#define recursive_mutex_flag PTHREAD_MUTEX_RECURSIVE_NP
#endif

/* Linux mutex implementation */
bool Mutex::attr_initalized = false;
pthread_mutexattr_t Mutex::attr;

Mutex::Mutex()
{
	if(!attr_initalized)
	{
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, recursive_mutex_flag);
		attr_initalized = true;
	}

	pthread_mutex_init(&mutex, &attr);
}

Mutex::~Mutex() { pthread_mutex_destroy(&mutex); }

#endif

bool Mutex::AttemptAcquire()
{
#ifndef WIN32
	return (pthread_mutex_trylock(&mutex) == 0);
#else
	return (TryEnterCriticalSection(&cs) == TRUE ? true : false);
#endif
}


void Mutex::Acquire()
{
#ifndef WIN32
	pthread_mutex_lock(&mutex);
#else
	EnterCriticalSection(&cs);
#endif
}


void Mutex::Release()
{
#ifndef WIN32
	pthread_mutex_unlock(&mutex);
#else
	LeaveCriticalSection(&cs);
#endif
}


#ifdef WIN32

bool FastMutex::AttemptAcquire()
{
	DWORD thread_id = GetCurrentThreadId();
	if(thread_id == (DWORD)m_lock)
	{
		++m_recursiveCount;
		return true;
	}

	DWORD owner = InterlockedCompareExchange(&m_lock, thread_id, 0);
	if(owner == 0)
	{
		++m_recursiveCount;
		return true;
	}

	return false;
}

void FastMutex::Acquire()
{
	DWORD thread_id = GetCurrentThreadId(), owner;
	if(thread_id == (DWORD)m_lock)
	{
		++m_recursiveCount;
		return;
	}

	for(;;)
	{
		owner = InterlockedCompareExchange(&m_lock, thread_id, 0);
		if(owner == 0)
			break;
		::Sleep(0);
	}
	++m_recursiveCount;

}

void FastMutex::Release()
{
	if((--m_recursiveCount) == 0)
		InterlockedExchange(&m_lock, 0);
}

#endif

