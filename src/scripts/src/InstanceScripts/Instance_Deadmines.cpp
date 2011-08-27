/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Setup.h"

enum DeadMinesGOIDs{
	GO_FACTORY_DOOR			= 13965,
	GO_IRONCLAD_DOOR		= 16397,
	GO_DEFIAS_CANNON		= 16398,
	GO_HEAVY_DOOR			= 17153,
	GO_FACTORY_DOOR_LEVER	= 101831,
	GO_SNEED_DOOR_LEVER		= 101832,
	GO_IRONCLAD_LEVER		= 101833,
	GO_GILNID_DOOR_LEVER	= 101834,
	GO_MR_SMITE_CHEST		= 144111
};

enum DeadMinesNPCIDs{
	NPC_EDWIN_VANCLEEF	= 639,
	NPC_SNEEDS_SHREDDER	= 642,
	NPC_SNEED			= 643,
	NPC_RHAHK_ZOR		= 644,
	NPC_MR_SMITE		= 646,
	NPC_GUARD1			= 657,	//Pirate
	NPC_GILNID			= 1763,
	NPC_GUARD2			= 3450	//Parrot
};

#define MAP_DEADMINES 36


static Location Doors[] =
{
	{-168.514f, -579.861f, 19.3159f, 0},	//Gilnid doors
	{-290.294f, -536.96f, 49.4353f, 0}		//Sneed doors
};

//it will useful for cannon event
static Location Guards[] =
{
	{-89.7001f, -691.332f, 8.24514f, 0}, //Guard
	{-102.521f, -697.942f, 8.84454f, 0}, //Guard
	{-89.6744f, -694.063f, 8.43202f, 0}  //Parrot
};

class DeadminesInstanceScript : public MoonInstanceScript
{
public:
	MOONSCRIPT_INSTANCE_FACTORY_FUNCTION(DeadminesInstanceScript, MoonInstanceScript);

	DeadminesInstanceScript(MapMgr* pMapMgr) : MoonInstanceScript(pMapMgr){
		mFactoryDoor_GUID = 0;
		mDefiasCannon_GUID = 0;
		mDoorLever_GUID = 0;
		mMrSmiteChest_GUID = 0;
		mIronCladDoor_GUID = 0;
		InstanceEncounter = 0;
	}

	void OnGameObjectPushToWorld(GameObject* pGameObject)
	{
		switch(pGameObject->GetEntry())
		{
		case GO_FACTORY_DOOR:	mFactoryDoor_GUID = pGameObject->GetGUID(); break;
		case GO_FACTORY_DOOR_LEVER:	mDoorLever_GUID = pGameObject->GetGUID(); break;
		case GO_IRONCLAD_DOOR:	mIronCladDoor_GUID = pGameObject->GetGUID(); break;
		}
	}

	void OnGameObjectActivate(GameObject* pGameObject, Player* pPlayer)
	{
		switch(pGameObject->GetEntry())
		{
		case GO_DEFIAS_CANNON:
			{
				GameObject* pDoor4 = GetGameObjectByGuid(mIronCladDoor_GUID);
				if(pDoor4!=NULL && pDoor4->GetState()!=2)
					pDoor4->SetState(2);
			}break;
		case GO_FACTORY_DOOR_LEVER:
			{
				GameObject* pDoor5 = GetGameObjectByGuid(mFactoryDoor_GUID);
				if(pDoor5!=NULL)
					pDoor5->SetState(pDoor5->GetState()==State_Inactive ? State_Active : State_Inactive);
			}break;
		case GO_IRONCLAD_LEVER:
			{
				GameObject* pDoor6 = GetGameObjectByGuid(mFactoryDoor_GUID);
				//Door can be opened by lever if state isn't 2
				if(pDoor6!=NULL && pDoor6->GetState()!=2)
					pDoor6->SetState(pDoor6->GetState() == State_Inactive ? State_Active : State_Inactive);
			}break;
		case GO_SNEED_DOOR_LEVER:
			{
				GameObject* pDoor7 = FindClosestGameObjectOnMap(GO_HEAVY_DOOR, Doors[1].x, Doors[1].y, Doors[1].z);
				if(pDoor7!=NULL)
					pDoor7->SetState(pDoor7->GetState() == State_Inactive ? State_Active : State_Inactive);
			}break;
		case GO_GILNID_DOOR_LEVER:
			{
				GameObject* pDoor8 = FindClosestGameObjectOnMap(GO_HEAVY_DOOR, Doors[0].x, Doors[0].y, Doors[0].z);
				if(pDoor8!=NULL)
					pDoor8->SetState(pDoor8->GetState() == State_Inactive ? State_Active : State_Inactive);
			}break;
		}
	}

