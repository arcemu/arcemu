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


#ifndef HEADER_WSG
#define HEADER_WSG

#ifndef BUFF_RESPAWN_TIME
#define BUFF_RESPAWN_TIME 90000
#endif

class WarsongGulch : public CBattleground
{
	GameObject * m_buffs[6];
	GameObject * m_homeFlags[2];
	GameObject * m_dropFlags[2];
	uint32 m_flagHolders[2];
	list<GameObject*> m_gates;
	uint32 m_scores[2];
	uint32 m_lgroup;
public:
	WarsongGulch(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t);
	~WarsongGulch();

	ARCEMU_INLINE bool IsFlagHolder(Player * plr)
	{
		return (m_flagHolders[plr->GetTeam()] == plr->GetLowGUID());
	}

	void ForceFlagDrop(Player *plr);
	void CapturedFlag(Player *plr);
	void GivePlayerFlag(Player * plr, bool fromDroppedFlag);
	void ReturnTeamFlag(Player *plr, GameObject * gobj);

	void OnAddPlayer(Player * plr);
	void OnRemovePlayer(Player * plr);
	void OnCreate();
	
	void SpawnBuff(uint32 x);
	LocationVector GetStartingCoords(uint32 Team);
	void ReturnFlag(uint32 team);
	void EventReturnFlags();

	static CBattleground * Create(MapMgr * m, uint32 i, uint32 l, uint32 t) { return new WarsongGulch(m, i, l, t); }

	uint32 GetNameID() { return 39; }
	void OnStart();

	void SetIsWeekend(bool isweekend);

	// event handler functions
	static bool IsInWarsongGulch(Player * plr);
	static void OnAreaTrigger(Player * plr, uint32 id);
	static void OnMount(Player * plr);
	static void OnFlagDrop(Player * plr);
	static void OnFlagStandPickup(Player * plr, GameObject * obj);
	static void OnFlagDropPickup(Player * plr, GameObject * obj);
	static void OnPlayerDeath(Player * plr);
	static bool OnRepopRequest(Player * plr);
	static void OnHonorKill(Player * plr);
	static void OnPlayerKill(Player * plr, Player * pVictim);
	static bool OnSpellCast(Player * caster, SpellEntry * pSpell);

};

#endif