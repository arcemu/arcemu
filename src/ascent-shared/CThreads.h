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

// Class CThread - Base class for all threads in the
// server, and allows for easy management by ThreadMgr.

#ifndef _MAPUPDATER_H
#define _MAPUPDATER_H

class MapMgr;
class Object;
class Player;
class WorldSession;
class Creature;
class GameObject;

#define MAPMGR_UPDATEOBJECT_LOOP_DELAY 100
#define MAPMGR_SESSION_UPDATE_DELAY	50

#define MAPMGR_UPDATE_DELAY			100

enum CThreadState
{
	THREADSTATE_TERMINATE = 0,
	THREADSTATE_PAUSED	= 1,
	THREADSTATE_SLEEPING  = 2,
	THREADSTATE_BUSY	  = 3,
	THREADSTATE_AWAITING  = 4,
};


struct NameTableEntry;

class SERVER_DECL CThread : public ThreadBase
{
public:
	CThread();
	~CThread();

	ASCENT_INLINE void SetThreadState(CThreadState thread_state) { ThreadState = thread_state; }
	ASCENT_INLINE CThreadState GetThreadState() { return ThreadState; }
	int GetThreadId() { return ThreadId; }
	time_t GetStartTime() { return start_time; }
	virtual bool run();
	virtual void OnShutdown();

protected:
	CThreadState ThreadState;
	time_t start_time;
	int ThreadId;
};

#endif
