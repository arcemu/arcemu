/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#ifndef _DAYWATCHERTHREAD_
#define _DAYWATCHERTHREAD_

enum DAYWATCHERSETTINGS
{
	WEEKLY		= 1,
	DAILY		= 2,
	MONTHLY		= 3,
	HOURLY		= 4,
};

class DayWatcherThread : public CThread
{
	bool m_running;
	bool m_busy;
	bool m_dirty;

	static const time_t timeout = 120;		/* check every 2 minutes */
	time_t currenttime;
	tm local_currenttime;
	time_t last_arena_time;
	tm local_last_arena_time;

	time_t last_daily_time;
	tm local_last_daily_time;

	uint32 arena_period;
	uint32 daily_period;

public:
	DayWatcherThread();
	~DayWatcherThread();

	bool run();
	void terminate();
	void maintenence();
	void dupe_tm_pointer(tm * returnvalue, tm * mypointer);
	void load_settings();
	void update_settings();
	void set_tm_pointers();
	uint32 get_timeout_from_string(const char * string, uint32 def);
	bool has_timeout_expired(tm * now_time, tm * last_time, uint32 timeoutval);
	void update_arena();
	void update_daily();
};

#endif
