/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
 * cebernic@gmail.com
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

#include "StdAfx.h"

#define THREAD_LOOP_INTERVAL 10 // cebernic: don't modify it

#ifdef WIN32
static HANDLE m_abortEvent = INVALID_HANDLE_VALUE;
#else
static pthread_cond_t abortcond;
static pthread_mutex_t abortmutex;
#endif

CommonScheduleThread::CommonScheduleThread()
{
	m_running = true;
	BCTimerCount = 0;
}

CommonScheduleThread::~CommonScheduleThread()
{

}

void CommonScheduleThread::terminate()
{
	m_running = false;
#ifdef WIN32
	SetEvent(m_abortEvent);
#else
	pthread_cond_signal(&abortcond);
#endif

	BCTimerCount = 0;
}

bool CommonScheduleThread::run()
{
	Log.Notice("CommonScheduleThread", "Started.");
	m_busy = false;

	if ( sWorld.BCSystemEnable && sWorld.BCOrderMode==1 )
		itOrderMSGEntry = objmgr.GetBCTotalItemBegin();
	// cebernic nothing in storage
	if ( objmgr.IsBCEntryStorageEmpty() ) sWorld.BCSystemEnable = 0;

#ifdef WIN32
	m_abortEvent = CreateEvent(NULL, NULL, FALSE, NULL);
#else
	struct timeval now;
	struct timespec tv;

	pthread_mutex_init(&abortmutex,NULL);
	pthread_cond_init(&abortcond,NULL);
#endif

	BCTimerCount = getMSTime() + ((uint32)sWorld.BCInterval*1000);

	while(ThreadState != THREADSTATE_TERMINATE)
	{
		m_busy=true;
		// do job -------------------
		BroadCastExec();
		// -----------------------
		m_busy=false;
		if(ThreadState == THREADSTATE_TERMINATE)
			break;

#ifdef WIN32
		if (m_abortEvent)
			WaitForSingleObject(m_abortEvent, THREAD_LOOP_INTERVAL*1000);
#else
		gettimeofday(&now, NULL);
		tv.tv_sec = now.tv_sec + THREAD_LOOP_INTERVAL;
		tv.tv_nsec = now.tv_usec * 1000;
		pthread_mutex_lock(&abortmutex);
		pthread_cond_timedwait(&abortcond, &abortmutex, &tv);
		pthread_mutex_unlock(&abortmutex);
#endif
		if(!m_running)
			break;
	}
#ifdef WIN32
	if (m_abortEvent)
		CloseHandle(m_abortEvent);		
#else
	pthread_mutex_destroy(&abortmutex);
	pthread_cond_destroy(&abortcond);
#endif
	return true;
}
/*
cebernic: AutoBroadCast System
*/
void CommonScheduleThread::BroadCastExec()
{
	if ( !sWorld.BCSystemEnable ) return;

	if ( (uint32)sWorld.BCInterval > THREAD_LOOP_INTERVAL )
	{
		if ( getMSTime() <= BCTimerCount ) {
			return;
		}else	BCTimerCount = getMSTime() + ((uint32)sWorld.BCInterval*1000);
	}

	switch ( sWorld.BCOrderMode )
	{
		case 0:
		{
			int entry =	objmgr.CalcCurrentBCEntry();
			if ( entry < 0 ) {
				sWorld.BCSystemEnable = false;
				Log.Notice("BCSystem", "table worldbroadcast loads failed,so BCSystem disabled already.");
				return;
			}

			if ( entry == 0 ) return; // no anymessagez hitted.
			else sWorld.SendBCMessageByID(entry);
				//printf("random entry: %u\n",entry);
		}break;
		case 1:
		{
			// re-assign
			if ( itOrderMSGEntry ==  objmgr.GetBCTotalItemEnd() ) itOrderMSGEntry = objmgr.GetBCTotalItemBegin();
				sWorld.SendBCMessageByID((uint32)itOrderMSGEntry->second);
			//printf("serial entry: %u\n",(uint32)itOrderMSGEntry->second);
			itOrderMSGEntry++;
		}break;
		default:
			return;
	}
}
