/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

#ifndef CONDITION_H
#define CONDITION_H

#ifdef WIN32
//#include <windows.h>
#define MAX_AWAITING_THREADS 10

struct list_entry
{
	HANDLE semaphore;
	long count;
	bool notified;
};

/*class Condition
{
   
public:
	
	Condition(Mutex*m)
	{
		external_mutex=m;
		wake_sem=NULL;
		memset(generations,0,sizeof(generations));
	}

			
	~Condition()
	{
		for(unsigned int i=0;i<MAX_AWAITING_THREADS;++i)
				dispose_entry(generations[i]);
		
		CloseHandle(wake_sem);
	}

	  
	void Signal()
	{
		internal_mutex.Acquire();
		if(wake_sem)
		{
			ReleaseSemaphore(wake_sem,1,NULL);
			for(unsigned int generation=MAX_AWAITING_THREADS;generation!=0;--generation)
			{
				list_entry& entry=generations[generation-1];
				if(entry.count)
				{
					entry.notified=true;
					ReleaseSemaphore(entry.semaphore,1,NULL);
					if(!--entry.count)
					{
						dispose_entry(entry);
					}
				}
			}
		}
		internal_mutex.Release();
	}

	void Broadcast()
	{
		internal_mutex.Acquire();
		if(wake_sem)
		{
			for(unsigned int generation=MAX_AWAITING_THREADS;generation!=0;--generation)
			{
				list_entry& entry=generations[generation-1];
				if(entry.count)
					broadcast_entry(entry);
			}
		}
		internal_mutex.Release();
	}
	 
	bool Wait()
	{
		HANDLE local_wake_sem;
		HANDLE sem;
		
			
			internal_mutex.Acquire();
		
				external_mutex->Release();
				if(!wake_sem)
				{
					wake_sem=create_anonymous_semaphore(0,LONG_MAX);
					ASSERT(wake_sem);
				}
					
				local_wake_sem=duplicate_handle(wake_sem);

				if(generations[0].notified)
				{
					shift_generations_down();
				}
						
				if(!generations[0].semaphore)
				{
					generations[0].semaphore=create_anonymous_semaphore(0,LONG_MAX);
					ASSERT(generations[0].semaphore);
				}
				
		
			++generations[0].count;
			sem=duplicate_handle(generations[0].semaphore);
			internal_mutex.Release();
				
			ASSERT(WaitForSingleObject(sem,INFINITE)==0);
			ASSERT(CloseHandle(sem));

			
			WaitForSingleObject(local_wake_sem,0);
	  
	
		ASSERT(CloseHandle(local_wake_sem));
		external_mutex->Acquire();//Must be acquire but it does not work with it
		return true;
	}

protected:
	HANDLE create_anonymous_semaphore(long cur,long max)
	{
		SECURITY_ATTRIBUTES attr;
		attr.nLength=sizeof(SECURITY_ATTRIBUTES);
		attr.lpSecurityDescriptor=NULL;
		attr.bInheritHandle=false;
		return CreateSemaphore(&attr,cur,max,NULL);
	}

		static bool no_waiters(list_entry const& entry)
		{
			return entry.count==0;
		}

		ARCEMU_INLINE void shift_generations_down()
		{
			if(std::remove_if(generations,generations+MAX_AWAITING_THREADS,no_waiters)==generations+MAX_AWAITING_THREADS)
			{
				broadcast_entry(generations[MAX_AWAITING_THREADS-1]);
			}
			std::copy_backward(generations,generations+MAX_AWAITING_THREADS,generations+MAX_AWAITING_THREADS);
			generations[0].semaphore=0;
			generations[0].count=0;
			generations[0].notified=false;
		}

		void broadcast_entry(list_entry& entry)
		{
			ReleaseSemaphore(wake_sem,entry.count,NULL);
			ReleaseSemaphore(entry.semaphore,entry.count,NULL);
			entry.count=0;
			dispose_entry(entry);
		}

		ARCEMU_INLINE void dispose_entry(list_entry& entry)
		{
			ASSERT(entry.count==0);
			if(entry.semaphore)
			{
				ASSERT(CloseHandle(entry.semaphore));
			}
			entry.semaphore=0;
			entry.notified=false;
		}

		ARCEMU_INLINE HANDLE duplicate_handle(HANDLE source)
		{
			HANDLE const current_process=GetCurrentProcess();

			HANDLE new_handle=0;
			ASSERT(DuplicateHandle(
				current_process,
				source,current_process,&new_handle,0,false,DUPLICATE_SAME_ACCESS))
			return new_handle;
		}

	   
	Mutex	internal_mutex;
	Mutex	*external_mutex;
	
	list_entry generations[MAX_AWAITING_THREADS];
	HANDLE wake_sem;

};*/

