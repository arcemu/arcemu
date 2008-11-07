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

//
// MapMgr.h
//

#ifndef __MAPMGR_H
#define __MAPMGR_H

#define IS_PERSISTENT_INSTANCE(p) ( ((p)->m_mapInfo->type == INSTANCE_MULTIMODE && (p)->m_difficulty >= MODE_HEROIC) || (p)->m_mapInfo->type == INSTANCE_RAID )
#define IS_RESETABLE_INSTANCE(p) ( !(p)->m_persistent && ((p)->m_mapInfo->type == INSTANCE_NONRAID || ((p)->m_mapInfo->type == INSTANCE_MULTIMODE && (p)->m_difficulty == MODE_NORMAL)) )
#define CHECK_INSTANCE_GROUP(p,g) ( (p)->m_creatorGroup == 0 || ((g) && (p)->m_creatorGroup == (g)->GetID()) )

#define GO_GUID_RECYCLE_INTERVAL	2048	//client will cache GO positions. Using same guid for same client will make GO appear at wrong possition so we try to avoid assigning same guid

#define ZONE_MASK_ALL -1

class MapCell;
class Map;
class Object;
class MapScriptInterface;
class WorldSession;
class GameObject;
class Creature;
class Player;
class Pet;
class Transporter;
class Corpse;
class CBattleground;
class Instance;


enum MapMgrTimers
{
	MMUPDATE_OBJECTS = 0,
	MMUPDATE_SESSIONS = 1,
	MMUPDATE_FIELDS = 2,
	MMUPDATE_IDLE_OBJECTS = 3,
	MMUPDATE_ACTIVE_OBJECTS = 4,
	MMUPDATE_COUNT = 5
};

enum ObjectActiveState
{
	OBJECT_STATE_NONE	 = 0,
	OBJECT_STATE_INACTIVE = 1,
	OBJECT_STATE_ACTIVE   = 2,
};

typedef std::set<Object*> ObjectSet;
typedef std::set<Object*> UpdateQueue;
typedef std::set<Player*> PUpdateQueue;
typedef std::set<Player*> PlayerSet;
typedef HM_NAMESPACE::hash_map<uint32, Object*> StorageMap;
typedef set<uint64> CombatProgressMap;
typedef set<Creature*> CreatureSet;
typedef set<GameObject*> GameObjectSet;
typedef HM_NAMESPACE::hash_map<uint32, Creature*> CreatureSqlIdMap;
typedef HM_NAMESPACE::hash_map<uint32, GameObject*> GameObjectSqlIdMap;

#define MAX_TRANSPORTERS_PER_MAP 25

class Transporter;
#define RESERVE_EXPAND_SIZE 1024

class SERVER_DECL MapMgr : public CellHandler <MapCell>, public EventableObject,public CThread
{
	friend class UpdateObjectThread;
	friend class ObjectUpdaterThread;
	friend class MapCell;
	friend class MapScriptInterface;
public:
		
	//This will be done in regular way soon
	std::set< MapCell * > m_forcedcells;

	void AddForcedCell( MapCell * c );
	void RemoveForcedCell( MapCell * c );

	Mutex m_objectinsertlock;
	ObjectSet m_objectinsertpool;
	void AddObject(Object *);

////////////////////////////////////////////////////////
// Local (mapmgr) storage/generation of GameObjects
/////////////////////////////////////////////
	uint32 m_GOArraySize;
	uint32 m_GOHighGuid;
	GameObject ** m_GOStorage;
	GameObject * CreateGameObject(uint32 entry);

	ARCEMU_INLINE uint32 GenerateGameobjectGuid() { return ++m_GOHighGuid; }
	ARCEMU_INLINE GameObject * GetGameObject(uint32 guid)
	{
		if(guid > m_GOHighGuid)
			return 0;
		return m_GOStorage[guid];
	}

/////////////////////////////////////////////////////////
// Local (mapmgr) storage/generation of Creatures
/////////////////////////////////////////////
	uint32 m_CreatureArraySize;
	uint32 m_CreatureHighGuid;
	Creature ** m_CreatureStorage;
	Creature * CreateCreature(uint32 entry);

