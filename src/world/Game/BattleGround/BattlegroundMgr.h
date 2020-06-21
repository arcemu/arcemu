/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#ifndef __BATTLEGROUNDMGR_H
#define __BATTLEGROUNDMGR_H

#define ANTI_CHEAT

class CBattleground;
class MapMgr;
class Player;
class Map;
class Group;
// AV - Corpse
class Corpse;


enum BattlegroundDbcIndex
{
    BGDBC_ALTERAC_VALLEY			= 1,
    BGDBC_WARSONG_GULCH			= 2,
    BGDBC_ARATHI_BASIN			= 3,
    BGDBC_ARENA_NAGRAND			= 4,
    BGDBC_ARENA_BLADES_EDGE			= 5,
    BGDBC_ARENA_ALLMAPS			= 6,
    BGDBC_EYE_OF_THE_STORM			= 7,
    BGDBC_RUINS_OF_LORDAERON		= 8,
    BGDBC_STRAND_OF_THE_ANCIENT		= 9,
    BGDBC_DALARAN_SEWERS			= 10,
    BGDBC_RING_OF_VALOR			= 11,
    BGDBC_ROWS				= 11,
};

enum BattleGroundTypes
{
    BATTLEGROUND_ALTERAC_VALLEY		= 1,
    BATTLEGROUND_WARSONG_GULCH		= 2,
    BATTLEGROUND_ARATHI_BASIN		= 3,
    BATTLEGROUND_ARENA_2V2			= 4,
    BATTLEGROUND_ARENA_3V3			= 5,
    BATTLEGROUND_ARENA_5V5			= 6,
    BATTLEGROUND_EYE_OF_THE_STORM		= 7,
    BATTLEGROUND_STRAND_OF_THE_ANCIENT	= 9,
    BATTLEGROUND_ISLE_OF_CONQUEST           = 30,
    BATTLEGROUND_RANDOM                     = 32,
    BATTLEGROUND_NUM_TYPES			= 33, //Based on BattlemasterList.dbc, make the storage arrays big enough! On 3.1.3 the last one was 11 The Ring of Valor, so 12 was enough here, but on 3.2.0 there is 32 All Battlegrounds!
};

#define IS_ARENA(x) ( (x) >= BATTLEGROUND_ARENA_2V2 && (x) <= BATTLEGROUND_ARENA_5V5 )

enum BattleGroundMasterTypes
{
    BGMASTER_CREATURE		= 1,
    BGMASTER_OBJECT			= 2,
    BGMASTER_ITEM			= 3,
};

enum BattleGroundStatus
{
    BGSTATUS_NOFLAGS		= 0, // wtfbbq, why aren't there any flags?
    BGSTATUS_INQUEUE		= 1, // Battleground has a queue, player is now in queue
    BGSTATUS_READY		= 2, // Battleground is ready to join
    BGSTATUS_TIME		= 3, // Ex. Wintergrasp time remaining
};


//////////////////////////////////////////////////////////////////////////////
//struct BGMaster;
// Contains creature -> battleground id association pairs
//
//fields:
// uint32 entry  -  creature entry of the battlemaster
// uint32 bg     -  ID of the battleground the creature is a battlemaster of
//
//////////////////////////////////////////////////////////////////////////////
struct BGMaster
{
	uint32 entry;
	uint32 bg;
};

struct BGScore
{
	uint32 KillingBlows;
	uint32 HonorableKills;
	uint32 Deaths;
	uint32 BonusHonor;
	uint32 DamageDone;
	uint32 HealingDone;
	uint32 MiscData[5];

	BGScore(){
		KillingBlows = 0;
		HonorableKills = 0;
		Deaths = 0;
		BonusHonor = 0;
		DamageDone = 0;
		HealingDone = 0;
		std::fill( &MiscData[ 0 ], &MiscData[ 5 ], 0 );
	}
};

