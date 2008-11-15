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
#include "EAS/EasyFunctions.h"



/*--------------------------------------------------------------------------------------------------------*/
// Lost!

bool SpragglesCanteen(uint32 i, Spell* pSpell)
{
  if(!pSpell->u_caster->IsPlayer())
    return true;

  Player *plr = (Player*)pSpell->u_caster;
  
  Creature *target = plr->GetMapMgr()->GetCreature((uint32)plr->GetSelection());
  if(target == NULL)
    return true;

  if(target->GetEntry() != 9999)
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(4492);
  if(qle == NULL)
    return true;

  target->SetStandState(0);
  target->setDeathState(ALIVE);

  target->Despawn(30*1000, 1*60*1000);

  qle->SetMobCount(0, 1);
  qle->SendUpdateAddKill(0);
  qle->UpdatePlayerFields();

  return true;
}

class RingoDeadNPC : public CreatureAIScript
{
public:
  ADD_CREATURE_FACTORY_FUNCTION(RingoDeadNPC);
  RingoDeadNPC(Creature* pCreature) : CreatureAIScript(pCreature) {}

  void OnLoad()
  {
    _unit->SetStandState(7);
    _unit->setDeathState(CORPSE);
    _unit->GetAIInterface()->m_canMove = false;
  }
};



/*--------------------------------------------------------------------------------------------------------*/
// The Northern Pylon

class NorthernPylon : public GameObjectAIScript
{
public:
	NorthernPylon(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new NorthernPylon(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->HasFinishedQuest(4284))
		{
			QuestLogEntry *en = pPlayer->GetQuestLogForEntry(4285);
			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				uint32 newcount = en->GetMobCount(0) + 1;
				en->SetMobCount(0, newcount);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();
				return;
			}
		}
		else if(pPlayer->HasFinishedQuest(4284) == false)
		{
			pPlayer->BroadcastMessage("You need to have completed the quest : Crystals of Power");
		}
	}
};



/*--------------------------------------------------------------------------------------------------------*/
// The Eastern Pylon

class EasternPylon : public GameObjectAIScript
{
public:
	EasternPylon(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new EasternPylon(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->HasFinishedQuest(4284))
		{
			QuestLogEntry *en = pPlayer->GetQuestLogForEntry(4287);
			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				uint32 newcount = en->GetMobCount(0) + 1;
				en->SetMobCount(0, newcount);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();
				return;
			}
		}
		else if(pPlayer->HasFinishedQuest(4284) == false)
		{
			pPlayer->BroadcastMessage("You need to have completed the quest : Crystals of Power");
		}
	}
};



/*--------------------------------------------------------------------------------------------------------*/
//The Western Pylon

class WesternPylon : public GameObjectAIScript
{
public:
	WesternPylon(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new WesternPylon(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->HasFinishedQuest(4284))
		{
			QuestLogEntry *en = pPlayer->GetQuestLogForEntry(4288);
			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				uint32 newcount = en->GetMobCount(0) + 1;
				en->SetMobCount(0, newcount);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();
				return;
			}	
		}
		else if(pPlayer->HasFinishedQuest(4284) == false)
		{
			pPlayer->BroadcastMessage("You need to have completed the quest : Crystals of Power");
		}
	}
};



/*--------------------------------------------------------------------------------------------------------*/
// The Bait for Lar'korwi

bool TheBaitforLarkorwi1(uint32 i, Spell* pSpell)
{
  	Player *pPlayer = (Player*)pSpell->u_caster;
  	if(!pPlayer)
	  	return true;

  	if(!pSpell->u_caster->IsPlayer())
    		return true;

  	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(4292);
  	if(qle == NULL)
    		return true;
  
	GameObject* obj = 0;

	obj = sEAS.SpawnGameobject(pPlayer, 169216, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 1);
     	sEAS.GameobjectDelete(obj, 1*60*1000);
     	
  	return true;
}

bool TheBaitforLarkorwi2(uint32 i, Spell* pSpell)
{
  	Player *pPlayer = (Player*)pSpell->u_caster;
  	if(!pPlayer)
	  	return true;

  	if(!pSpell->u_caster->IsPlayer())
    		return true;

  	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(4292);
  	if(qle == NULL)
    		return true;
  
	Creature *larkowi = sEAS.SpawnCreature(pPlayer, 9684, pPlayer->GetPositionX()+2, pPlayer->GetPositionY()+3, pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
  	larkowi->Despawn(5*60*1000, 0);

  	return true;
}


void SetupUnGoro(ScriptMgr * mgr)
{
  	mgr->register_dummy_spell(15591, &SpragglesCanteen);
  	mgr->register_creature_script(9999, &RingoDeadNPC::Create);
  	mgr->register_gameobject_script(164955, &NorthernPylon::Create);
  	mgr->register_gameobject_script(164957, &EasternPylon::Create);
  	mgr->register_gameobject_script(164956, &WesternPylon::Create);
  	mgr->register_dummy_spell(15118, &TheBaitforLarkorwi1);
  	mgr->register_dummy_spell(15119, &TheBaitforLarkorwi2);
}