	__inline Creature * GetCreature(uint32 guid)
	{
		if(guid > m_CreatureHighGuid)
			return NULL;
		return m_CreatureStorage[guid];
	}

//////////////////////////////////////////////////////////
// Local (mapmgr) storage/generation of DynamicObjects
////////////////////////////////////////////
	uint32 m_DynamicObjectHighGuid;
	typedef HM_NAMESPACE::hash_map<uint32, DynamicObject*> DynamicObjectStorageMap;
	DynamicObjectStorageMap m_DynamicObjectStorage;
	DynamicObject * CreateDynamicObject();
	
	ARCEMU_INLINE DynamicObject * GetDynamicObject(uint32 guid)
	{
		DynamicObjectStorageMap::iterator itr = m_DynamicObjectStorage.find(guid);
		return (itr != m_DynamicObjectStorage.end()) ? itr->second : 0;
	}

//////////////////////////////////////////////////////////
// Local (mapmgr) storage of pets
///////////////////////////////////////////
	typedef HM_NAMESPACE::hash_map<uint32, Pet*> PetStorageMap;
	PetStorageMap m_PetStorage;
	__inline Pet * GetPet(uint32 guid)
	{
		PetStorageMap::iterator itr = m_PetStorage.find(guid);
		return (itr != m_PetStorage.end()) ? itr->second : 0;
	}

//////////////////////////////////////////////////////////
// Local (mapmgr) storage of players for faster lookup
////////////////////////////////
    
    // double typedef lolz// a compile breaker..
	typedef HM_NAMESPACE::hash_map<uint32, Player*>                     PlayerStorageMap;
	PlayerStorageMap m_PlayerStorage;
	__inline Player * GetPlayer(uint32 guid)
	{
		PlayerStorageMap::iterator itr = m_PlayerStorage.find(guid);
		return (itr != m_PlayerStorage.end()) ? itr->second : 0;
	}

//////////////////////////////////////////////////////////
// Local (mapmgr) storage of combats in progress
////////////////////////////////
	CombatProgressMap _combatProgress;
	void AddCombatInProgress(uint64 guid)
	{
		_combatProgress.insert(guid);
	}
	void RemoveCombatInProgress(uint64 guid)
	{
		_combatProgress.erase(guid);
	}

//////////////////////////////////////////////////////////
// Lookup Wrappers
///////////////////////////////////
	Unit * GetUnit(const uint64 & guid);
	Object * _GetObject(const uint64 & guid);

	bool run();
	bool Do();

	MapMgr(Map *map, uint32 mapid, uint32 instanceid);
	~MapMgr();

	void PushObject(Object *obj);
	void PushStaticObject(Object * obj);
	void RemoveObject(Object *obj, bool free_guid);
	void ChangeObjectLocation(Object *obj); // update inrange lists
	void ChangeFarsightLocation(Player *plr, DynamicObject *farsight);

	//! Mark object as updated
	void ObjectUpdated(Object *obj);
	void UpdateCellActivity(uint32 x, uint32 y, int radius);

	// Terrain Functions
	ARCEMU_INLINE float  GetLandHeight(float x, float y) { return GetBaseMap()->GetLandHeight(x, y); }
	ARCEMU_INLINE bool   IsUnderground(float x, float y,float z) { return GetBaseMap()->GetLandHeight(x, y) > (z+0.5f); }
	ARCEMU_INLINE float  GetWaterHeight(float x, float y) { return GetBaseMap()->GetWaterHeight(x, y); }
	ARCEMU_INLINE uint8  GetWaterType(float x, float y) { return GetBaseMap()->GetWaterType(x, y); }
	ARCEMU_INLINE uint8  GetWalkableState(float x, float y) { return GetBaseMap()->GetWalkableState(x, y); }
	ARCEMU_INLINE uint16 GetAreaID(float x, float y) { return GetBaseMap()->GetAreaID(x, y); }

	ARCEMU_INLINE uint32 GetMapId() { return _mapId; }

	void PushToProcessed(Player* plr);

	ARCEMU_INLINE bool HasPlayers() { return (m_PlayerStorage.size() > 0); }
	ARCEMU_INLINE bool IsCombatInProgress() { return (_combatProgress.size() > 0); }
	void TeleportPlayers();

