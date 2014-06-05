/*
 * EasyWoW MMORPG Server
 * Copyright (C) 2014 EasyWoW <http://github.com/EasyWoW>
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

#define BUFF_RESPAWN_TIME 90000
#define FACTION_WARSONG_OUTSTRIDERS 889
#define FACTION_SILVERING_SENTINELS 890

#define MAP_WARSONG_GULCH 489

struct wsg_go_spawn{
    uint32 entry;
    uint32 map;
    float x, y, z, o;
    uint32 flags;
    uint32 faction;
    float scale;
    uint8 state;
    uint32 animprogress;
};

struct wsg_go_spawn2{
    wsg_go_spawn spawn;
    float orientation[3];
};

static wsg_go_spawn WarsongFlags[]= 
{
    { 179831, MAP_WARSONG_GULCH, 915.367f, 1433.78f, 346.089f, 3.17301f, 0, 210, 2.5f, GAMEOBJECT_STATE_CLOSED, 100 },    //horde
    { 179830, MAP_WARSONG_GULCH, 1540.29f, 1481.34f, 352.64f, 3.17301f, 0, 1314, 2.5f, GAMEOBJECT_STATE_CLOSED, 100 }     //alliance
};

#define WSG_GATES_COUNT 5
static wsg_go_spawn WarsongGates[WSG_GATES_COUNT]=
{
    // Alliance gates
    { 179921, MAP_WARSONG_GULCH, 1471.554688f, 1458.778076f, 362.633240f, 0, 33, 114, 2.33271f, GAMEOBJECT_STATE_CLOSED, 100 },
    { 179919, MAP_WARSONG_GULCH, 1492.477783f, 1457.912354f, 342.968933f, 0, 33, 114, 2.68149f, GAMEOBJECT_STATE_CLOSED, 100 },
    { 179918, MAP_WARSONG_GULCH, 1503.335327f, 1493.465820f, 352.188843f, 0, 33, 114, 2.26f, GAMEOBJECT_STATE_CLOSED, 100 },

    // Horde gates
    { 179916, MAP_WARSONG_GULCH, 949.1663208f, 1423.7717285f, 345.6241455f, -0.5756807f, 32, 114, 0.900901f, GAMEOBJECT_STATE_CLOSED, 100 },
    { 179917, MAP_WARSONG_GULCH, 953.0507202f, 1459.8424072f, 340.6525573f, -1.9966197f, 32, 114, 0.854700f, GAMEOBJECT_STATE_CLOSED, 100 }
};

#define WSG_BUFFS_COUNT 6
static wsg_go_spawn2 wsg_buffs[WSG_BUFFS_COUNT] =
{
    { (179871, MAP_WARSONG_GULCH, 1449.9296875f, 1470.70971679688f, 342.634552001953f, -1.64060950279236f, 0, 114, 1, GAMEOBJECT_STATE_CLOSED, 100), 0, 0.73135370016098f, -0.681998312473297f },
    { (179899, MAP_WARSONG_GULCH, 1005.17071533203f, 1447.94567871094f, 335.903228759766f, 1.64060950279236f, 0, 114, 1, GAMEOBJECT_STATE_CLOSED, 100), 0, 0.73135370016098f, 0.681998372077942f },
    { (179904, MAP_WARSONG_GULCH, 1317.50573730469f, 1550.85070800781f, 313.234375f, -0.26179963350296f, 0, 114, 1, GAMEOBJECT_STATE_CLOSED, 100), 0, 0.130526319146156f, -0.991444826126099f },
    { (179906, MAP_WARSONG_GULCH, 1110.45129394531f, 1353.65563964844f, 316.518096923828f, -0.68067866563797f, 0, 114, 1, GAMEOBJECT_STATE_CLOSED, 100), 0, 0.333806991577148f, -0.94264143705368f },
    { (179905, MAP_WARSONG_GULCH, 1320.09375f, 1378.78967285156f, 314.753234863281f, 1.18682384490967f, 0, 114, 1, GAMEOBJECT_STATE_CLOSED, 100), 0, 0.559192895889282f, 0.829037606716156f },
    { (179907, MAP_WARSONG_GULCH, 1139.68774414063f, 1560.28771972656f, 306.843170166016f, -2.4434609413147f, 0, 114, 1, GAMEOBJECT_STATE_CLOSED, 100), 0, 0.939692616462708f, -0.342020124197006f }
};

static LocationVector wsg_start_pos[]=
{
    LocationVector(933.989685f, 1430.735840f, 345.537140f),
    LocationVector(1519.530273f, 1481.868408f, 352.023743f)
};

static LocationVector wsg_repop_pos[] =
{
    LocationVector(1032.644775f, 1388.316040f, 340.559937f, 0.043200f),
    LocationVector(1423.218872f, 1554.663574f, 342.833801f, 3.124139f)
};

static LocationVector wsg_spirit_guides[] =
{
    LocationVector(1423.218872f, 1554.663574f, 342.833801f, 3.124139f),
    LocationVector(1032.644775f, 1388.316040f, 340.559937f, 0.043200f)
};

class WarsongGulch : public CBattleground
{
		GameObject* m_buffs[6];
		GameObject* m_homeFlags[2];
		GameObject* m_dropFlags[2];
		uint32 m_flagHolders[2];
		list<GameObject*> m_gates;
		uint32 m_scores[2];
		uint32 m_lgroup;
	public:
		WarsongGulch(MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t);
		~WarsongGulch();

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
		void HookOnShadowSight();
		void HookGenerateLoot(Player* plr, Object* pCorpse);
		LocationVector GetStartingCoords(uint32 Team);
		void HookOnFlagDrop(Player* plr);
		void ReturnFlag(uint32 team);

		void EventReturnFlags();
        void SpawnBuffs();
        void SpawnGates();
        void EventVictory(uint8 Team);
		static CBattleground* Create(MapMgr* m, uint32 i, uint32 l, uint32 t) { return new WarsongGulch(m, i, l, t); }

        void UpdateTeamScore(uint8 Team);

        uint32 GetNameID() { return WORLDSTRING_WARSONG_GULCH; }
		uint64 GetFlagHolderGUID( uint32 faction ) const{ return m_flagHolders[ faction ]; }

		void OnStart();

		void SetIsWeekend(bool isweekend);
		void DespawnGates(uint32 delay);
};
