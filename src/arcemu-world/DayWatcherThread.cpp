/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

/* Arena and Honor Point Calculation System
 *    Copyright (c) 2007 Burlex
 */

#include "StdAfx.h"

#define THREAD_LOOP_INTERVAL 120 // seconds

#ifdef WIN32
static HANDLE m_abortEvent = INVALID_HANDLE_VALUE;
#else
static pthread_cond_t abortcond;
static pthread_mutex_t abortmutex;
#endif

DayWatcherThread::DayWatcherThread()
{
	m_running = true;
	m_dirty = false;
}

DayWatcherThread::~DayWatcherThread()
{

}

void DayWatcherThread::terminate()
{
	m_running = false;
#ifdef WIN32
	SetEvent(m_abortEvent);
#else
	pthread_cond_signal(&abortcond);
#endif
}

void DayWatcherThread::dupe_tm_pointer(tm * returnvalue, tm * mypointer)
{
	memcpy(mypointer, returnvalue, sizeof(tm));
}

void DayWatcherThread::update_settings()
{
	CharacterDatabase.Execute("REPLACE INTO server_settings VALUES(\"last_arena_update_time\", %u)", last_arena_time);
	CharacterDatabase.Execute("REPLACE INTO server_settings VALUES(\"last_daily_update_time\", %u)", last_daily_time);
}

void DayWatcherThread::load_settings()
{
	string arena_timeout = Config.MainConfig.GetStringDefault("Periods", "ArenaUpdate", "weekly");
	arena_period = get_timeout_from_string(arena_timeout.c_str(), WEEKLY);

	QueryResult * result = CharacterDatabase.Query("SELECT setting_value FROM server_settings WHERE setting_id = \"last_arena_update_time\"");
	if(result)
	{
		last_arena_time = result->Fetch()[0].GetUInt32();
		delete result;
	}
	else
	{
		Log.Notice("DayWatcherThread", "Initializing Arena Updates to zero.");
		last_arena_time = 0;
	}

	string daily_timeout = Config.MainConfig.GetStringDefault("Periods", "DailyUpdate", "daily");
	daily_period = get_timeout_from_string(daily_timeout.c_str(), DAILY);

	QueryResult * result2 = CharacterDatabase.Query("SELECT setting_value FROM server_settings WHERE setting_id = \"last_daily_update_time\"");
	if(result2)
	{
		last_daily_time = result2->Fetch()[0].GetUInt32();
		delete result2;
	}
	else
	{
		Log.Notice("DayWatcherThread", "Initializing Daily Updates to zero.");
		last_daily_time = 0;
	}
}

void DayWatcherThread::set_tm_pointers()
{
	dupe_tm_pointer(localtime(&last_arena_time), &local_last_arena_time);
	dupe_tm_pointer(localtime(&last_daily_time), &local_last_daily_time);
}

uint32 DayWatcherThread::get_timeout_from_string(const char * string, uint32 def)
{
	if(!stricmp(string, "weekly"))
		return WEEKLY;
	else if(!stricmp(string, "monthly"))
		return MONTHLY;
	else if(!stricmp(string, "daily"))
		return DAILY;
	else if(!stricmp(string, "hourly"))
		return HOURLY;
	else
		return def;
}

bool DayWatcherThread::has_timeout_expired(tm * now_time, tm * last_time, uint32 timeoutval)
{
	switch(timeoutval)
	{
	case WEEKLY:
		{
			return ( abs( now_time->tm_yday - last_time->tm_yday ) >= 7 );
		}
		
	case MONTHLY:
		return (now_time->tm_mon != last_time->tm_mon);

	case HOURLY:
		return ((now_time->tm_hour != last_time->tm_hour) || (now_time->tm_mday != last_time->tm_mday) || (now_time->tm_mon != last_time->tm_mon));

	case DAILY:
		return (now_time->tm_mday != last_time->tm_mday);
	}
	return false;
}

