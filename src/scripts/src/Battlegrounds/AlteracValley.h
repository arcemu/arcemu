/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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
enum AVControlPoints
{
    AV_CONTROL_POINT_STORMPIKE_AID_STATION	= 0,
    AV_CONTROL_POINT_STORMPIKE_GRAVEYARD	= 1,
    AV_CONTROL_POINT_STONEHEARTH_GRAVEYARD	= 2,
    AV_CONTROL_POINT_SNOWFALL_GRAVEYARD		= 3,
    AV_CONTROL_POINT_COLDTOOTH_MINE			= 4,
    AV_CONTROL_POINT_IRONDEEP_MINE			= 5,
    AV_CONTROL_POINT_ICEBLOOD_GRAVEYARD		= 6,
    AV_CONTROL_POINT_FROSTWOLF_GRAVEYARD	= 7,
    AV_CONTROL_POINT_FROSTWOLF_RELIEF_HUT	= 8,
    AV_CONTROL_POINT_DUN_BALDAR_NORTH_BUNKER		= 9,
    AV_CONTROL_POINT_DUN_BALDAR_SOUTH_BUNKER		= 10,
    AV_CONTROL_POINT_ICEWING_BUNKER			= 11,
    AV_CONTROL_POINT_STONEHEARTH_BUNKER		= 12,
    AV_CONTROL_POINT_ICEBLOOD_TOWER			= 13,
    AV_CONTROL_POINT_TOWER_POINT			= 14,
    AV_CONTROL_POINT_EAST_FROSTWOLF_TOWER	= 15,
    AV_CONTROL_POINT_WEST_FROSTWOLF_TOWER	= 16,
    AV_NUM_CONTROL_POINTS					= 17,
};

enum AVSpawnTypes
{
    AV_SPAWN_TYPE_NEUTRAL							= 0,
    AV_SPAWN_TYPE_ALLIANCE_ASSAULT					= 1,
    AV_SPAWN_TYPE_HORDE_ASSAULT						= 2,
    AV_SPAWN_TYPE_ALLIANCE_CONTROLLED_OR_DESTROYED	= 3,
    AV_SPAWN_TYPE_HORDE_CONTROLLED_OR_DESTROYED		= 4,
    AV_NUM_SPAWN_TYPES								= 5
};

enum AVNodeStates
{
    AV_NODE_STATE_NEUTRAL_CONTROLLED		= 0,
    AV_NODE_STATE_ALLIANCE_ASSAULTING		= 1,
    AV_NODE_STATE_ALLIANCE_CONTROLLED		= 2,
    AV_NODE_STATE_HORDE_ASSAULTING			= 3,
    AV_NODE_STATE_HORDE_CONTROLLED			= 4,
    AV_NODE_STATE_COUNT						= 5
};

struct AVLocation { float x; float y; float z; };
struct AVSpawnLocation { float x; float y; float z; float o; };
struct AVGameObject { uint32 id[AV_NODE_STATE_COUNT]; float x; float y; float z; float o; float rot1; float rot2; };
struct AVNodeTemplate
{
	const char* m_name;										// Stormpike Aid Station
	bool m_isGraveyard;								// Is this a graveyard?
	bool m_capturable;								// Is this capturable?
	AVLocation m_graveyardLocation;					// Revive location, also location of spirit guide
	AVGameObject m_flagLocation;						// Flag location (need to add GO id/properties here)
	AVGameObject m_auraLocation;						// Aura location
	AVGameObject m_glowLocation;						// Aura glow location
	uint32 m_guardId[3];								// Horde/alliance guard ids
	uint32 m_guardCount;								// Count of guards to spawn
	uint32 m_bossId[3];								// Boss ID (e.g. Balinda), 0 = A, 1 = H, 2 = Neutral
	AVLocation* m_peonLocations;						// Used by mines.
	AVLocation m_bossLocation;						// Location of boss if there is one
	uint32 m_initialSpawnId;							// Initial spawn (Bowmen) ID
	uint32 m_worldStateFields[AV_NODE_STATE_COUNT];	// State fields
	uint32 m_defaultState;							// State of the node when battleground is spawned
};

// GENERAL AV DEFINES
#define AV_NUM_REINFORCEMENTS				600		// Amount of reinforcements we start off with
#define AV_SCORE_WARNING					530		// Dunno what this should be ;p
#define AV_ADD_POINTS_ON_CONTROLLED_MINE	1		// Points to give the team who controls (a) mine(s)
#define AV_REINFORCEMENT_ADD_INTERVAL		45000	// The interval (in milliseconds) that points from mines are awarded
#define AV_POINTS_ON_DESTROY_BUNKER			75		// Points to remove for destroying a team's bunker
#define AV_POINTS_ON_KILL					1		// Points to remove when killing a member of the opposite team
#define AV_POINTS_ON_KILL_CAPTAIN			100		// Points  to remove for killing a team's captain
#define AV_HONOR_ON_KILL_BOSS				62		// Amount of honor awarded to players for killing a boss of the opposite team
#define AV_NUM_CONTESTED_AREAS				9		// Total contested areas (graveyards/mines)
#define AV_NUM_DESTROYABLE_AREAS			8		// Total destroyable areas (towers/bunkers)
#define AV_NUM_BOSS_UNITS					14		// Boss units (generals/captains/wing commanders etc)
#define AV_NUM_COLDTOOTH_UNITS				5		// Coldtooth mine NPC types
#define AV_NUM_IRONDEEP_UNITS				4		// Irondeep mine NPC types
#define AV_NUM_SNOWFALL_FLAGS				2		// Count of Snowfall flags (used when changing them on a team's acquiration of the CP
#define AV_CONTESTED_AREAS_START			0		// ID at which contested points start (for loops/ifs)
#define AV_CONTESTED_AREAS_END				8		// ID at which contested points end (for loops/ifs)
#define AV_DESTROYABLE_AREAS_START			9		// ID at which destroyable points start (for loops/ifs)
#define AV_DESTROYABLE_AREAS_END			16		// ID at which contested points finish (for loops/ifs)

