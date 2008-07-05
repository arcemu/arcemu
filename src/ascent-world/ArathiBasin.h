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

#define AB_BUFF_RESPAWN_TIME 90000

enum ABBuffs
{
	AB_BUFF_STABLES						= 0,
	AB_BUFF_BLACKSMITH					= 1,
	AB_BUFF_FARM						= 2,
	AB_BUFF_LUMBERMILL					= 3,
	AB_BUFF_MINE						= 4,
	AB_NUM_BUFFS						= 5,
};

enum ABControlPoints
{
	AB_CONTROL_POINT_STABLE				= 0,
	AB_CONTROL_POINT_FARM				= 1,
	AB_CONTROL_POINT_BLACKSMITH			= 2,
	AB_CONTROL_POINT_MINE				= 3,
	AB_CONTROL_POINT_LUMBERMILL			= 4,
	AB_NUM_CONTROL_POINTS				= 5,
};

enum ABSpawnTypes
{
	AB_SPAWN_TYPE_NEUTRAL				= 0,
	AB_SPAWN_TYPE_ALLIANCE_ASSAULT		= 1,
	AB_SPAWN_TYPE_HORDE_ASSAULT			= 2,
	AB_SPAWN_TYPE_ALLIANCE_CONTROLLED	= 3,
	AB_SPAWN_TYPE_HORDE_CONTROLLED		= 4,
	AB_NUM_SPAWN_TYPES					= 5,
};

class ArathiBasin : public CBattleground
{
public:
	GameObject * m_buffs[AB_NUM_BUFFS];
	GameObject * m_controlPoints[AB_NUM_CONTROL_POINTS];
	GameObject * m_controlPointAuras[AB_NUM_CONTROL_POINTS];

protected:
	list<GameObject*> m_gates;
	
	uint32 m_resources[2];
	uint32 m_capturedBases[2];
	uint32 m_lastHonorGainResources[2];
	int32 m_basesOwnedBy[AB_NUM_CONTROL_POINTS];
	int32 m_basesAssaultedBy[AB_NUM_CONTROL_POINTS];
	Creature * m_spiritGuides[AB_NUM_CONTROL_POINTS];
	bool m_nearingVictory[2];

public:
	ArathiBasin(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t);
	~ArathiBasin();

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

	static CBattleground * Create(MapMgr * m, uint32 i, uint32 l, uint32 t) { return new ArathiBasin(m, i, l, t); }

	const char * GetName() { return "Arathi Basin"; }
	void OnStart();

	void EventUpdateResources(uint32 Team);
	bool HookSlowLockOpen(GameObject * pGo, Player * pPlayer, Spell * pSpell);

	/* AB Game Mechanics */
	void SpawnControlPoint(uint32 Id, uint32 Type);
	void CaptureControlPoint(uint32 Id, uint32 Team);
	void AssaultControlPoint(Player * pPlayer, uint32 Id);
};
