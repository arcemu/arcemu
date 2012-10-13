/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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
	BCTimerCount = 0;
	m_running = false;

	cond.Signal();
}

bool CommonScheduleThread::run()
{
	Log.Success("CommonScheduleThread", "Started.");
	m_busy = false;

	if(sWorld.BCSystemEnable && sWorld.BCOrderMode == 1)
		itOrderMSGEntry = objmgr.GetBCTotalItemBegin();
	// cebernic nothing in storage
	if(objmgr.IsBCEntryStorageEmpty()) sWorld.BCSystemEnable = 0;

	BCTimerCount = getMSTime() + ((uint32)sWorld.BCInterval * 1000);

	while(GetThreadState() != THREADSTATE_TERMINATE)
	{
		m_busy = true;
		// do job -------------------
		BroadCastExec();
		// -----------------------
		m_busy = false;
		if(GetThreadState() == THREADSTATE_TERMINATE)
			break;

		cond.Wait(THREAD_LOOP_INTERVAL * 1000);

		if(!m_running)
			break;
	}

	return true;
}
/*
cebernic: AutoBroadCast System
*/
void CommonScheduleThread::BroadCastExec()
{
	if(!sWorld.BCSystemEnable) return;

	if((uint32)sWorld.BCInterval > THREAD_LOOP_INTERVAL)
	{
		if(getMSTime() <= BCTimerCount)
		{
			return;
		}
		else	BCTimerCount = getMSTime() + ((uint32)sWorld.BCInterval * 1000);
	}

	switch(sWorld.BCOrderMode)
	{
		case 0:
			{
				int entry =	objmgr.CalcCurrentBCEntry();
				if(entry < 0)
				{
					sWorld.BCSystemEnable = false;
					Log.Notice("BCSystem", "table worldbroadcast loads failed,so BCSystem disabled already.");
					return;
				}

				if(entry == 0) return;    // no anymessagez hitted.
				else sWorld.SendBCMessageByID(entry);
				//printf("random entry: %u\n",entry);
			}
			break;
		case 1:
			{
				// re-assign
				if(itOrderMSGEntry ==  objmgr.GetBCTotalItemEnd()) itOrderMSGEntry = objmgr.GetBCTotalItemBegin();
				sWorld.SendBCMessageByID((uint32)itOrderMSGEntry->second);
				//printf("serial entry: %u\n",(uint32)itOrderMSGEntry->second);
				itOrderMSGEntry++;
			}
			break;
		default:
			return;
	}
}