class Condition
{
public:
	ARCEMU_INLINE Condition(Mutex * mutex) : m_nLockCount(0), m_externalMutex(mutex)
	{
		  ::InitializeCriticalSection(&m_critsecWaitSetProtection);
	}

	~Condition()
	{
		::DeleteCriticalSection(&m_critsecWaitSetProtection);
		assert(m_deqWaitSet.empty());
	}

	ARCEMU_INLINE void BeginSynchronized()
	{
		m_externalMutex->Acquire();
		++m_nLockCount;
	}

	ARCEMU_INLINE void EndSynchronized()
	{
		assert(LockHeldByCallingThread());
		--m_nLockCount;
		m_externalMutex->Release();
	}

	DWORD Wait(time_t timeout)
	{
		DWORD dwMillisecondsTimeout = (DWORD)timeout * 1000;
		BOOL bAlertable = FALSE;
		ASSERT(LockHeldByCallingThread());

		// Enter a new event handle into the wait set.
		HANDLE hWaitEvent = Push();
		if( NULL == hWaitEvent )
			return WAIT_FAILED;

		// Store the current lock count for re-acquisition.
		int nThisThreadsLockCount = m_nLockCount;
		m_nLockCount = 0;

		// Release the synchronization lock the appropriate number of times.
		// Win32 allows no error checking here.
		for( int i=0; i<nThisThreadsLockCount; ++i)
		{
			//::LeaveCriticalSection(&m_critsecSynchronized);
			m_externalMutex->Release();
		}

		// NOTE: Conceptually, releasing the lock and entering the wait
		// state is done in one atomic step. Technically, that is not
		// true here, because we first leave the critical section and
		// then, in a separate line of code, call WaitForSingleObjectEx.
		// The reason why this code is correct is that our thread is placed
		// in the wait set *before* the lock is released. Therefore, if
		// we get preempted right here and another thread notifies us, then
		// that notification will *not* be missed: the wait operation below
		// will find the event signalled.

		// Wait for the event to become signalled.
		DWORD dwWaitResult = ::WaitForSingleObjectEx(
			hWaitEvent,
			dwMillisecondsTimeout,
			bAlertable
			);

		// If the wait failed, store the last error because it will get
		// overwritten when acquiring the lock.
		DWORD dwLastError = 0;
		if( WAIT_FAILED == dwWaitResult )
			dwLastError = ::GetLastError();

		// Acquire the synchronization lock the appropriate number of times.
		// Win32 allows no error checking here.
		for( int j=0; j<nThisThreadsLockCount; ++j)
		{
			//::EnterCriticalSection(&m_critsecSynchronized);
			m_externalMutex->Acquire();
		}

		// Restore lock count.
		m_nLockCount = nThisThreadsLockCount;

		// Close event handle
		if( ! CloseHandle(hWaitEvent) )
			return WAIT_FAILED;

		if( WAIT_FAILED == dwWaitResult )
			::SetLastError(dwLastError);

		return dwWaitResult;
	}

	DWORD Wait()
	{
		DWORD dwMillisecondsTimeout = INFINITE;
		BOOL bAlertable = FALSE;
		ASSERT(LockHeldByCallingThread());

		// Enter a new event handle into the wait set.
		HANDLE hWaitEvent = Push();
		if( NULL == hWaitEvent )
			return WAIT_FAILED;

		// Store the current lock count for re-acquisition.
		int nThisThreadsLockCount = m_nLockCount;
		m_nLockCount = 0;

		// Release the synchronization lock the appropriate number of times.
		// Win32 allows no error checking here.
		for( int i=0; i<nThisThreadsLockCount; ++i)
		{
			//::LeaveCriticalSection(&m_critsecSynchronized);
			m_externalMutex->Release();
		}

		// NOTE: Conceptually, releasing the lock and entering the wait
		// state is done in one atomic step. Technically, that is not
		// true here, because we first leave the critical section and
		// then, in a separate line of code, call WaitForSingleObjectEx.
		// The reason why this code is correct is that our thread is placed
		// in the wait set *before* the lock is released. Therefore, if
		// we get preempted right here and another thread notifies us, then
		// that notification will *not* be missed: the wait operation below
		// will find the event signalled.

		// Wait for the event to become signalled.
		DWORD dwWaitResult = ::WaitForSingleObjectEx(
			hWaitEvent,
			dwMillisecondsTimeout,
			bAlertable
			);

		// If the wait failed, store the last error because it will get
		// overwritten when acquiring the lock.
		DWORD dwLastError = 0;
		if( WAIT_FAILED == dwWaitResult )
			dwLastError = ::GetLastError();

		// Acquire the synchronization lock the appropriate number of times.
		// Win32 allows no error checking here.
		for( int j=0; j<nThisThreadsLockCount; ++j)
		{
			//::EnterCriticalSection(&m_critsecSynchronized);
			m_externalMutex->Acquire();
		}

		// Restore lock count.
		m_nLockCount = nThisThreadsLockCount;

		// Close event handle
		if( ! CloseHandle(hWaitEvent) )
			return WAIT_FAILED;

		if( WAIT_FAILED == dwWaitResult )
			::SetLastError(dwLastError);

		return dwWaitResult;

	}

