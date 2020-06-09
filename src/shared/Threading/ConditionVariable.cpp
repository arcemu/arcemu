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

#include "System/Platform.h"

namespace Arcemu
{

	namespace Threading
	{

		ConditionVariable::ConditionVariable()
		{

#ifdef WIN32
			hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
#else
			pthread_mutex_init(&mutex, NULL);
			pthread_cond_init(&cond, NULL);
#endif
		}

		ConditionVariable::~ConditionVariable()
		{
#ifdef WIN32
			CloseHandle(hEvent);
#else
			pthread_cond_destroy(&cond);
			pthread_mutex_destroy(&mutex);
#endif
		}

		void ConditionVariable::Signal()
		{
#ifdef WIN32
			SetEvent(hEvent);
#else
			pthread_cond_signal(&cond);
#endif
		}

		void ConditionVariable::Wait(unsigned long timems)
		{
#ifdef WIN32
			WaitForSingleObject(hEvent, timems);
#else
			unsigned long times = timems / 1000;
			timems = timems - times * 1000;

			timeval now;
			timespec tv;

			gettimeofday(&now, NULL);

			tv.tv_sec = now.tv_sec;
			tv.tv_nsec = now.tv_usec * 1000;
			tv.tv_sec += times;
			tv.tv_nsec += (timems * 1000 * 1000);

			pthread_mutex_lock(&mutex);
			pthread_cond_timedwait(&cond, &mutex, &tv);
			pthread_mutex_unlock(&mutex);

#endif
		}

	}
}