	ARCEMU_INLINE uint32 GetInstanceID() { return m_instanceID; }
	ARCEMU_INLINE MapInfo *GetMapInfo() { return pMapInfo; }

	bool _shutdown;

	ARCEMU_INLINE MapScriptInterface * GetInterface() { return ScriptInterface; }
	virtual int32 event_GetInstanceID() { return m_instanceID; }

	void LoadAllCells();
	ARCEMU_INLINE size_t GetPlayerCount() { return m_PlayerStorage.size(); }
	uint32 GetTeamPlayersCount(uint32 teamId);

	void _PerformObjectDuties();
	uint32 mLoopCounter;
	uint32 lastGameobjectUpdate;
	uint32 lastUnitUpdate;
	void EventCorpseDespawn(uint64 guid);

	time_t InactiveMoveTime;
    uint32 iInstanceMode;

	void UnloadCell(uint32 x,uint32 y);
	void EventRespawnCreature(Creature * c, MapCell * p);
	void EventRespawnGameObject(GameObject * o, MapCell * c);
	void SendMessageToCellPlayers(Object * obj, WorldPacket * packet, uint32 cell_radius = 2);
	void SendChatMessageToCellPlayers(Object * obj, WorldPacket * packet, uint32 cell_radius, uint32 langpos, int32 lang, WorldSession * originator);
	void SendPvPCaptureMessage(int32 ZoneMask, uint32 ZoneId, const char * Message, ...);

	Instance * pInstance;
	void BeginInstanceExpireCountdown();
	void HookOnAreaTrigger(Player * plr, uint32 id);

	ARCEMU_INLINE void SetWorldState(uint32 state, uint32 value);
	ARCEMU_INLINE uint32 GetWorldState(uint32 state);
	
	// better hope to clear any references to us when calling this :P
	void InstanceShutdown()
	{
		pInstance = NULL;
		SetThreadState(THREADSTATE_TERMINATE);
	}

	// kill the worker thread only
	void KillThread()
	{
		pInstance=NULL;
		thread_kill_only = true;
		SetThreadState(THREADSTATE_TERMINATE);
		while(thread_running)
		{
			Sleep(100);
		}
	}

	float GetFirstZWithCPZ(float x,float y ,float z);

//#ifdef FORCED_SERVER_KEEPALIVE
	void	KillThreadWithCleanup();
	void	TeleportCorruptedPlayers();	//we have to be prepared something is corrupted here
//#endif

protected:

	//! Collect and send updates to clients
	void _UpdateObjects();

private:
	//! Objects that exist on map
 
	uint32 _mapId;
	set<Object*> _mapWideStaticObjects;

	std::map<uint32,uint32> _worldStateSet;

	bool _CellActive(uint32 x, uint32 y);
	void UpdateInRangeSet(Object *obj, Player *plObj, MapCell* cell, ByteBuffer ** buf);

	WorldPacket* BuildInitialWorldState();

public:
	// Distance a Player can "see" other objects and receive updates from them (!! ALREADY dist*dist !!)
	float m_UpdateDistance;

private:
	/* Update System */
	FastMutex m_updateMutex;		// use a user-mode mutex for extra speed
	UpdateQueue _updates;
	PUpdateQueue _processQueue;

	/* Sessions */
	
	SessionSet Sessions;

	/* Map Information */
	MapInfo *pMapInfo;
	uint32 m_instanceID;

	MapScriptInterface * ScriptInterface;

public:
#ifdef WIN32
	DWORD threadid;
#endif

	GameObjectSet activeGameObjects;
	CreatureSet activeCreatures;
	EventableObjectHolder eventHolder;
	CBattleground * m_battleground;
	set<Corpse*> m_corpses;
	CreatureSqlIdMap _sqlids_creatures;
	GameObjectSqlIdMap _sqlids_gameobjects;

	Creature * GetSqlIdCreature(uint32 sqlid);
	GameObject * GetSqlIdGameObject(uint32 sqlid);
	deque<uint32> _reusable_guids_gameobject;
	deque<uint32> _reusable_guids_creature;

	bool forced_expire;
	bool thread_kill_only;
	bool thread_running;
};

#endif