	void Signal()
	{
		// Pop the first handle, if any, off the wait set.
		HANDLE hWaitEvent = Pop();

		// If there is not thread currently waiting, that's just fine.
		if(NULL == hWaitEvent)
			return;

		// Signal the event.
		SetEvent(hWaitEvent);
	}

	void Broadcast()
	{
		// Signal all events on the deque, then clear it. Win32 allows no
		// error checking on entering and leaving the critical section.
		//
		::EnterCriticalSection(&m_critsecWaitSetProtection);
		std::deque<HANDLE>::const_iterator it_run = m_deqWaitSet.begin();
		std::deque<HANDLE>::const_iterator it_end = m_deqWaitSet.end();
		for( ; it_run < it_end; ++it_run )
		{
			if( ! SetEvent(*it_run) )
				return;
		}
		m_deqWaitSet.clear();
		::LeaveCriticalSection(&m_critsecWaitSetProtection);
	}

private:

	HANDLE Push()
	{
		// Create the new event.
		HANDLE hWaitEvent = ::CreateEvent(
			NULL, // no security
			FALSE, // auto-reset event
			FALSE, // initially unsignalled
			NULL // string name
			);
		//
		if( NULL == hWaitEvent ) {
			return NULL;
		}

		// Push the handle on the deque.
		::EnterCriticalSection(&m_critsecWaitSetProtection);
		m_deqWaitSet.push_back(hWaitEvent);
		::LeaveCriticalSection(&m_critsecWaitSetProtection);

		return hWaitEvent;
	}

	HANDLE Pop()
	{
		// Pop the first handle off the deque.
		//
		::EnterCriticalSection(&m_critsecWaitSetProtection);
		HANDLE hWaitEvent = NULL; 
		if( 0 != m_deqWaitSet.size() )
		{
			hWaitEvent = m_deqWaitSet.front();
			m_deqWaitSet.pop_front();
		}
		::LeaveCriticalSection(&m_critsecWaitSetProtection);

		return hWaitEvent;
	}

	BOOL LockHeldByCallingThread()
	{
		//BOOL bTryLockResult = ::TryEnterCriticalSection(&m_critsecSynchronized);
		BOOL bTryLockResult = m_externalMutex->AttemptAcquire();

		// If we didn't get the lock, someone else has it.
		//
		if( ! bTryLockResult )
		{
			return FALSE;
		}

		// If we got the lock, but the lock count is zero, then nobody had it.
		//
		if( 0 == m_nLockCount )
		{
			assert( bTryLockResult );
			//::LeaveCriticalSection(&m_critsecSynchronized);
			m_externalMutex->Release();
			return FALSE;
		}

		// Release lock once. NOTE: we still have it after this release.
		// Win32 allows no error checking here.
		assert( bTryLockResult && 0 < m_nLockCount );
		//::LeaveCriticalSection(&m_critsecSynchronized);
		m_externalMutex->Release();

		return TRUE;
	}

	std::deque<HANDLE> m_deqWaitSet;
	CRITICAL_SECTION m_critsecWaitSetProtection;
	Mutex * m_externalMutex;
	int m_nLockCount;
};

#else

class Condition
{
public:
	ARCEMU_INLINE Condition(Mutex *m)
	{
		mut=m;
		pthread_cond_init(&cond,NULL);
	}
	ARCEMU_INLINE ~Condition()
	{
		pthread_cond_destroy(&cond);
	}

	ARCEMU_INLINE void Signal()
	{
		pthread_cond_signal(&cond);
	}
	ARCEMU_INLINE void Broadcast()
	{
		pthread_cond_broadcast(&cond);
	}
	ARCEMU_INLINE void Wait()
	{
		pthread_cond_wait(&cond,&mut->mutex);
	}
	ARCEMU_INLINE bool Wait(time_t seconds)
	{
		timespec tv;
		tv.tv_nsec = 0;
		tv.tv_sec = seconds;
		if(pthread_cond_timedwait(&cond, &mut->mutex, &tv) == 0)
			return true;
		else
			return false;
	}
	ARCEMU_INLINE void BeginSynchronized()
	{
		mut->Acquire();
	}
	ARCEMU_INLINE void EndSynchronized()
	{
		mut->Release();
	}
private:
	pthread_cond_t cond;
	Mutex *mut;

};

#endif

#endif

	
