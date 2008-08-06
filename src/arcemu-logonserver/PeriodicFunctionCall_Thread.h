/*
 * OpenAscent MMORPG Server
 * Copyright (C) 2008 <http://www.openascent.com/>
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

#include "../arcemu-shared/CallBack.h"

#ifndef WIN32
static pthread_cond_t abortcond;
static pthread_mutex_t abortmutex;
#endif

template<class Type>
class PeriodicFunctionCaller : public ThreadBase
{
public:
	template<class T>
		PeriodicFunctionCaller(T * callback, void (T::*method)(), uint32 Interval)
	{
		cb = new CallbackP0<T>(callback, method);
		interval = Interval;
		running = true;
	}

	~PeriodicFunctionCaller()
	{
		delete cb;
	}

	bool run()
	{
#ifndef WIN32
		struct timeval now;
		struct timespec tv;
		uint32 next = getMSTime() + interval;

		pthread_mutex_init( &abortmutex, NULL );
		pthread_cond_init( &abortcond, NULL );

		while ( running && mrunning )
		{
			if ( getMSTime() > next )
			{
				cb->execute();
				next = getMSTime() + interval;
			}
			gettimeofday( &now, NULL );
			tv.tv_sec = now.tv_sec + 120;
			tv.tv_nsec = now.tv_usec * 1000;
			pthread_mutex_lock( &abortmutex );
			pthread_cond_timedwait( &abortcond, &abortmutex, &tv );
			pthread_mutex_unlock( &abortmutex );
		}
#else
		thread_active = true;
		hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
		for ( ;; )
		{
			if (hEvent)
				WaitForSingleObject( hEvent, interval );

			if ( !running )
				break;	/* we got killed */

			/* times up */
			if (hEvent)
				ResetEvent( hEvent );
			cb->execute();
		}
		thread_active = false;
#endif
		return false;
	}

	void kill()
	{
		running = false;
#ifdef WIN32
		/* push the event */
		SetEvent( hEvent );
		printf( "Waiting for PFC thread to exit..." );
		/* wait for the thread to exit */
		while( thread_active )
		{
			Sleep( 100 );
		}
		printf( " done.\n" );
#else
		pthread_cond_signal( &abortcond );
#endif
	}

private:
	CallbackBase * cb;
	uint32 interval;
	bool running;
#ifdef WIN32
	bool thread_active;
	HANDLE hEvent;
#endif
};

#define SpawnPeriodicCallThread(otype, ptr, method, interval) \
	launch_thread(new PeriodicFunctionCaller<otype>(ptr, method, interval));