	void OnCreatureDeath(Creature* pCreature, Unit* pUnit)
	{
		switch(pCreature->GetEntry())
		{
		case NPC_RHAHK_ZOR:
			{
				GameObject* pDoor1 = GetGameObjectByGuid(mFactoryDoor_GUID);
				if(pDoor1!=NULL)
					pDoor1->SetState(State_Active);
			}break;
		case NPC_SNEEDS_SHREDDER:
			SpawnCreature(NPC_SNEED, pCreature->GetPositionX(), pCreature->GetPositionY(), pCreature->GetPositionZ(), pCreature->GetOrientation());
			break;
		case NPC_GILNID:
			{
				GameObject* pDoor2 = FindClosestGameObjectOnMap(GO_HEAVY_DOOR, Doors[0].x, Doors[0].y, Doors[0].z);
				if(pDoor2!=NULL)
					pDoor2->SetState(State_Active);
			}break;
		case NPC_SNEED:
			{
				GameObject* pDoor3 = FindClosestGameObjectOnMap(GO_HEAVY_DOOR, Doors[1].x, Doors[1].y, Doors[1].z);
				if(pDoor3!=NULL)
					pDoor3->SetState(State_Active);
			}break;
		}
	}

protected:
	uint32 mFactoryDoor_GUID;
	uint32 mDefiasCannon_GUID;
	uint32 mDoorLever_GUID;
	uint32 mMrSmiteChest_GUID;
	uint32 mIronCladDoor_GUID;
	uint32 InstanceEncounter;
};

class RhahkZorAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RhahkZorAI, MoonScriptCreatureAI);
	RhahkZorAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		// Rhahk'Zor Slam
		AddSpell(6304, Target_Current, 8, 0, 3);
		AddEmote(Event_OnCombatStart, "Van Cleef pay big for your heads!", Text_Yell, 5774);
	}
};

enum MrSmiteSpells
{
	SMITE_STOMP			= 6432,
	SMITES_HAMMER		= 6436,
	SMITE_SLAM			= 6435
};

class MrSmiteAI : public MoonScriptBossAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(MrSmiteAI, MoonScriptBossAI);
	MrSmiteAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(SMITE_SLAM, Target_Current, 25, 0.0f, 15, 0.0f, 8.0f, true);
		mStomp = AddSpell(SMITE_STOMP, Target_Self, 0, 0, 0);
		mWaitAtChest = INVALIDATE_TIMER;
		SetWieldWeapon(true);
	}

	void OnCombatStop(Unit* pTarget)
	{
		if(GetPhase() == 4)
			RemoveAura(SMITES_HAMMER);
		if(!IsAlive())
			SetWieldWeapon(false);

		SetPhase(1);
		SwitchWeapons();
		RemoveTimer(mWaitAtChest);
		ParentClass::OnCombatStop(pTarget);
	}

	void AIUpdate()
	{
		if(GetHealthPercent() <= 66 && GetPhase() == 1)
		{
			Emote("You landlubbers are tougher than I thought. I'll have to improvise!", Text_Yell, 5778);
			SetPhase(2, mStomp);
		}else if(GetHealthPercent() <= 33 && GetPhase() == 3)
		{
			Emote("D'ah! Now you're making me angry!", Text_Yell, 5779);
			SetPhase(4, mStomp);
		}

		if(GetPhase() == 2 || GetPhase() == 4)
		{
			if(NearChest())
				SwitchWeapons();
			else if(_unit->GetAIInterface()->getAIState() != STATE_SCRIPTMOVE)
			{
				MoveToChest();
			}
		}

		if(IsTimerFinished(mWaitAtChest))
			MoveToPlayer();

		ParentClass::AIUpdate();
	}

	void MoveToChest()
	{
		if(GetCanEnterCombat())
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);

		StopMovement();
		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		MoveTo(1.100060f, -780.026367f, 9.811194f);
	}

	void MoveToPlayer()
	{
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
	}

	bool NearChest()
	{
		if(_unit->GetPositionX() == 1.100060f && _unit->GetPositionY() == -780.026367f)
			return true;
		else if(_unit->GetAIInterface()->getAIState() != STATE_SCRIPTMOVE)
		{
			// Too small distance - let's prevent from blocking
			float XDiff, YDiff;
			XDiff = _unit->GetPositionX() - 1.100060f;
			YDiff = _unit->GetPositionY() + 780.026367f;
			float Distance = static_cast< float >(sqrt(XDiff * XDiff + YDiff * YDiff));
			if(Distance <= 5.0f)
				return true;
		}

		return false;
	}

	void SwitchWeapons()
	{
		// CREDITS to Skyboat on ascentemu.com/forums  he had some of this info on one of his releases
		switch(GetPhase())
		{
			case 1: // Phase 1 (Default)
				SetDisplayWeaponIds(5192, 0);
				_unit->SetBaseAttackTime(MELEE, _unit->GetBaseAttackTime(MELEE));	// 1483 is taken from NCDB creature_proto
				break;
			case 2: // Phase 2
				SetDisplayWeaponIds(5196, 5196);
				_unit->SetBaseAttackTime(MELEE, _unit->GetBaseAttackTime(MELEE) / 2);
				break;
			case 4: // Phase 4
				// Is base attack time change needed if we use aura ?
				SetDisplayWeaponIds(7230, 0);
				_unit->SetBaseAttackTime(MELEE, _unit->GetBaseAttackTime(MELEE) * 2);
				ApplyAura(SMITES_HAMMER);
				break;
		}

		// Wait at the chest for 4.5seconds -- Still needs work
		_unit->setAttackTimer(4500, false);
		mWaitAtChest = AddTimer(4500);
		SetPhase(GetPhase() + 1);
	}

