/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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

#define TEAM_DEFENDER	0
#define TEAM_ATTACKER	1
#define GUN_LEFT		0
#define GUN_RIGHT		1

enum Gate
{
	GATE_GREEN	= 0,
	GATE_YELLOW = 1,
	GATE_BLUE	= 2,
	GATE_RED	= 3,
	GATE_PURPLE	= 4,
	GATE_COUNT	= 5,
};

class StrandOfTheAncient : public CBattleground
{
private:
	//Transporter * m_boats[2];
	//TransportPath path;
	uint32		 m_scores[2];
	GameObject * m_boats[2];
	GameObject * m_buffs[BUFF_COUNT];
	GameObject * m_relic;
	GameObject * m_endgate;
	GameObject * m_gates[GATE_COUNT];
	GameObject * m_gateSigils[GATE_COUNT];
	GameObject * m_gateTransporters[GATE_COUNT];
	list<Player *> sota_players;
	PassengerMap boat1Crew;
	PassengerMap boat2Crew;

public:
	static CBattleground * Create(MapMgr * m, uint32 i, uint32 l, uint32 t) { return new StrandOfTheAncient(m, i, l, t); }

	StrandOfTheAncient(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t);
	~StrandOfTheAncient();

	uint32 GetNameID() { return 34; } // in worldstring_tables

	void HookOnAreaTrigger(Player * plr, uint32 id);
	void HookFlagStand(Player * plr, GameObject * obj);
	void HookOnFlagDrop(Player * plr);
	void HookFlagDrop(Player * plr, GameObject * obj);
	void HookOnPlayerKill(Player * plr, Player * pVictim);
	void HookOnHK(Player * plr);
	void OnAddPlayer(Player * plr);
	void OnRemovePlayer(Player * plr);
	LocationVector GetStartingCoords(uint32 team);
	void HookOnPlayerDeath(Player * plr);
	void HookOnMount(Player * plr);
	bool HookHandleRepop(Player * plr);
	void OnPlatformTeleport(Player *plr);
	void OnCreate();
	void OnStart();
	void HookOnShadowSight();
	void HookGenerateLoot(Player *plr, Object * pOCorpse);
	void HookOnUnitKill(Player * plr, Unit * pVictim);
	void SetIsWeekend(bool isweekend);
	bool HookSlowLockOpen(GameObject * pGo, Player * pPlayer, Spell * pSpell);

protected:
	uint32 m_flagHolders[2];

	void SpawnBuff(uint32 x);

};
