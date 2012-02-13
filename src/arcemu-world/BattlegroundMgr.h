/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2012 ArcEmu Team <http://arcemu.org/>
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


class CBattlegroundManager : public Singleton<CBattlegroundManager>, public EventableObject
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

	public:
		CBattlegroundManager();
		~CBattlegroundManager();

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

class SERVER_DECL CBattleground : public EventableObject
{

		friend class AVNode;
	protected:
		/* Groups */
		Group* m_groups[2];

		time_t m_nextPvPUpdateTime;
		MapMgr* m_mapMgr;
		uint32 m_id;
		uint32 m_type;
		uint32 m_levelGroup;
		uint32 m_deltaRating[2];
		uint32 m_invisGMs;
		uint32 m_honorPerKill;
		uint32 m_zoneid;

	public:
		/* Team->Player Map */
		set<Player*> m_players[2];
		void Lock() { m_mainLock.Acquire(); }
		void Unlock() { m_mainLock.Release(); }
		void AddInvisGM() {Lock(); m_invisGMs++; Unlock();}
		void RemoveInvisGM() {Lock(); m_invisGMs--; Unlock();}
	protected:

		/* PvP Log Data Map */
		map<uint32, BGScore> m_pvpData;

		/* Lock for all player data */
		Mutex m_mainLock;

		/* Player count per team */
		uint32 m_playerCountPerTeam;

		/* "pending" players */
		set<uint32> m_pendPlayers[2];

		/* starting time */
		uint32 m_startTime;
		bool m_started;

		/* countdown stuff */
		uint32 m_countdownStage;

		/* winner stuff */
		bool m_ended;
		uint8 m_winningteam;

		/* resurrect queue */
		map<Creature*, set<uint32> > m_resurrectMap;
		uint32 m_lastResurrect;

		bool m_isWeekend;

	public:
		void AddHonorToTeam( uint32 team, uint32 amount );

		void CastSpellOnTeam( uint32 team, uint32 spell );

		void RemoveAuraFromTeam( uint32 team, uint32 aura );

		void SendChatMessage(uint32 Type, uint64 Guid, const char* Format, ...);

		/* Hook Functions */
		virtual void HookOnPlayerDeath(Player* plr) = 0;

		virtual void HookOnPlayerResurrect( Player *player ){}

		/* Repopping - different battlegrounds have different ways of handling this */
		virtual bool HookHandleRepop(Player* plr) = 0;

		/* In CTF battlegrounds mounting will cause you to lose your flag. */
		virtual void HookOnMount(Player* plr) = 0;

		/* Only used in CTF (as far as I know) */
		virtual void HookFlagDrop(Player* plr, GameObject* obj) = 0;
		virtual void HookFlagStand(Player* plr, GameObject* obj) = 0;
		virtual void HookOnFlagDrop(Player* plr) = 0;

		/* Used when a player kills a player */
		virtual void HookOnPlayerKill(Player* plr, Player* pVictim) = 0;
		virtual void HookOnHK(Player* plr) = 0;

		/* On Area Trigger */
		virtual void HookOnAreaTrigger(Player* plr, uint32 id) = 0;

		/* On Shadow Sight */
		virtual void HookOnShadowSight() = 0;

		/* On Loot Generating */
		virtual void HookGenerateLoot(Player* plr, Object* pCorpse) = 0;

		/* On Unit Killing */
		virtual void HookOnUnitKill(Player* plr, Unit* pVictim) = 0;

		virtual void HookOnUnitDied( Unit *victim ){}

		/* Retrieval Functions */
		ARCEMU_INLINE uint32 GetId() { return m_id; }
		ARCEMU_INLINE uint32 GetLevelGroup() { return m_levelGroup; }
		ARCEMU_INLINE MapMgr* GetMapMgr() { return m_mapMgr; }

		/* Creating a battleground requires a pre-existing map manager */
		CBattleground(MapMgr* mgr, uint32 id, uint32 levelgroup, uint32 type);
		virtual ~CBattleground();