protected:
	SpellDesc*	mStomp;
	int		mWaitAtChest;
};

// VanCleef
class VanCleefAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(VanCleefAI, MoonScriptBossAI);
	VanCleefAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddEmote(Event_OnCombatStart, "None may challenge the brotherhood.", Text_Yell, 5780);
		AddEmote(Event_OnDied, "The Brotherhood will prevail!", Text_Yell, 5784);
		AddSpell(3391, Target_Self, 25, 0, 0);	//Thrash (Gives the caster 2 extra attacks.)
	}

	void OnTargetDied(Unit* pTarget)
	{
		char msg[200];
		if(pTarget->IsPlayer())
			sprintf(msg, "And stay down, %s.", TO_PLAYER(pTarget)->GetName());
		else if(pTarget->GetTypeFromGUID() == HIGHGUID_TYPE_PET)
			sprintf(msg, "And stay down, %s.", TO_PET(pTarget)->GetName().c_str());

		Emote(msg, Text_Yell, 5781);
		ParentClass::OnTargetDied(pTarget);
	}

	void AIUpdate()
	{
		if(GetHealthPercent() <= 75 && GetPhase() == 1)
		{
			Emote("Lap dogs, all of you.", Text_Yell, 5782);
			SetPhase(2);
		}else if(GetHealthPercent() <= 50 && GetPhase() == 2)
		{
			Emote("Fools! Our cause is righteous.", Text_Yell, 5783);
			// Defias Blackguard x 2
			for(int x = 0; x < 2; x++)
			{
				MoonScriptCreatureAI* Guard = SpawnCreature(636);
				if( Guard != NULL ){
					Guard->SetDespawnWhenInactive(true);
					Guard->GetUnit()->m_noRespawn = true;
				}
			}

			SetPhase(3);

		}else if(GetHealthPercent() <= 25 && GetPhase() == 3)
		{
			Emote("The brotherhood shall remain.", Text_Yell, 5784);
			SetPhase(4);
		}
		ParentClass::AIUpdate();
	}
};

void SetupDeadmines(ScriptMgr* mgr)
{
	mgr->register_instance_script(MAP_DEADMINES, &DeadminesInstanceScript::Create);
	mgr->register_creature_script(NPC_RHAHK_ZOR, &RhahkZorAI::Create);
	mgr->register_creature_script(NPC_MR_SMITE, &MrSmiteAI::Create);
	mgr->register_creature_script(NPC_EDWIN_VANCLEEF, &VanCleefAI::Create);
}
