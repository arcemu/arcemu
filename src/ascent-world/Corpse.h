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

#ifndef WOWSERVER_CORPSE_H
#define WOWSERVER_CORPSE_H

enum CORPSE_STATE
{
	CORPSE_STATE_BODY = 0,
	CORPSE_STATE_BONES = 1,
};

struct CorpseData
{
	uint32 LowGuid;
	uint32 mapid;
	uint64 owner;
	uint32 instancemapid;
	float x;
	float y;
	float z;
	void DeleteFromDB();
};

#define CORPSE_RECLAIM_TIME 30
#define CORPSE_RECLAIM_TIME_MS CORPSE_RECLAIM_TIME * 1000
#define CORPSE_MINIMUM_RECLAIM_RADIUS 39 
#define CORPSE_MINIMUM_RECLAIM_RADIUS_SQ CORPSE_MINIMUM_RECLAIM_RADIUS * CORPSE_MINIMUM_RECLAIM_RADIUS

class SERVER_DECL Corpse : public Object
{
public:
	Corpse( uint32 high, uint32 low );
	~Corpse();

   // void Create();
	void Create (Player *owner, uint32 mapid, float x, float y, float z, float ang );

	void SaveToDB();
	void DeleteFromDB();
	ASCENT_INLINE void SetCorpseState(uint32 state) { m_state = state; }
	ASCENT_INLINE uint32 GetCorpseState() { return m_state; }
	void Despawn();
	
	ASCENT_INLINE void SetLoadedFromDB(bool value) { _loadedfromdb = value; }
	ASCENT_INLINE bool GetLoadedFromDB(void) { return _loadedfromdb; }
	Loot loot;
	void generateLoot();

	void SpawnBones();
	void Delink();

	void ResetDeathClock(){ m_time = time( NULL ); }
	time_t GetDeathClock(){ return m_time; }

private:
	uint32 m_state;
	time_t m_time;
	uint32 _fields[CORPSE_END];
	bool _loadedfromdb;
};

#endif