bool DayWatcherThread::run()
{
	Log.Notice("DayWatcherThread", "Started.");
	currenttime = UNIXTIME;
	dupe_tm_pointer(localtime(&currenttime), &local_currenttime);
	load_settings();
	set_tm_pointers();
	m_busy = false;
#ifdef WIN32
	m_abortEvent = CreateEvent(NULL, NULL, FALSE, NULL);
#else
	struct timeval now;
	struct timespec tv;

	pthread_mutex_init(&abortmutex,NULL);
	pthread_cond_init(&abortcond,NULL);
#endif
	
	while(ThreadState != THREADSTATE_TERMINATE)
	{
		m_busy=true;
		currenttime = UNIXTIME;
		dupe_tm_pointer(localtime(&currenttime), &local_currenttime);

		if(has_timeout_expired(&local_currenttime, &local_last_arena_time, arena_period))
			update_arena();

		if(has_timeout_expired(&local_currenttime, &local_last_daily_time, daily_period))
			update_daily();
        
		if(m_dirty)
			update_settings();

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

void DayWatcherThread::update_daily()
{
	Log.Notice("DayWatcherThread", "Running Daily Quest Reset...");
	CharacterDatabase.WaitExecute("UPDATE characters SET finisheddailies = ''");
	objmgr.ResetDailies();
	last_daily_time = UNIXTIME;
	dupe_tm_pointer(localtime(&last_daily_time), &local_last_daily_time);
	m_dirty = true;
}

void DayWatcherThread::update_arena()
{
	Log.Notice("DayWatcherThread", "Running Weekly Arena Point Maintenance...");
	QueryResult * result = CharacterDatabase.Query("SELECT guid, arenaPoints FROM characters");		/* this one is a little more intensive. */
	Player * plr;
	uint32 guid, arenapoints, orig_arenapoints;
	ArenaTeam * team;
	uint32 arenapointsPerTeam[3] = {0};
	double X, Y;
	if(result)
	{
		do
		{
			Field * f = result->Fetch();
			guid = f[0].GetUInt32();
			arenapoints = f[1].GetUInt32();
			orig_arenapoints = arenapoints;

			for(uint32 i = 0; i < 3; ++i)
				arenapointsPerTeam[i] = 0;

			/* are we in any arena teams? */
			for(uint32 i = 0; i < 3; ++i)			// 3 arena team types
			{
				team = objmgr.GetArenaTeamByGuid(guid, i);
				if(team)
				{
					ArenaTeamMember *member = team->GetMemberByGuid(guid);
					if(member == NULL || team->m_stat_gamesplayedweek < 10 || ((member->Played_ThisWeek * 100) / team->m_stat_gamesplayedweek < 30))
 						continue;

					/* we're in an arena team of this type! */
					/* Source: http://www.wowwiki.com/Arena_point */
					X = (double)team->m_stat_rating;
					if(X <= 510.0)	// "if X<=510"
						continue;		// no change
					else if(X > 510.0 && X <= 1500.0)		// "if 510 < X <= 1500"
					{
						Y = (0.22 * X) + 14.0;
					}
					else			// "if X > 1500"
					{
						// http://eu.wowarmory.com/arena-calculator.xml
						//              1511.26
						//   ---------------------------
						//                   -0.00412*X
						//    1+1639.28*2.71828

						double power = ((-0.00412) * X);
						//if(power < 1.0)
						//	power = 1.0;

						double divisor = pow(((double)(2.71828)), power);						
						divisor *= 1639.28;
						divisor += 1.0;
						//if(divisor < 1.0)
						//	divisor = 1.0;

						Y = 1511.26 / divisor;
					}

					// 2v2 teams only earn 70% (Was 60% until 13th March 07) of the arena points, 3v3 teams get 80%, while 5v5 teams get 100% of the arena points.
					// 2v2 - 76%, 3v3 - 88% as of patch 2.2
					if(team->m_type == ARENA_TEAM_TYPE_2V2)
					{
						Y *= 0.76;
						Y *= sWorld.getRate(RATE_ARENAPOINTMULTIPLIER2X);
					}
					else if(team->m_type == ARENA_TEAM_TYPE_3V3)
					{
						Y *= 0.88;
						Y *= sWorld.getRate(RATE_ARENAPOINTMULTIPLIER3X);
					}
					else
					{
						Y *= sWorld.getRate(RATE_ARENAPOINTMULTIPLIER5X);
					}
					
					if(Y > 1.0)
						arenapointsPerTeam[i] += long2int32(double(ceil(Y)));
				}
			}

			arenapointsPerTeam[0] = (uint32)max(arenapointsPerTeam[0],arenapointsPerTeam[1]);
			arenapoints += (uint32)max(arenapointsPerTeam[0],arenapointsPerTeam[2]);
			if (arenapoints > 5000) arenapoints = 5000;

			if(orig_arenapoints != arenapoints)
			{
				plr = objmgr.GetPlayer(guid);
				if(plr)
				{
					plr->m_arenaPoints = arenapoints;
					
					/* update visible fields (must be done through an event because of no uint lock */
					sEventMgr.AddEvent(plr, &Player::RecalculateHonor, EVENT_PLAYER_UPDATE, 100, 1, 0);
	
					/* send a little message :> */
					sChatHandler.SystemMessage(plr->GetSession(), "Your arena points have been updated! Check your PvP tab!");
				}

				/* update in sql */
				CharacterDatabase.Execute("UPDATE characters SET arenaPoints = %u WHERE guid = %u", arenapoints, guid);
			}
		}while(result->NextRow());
		delete result;
	}

	objmgr.UpdateArenaTeamWeekly();

	//===========================================================================
	last_arena_time = UNIXTIME;
	dupe_tm_pointer(localtime(&last_arena_time), &local_last_arena_time);
	m_dirty = true;
}

