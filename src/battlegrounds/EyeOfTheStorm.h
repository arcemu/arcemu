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


#ifndef _EOTS_H
#define _EOTS_H

#define EOTS_TOWER_COUNT 4

#define EOTS_BUFF_RESPAWN_TIME 90000

class EyeOfTheStorm : public CBattleground
{
public:
	EyeOfTheStorm(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t);
	~EyeOfTheStorm();

	void OnAddPlayer(Player * plr);
	void OnRemovePlayer(Player * plr);
	void OnCreate();
	void SpawnBuff(uint32 x);
	LocationVector GetStartingCoords(uint32 Team);
	static CBattleground * Create(MapMgr * m, uint32 i, uint32 l, uint32 t) { return new EyeOfTheStorm(m, i, l, t); }

	uint32 GetNameID() { return 44; }
	void OnStart();

	void UpdateCPs();
	void UpdateIcons();
	void GeneratePoints();

	// returns true if that team won
	bool GivePoints(uint32 team, uint32 points);

	void RespawnCPFlag(uint32 i, uint32 id);		// 0 = Neutral, <0 = Leaning towards alliance, >0 Leaning towards horde

	bool HookSlowLockOpen(GameObject * pGo, Player * pPlayer, Spell * pSpell);
	void EventResetFlag();
	void RepopPlayersOfTeam(int32 team, Creature * sh);

	void HookOnSpellCast(Player *caster, uint32 spellID);
	void HookOnDestoryGameObject(GameObject * gameobject);

	void SetIsWeekend(bool isweekend);

	ARCEMU_INLINE void PlayerDropFlag(Player * plr)
	{
		if (m_flagHolder == plr->GetLowGUID())
			sHookInterface.OnFlagDrop(plr);
		UpdatePvPData();
	}

	ARCEMU_INLINE bool IsFlagHolder(Player *plr)
	{
		return (m_flagHolder == plr->GetLowGUID());
	}

	ARCEMU_INLINE void SetFlagHolder(Player *plr)
	{
		m_flagHolder = plr->GetLowGUID();
	}

	ARCEMU_INLINE GameObject * GetBuff(uint32 buffIndex)
	{
		ASSERT(buffIndex >= 0 && buffIndex < 4);
		return EOTSm_buffs[buffIndex];
	}

	ARCEMU_INLINE void PushFlagToStand()
	{
		m_standFlag->PushToWorld( m_mapMgr );
		m_flagHolder = 0;
		SetWorldState( 2757, 1 );
	}

	void GivePlayerFlag(Player * plr);

	// Event Handlers
	static bool IsInEyeOfTheStorm(Player * plr);
	static void OnPlayerDeath(Player * plr);
	static void OnFlagDropPickup(Player * plr, GameObject * obj);
	static void OnMount(Player * plr);
	static void OnAreaTrigger(Player * plr, uint32 id);
	static bool OnRepopRequest(Player * plr);
	static void OnFlagDrop(Player * plr);
	static void OnPlayerKill(Player * plr, Player * pVictim);
	static void OnHonorKill(Player * plr);

protected:
	int32 m_CPStatus[EOTS_TOWER_COUNT];		
	uint32 m_flagHolder;

	GameObject * m_standFlag;
	GameObject * m_dropFlag;

	GameObject * m_CPStatusGO[EOTS_TOWER_COUNT];
	GameObject * m_CPBanner[EOTS_TOWER_COUNT];
	GameObject * m_bubbles[2];
	GameObject * EOTSm_buffs[4];

	typedef set<Player*> EOTSCaptureDisplayList;
	EOTSCaptureDisplayList m_CPDisplay[EOTS_TOWER_COUNT];

	uint32 m_points[2];
	Creature * m_spiritGuides[EOTS_TOWER_COUNT];
};

#endif		// _EOTS_H