#define BG_SCORE_AB_BASES_ASSAULTED 0
#define BG_SCORE_AB_BASES_CAPTURED 1
#define BG_SCORE_AV_GRAVEYARDS_ASSAULTED 0
#define BG_SCORE_AV_GRAVEYARDS_DEFENDED 1
#define BG_SCORE_AV_TOWERS_ASSAULTED 2
#define BG_SCORE_AV_TOWERS_DEFENDED 3
#define BG_SCORE_AV_MINES_CAPTURES 4
#define BG_SCORE_EOTS_FLAGS_CAPTURED 0
#define BG_SCORE_WSG_FLAGS_CAPTURED 0
#define BG_SCORE_WSG_FLAGS_RETURNED 1
#define BG_SCORE_SOTA_DEMOLISHERS_DESTROYED 0
#define BG_SCORE_SOTA_GATES_DESTROYED 1
#define BG_SCORE_IOC_BASES_ASSAULTED 0
#define BG_SCORE_IOC_BASES_DEFENDED 1

#define SOUND_BATTLEGROUND_BEGIN			0xD6F
#define SOUND_HORDE_SCORES				8213
#define SOUND_ALLIANCE_SCORES				8173
#define SOUND_ALLIANCE_CAPTURE				8174
#define SOUND_HORDE_CAPTURE				8212
#define SOUND_ALLIANCE_RETURNED				8192
#define SOUND_HORDE_RETURNED				8192	// huh?
#define SOUND_HORDEWINS					8454
#define SOUND_ALLIANCEWINS				8455
#define SOUND_HORDE_BGALMOSTEND				0x2108
#define SOUND_ALLIANCE_BGALMOSTEND			0x2109

#define BG_PREPARATION					44521
#define BG_REVIVE_PREPARATION				44535
#define RESURRECT_SPELL					21074	// Spirit Healer Res
#define BG_DESERTER					26013

/* get level grouping for player */
static inline uint32 GetLevelGrouping(uint32 level)
{
	if(level < 10)
		return 0;
	else if(level < 20)
		return 1;
	else if(level < 30)
		return 2;
	else if(level < 40)
		return 3;
	else if(level < 50)
		return 4;
	else if(level < 60)
		return 5;
	else if(level < 70)
		return 6;
	else if(level < 80)
		return 7;
	else
		return 8;
}

static inline uint32 GetFieldCount(uint32 BGType)
{
	switch(BGType)
	{
		case BATTLEGROUND_ALTERAC_VALLEY:
			return 5;
		case BATTLEGROUND_ARATHI_BASIN:
		case BATTLEGROUND_WARSONG_GULCH:
		case BATTLEGROUND_STRAND_OF_THE_ANCIENT:
		case BATTLEGROUND_ISLE_OF_CONQUEST:
			return 2;
		case BATTLEGROUND_EYE_OF_THE_STORM:
			return 1;
		default:
			return 0;
	}
}

#define MAX_LEVEL_GROUP 9
#define MINIMUM_PLAYERS_ON_EACH_SIDE_FOR_BG 1
#define MAXIMUM_BATTLEGROUNDS_PER_LEVEL_GROUP 50
#define LEVEL_GROUP_70 8

class Arena;

typedef CBattleground* (*BattlegroundFactoryMethod)(MapMgr* mgr, uint32 iid, uint32 group, uint32 type);
typedef CBattleground* ( *ArenaFactoryMethod )( MapMgr *mgr, uint32 iid, uint32 group, uint32 type, uint32 players_per_side );


class SERVER_DECL CBattlegroundManager : public Singleton<CBattlegroundManager>, public EventableObject
{
		/* Battleground Instance Map */
		map<uint32, CBattleground*> m_instances[BATTLEGROUND_NUM_TYPES];
		Mutex m_instanceLock;

		/* Max Id */
		uint32 m_maxBattlegroundId[BATTLEGROUND_NUM_TYPES];

		/* Queue System */
		// Instance Id -> list<Player guid> [ BattlegroundType ] (instance 0 - first available)
		list<uint32> m_queuedPlayers[BATTLEGROUND_NUM_TYPES][MAX_LEVEL_GROUP];

		// Instance Id -> list<Group id> [BattlegroundType][LevelGroup]
		list<uint32> m_queuedGroups[BATTLEGROUND_NUM_TYPES];

		Mutex m_queueLock;

		// Bg factory methods by Bg map Id
		std::map< uint32, BattlegroundFactoryMethod > bgFactories;
		// Arena factory methods
		std::vector< ArenaFactoryMethod > arenaFactories;
		// Bg map IDs by Bg type Id
		std::map< uint32, uint32 > bgMaps;
		// Arena map IDs
		std::vector< uint32 > arenaMaps;

