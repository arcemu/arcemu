/*
 * Thread Pool Class
 * Copyright (C) Burlex <burlex@gmail.com>
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

#include "ThreadPool.h"
#include "../NGLog.h"

#ifdef WIN32

	#include <process.h>

#else
	
	volatile int threadid_count = 0;
	int GenerateThreadId()
	{
		int i = ++threadid_count;
		return i;
	}

#endif

#define THREAD_RESERVE 10
SERVER_DECL CThreadPool ThreadPool;

CThreadPool::CThreadPool()
{
	_threadsExitedSinceLastCheck = 0;
	_threadsRequestedSinceLastCheck = 0;
	_threadsEaten = 0;
	_threadsFreedSinceLastCheck = 0;
}

bool CThreadPool::ThreadExit(Thread * t)
{
	_mutex.Acquire();
	
	// we're definitely no longer active
	m_activeThreads.erase(t);

	// do we have to kill off some threads?
	if(_threadsToExit > 0)
	{
		// kill us.
		--_threadsToExit;
		++_threadsExitedSinceLastCheck;
		if(t->DeleteAfterExit)
			m_freeThreads.erase(t);

		_mutex.Release();
		delete t;
		return false;
	}

	// enter the "suspended" pool
	++_threadsExitedSinceLastCheck;
	++_threadsEaten;
	std::set<Thread*>::iterator itr = m_freeThreads.find(t);

	if(itr != m_freeThreads.end())
	{
		printf("Thread %u duplicated with thread %u\n", (*itr)->ControlInterface.GetId(), t->ControlInterface.GetId());
	}
	m_freeThreads.insert(t);
	
	Log.Debug("ThreadPool", "Thread %u entered the free pool.", t->ControlInterface.GetId());
	_mutex.Release();
	return true;
}

void CThreadPool::ExecuteTask(ThreadBase * ExecutionTarget)
{
	Thread * t;
	_mutex.Acquire();
	++_threadsRequestedSinceLastCheck;
	--_threadsEaten;

	// grab one from the pool, if we have any.
	if(m_freeThreads.size())
	{
		t = *m_freeThreads.begin();
		m_freeThreads.erase(m_freeThreads.begin());

		// execute the task on this thread.
		t->ExecutionTarget = ExecutionTarget;

		// resume the thread, and it should start working.
		t->ControlInterface.Resume();
		Log.Debug("ThreadPool", "Thread %u left the thread pool.", t->ControlInterface.GetId());
	}
	else
	{
		// creating a new thread means it heads straight to its task.
		// no need to resume it :)
		t = StartThread(ExecutionTarget);
	}

	// add the thread to the active set
#ifdef WIN32
	Log.Debug("ThreadPool", "Thread %u is now executing task at 0x%p.", t->ControlInterface.GetId(), ExecutionTarget);
#else
	Log.Debug("ThreadPool", "Thread %u is now executing task at %p.", t->ControlInterface.GetId(), ExecutionTarget);
#endif
	m_activeThreads.insert(t);
	_mutex.Release();
}

void CThreadPool::Startup()
{
	int i;
	int tcount = THREAD_RESERVE;

	for(i=0; i < tcount; ++i)
		StartThread(NULL);

	Log.Debug("ThreadPool", "Startup, launched %u threads.", tcount);
}

void CThreadPool::ShowStats()
{
	_mutex.Acquire();
	Log.Debug("ThreadPool", "============ ThreadPool Status =============");
	Log.Debug("ThreadPool", "Active Threads: %u", m_activeThreads.size());
	Log.Debug("ThreadPool", "Suspended Threads: %u", m_freeThreads.size());
	Log.Debug("ThreadPool", "Requested-To-Freed Ratio: %.3f%% (%u/%u)", float( float(_threadsRequestedSinceLastCheck+1) / float(_threadsExitedSinceLastCheck+1) * 100.0f ), _threadsRequestedSinceLastCheck, _threadsExitedSinceLastCheck);
	Log.Debug("ThreadPool", "Eaten Count: %d (negative is bad!)", _threadsEaten);
	Log.Debug("TrheadPool", "============================================");
	_mutex.Release();
}

void CThreadPool::IntegrityCheck()
{
	_mutex.Acquire();
	int32 gobbled = _threadsEaten;

    if(gobbled < 0)
	{
		// this means we requested more threads than we had in the pool last time.
        // spawn "gobbled" + THREAD_RESERVE extra threads.
		uint32 new_threads = abs(gobbled) + THREAD_RESERVE;
		_threadsEaten=0;

		for(uint32 i = 0; i < new_threads; ++i)
			StartThread(NULL);

		Log.Debug("ThreadPool", "IntegrityCheck: (gobbled < 0) Spawning %u threads.", new_threads);
	}
	else if(gobbled < THREAD_RESERVE)
	{
        // this means while we didn't run out of threads, we were getting damn low.
		// spawn enough threads to keep the reserve amount up.
		uint32 new_threads = (THREAD_RESERVE - gobbled);
		for(uint32 i = 0; i < new_threads; ++i)
			StartThread(NULL);

		Log.Debug("ThreadPool", "IntegrityCheck: (gobbled <= 5) Spawning %u threads.", new_threads);
	}
	else if(gobbled > THREAD_RESERVE)
	{
		// this means we had "excess" threads sitting around doing nothing.
		// lets kill some of them off.
		uint32 kill_count = (gobbled - THREAD_RESERVE);
		KillFreeThreads(kill_count);
		_threadsEaten -= kill_count;
		Log.Debug("ThreadPool", "IntegrityCheck: (gobbled > 5) Killing %u threads.", kill_count);
	}
	else
	{
		// perfect! we have the ideal number of free threads.
		Log.Debug("ThreadPool", "IntegrityCheck: Perfect!");
	}
	/*if(m_freeThreads.size() < 5)
	{
		uint32 j = 5 - m_freeThreads.size();
		Log.Debug("ThreadPool", "Spawning %u threads.", j);
		for(uint32 i = 0; i < j; ++i)
			StartThread(NULL);
	}*/

	_threadsExitedSinceLastCheck = 0;
	_threadsRequestedSinceLastCheck = 0;
	_threadsFreedSinceLastCheck = 0;

	_mutex.Release();
}

