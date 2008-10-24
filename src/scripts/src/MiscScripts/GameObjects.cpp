/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
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

#include "StdAfx.h"
#include "Setup.h"

class OrbOfCommand : public GameObjectAIScript
{
public:
	OrbOfCommand(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new OrbOfCommand(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->HasFinishedQuest(7761) && pPlayer->getLevel() >= 58 && pPlayer->InGroup() == true)
		{
			pPlayer->SafeTeleport(469, 0, -7672.939941f, -1107.307617f, 396.649994f, 0.616532f);
		}
		else if(pPlayer->getLevel() <= 57 || pPlayer->HasFinishedQuest(7761) == false)
		{
			pPlayer->BroadcastMessage("You need to be level 58 and have completed the quest : Blackhand's Command");
		}
		else if(pPlayer->HasFinishedQuest(7761) == true && pPlayer->getLevel() >= 58 && pPlayer->InGroup() == false)
		{
			pPlayer->BroadcastMessage("You need to be in a raid group to be able to enter this instance");
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class OrbOfTransLocUC : public GameObjectAIScript
{
public:
	OrbOfTransLocUC(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new OrbOfTransLocUC(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->getRace() == RACE_TAUREN || pPlayer->getRace() == RACE_TROLL || pPlayer->getRace() == RACE_ORC || pPlayer->getRace() == RACE_UNDEAD || pPlayer->getRace() == RACE_BLOODELF )
		{
			pPlayer->SafeTeleport(530, 0, 10021.254883f, -7014.893066f, 49.717690f, 4.00532f);		 // teleports to SilverArcScript
		}
		else
		{
			pPlayer->BroadcastMessage("You are not alowed to use this orb");
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class OrbOfTransLocSILVM : public GameObjectAIScript
{
public:
	OrbOfTransLocSILVM(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new OrbOfTransLocSILVM(GO); }

	void OnActivate(Player * pPlayer)
	{
		if( pPlayer->getRace() == RACE_TAUREN || pPlayer->getRace() == RACE_TROLL || pPlayer->getRace() == RACE_ORC || pPlayer->getRace() == RACE_UNDEAD || pPlayer->getRace() == RACE_BLOODELF )
		{
			pPlayer->SafeTeleport(0, 0, 1805.823975f, 334.009796f, 70.397469f, 4.724170f);		 // teleports to UC
		}
		else
		{
			pPlayer->BroadcastMessage("You are not alowed to use this orb");
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class FalconTowerUpper : public GameObjectAIScript
{
public:
	FalconTowerUpper(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new FalconTowerUpper(GO); }

	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(530, 0, -588.9f, 4070.8f, 93.8f, 4.724170f);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class FalconTowerLower : public GameObjectAIScript
{
public:
	FalconTowerLower(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new FalconTowerLower(GO); }

	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(530, 0, -592.2f, 4070.2f, 143.3f, 4.724170f);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class FirewingPointOrb1 : public GameObjectAIScript
{
public:
	FirewingPointOrb1(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new FirewingPointOrb1(GO); }

	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(530, 0, -2260.6f, 3186.5f, 23.6f, 4.724170f);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class FirewingPointOrb2 : public GameObjectAIScript
{
public:
	FirewingPointOrb2(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new FirewingPointOrb2(GO); }

	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(530, 0, -2250.4f, 3120.7f, 27.3f, 4.724170f);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class duskwitherspirebottom : public GameObjectAIScript
{
public:
	duskwitherspirebottom(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new duskwitherspirebottom(GO); }

	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(530, 0, 9334.352f, -7880.743f, 74.910f, 4.724170f);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class duskwitherspiretop : public GameObjectAIScript
{
public:
	duskwitherspiretop(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new duskwitherspiretop(GO); }

	void OnActivate(Player * pPlayer)
	{
		pPlayer->SafeTeleport(530, 0, 9291.155f, -7913.556f, 176.130f, 4.724170f);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class Blacksmithing_Plans_Use : public GameObjectAIScript
{
public:
	Blacksmithing_Plans_Use(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new Blacksmithing_Plans_Use(GO); }

	void OnLootTaken(Player * pLooter, ItemPrototype *pItemInfo)
	{
		float SSX = pLooter->GetPositionX();
		float SSY = pLooter->GetPositionY();
		float SSZ = pLooter->GetPositionZ();
		float SSO = pLooter->GetOrientation();
				
		pLooter->GetMapMgr()->GetInterface()->SpawnCreature(11120, SSX, SSY, SSZ, SSO, true, false, 0, 0)->Despawn(600000, 0);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class GongOfBethekk : public GameObjectAIScript
{
public:
	GongOfBethekk(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new GongOfBethekk(GO); }

	void OnActivate(Player * pPlayer)
	{
		pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(14515, -11556.3f, -1628.32f, 41.299f, 4.1f, true, false, 0, 0)->Despawn(1200000, 0);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class TerokksDownfall : public GameObjectAIScript
{
public:
	TerokksDownfall(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new TerokksDownfall(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->GetQuestLogForEntry(11073) && pPlayer->GetItemInterface()->GetItemCount(32720, 1))
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();
				
			pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(21838, SSX, SSY, SSZ, SSO, true, false, 0, 0)->Despawn(600000, 0);
		}
		else
		{
			pPlayer->BroadcastMessage("You need to have item : Time-Lost Offering and to have quest : Terokk's Downfall");
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class VilebranchKidnapper : public GameObjectAIScript
{
public:
	VilebranchKidnapper(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new VilebranchKidnapper(GO); }

	void OnActivate(Player * pPlayer)
	{
		float SSX = pPlayer->GetPositionX();
		float SSY = pPlayer->GetPositionY();
		float SSZ = pPlayer->GetPositionZ();
		float SSO = pPlayer->GetOrientation();
				
		pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(14748, SSX+1, SSY, SSZ, SSO, true, false, 0, 0)->Despawn(600000, 0);
		pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(14748, SSX, SSY, SSZ, SSO, true, false, 0, 0)->Despawn(600000, 0);
		pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(14748, SSX-1, SSY, SSZ, SSO, true, false, 0, 0)->Despawn(600000, 0);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class GongOfZulFarrak : public GameObjectAIScript
{
public:
	GongOfZulFarrak(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new GongOfZulFarrak(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->GetItemInterface()->GetItemCount(9240, 1))
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();
				
			pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(7273, SSX+1, SSY, SSZ, SSO, true, false, 0, 0)->Despawn(1800000, 0);
		}
		else
		{
			pPlayer->BroadcastMessage("Missing required item : Mallet of Zul'Farrak");
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class Obsidias_Egg : public GameObjectAIScript
{
public:
	Obsidias_Egg(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new Obsidias_Egg(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->GetQuestLogForEntry(11078))
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();
				
			pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(23282, SSX, SSY+1, SSZ, SSO, true, false, 0, 0)->Despawn(600000, 0);
		}
		else
		{
			pPlayer->BroadcastMessage("Missing required quest : To Rule The Skies");
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class Rivendarks_Egg : public GameObjectAIScript
{
public:
	Rivendarks_Egg(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new Obsidias_Egg(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->GetQuestLogForEntry(11078))
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();
				
			pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(23061, SSX, SSY+1, SSZ, SSO, true, false, 0, 0)->Despawn(600000, 0);
		}
		else
		{
			pPlayer->BroadcastMessage("Missing required quest : To Rule The Skies");
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class Furywings_Egg : public GameObjectAIScript
{
public:
	Furywings_Egg(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new Furywings_Egg(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->GetQuestLogForEntry(11078))
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();
				
			pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(23261, SSX, SSY+1, SSZ, SSO, true, false, 0, 0)->Despawn(600000, 0);
		}
		else
		{
			pPlayer->BroadcastMessage("Missing required quest : To Rule The Skies");
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class Insidions_Egg : public GameObjectAIScript
{
public:
	Insidions_Egg(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new Insidions_Egg(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->GetQuestLogForEntry(11078))
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();
				
			pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(23281, SSX, SSY+1, SSZ, SSO, true, false, 0, 0)->Despawn(600000, 0);
		}
		else
		{
			pPlayer->BroadcastMessage("Missing required quest : To Rule The Skies");
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class Corrupt_Minor_Manifestation_Water_Object : public GameObjectAIScript
{
public:
	Corrupt_Minor_Manifestation_Water_Object(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new Corrupt_Minor_Manifestation_Water_Object(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->GetQuestLogForEntry(63))
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();
				
			pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(5894, SSX, SSY+1, SSZ, SSO, true, false, 0, 0)->Despawn(600000, 0);
		}
		else
		{
			pPlayer->BroadcastMessage("Missing required quest : Call of Water");
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class Telathion_the_Impure_Object : public GameObjectAIScript
{
public:
	Telathion_the_Impure_Object(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new Telathion_the_Impure_Object(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->GetQuestLogForEntry(9508))
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();
				
			pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(17359, SSX, SSY+1, SSZ, SSO, true, false, 0, 0)->Despawn(600000, 0);
		}
		else
		{
			pPlayer->BroadcastMessage("Missing required quest : Call of Water");
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class UlagTheCleaver : public GameObjectAIScript
{
public:
	UlagTheCleaver(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new UlagTheCleaver(GO); }

	void OnActivate(Player * pPlayer)
	{
		
		Creature * Ulag = NULL;
		GameObject * pDoor = NULL;
		Ulag = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2390.101807f, 336.676788f, 40.015614f, 6390);
		pDoor = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2388.480029f, 338.3901f, 40.092899f, 176594);
		QuestLogEntry * en = pPlayer->GetQuestLogForEntry(1819);
		if(!en || pDoor==0 || Ulag)
		return;

		pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(6390, 2390.101807f, 336.676788f, 40.015614f, 2.259590f, true, false, 0, 0)->Despawn(180000, 0);
		Creature * pCreature = NULL;
		pCreature = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(2390.101807f, 336.676788f, 40.015614f, 6390);
		pCreature->GetAIInterface()->SetNextTarget(pPlayer);
		pCreature->GetAIInterface()->AttackReaction(pPlayer, 1);
		pDoor->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
		pDoor->SetUInt32Value(GAMEOBJECT_STATE, 0);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

class DustySpellbooks : public GameObjectAIScript
{
public:
	DustySpellbooks(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new DustySpellbooks(GO); }

	void OnLootTaken(Player * pLooter, ItemPrototype *pItemInfo)
	{
		QuestLogEntry * en = pLooter->GetQuestLogForEntry(422);
		if(!en)
		return;

		float SSX = pLooter->GetPositionX();
		float SSY = pLooter->GetPositionY();
		float SSZ = pLooter->GetPositionZ();
		float SSO = pLooter->GetOrientation();
		pLooter->GetMapMgr()->GetInterface()->SpawnCreature(1770, SSX, SSY, SSZ, SSO, true, false, 0, 0)->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Sons of Arugal will rise against all who challenge the power of the ArcScriptrage!");
	}
};

/*--------------------------------------------------------------------------------------------------------*/

#define REPAIR_BOT_REQUIRED_SKILL_LINE 202
#define REPAIR_BOT_REQUIRED_SKILL 300
class LearnFieldRepairBot : public GameObjectAIScript
{
public:
	LearnFieldRepairBot(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new LearnFieldRepairBot(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(!pPlayer)
			return;
		
		_gameobject->SetUInt32Value(GAMEOBJECT_TYPE_ID, 5);
		
		if(!pPlayer->_HasSkillLine(REPAIR_BOT_REQUIRED_SKILL_LINE))
		{
			return;
		}

		if(pPlayer->_GetSkillLineCurrent(REPAIR_BOT_REQUIRED_SKILL_LINE, false) < REPAIR_BOT_REQUIRED_SKILL)
		{
			return;
		}
		_gameobject->SetUInt32Value(GAMEOBJECT_TYPE_ID, 22);
	}
};

/*--------------------------------------------------------------------------------------------------------*/

void SetupGoHandlers(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(179879, &OrbOfCommand::Create);
	mgr->register_gameobject_script(184503, &OrbOfTransLocUC::Create);
	mgr->register_gameobject_script(184502, &OrbOfTransLocSILVM::Create);
	mgr->register_gameobject_script(184500, &FalconTowerUpper::Create);
	mgr->register_gameobject_script(184501, &FalconTowerLower::Create);
	mgr->register_gameobject_script(182543, &FirewingPointOrb1::Create);
	mgr->register_gameobject_script(182548, &FirewingPointOrb2::Create);
	mgr->register_gameobject_script(180912, &duskwitherspirebottom::Create);
	mgr->register_gameobject_script(180911, &duskwitherspiretop::Create);
	mgr->register_gameobject_script(173232, &Blacksmithing_Plans_Use::Create);
	mgr->register_gameobject_script(180526, &GongOfBethekk::Create);
	mgr->register_gameobject_script(185928, &TerokksDownfall::Create);
	mgr->register_gameobject_script(179910, &VilebranchKidnapper::Create);
	mgr->register_gameobject_script(141832, &GongOfZulFarrak::Create);
	mgr->register_gameobject_script(185932, &Obsidias_Egg::Create);
	mgr->register_gameobject_script(185936, &Rivendarks_Egg::Create);
	mgr->register_gameobject_script(185937, &Furywings_Egg::Create);
	mgr->register_gameobject_script(185938, &Insidions_Egg::Create);
	mgr->register_gameobject_script(113791, &Corrupt_Minor_Manifestation_Water_Object::Create);
	mgr->register_gameobject_script(181699, &Telathion_the_Impure_Object::Create);
	mgr->register_gameobject_script(104593, &UlagTheCleaver::Create);
	mgr->register_gameobject_script(1571, &DustySpellbooks::Create);
	mgr->register_gameobject_script(179552, &LearnFieldRepairBot::Create);
}