	public:
		CBattlegroundManager();
		~CBattlegroundManager();

		////////////////////////////////////////////////////////////////////////////////////////////////
		//void RegisterBgFactory( uint32 map, BattlegroundFactoryMethod method )
		//  Registers the specified Battleground class factory method for
		//  the specified Battleground type.
		//  When trying to register a duplicate, the duplicate will be ignored.
		//
		//Parameter(s)
		//  uint32 map                        -  The map of the Battleground
		//  BattlegroundFactoryMethod method  -  The Battleground factory method
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////////
		void RegisterBgFactory( uint32 map, BattlegroundFactoryMethod method );

		////////////////////////////////////////////////////////////////////////////////////////////////
		//void RegisterArenaFactory( uint32 map, ArenaFactoryMethod method )
		//  Registers the specified Arena class factory method for
		//  the specified Battleground type.
		//  When trying to register a duplicate, the duplicate will be ignored.
		//
		//Parameter(s)
		//  uint32 map                 -  Map id
		//  ArenaFactoryMethod method  -  The Arena factory method
		//
		//Return Value(s)
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////////
		void RegisterArenaFactory( uint32 map, ArenaFactoryMethod method );


		////////////////////////////////////////////////////////////////////////////////////////////////
		//void RegisterMapForBgType( uint32 type, uint32 map )
		//  Registers a Map Id for the specified Battleground type.
		//  When trying to register a duplicate, the duplicate will be ignored.
		//
		//Parameter(s)
		//  uint32 type  -  The Battleground type
		//  uint32 map   -  The map Id
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////////
		void RegisterMapForBgType( uint32 type, uint32 map );

		/* Packet Handlers */
		void HandleBattlegroundListPacket(WorldSession* m_session, uint32 BattlegroundType, uint8 from = 0);
		void HandleArenaJoin(WorldSession* m_session, uint32 BattlegroundType, uint8 as_group, uint8 rated_match);

		/* Player Logout Handler */
		void OnPlayerLogout(Player* plr);

		/* Handler On Update Event */
		void EventQueueUpdate();
		void EventQueueUpdate(bool forceStart);

		/* Handle GetBattlegroundQueue Command */
		void HandleGetBattlegroundQueueCommand(WorldSession* m_session);

		/* Handle Battleground Join */
		void HandleBattlegroundJoin(WorldSession* m_session, WorldPacket & pck);

		/* Remove Player From All Queues */
		void RemovePlayerFromQueues(Player* plr);
		void RemoveGroupFromQueues(Group* grp);

		/* Create a battleground instance of type x */
		CBattleground* CreateInstance(uint32 Type, uint32 LevelGroup);

		/* Can we create a new instance of type x level group y? (NO LOCK!) */
		bool CanCreateInstance(uint32 Type, uint32 LevelGroup);

		/* Deletes a battleground (called from MapMgr) */
		void DeleteBattleground(CBattleground* bg);

		/* Build SMSG_BATTLEFIELD_STATUS */
		void SendBattlefieldStatus(Player* plr, BattleGroundStatus Status, uint32 Type, uint32 InstanceID, uint32 Time, uint32 MapId, uint8 RatedMatch);

		/* Gets ArenaTeam info from group */
		uint32 GetArenaGroupQInfo(Group* group, int type, uint32* avgRating);

		/* Creates an arena with groups group1 and group2 */
		int CreateArenaType(int type, Group* group1, Group* group2);

		/* Add player to bg team */
		void AddPlayerToBgTeam(CBattleground* bg, deque<uint32> *playerVec, uint32 i, uint32 j, int Team);

		/* Add player to bg */
		void AddPlayerToBg(CBattleground* bg, deque<uint32> *playerVec, uint32 i, uint32 j);

		/* Add a group to an arena */
		void AddGroupToArena(CBattleground* bg, Group* group, int nteam);

		/* Returns the minimum number of players (Only valid for battlegrounds) */
		uint32 GetMinimumPlayers(uint32 dbcIndex);

		/* Returns the maximum number of players (Only valid for battlegrounds) */
		uint32 GetMaximumPlayers(uint32 dbcIndex);
};

#define BattlegroundManager CBattlegroundManager::getSingleton()

#endif
