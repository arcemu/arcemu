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

#define BUFF_RESPAWN_TIME 90000

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

static wsg_go_spawn WarsongFlags[]= 
{
    { 179831, 489, 915.367f, 1433.78f, 346.089f, 3.17301f, 0, 210, 2.5f, GAMEOBJECT_STATE_CLOSED, 100 },    //horde
    { 179830, 489, 1540.29f, 1481.34f, 352.64f, 3.17301f, 0, 1314, 2.5f, GAMEOBJECT_STATE_CLOSED, 100 }     //alliance
};

static LocationVector wsg_start_pos[]=
{
    LocationVector(933.989685f, 1430.735840f, 345.537140f),
    LocationVector(1519.530273f, 1481.868408f, 352.023743f)
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
        void SpawnBuff(uint32 x);
        void EventVictory(uint8 Team);
		static CBattleground* Create(MapMgr* m, uint32 i, uint32 l, uint32 t) { return new WarsongGulch(m, i, l, t); }

		uint32 GetNameID() { return 39; }
		uint64 GetFlagHolderGUID( uint32 faction ) const{ return m_flagHolders[ faction ]; }
		void OnStart();

		void SetIsWeekend(bool isweekend);
		void DespawnGates(uint32 delay);
};