void CThreadPool::KillFreeThreads(uint32 count)
{
	Log.Debug("ThreadPool", "Killing %u excess threads.", count);
	_mutex.Acquire();
	Thread * t;
	ThreadSet::iterator itr;
	uint32 i;
	for(i = 0, itr = m_freeThreads.begin(); i < count && itr != m_freeThreads.end(); ++i, ++itr)
	{
		t = *itr;
		t->ExecutionTarget = NULL; 
		t->DeleteAfterExit = true;
		++_threadsToExit;
		t->ControlInterface.Resume();
	}
	_mutex.Release();
}

void CThreadPool::Shutdown()
{
	_mutex.Acquire();
	size_t tcount = m_activeThreads.size() + m_freeThreads.size();		// exit all
	Log.Debug("ThreadPool", "Shutting down %u threads.", tcount);
	KillFreeThreads((uint32)m_freeThreads.size());
	_threadsToExit += (uint32)m_activeThreads.size();

	for(ThreadSet::iterator itr = m_activeThreads.begin(); itr != m_activeThreads.end(); ++itr)
	{
		if((*itr)->ExecutionTarget)
			(*itr)->ExecutionTarget->OnShutdown();
	}
	_mutex.Release();

	for(;;)
	{
		_mutex.Acquire();
		if(m_activeThreads.size() || m_freeThreads.size())
		{
			Log.Debug("ThreadPool", "%u threads remaining...",m_activeThreads.size() + m_freeThreads.size() );
			_mutex.Release();
			Sleep(1000);
			continue;
		}

		break;
	}
}

/* this is the only platform-specific code. neat, huh! */
#ifdef WIN32

static unsigned long WINAPI thread_proc(void* param)
{
	Thread * t = (Thread*)param;
	t->SetupMutex.Acquire();
	uint32 tid = t->ControlInterface.GetId();
	bool ht = (t->ExecutionTarget != NULL);
	t->SetupMutex.Release();
	//Log.Debug("ThreadPool", "Thread %u started.", t->ControlInterface.GetId());

	for(;;)
	{
		if(t->ExecutionTarget != NULL)
		{
			if(t->ExecutionTarget->run())
				delete t->ExecutionTarget;

			t->ExecutionTarget = NULL;
		}

		if(!ThreadPool.ThreadExit(t))
		{
			Log.Debug("ThreadPool", "Thread %u exiting.", tid);
			break;
		}
		else
		{
			if(ht)
				Log.Debug("ThreadPool", "Thread %u waiting for a new task.", tid);
			// enter "suspended" state. when we return, the threadpool will either tell us to fuk off, or to execute a new task.
			t->ControlInterface.Suspend();
			// after resuming, this is where we will end up. start the loop again, check for tasks, then go back to the threadpool.
		}
	}

	// at this point the t pointer has already been freed, so we can just cleanly exit.
	ExitThread(0);
}

Thread * CThreadPool::StartThread(ThreadBase * ExecutionTarget)
{
	HANDLE h;
	Thread * t = new Thread;
	
	t->DeleteAfterExit = false;
	t->ExecutionTarget = ExecutionTarget;
	//h = (HANDLE)_beginthreadex(NULL, 0, &thread_proc, (void*)t, 0, NULL);
	t->SetupMutex.Acquire();
	h = CreateThread(NULL, 0, &thread_proc, (LPVOID)t, 0, (LPDWORD)&t->ControlInterface.thread_id);
	t->ControlInterface.Setup(h);
	t->SetupMutex.Release();

	return t;
}

#else

static void * thread_proc(void * param)
{
	Thread * t = (Thread*)param;
	t->SetupMutex.Acquire();
	Log.Debug("ThreadPool", "Thread %u started.", t->ControlInterface.GetId());
	t->SetupMutex.Release();

	for(;;)
	{
		if(t->ExecutionTarget != NULL)
		{
			if(t->ExecutionTarget->run())
				delete t->ExecutionTarget;

			t->ExecutionTarget = NULL;
		}

		if(!ThreadPool.ThreadExit(t))
			break;
		else
		{
			// enter "suspended" state. when we return, the threadpool will either tell us to fuk off, or to execute a new task.
			t->ControlInterface.Suspend();
			// after resuming, this is where we will end up. start the loop again, check for tasks, then go back to the threadpool.
		}
	}

	pthread_exit(0);
}

Thread * CThreadPool::StartThread(ThreadBase * ExecutionTarget)
{
	pthread_t target;
	Thread * t = new Thread;
	t->ExecutionTarget = ExecutionTarget;
	t->DeleteAfterExit = false;

	// lock the main mutex, to make sure id generation doesn't get messed up
	_mutex.Acquire();
	t->SetupMutex.Acquire();
	pthread_create(&target, NULL, &thread_proc, (void*)t);
	t->ControlInterface.Setup(target);
	t->SetupMutex.Release();
	_mutex.Release();
	return t;
}

#endif
