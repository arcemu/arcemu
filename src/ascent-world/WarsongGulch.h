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

#define BUFF_RESPAWN_TIME 90000

class WarsongGulch : public CBattleground
{
	GameObject * m_buffs[6];
	GameObject * m_homeFlags[2];
	GameObject * m_dropFlags[2];
	uint32 m_flagHolders[2];
	list<GameObject*> m_gates;
	uint32 m_scores[2];
public:
	WarsongGulch(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t);
	~WarsongGulch();

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
	void DropFlag(Player * plr);
	void ReturnFlag(uint32 team);

	static CBattleground * Create(MapMgr * m, uint32 i, uint32 l, uint32 t) { return new WarsongGulch(m, i, l, t); }

	const char * GetName() { return "Warsong Gulch"; }
	void OnStart();
};
