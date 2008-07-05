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

//
// WorldCreator.h
//

#ifndef __WORLDCREATOR_H
#define __WORLDCREATOR_H

enum INSTANCE_TYPE
{
	INSTANCE_NULL,
	INSTANCE_RAID,
	INSTANCE_NONRAID,
	INSTANCE_PVP,
	INSTANCE_MULTIMODE,
};

enum INSTANCE_MODE
{
    MODE_NORMAL = 0,
    MODE_HEROIC = 1,
    MODE_EPIC   = 2,
};

enum INSTANCE_ABORT_ERROR
{
	INSTANCE_ABORT_ERROR_ERROR	 = 0x00,
	INSTANCE_ABORT_FULL	  = 0x01,
	INSTANCE_ABORT_NOT_FOUND = 0x02,
	INSTANCE_ABORT_TOO_MANY  = 0x03,
	INSTANCE_ABORT_ENCOUNTER = 0x04,
	INSTANCE_ABORT_NON_CLIENT_TYPE = 0x05,
	INSTANCE_ABORT_HEROIC_MODE_NOT_AVAILABLE = 0x06,
	INSTANCE_ABORT_NOT_IN_RAID_GROUP = 0x07,

	INSTANCE_OK = 0x10,
};

extern const char * InstanceAbortMessages[];

class Map;
class MapMgr;

class Object;
class Group;
class Player;
class MapUpdaterThread;
class Battleground;

class SERVER_DECL FormationMgr : public Singleton < FormationMgr >
{
	map<uint32, Formation*> m_formations;
public:
	typedef std::map<uint32, Formation*> FormationMap;
    FormationMgr();
	~FormationMgr();

	Formation * GetFormation(uint32 sqlid)
	{
		FormationMap::iterator itr = m_formations.find(sqlid);
		return (itr == m_formations.end()) ? 0 : itr->second;
	}
};

class Instance
{
public:
	uint32 m_instanceId;
	uint32 m_mapId;
	MapMgr * m_mapMgr;
	uint32 m_creatorGuid;
	uint32 m_creatorGroup;
	uint32 m_difficulty;
    set<uint32> m_killedNpcs;
	time_t m_creation;
	time_t m_expiration;
	MapInfo * m_mapInfo;
	bool m_isBattleground;

	void LoadFromDB(Field * fields);
	void SaveToDB();
	void DeleteFromDB();
};

#define NUM_MAPS 600
typedef HM_NAMESPACE::hash_map<uint32, Instance*> InstanceMap; 

class SERVER_DECL InstanceMgr
{
	friend class MapMgr;
public:
	InstanceMgr();	
	~InstanceMgr();

	ASCENT_INLINE Map* GetMap(uint32 mapid)
	{
		if(mapid>NUM_MAPS)
			return NULL;
		else
			return m_maps[mapid];
	}

	uint32 PreTeleport(uint32 mapid, Player * plr, uint32 instanceid);
	MapMgr * GetInstance(Object* obj);
	uint32 GenerateInstanceID();
	void BuildXMLStats(char * m_file);
	void Load(TaskList * l);

	// deletes all instances owned by this player.
	void ResetSavedInstances(Player * plr);

	// deletes all instances owned by this group
	void OnGroupDestruction(Group * pGroup);

	// player left a group, boot him out of any instances he's not supposed to be in.
	void PlayerLeftGroup(Group * pGroup, Player * pPlayer);

	// has an instance expired?
	// can a player join?
    ASCENT_INLINE bool PlayerOwnsInstance(Instance * pInstance, Player * pPlayer)
	{
		// expired?
		if( pInstance->m_expiration && (UNIXTIME+20) >= pInstance->m_expiration)
		{
			// delete the instance
			_DeleteInstance(pInstance, true);
			return false;
		}

		if( (pPlayer->GetGroup() && pInstance->m_creatorGroup == pPlayer->GetGroup()->GetID()) || pPlayer->GetLowGUID() == pInstance->m_creatorGuid )
			return true;

		return false;
	}

	// has an instance expired?
	ASCENT_INLINE bool HasInstanceExpired(Instance * pInstance)
	{
		// expired?
		if( pInstance->m_expiration && (UNIXTIME+20) >= pInstance->m_expiration)
			return true;

		return false;
	}

	// check for expired instances
	void CheckForExpiredInstances();

	// delete all instances
	void Shutdown();

	// packets, w000t! we all love packets!
	void BuildRaidSavedInstancesForPlayer(Player * plr);
	void BuildSavedInstancesForPlayer(Player * plr);
	MapMgr * CreateBattlegroundInstance(uint32 mapid);

	// this only frees the instance pointer, not the mapmgr itself
	void DeleteBattlegroundInstance(uint32 mapid, uint32 instanceid);
	MapMgr* GetMapMgr(uint32 mapId);

private:
	void _LoadInstances();
	void _CreateMap(uint32 mapid);
	MapMgr* _CreateInstance(Instance * in);
	MapMgr* _CreateInstance(uint32 mapid, uint32 instanceid);		// only used on main maps!
	bool _DeleteInstance(Instance * in, bool ForcePlayersOut);

	uint32 m_InstanceHigh;

	Mutex m_mapLock;
	Map * m_maps[NUM_MAPS];
	InstanceMap* m_instances[NUM_MAPS];
	MapMgr * m_singleMaps[NUM_MAPS];
};

extern SERVER_DECL InstanceMgr sInstanceMgr;

#endif