// GAMEOBJECTS
#define	AV_GAMEOBJECT_FIRE					179065
#define AV_GAMEOBJECT_GATE					180424

// NPCS
#define AV_NPC_GENERAL_VANNDAR_STORMPIKE	11948
#define AV_NPC_CAPTAIN_BALINDA_STONEHEARTH	11949
#define AV_NPC_ARCH_DRUID_RENFERAL			13442
#define AV_NPC_WING_COMMANDER_SLIDORE		13438
#define AV_NPC_WING_COMMANDER_VIPORE		13439
#define AV_NPC_WING_COMMANDER_ICHMAN		13437

#define AV_NPC_GENERAL_DREK_THAR			11946
#define AV_NPC_CAPTAIN_GALVANGAR			11947
#define AV_NPC_PRIMALIST_THURLOGA			13236
#define AV_NPC_WING_COMMANDER_GUSE			13179
#define AV_NPC_WING_COMMANDER_JEZTOR		13180
#define AV_NPC_WING_COMMANDER_MULVERICK		13181

#define AV_NPC_IVUS_FOREST_LORD				13419
#define AV_NPC_LOKHOLAR_ICE_LORD			13256

#define AV_NPC_TASKMASTER_SNIVVLE			11677
#define AV_NPC_WHITEWISKER_DIGGER			11603
#define AV_NPC_WHITEWISKER_GEOMANCER		11604
#define AV_NPC_WHITEWISKER_OVERSEER			11605
#define AV_NPC_WHITEWISKER_VERMIN			10982

#define AV_NPC_MORLOCH						11657
#define AV_NPC_IRONDEEP_TROGG				10987
#define AV_NPC_IRONDEEP_SHAMAN				11600
#define AV_NPC_IRONDEEP_SKULLTHUMPER		11602

class AlteracValley : public CBattleground
{
	protected:
		list<GameObject*> m_gates;
		uint32 m_reinforcements[2];
		bool m_nearingVictory[2];
		ARCEMU_INLINE map<Creature*, set<uint32> > Get_m_resurrectMap() { return CBattleground::m_resurrectMap; }
	public:
		AlteracValley(MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t);
		~AlteracValley();

		void EventAssaultControlPoint(uint32 x);

		void HookOnPlayerDeath(Player* plr);
		void HookFlagDrop(Player* plr, GameObject* obj);
		void HookFlagStand(Player* plr, GameObject* obj);
		void HookOnMount(Player* plr);
		void HookOnAreaTrigger(Player* plr, uint32 id);
		bool HookHandleRepop(Player* plr);
		void OnAddPlayer(Player* plr);
		void OnRemovePlayer(Player* plr);
		void OnCreate();
		void HookOnPlayerKill(Player* plr, Player* pVictim);
		void HookOnUnitKill(Player* plr, Unit* pVictim);
		void HookOnHK(Player* plr);
		LocationVector GetStartingCoords(uint32 Team);
		void DropFlag(Player* plr);

		static CBattleground* Create(MapMgr* m, uint32 i, uint32 l, uint32 t) { return new AlteracValley(m, i, l, t); }

		const char* GetName() { return "Alterac Valley"; }
		void OnStart();

		void EventUpdateResources();
		bool HookSlowLockOpen(GameObject* pGo, Player* pPlayer, Spell* pSpell);

		/* AV Functions */
		void AddReinforcements(uint32 teamId, uint32 amt);
		void RemoveReinforcements(uint32 teamId, uint32 amt);
		void Finish(uint32 losingTeam);

		/* looooooot */
		bool SupportsPlayerLoot() { return true; }
		void HookGenerateLoot(Player* plr, Object* pCorpse);

		/* herald */
		void Herald(const char* format, ...);

		void HookOnFlagDrop(Player* plr);
		void HookOnShadowSight();

		class AVNode
		{
				AlteracValley* m_bg;
				AVNodeTemplate* m_template;

				// boss, changes ownership upon death?
				Creature* m_boss;

				// guards, need to be respawned when changes ownership
				vector<Creature*> m_guards;

				// peon locations, used in mines (todo)
				vector<Creature*> m_peonLocations;

				// control point (capturable)
				GameObject* m_flag;

				// aura (light-shiny stuff)
				GameObject* m_aura;
				GameObject* m_glow;

				// home NPc
				Creature* m_homeNPC;

				// destroyed flag (prevent all actions)
				bool m_destroyed;

				// state
				uint32 m_state;
				uint32 m_lastState;
				uint32 m_nodeId;

				// spirit guides
				Creature* m_spiritGuide;

			public:
				friend class AlteracValley;

				// constructor
				AVNode(AlteracValley* parent, AVNodeTemplate* tmpl, uint32 node_id);
				~AVNode();

				// initial spawn
				void Spawn();

				// assault
				void Assault(Player* plr);

				// capture event
				void Capture();

				// spawn guards
				void SpawnGuards(uint32 x);

				// state change
				void ChangeState(uint32 new_state);

				// spawn home buff guard
				void SpawnHomeGuard();
		};
	protected:
		AVNode* m_nodes[AV_NUM_CONTROL_POINTS];
};

