/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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

#ifndef CONDITIONVARIABLE_H
#define CONDITIONVARIABLE_H

namespace Arcemu
{

	namespace Threading
	{

		/////////////////////////////////////////////////////////////////
		//class ConditionVariable
		//  Class implementing a platform independent condition variable
		//
		//
		/////////////////////////////////////////////////////////////////
		class ConditionVariable
		{

			public:
				ConditionVariable();

				~ConditionVariable();

				//////////////////////////////////////////////////////////////////////////
				//void Signal()
				//  Signals the condition variable, allowing the blocked thread to proceed
				//
				//Parameter(s)
				//  None
				//
				//Return Value
				//  None
				//
				//
				//////////////////////////////////////////////////////////////////////////
				void Signal();


				/////////////////////////////////////////////////////////////////////////
				//void Wait( unsigned long timems )
				//  Blocks execution of the calling thread until signaled or
				//  until the timer runs out.
				//
				//Parameter(s)
				//  unsigned long timems  -  Maximum time to block in milliseconds
				//
				//Return Value(s)
				//  None
				//
				//
				/////////////////////////////////////////////////////////////////////////
				void Wait(unsigned long timems);

			private:

#ifdef WIN32
				HANDLE hEvent;
#else
				pthread_cond_t cond;
				pthread_mutex_t mutex;
#endif
		};
	}
}

#endif