		/* Has it ended? */
		ARCEMU_INLINE bool HasEnded() { return m_ended; }
		/* Has it started? */
		ARCEMU_INLINE bool HasStarted() { return m_started; }

		/* Send the pvp log data of all players to this player. */
		void SendPVPData(Player* plr);

		/* Get the starting position for this team. */
		virtual LocationVector GetStartingCoords(uint32 Team) = 0;

		/* Send a packet to the entire battleground */
		void DistributePacketToAll(WorldPacket* packet);

		/* send a packet to only this team */
		void DistributePacketToTeam(WorldPacket* packet, uint32 Team);

		/* play sound to this team only */
		void PlaySoundToTeam(uint32 Team, uint32 Sound);

		/* play sound to all */
		void PlaySoundToAll(uint32 Sound);

		/* Full? */
		ARCEMU_INLINE bool IsFull() { return !(HasFreeSlots(0, m_type) || HasFreeSlots(1, m_type)); }

		/* Are we full? */
		bool HasFreeSlots(uint32 Team, uint32 type);

		/* Add Player */
		void AddPlayer(Player* plr, uint32 team);
		virtual void OnAddPlayer(Player* plr) = 0;

		/* Remove Player */
		void RemovePlayer(Player* plr, bool logout);
		virtual void OnRemovePlayer(Player* plr) = 0;

		/* Port Player */
		void PortPlayer(Player* plr, bool skip_teleport = false);
		virtual void OnCreate() = 0;

		/* Remove pending player */
		void RemovePendingPlayer(Player* plr);

		/* Gets the number of free slots */
		uint32 GetFreeSlots(uint32 t, uint32 type);

		GameObject* SpawnGameObject(uint32 entry, uint32 MapId , float x, float y, float z, float o, uint32 flags, uint32 faction, float scale);
		GameObject* SpawnGameObject( uint32 entry, LocationVector &v, uint32 flags, uint32 faction, float scale );
		Creature* SpawnCreature(uint32 entry, float x, float y, float z, float o, uint32 faction = 0 );
		Creature* SpawnCreature( uint32 entry, LocationVector &v, uint32 faction = 0 );
		void UpdatePvPData();

		ARCEMU_INLINE uint32 GetStartTime() { return m_startTime; }
		ARCEMU_INLINE uint32 GetType() { return m_type; }

		// events should execute in the correct context
		int32 event_GetInstanceID();
		void EventCreate();

		virtual uint32 GetNameID() { return 34;}
		void EventCountdown();

		virtual void Start();
		virtual void OnStart() {}
		void Close();
		virtual void OnClose() {}

		void SetWorldState(uint32 Index, uint32 Value);
		Creature* SpawnSpiritGuide(float x, float y, float z, float o, uint32 horde);
		Creature* SpawnSpiritGuide( LocationVector &v, uint32 faction );

		ARCEMU_INLINE uint32 GetLastResurrect() { return m_lastResurrect; }
		void AddSpiritGuide(Creature* pCreature);
		void RemoveSpiritGuide(Creature* pCreature);
		void QueuePlayerForResurrect(Player* plr, Creature* spirit_healer);
		void RemovePlayerFromResurrect(Player* plr, Creature* spirit_healer);
		void EventResurrectPlayers();
		virtual bool CanPlayerJoin(Player* plr, uint32 type);
		virtual bool CreateCorpse(Player* plr) { return true; }
		virtual bool HookSlowLockOpen(GameObject* pGo, Player* pPlayer, Spell* pSpell) { return false; }
		virtual bool HookQuickLockOpen( GameObject *go, Player *player, Spell *spell ){ return false; }

		void BuildPvPUpdateDataPacket(WorldPacket* data);
		virtual uint8 Rated() { return 0; }
		void OnPlayerPushed(Player* plr);

		virtual void SetIsWeekend(bool isweekend) {}
		void QueueAtNearestSpiritGuide(Player* plr, Creature* old);
};

#define BattlegroundManager CBattlegroundManager::getSingleton( )


#endif
