/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
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

	void HookOnPlayerDeath(Player * plr);
	void HookFlagDrop(Player * plr, GameObject * obj);
	void HookFlagStand(Player * plr, GameObject * obj);
	void HookOnMount(Player * plr);
	void HookOnAreaTrigger(Player * plr, uint32 id);
	bool HookHandleRepop(Player * plr);
	void OnAddPlayer(Player * plr);
	void OnRemovePlayer(Player * plr);
	void OnCreate();
	void HookOnPlayerKill(Player * plr, Unit * pVictim);
	void HookOnHK(Player * plr);
	void SpawnBuff(uint32 x);
	LocationVector GetStartingCoords(uint32 Team);
	static CBattleground * Create(MapMgr * m, uint32 i, uint32 l, uint32 t) { return new EyeOfTheStorm(m, i, l, t); }

	const char * GetName() { return "Eye of the Storm"; }
	void OnStart();

	void UpdateCPs();
	void GeneratePoints();

	// returns true if that team won
	bool GivePoints(uint32 team, uint32 points);

	void RespawnCPFlag(uint32 i, uint32 id);		// 0 = Neutral, <0 = Leaning towards alliance, >0 Leaning towards horde

	bool HookSlowLockOpen(GameObject * pGo, Player * pPlayer, Spell * pSpell);
	void DropFlag(Player * plr);
	void EventResetFlag();
	void RepopPlayersOfTeam(int32 team, Creature * sh);

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
