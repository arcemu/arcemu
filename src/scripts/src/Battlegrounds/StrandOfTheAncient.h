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

#define BUFF_COUNT		3

#define SOTA_NUM_CANONS 10
#define SOTA_NUM_DEMOLISHERS 6
#define SOTA_NUM_DOCK_DEMOLISHERS 4
#define SOTA_EAST_WS_DEMOLISHER_INDEX 5
#define SOTA_WEST_WS_DEMOLISHER_INDEX 4

#define TEAM_DEFENDER	0
#define TEAM_ATTACKER	1
#define GUN_LEFT		0
#define GUN_RIGHT		1

#define ROUND_LENGTH 600 //in secs

enum SOTAControlPoints{
	SOTA_CONTROL_POINT_EAST_GY    = 0,
	SOTA_CONTROL_POINT_WEST_GY    = 1,
	SOTA_CONTROL_POINT_SOUTH_GY   = 2,
	NUM_SOTA_CONTROL_POINTS
};

enum SOTAGraveyards{
	SOTA_GY_EAST            = 0,
	SOTA_GY_WEST            = 1,
	SOTA_GY_SOUTH           = 2,
	SOTA_GY_DEFENDER        = 3,
	SOTA_GY_ATTACKER_BEACH  = 4,
	NUM_SOTA_GRAVEYARDS
};

enum SOTACPStates{
	SOTA_CP_STATE_UNCONTROLLED    = 0,
	SOTA_CP_STATE_ALLY_CONTROL    = 1,
	SOTA_CP_STATE_HORDE_CONTROL   = 2,
	MAX_SOTA_CP_STATES
};

enum Gate
{
    GATE_GREEN	= 0,
    GATE_YELLOW = 1,
    GATE_BLUE	= 2,
    GATE_RED	= 3,
    GATE_PURPLE	= 4,
    GATE_COUNT	= 5,
};

enum SOTABattleRoundProgress{
	SOTA_ROUND_PREPARATION,
	SOTA_ROUND_STARTED,
	SOTA_NUM_ROUND_STAGES
};

struct SOTAControlPoint{
	GameObject *pole;
	GameObject *banner;
	SOTACPStates state;
	uint32 worldstate;

	SOTAControlPoint(){
		pole = NULL;
		banner = NULL;
		state = SOTA_CP_STATE_UNCONTROLLED;
		worldstate = 0;
	}

	~SOTAControlPoint(){
		pole = NULL;
		banner = NULL;
		state = SOTA_CP_STATE_UNCONTROLLED;
		worldstate = 0;
	}
};

struct SOTAGraveyard{
	Creature *spiritguide;
	uint32 faction;

	SOTAGraveyard(){
		spiritguide = NULL;
		faction = MAX_PLAYER_TEAMS;
	}

	~SOTAGraveyard(){
		spiritguide = NULL;
		faction = MAX_PLAYER_TEAMS;
	}
};

class StrandOfTheAncient : public CBattleground
{
	private:
		uint32 Attackers; // 0 - horde / 1 - alliance
		uint32 Defenders;
		uint32 BattleRound;
		uint32 RoundTime;
		uint32 RoundFinishTime[ 2 ];
		SOTABattleRoundProgress roundprogress;
		GameObject* m_boats[4];
		GameObject* m_buffs[BUFF_COUNT];
		GameObject* m_relic;
		GameObject* m_endgate;
		GameObject* m_gates[GATE_COUNT];
		GameObject* m_gateSigils[GATE_COUNT];
		GameObject* m_gateTransporters[GATE_COUNT];
		PassengerMap boat1Crew;
		PassengerMap boat2Crew;
		Creature *canon[ SOTA_NUM_CANONS ];
		Creature *demolisher[ SOTA_NUM_DEMOLISHERS ];

		SOTAControlPoint controlpoint[ NUM_SOTA_CONTROL_POINTS ];
		SOTAGraveyard graveyard[ NUM_SOTA_GRAVEYARDS ];

	public:
		static CBattleground* Create(MapMgr* m, uint32 i, uint32 l, uint32 t) { return new StrandOfTheAncient(m, i, l, t); }

		StrandOfTheAncient(MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t);
		~StrandOfTheAncient();

		uint32 GetNameID() { return 34; } // in worldstring_tables

		uint32 GetRoundTime() { return RoundTime; };
		LocationVector GetStartingCoords(uint32 team);
		void HookOnAreaTrigger(Player* plr, uint32 id);
		void HookFlagStand(Player* plr, GameObject* obj);
		void HookOnFlagDrop(Player* plr);
		void HookFlagDrop(Player* plr, GameObject* obj);
		void HookOnPlayerKill(Player* plr, Player* pVictim);
		void HookOnHK(Player* plr);
		void HookOnShadowSight();
		void HookGenerateLoot(Player* plr, Object* pOCorpse);
		void HookOnUnitKill(Player* plr, Unit* pVictim);
		void HookOnUnitDied( Unit *victim );
		bool HookSlowLockOpen( GameObject *go, Player *player, Spell *spell );
		bool HookQuickLockOpen( GameObject *go, Player *player, Spell *spell );
		void HookOnPlayerDeath(Player* plr);
		void HookOnMount(Player* plr);
		bool HookHandleRepop(Player* plr);
		void OnAddPlayer(Player* plr);
		void OnRemovePlayer(Player* plr);
		void OnPlatformTeleport(Player* plr);
		void OnCreate();
		void OnStart();
		void SetIsWeekend(bool isweekend);
		void SetRoundTime(uint32 secs) { RoundTime = secs; };
		void SetTime( uint32 secs );
		void TimeTick();
		void PrepareRound();
		void StartRound();
		void FinishRound();
		void Finish( uint32 winningteam );

		void SpawnControlPoint( SOTAControlPoints point, SOTACPStates state );
		void CaptureControlPoint( SOTAControlPoints point );
		void SpawnGraveyard( SOTAGraveyards gyid, uint32 team );

};
