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
// Fel Orc Scavengers

class FelOrcScavengersQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FelOrcScavengersQAI);
    FelOrcScavengersQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit * mKiller)
	{
		if (mKiller->IsPlayer())
		{
			QuestLogEntry *en = NULL;
			en = static_cast<Player*>(mKiller)->GetQuestLogForEntry(10482);
			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				uint32 newcount = en->GetMobCount(0) + 1;
				en->SetMobCount(0, newcount);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();
			}
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/
// A Burden Of Souls

class BurdenOfSoulsQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BurdenOfSoulsQAI);
    BurdenOfSoulsQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit * mKiller)
	{
		if(mKiller->GetTypeId() != TYPEID_PLAYER)
			return;

		QuestLogEntry *en = NULL;
		en = static_cast<Player*>(mKiller)->GetQuestLogForEntry(10864);
		if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
		{
			uint32 newcount = en->GetMobCount(0) + 1;
			en->SetMobCount(0, newcount);
			en->SendUpdateAddKill(0);
			en->UpdatePlayerFields();
		}
	}
};



/*--------------------------------------------------------------------------------------------------------*/
// Testing the Antidote

bool TestingTheAntidote(uint32 i, Spell* pSpell)
{
	if(!pSpell->GetUnitTarget() || pSpell->GetUnitTarget()->GetTypeId() != TYPEID_UNIT)
		return true;

	Creature *target = NULL;
	target = static_cast<Creature*>(pSpell->GetUnitTarget());
	if(!target || target->GetEntry() != 16880) // Hulking Helboar
		return true;

	Creature *spawned = NULL;
	spawned = target->GetMapMgr()->GetInterface()->SpawnCreature(16992, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), false, false, 0, 0);
	if(!spawned)
		return true;

	target->Despawn(0, 300000);

	spawned->GetAIInterface()->SetNextTarget(pSpell->u_caster);

	return true;
}

class Dreadtusk : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Dreadtusk);
	Dreadtusk(Creature* pCreature) : CreatureAIScript(pCreature) { }
	void OnDied(Unit *mKiller)
	{
		if(mKiller->GetTypeId() != TYPEID_PLAYER)
			return;

		QuestLogEntry *qle = NULL;
		qle = static_cast<Player*>(mKiller)->GetQuestLogForEntry(10255);
		if(qle)
		{
			qle->SetMobCount(0,1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/
// Zeth'Gor Must Burn!

class ZethGorMustBurnAllianz : public GameObjectAIScript
{
public:
	ZethGorMustBurnAllianz(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new ZethGorMustBurnAllianz(GO); }

	void OnActivate(Player * pPlayer)
	{
		float SSX = pPlayer->GetPositionX();
		float SSY = pPlayer->GetPositionY();
		float SSZ = pPlayer->GetPositionZ();

		GameObject *nothern = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-820, 2029, 55, 300150);
		GameObject *southern = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-1150, 2110, 84, 300150);
		GameObject *forge = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-893, 1919, 82, 300150);
		GameObject *foothill = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-978, 1879, 111, 300150);
		GameObject *Beacon = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( SSX, SSY, SSZ, 184661);

		if(Beacon)
		{
			Beacon->SetUInt32Value(GAMEOBJECT_FLAGS, (Beacon->GetUInt32Value(GAMEOBJECT_FLAGS)-1));
		}

		if(pPlayer->GetQuestLogForEntry(10895))
		{
			// Northern Zeth'Gor Tower
			if (nothern != NULL)
                        {
                        	if(pPlayer->CalcDistance(pPlayer, nothern) < 40) // if reduced the server will crash when out of range
                        	{
					QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10895);

	      				qle->SetMobCount(0, qle->GetMobCount(0)+1);
					qle->SendUpdateAddKill(0);
					qle->UpdatePlayerFields();

					GameObject* obj = 0;

					obj = sEAS.SpawnGameobject(pPlayer, 183816, -819.77, 2029.09, 55.6082, 0, 4);
     					sEAS.GameobjectDelete(obj, 1*60*1000);

					return;
				}
			}

			// Southern Zeth'Gor Tower
			if (southern != NULL)
                        {
                        	if(pPlayer->CalcDistance(pPlayer, southern) < 40)
                        	{
					QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10895);
	
					qle->SetMobCount(1, qle->GetMobCount(1)+1);
					qle->SendUpdateAddKill(1);
					qle->UpdatePlayerFields();
	
					GameObject* obj = 0;
	
					obj = sEAS.SpawnGameobject(pPlayer, 183816, -1150.53, 2109.92, 84.4204, 0, 4);
	     				sEAS.GameobjectDelete(obj, 1*60*1000);
	
					return;
				}
			}

			// Forge Zeth'Gor Tower
			if (forge)
                        {
                        	if(pPlayer->CalcDistance(pPlayer, forge) < 40)
                        	{
					QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10895);
	
					qle->SetMobCount(2, qle->GetMobCount(2)+1);
					qle->SendUpdateAddKill(2);
					qle->UpdatePlayerFields();
	
					GameObject* obj = 0;
	
					obj = sEAS.SpawnGameobject(pPlayer, 183816, -893.499, 1919.27, 81.6449, 0, 4);
	     				sEAS.GameobjectDelete(obj, 1*60*1000);
	
					return;
				}
			}

			// Foothill Zeth'Gor Tower
			if (foothill)
                        {
                        	if(pPlayer->CalcDistance(pPlayer, foothill) < 40)
                        	{
					QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10895);
	
					qle->SetMobCount(3, qle->GetMobCount(3)+1);
					qle->SendUpdateAddKill(3);
					qle->UpdatePlayerFields();
	
					GameObject* obj = 0;
	
					obj = sEAS.SpawnGameobject(pPlayer, 183816, -977.713, 1879.500, 110.892, 0, 4);
	     				sEAS.GameobjectDelete(obj, 1*60*1000);
	
					return;
				}
			}
			else
			{
				pPlayer->BroadcastMessage("You are to far away!");
			}

		}
		else
		{
				pPlayer->BroadcastMessage("Missing required quest : Zeth'Gor Must Burn");
		}
	}
};

/*--------------------------------------------------------------------------------------------------------*/
// Zeth'Gor Must Burn!

bool ZethGorMustBurnHorde(uint32 i, Spell* pSpell)
{
	Player *caster = pSpell->p_caster;
	if(caster == NULL)
		return true;
	
	GameObject *barracks = caster->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-1137, 1970, 74, 300151);
	GameObject *eastern = caster->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-940, 1920, 69, 300151);
	GameObject *western = caster->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-1155, 2061, 68, 300151);
	GameObject *stable = caster->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-1052, 2007, 66, 300151);

	if(caster->GetQuestLogForEntry(10792))
	{
		// Barracks
		if (barracks)
        {
            if(caster->CalcDistance(caster, barracks) < 30)
            {
				QuestLogEntry *qle = caster->GetQuestLogForEntry(10792);
	
	      			qle->SetMobCount(0, qle->GetMobCount(0)+1);
				qle->SendUpdateAddKill(0);
				qle->UpdatePlayerFields();
	
				GameObject* obj = 0;
	
				obj = sEAS.SpawnGameobject(caster, 183816, -1129.08, 1921.77, 94.0074, 0, 4);
	     			sEAS.GameobjectDelete(obj, 1*60*1000);
	     			obj = sEAS.SpawnGameobject(caster, 183816, -1135.00, 1944.05, 84.7084, 0, 4);
	     			sEAS.GameobjectDelete(obj, 1*60*1000);
	     			obj = sEAS.SpawnGameobject(caster, 183816, -1152.01, 1945.00, 102.901, 0, 4);
	     			sEAS.GameobjectDelete(obj, 1*60*1000);
	     			obj = sEAS.SpawnGameobject(caster, 183816, -1159.60, 1958.76, 83.0412, 0, 4);
	     			sEAS.GameobjectDelete(obj, 1*60*1000);
	     			obj = sEAS.SpawnGameobject(caster, 183816, -1126.17, 1880.96, 95.065, 0, 4);
	     			sEAS.GameobjectDelete(obj, 1*60*1000);
	     			obj = sEAS.SpawnGameobject(caster, 183816, -1185.79, 1968.29, 90.931, 0, 4);
	     			sEAS.GameobjectDelete(obj, 1*60*1000);
			}

			return true;
		}

		// Eastern Hovel
		if (eastern)
        {
            if(caster->CalcDistance(caster, eastern) < 30)
            {
				QuestLogEntry *qle = caster->GetQuestLogForEntry(10792);
	
	      			qle->SetMobCount(1, qle->GetMobCount(1)+1);
				qle->SendUpdateAddKill(1);
				qle->UpdatePlayerFields();
	
				GameObject* obj = 0;
	
				obj = sEAS.SpawnGameobject(caster, 183816, -938.034, 1924.153, 73.590, 0, 4);
	     			sEAS.GameobjectDelete(obj, 1*60*1000);
			}

			return true;
		}

		// Western Hovel
		if (western)
        {
            if(caster->CalcDistance(caster, western) < 30)
            {
				QuestLogEntry *qle = caster->GetQuestLogForEntry(10792);
	
	      			qle->SetMobCount(2, qle->GetMobCount(2)+1);
				qle->SendUpdateAddKill(2);
				qle->UpdatePlayerFields();
	
				GameObject* obj = 0;
	
				obj = sEAS.SpawnGameobject(caster, 183816, -1152.10, 2066.20, 72.959, 0, 4);
	     			sEAS.GameobjectDelete(obj, 1*60*1000);
			}

			return true;
		}

		// Stable
		if (stable)
        {
            if(caster->CalcDistance(caster, stable) < 30)
            {
				QuestLogEntry *qle = caster->GetQuestLogForEntry(10792);
	
	      			qle->SetMobCount(3, qle->GetMobCount(3)+1);
				qle->SendUpdateAddKill(3);
				qle->UpdatePlayerFields();
	
				GameObject* obj = 0;
	
				obj = sEAS.SpawnGameobject(caster, 183816, -1058.85, 2010.95, 68.776, 0, 4);
	     			sEAS.GameobjectDelete(obj, 1*60*1000);
			}

			return true;
		}
	}
	else
	{
		caster->BroadcastMessage("Missing required quest : Zeth'Gor Must Burn");
	}

	return true;
}

/*--------------------------------------------------------------------------------------------------------*/
// Laying Waste to the Unwanted

bool LayingWasteToTheUnwantedAllianz(uint32 i, Spell* pSpell)
{
	Player *caster = pSpell->p_caster;
	if(caster == NULL)
		return true;
	
	GameObject *eastern = caster->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-155, 2517, 43, 300152);
	GameObject *ceastern = caster->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-152, 2661, 44, 300152);
	GameObject *cwestern = caster->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-174, 2772, 32, 300152);
	GameObject *western = caster->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-166, 2818, 29, 300152);

	if(caster->GetQuestLogForEntry(10078))
	{
		// Eastern Thrower
		if (eastern)
		{
			if(caster->CalcDistance(caster, eastern) < 30)
            {
				QuestLogEntry *qle = caster->GetQuestLogForEntry(10078);
	
	      			qle->SetMobCount(0, qle->GetMobCount(0)+1);
				qle->SendUpdateAddKill(0);
				qle->UpdatePlayerFields();
	
				GameObject* obj = 0;
	
				obj = sEAS.SpawnGameobject(caster, 183816, -157.916, 2517.71, 58.5508, 0, 4);
	     			sEAS.GameobjectDelete(obj, 1*60*1000);
			}

			return true;
		}

		// Central Eastern Thrower
		if (ceastern)
		{
			if(caster->CalcDistance(caster, ceastern) < 30)
			{
				QuestLogEntry *qle = caster->GetQuestLogForEntry(10078);
	
	      			qle->SetMobCount(1, qle->GetMobCount(1)+1);
				qle->SendUpdateAddKill(1);
				qle->UpdatePlayerFields();
	
				GameObject* obj = 0;
	
				obj = sEAS.SpawnGameobject(caster, 183816, -152.527, 2661.99, 60.8123, 0, 4);
	     			sEAS.GameobjectDelete(obj, 1*60*1000);
			}

			return true;
		}

		// Central Western Thrower
		if (cwestern)
		{
			if(caster->CalcDistance(caster, cwestern) < 30)
			{
				QuestLogEntry *qle = caster->GetQuestLogForEntry(10078);
	
	      			qle->SetMobCount(2, qle->GetMobCount(2)+1);
				qle->SendUpdateAddKill(2);
				qle->UpdatePlayerFields();
	
				GameObject* obj = 0;
	
				obj = sEAS.SpawnGameobject(caster, 183816, -177.916, 2773.75, 48.636, 0, 4);
	     			sEAS.GameobjectDelete(obj, 1*60*1000);
			}

			return true;
		}

		// Western Thrower
		if (western)
		{
			if(caster->CalcDistance(caster, western) < 30)
			{
				QuestLogEntry *qle = caster->GetQuestLogForEntry(10078);
	
	      			qle->SetMobCount(3, qle->GetMobCount(3)+1);
				qle->SendUpdateAddKill(3);
				qle->UpdatePlayerFields();
	
				GameObject* obj = 0;
	
				obj = sEAS.SpawnGameobject(caster, 183816, -166, 2818, 29, 0, 4);
	     			sEAS.GameobjectDelete(obj, 1*60*1000);
			}

			return true;
		}
	}
	else
	{
		caster->BroadcastMessage("Missing required quest : Laying Waste to the Unwanted");
	}

	return true;
}


/*--------------------------------------------------------------------------------------------------------*/
// Burn It Up... For the Horde!

bool BurnItUp(uint32 i, Spell* pSpell)
{
	Player *pPlayer = (Player*)pSpell->u_caster;
	if(!pPlayer)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10087);
	if(qle == NULL)
		return true;

	GameObject *east = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-300, 2407, 50, 183122);
	GameObject *west = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-350, 2708, 35, 185122);


	if(east)
	{
		if(pPlayer->CalcDistance(pPlayer, east) < 30)
		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			qle->SetMobCount(0, qle->GetMobCount(0)+1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
			
			GameObject* obj = 0;

			obj = sEAS.SpawnGameobject(pPlayer, 183816, -300, 2407, 50, 0, 4);
     			sEAS.GameobjectDelete(obj, 1*60*1000);

			return true;
		}
	}

	if(west)
	{
		if(pPlayer->CalcDistance(pPlayer, west) < 30)
		if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
		{
			qle->SetMobCount(1, qle->GetMobCount(1)+1);
			qle->SendUpdateAddKill(1);
			qle->UpdatePlayerFields();
			
			GameObject* obj = 0;

			obj = sEAS.SpawnGameobject(pPlayer, 183816, -350, 2708, 35, 0, 4);
     			sEAS.GameobjectDelete(obj, 1*60*1000);

			return true;
		}
	}
	return true;
}



/*--------------------------------------------------------------------------------------------------------*/
// Cleansing the Waters

bool CleansingtheWater(uint32 i, Spell* pSpell) 
{
  Player *pPlayer = (Player*)pSpell->u_caster;
  if(!pPlayer)
	  return true;
  if(!pSpell->u_caster->IsPlayer())
    return true;

  QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9427);
  if(qle == NULL)
    return true;

  Creature *agg = sEAS.SpawnCreature(pPlayer, 17000, 428.15, 3461.73, 63.40, 0, 0);
  agg->Despawn(6*60*1000, 0);
  return true;
}



/*--------------------------------------------------------------------------------------------------------*/
// The Dreghood Elders

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, pPlayer); \
    Menu->SendTo(pPlayer);


class SCRIPT_DECL Prisoner1 : public GossipScript
{
public:
    	void GossipHello(Object* pObject, Player* pPlayer, bool AutoSend)
    	{
		if(!pPlayer)
			return;

        	GossipMenu *Menu;
		Creature *Prisoner1 = (Creature*)(pObject);
		
		if (Prisoner1 == NULL)
			return;
		
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10104, pPlayer);
		
		QuestLogEntry *en = pPlayer->GetQuestLogForEntry(10368);
		if(en && en->GetMobCount(0) == en->GetQuest()->required_mobcount[0])
			return;

		if(pPlayer->GetQuestLogForEntry(10368) && pPlayer->GetItemInterface()->GetItemCount(29501) > 0)
			Menu->AddItem( 0, "Walk free, Elder. Bring the spirits back to your tribe.", 1);
	 
		if(AutoSend)
			Menu->SendTo(pPlayer);
    	}
 
	void GossipSelectOption(Object* pObject, Player* pPlayer, uint32 Id, uint32 IntId, const char * EnteredCode)
    	{
		if(!pPlayer)
			return;

		Creature *Prisoner1 = (Creature*)(pObject);
		if (Prisoner1 == NULL)
			return;

		switch (IntId)
		{
			case 0:
				GossipHello(pObject, pPlayer, true);
				break;
 
			case 1:
			{
			QuestLogEntry *en = pPlayer->GetQuestLogForEntry(10368);
			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				en->SetMobCount(0, en->GetMobCount(0) + 1);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();

				if(!Prisoner1)
					return;

				Prisoner1->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You've freed me! The winds speak to my people one again and grant us their strength. I thank you, stranger.");
				Prisoner1->Despawn(5000, 6*60*1000);
				Prisoner1->SetStandState(STANDSTATE_STAND);
				return;
			}break;
			}
		}
	}
 
    	void Destroy()
    	{
       		delete this;
    	}
};


class SCRIPT_DECL Prisoner2 : public GossipScript
{
public:
    	void GossipHello(Object* pObject, Player* pPlayer, bool AutoSend)
    	{
		if(!pPlayer)
			return;

        GossipMenu *Menu;
		Creature *Prisoner2 = (Creature*)(pObject);
		if (Prisoner2 == NULL)
			return;
			
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10105, pPlayer);

		QuestLogEntry *en = pPlayer->GetQuestLogForEntry(10368);
		if(en && en->GetMobCount(0) == en->GetQuest()->required_mobcount[0])
			return;

		if(pPlayer->GetQuestLogForEntry(10368) && pPlayer->GetItemInterface()->GetItemCount(29501) > 0)
			Menu->AddItem( 0, "Walk free, Elder. Bring the spirits back to your tribe.", 1);
	 
		if(AutoSend)
			Menu->SendTo(pPlayer);
    	}
 
	void GossipSelectOption(Object* pObject, Player* pPlayer, uint32 Id, uint32 IntId, const char * EnteredCode)
    	{
		if(!pPlayer)
			return;

		Creature *Prisoner2 = (Creature*)(pObject);
		if (Prisoner2 == NULL)
			return;

		switch (IntId)
		{
			case 0:
				GossipHello(pObject, pPlayer, true);
				break;
 
			case 1:
			{
			QuestLogEntry *en = pPlayer->GetQuestLogForEntry(10368);
			if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
			{
				en->SetMobCount(1, en->GetMobCount(1) + 1);
				en->SendUpdateAddKill(1);
				en->UpdatePlayerFields();

				if(!Prisoner2)
					return;

				Prisoner2->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I am free! I will ask the spirit of fire to return to us so that my people may have the courage to fight their masters! I thank you.");
				Prisoner2->Despawn(5000, 6*60*1000);
				Prisoner2->SetStandState(STANDSTATE_STAND);
				return;
			}break;
			}
		}
	}
 
    	void Destroy()
    	{
        	delete this;
    	}
};


class SCRIPT_DECL Prisoner3 : public GossipScript
{
public:
    	void GossipHello(Object* pObject, Player* pPlayer, bool AutoSend)
    	{
		if(!pPlayer)
			return;

        GossipMenu *Menu;
		Creature *Prisoner3 = (Creature*)(pObject);
		if (Prisoner3 == NULL)
			return;

		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10103, pPlayer);

		QuestLogEntry *en = pPlayer->GetQuestLogForEntry(10368);
		if(en && en->GetMobCount(0) == en->GetQuest()->required_mobcount[0])
			return;

		if(pPlayer->GetQuestLogForEntry(10368) && pPlayer->GetItemInterface()->GetItemCount(29501) > 0)
			Menu->AddItem( 0, "Walk free, Elder. Bring the spirits back to your tribe.", 1);
	 
		if(AutoSend)
			Menu->SendTo(pPlayer);
    	}
 
	void GossipSelectOption(Object* pObject, Player* pPlayer, uint32 Id, uint32 IntId, const char * EnteredCode)
    	{
		if(!pPlayer)
			return;

		Creature *Prisoner3 = (Creature*)(pObject);
		if (Prisoner3 == NULL)
			return;

		switch (IntId)
		{
			case 0:
				GossipHello(pObject, pPlayer, true);
				break;
 
			case 1:
			{
			QuestLogEntry *en = pPlayer->GetQuestLogForEntry(10368);
			if(en && en->GetMobCount(2) < en->GetQuest()->required_mobcount[2])
			{
				en->SetMobCount(2, en->GetMobCount(2) + 1);
				en->SendUpdateAddKill(2);
				en->UpdatePlayerFields();

				if(!Prisoner3)
					return;

				Prisoner3->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I'm free! The spirit of water returns to my people. It will bring us the wisdom we need to survive in this harsh land. I am in your debt.");
				Prisoner3->Despawn(5000, 6*60*1000);
				Prisoner3->SetStandState(STANDSTATE_STAND);
				return;
			}break;
			}
		}
	}
 
    	void Destroy()
    	{
        	delete this;
    	}
};

class PrisonersDreghoodElders : public CreatureAIScript
{
public:
  	ADD_CREATURE_FACTORY_FUNCTION(PrisonersDreghoodElders);
  	PrisonersDreghoodElders(Creature* pCreature) : CreatureAIScript(pCreature) {}

  	void OnLoad()
  	{
    		_unit->SetStandState(STANDSTATE_SIT);
    		_unit->setDeathState(CORPSE);
    		_unit->GetAIInterface()->m_canMove = false;
  	}
};



/*--------------------------------------------------------------------------------------------------------*/
// The Seer's Relic

bool TheSeersRelic(uint32 i, Spell* pSpell)
{
  if(!pSpell->u_caster->IsPlayer())
    return true;

  Player *plr = (Player*)pSpell->u_caster;
  
  Creature *target = plr->GetMapMgr()->GetCreature((uint32)plr->GetSelection());
  if(target == NULL)
    return true;

  if(target->GetEntry() != 16852)
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(9545);
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


/*--------------------------------------------------------------------------------------------------------*/
// Disrupt Their Reinforcements


bool DisruptTheirReinforcements(uint32 i, Spell* pSpell)
{
	Player *pPlayer = (Player*)pSpell->u_caster;
	if(!pPlayer)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;
	
	//Alliace
	GameObject *grimh = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-419, 1847, 80, 184414);
	GameObject *kaalez = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-548, 1782, 58, 184415);
	//Horde
	GameObject *xilus = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-85, 1880, 74, 184290);	
	GameObject *kruul = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(147, 1717, 38, 184289);


	QuestLogEntry *qleA = pPlayer->GetQuestLogForEntry(10144);
	QuestLogEntry *qleH = pPlayer->GetQuestLogForEntry(10208);

	if(qleA)
	{
		if(grimh)
		{
			if(pPlayer->CalcDistance(pPlayer, grimh) < 10)
			{
				if(qleA->GetMobCount(0) < qleA->GetQuest()->required_mobcount[0])
				{
					qleA->SetMobCount(0, qleA->GetMobCount(0)+1);
					qleA->SendUpdateAddKill(0);
					qleA->UpdatePlayerFields();

					return true;
				}
			}
			else
			{
				pPlayer->BroadcastMessage("Go to the Port of the Dark Legion!");
			}
		}

		if(kaalez)
		{
			if(pPlayer->CalcDistance(pPlayer, kaalez) < 10)
			{
				if(qleA->GetMobCount(1) < qleA->GetQuest()->required_mobcount[1])
				{
					qleA->SetMobCount(1, qleA->GetMobCount(1)+1);
					qleA->SendUpdateAddKill(1);
					qleA->UpdatePlayerFields();
		
					return true;
				}
			}
			else
			{
				pPlayer->BroadcastMessage("Go to the Port of the Dark Legion!");
			}
		}
		else
		{
			pPlayer->BroadcastMessage("Go to the Port of the Dark Legion!");
		}
	}

	if(qleH)
	{
		if(xilus)
		{
			if(pPlayer->CalcDistance(pPlayer, xilus) < 10)
			{
				if(qleH->GetMobCount(0) < qleH->GetQuest()->required_mobcount[0])
				{
					qleH->SetMobCount(0, qleH->GetMobCount(0)+1);
					qleH->SendUpdateAddKill(0);
					qleH->UpdatePlayerFields();

					return true;
				}
			}
			else
			{
				pPlayer->BroadcastMessage("Go to the Port of the Dark Legion!");
			}
		}

		if(kruul)
		{
			if(pPlayer->CalcDistance(pPlayer, kruul) < 10)
			{
				if(qleH->GetMobCount(1) < qleH->GetQuest()->required_mobcount[1])
				{
					qleH->SetMobCount(1, qleH->GetMobCount(1)+1);
					qleH->SendUpdateAddKill(1);
					qleH->UpdatePlayerFields();
		
					return true;
				}
			}
			else
			{
				pPlayer->BroadcastMessage("Go to the Port of the Dark Legion!");
			}
		}
		else
		{
			pPlayer->BroadcastMessage("Go to the Port of the Dark Legion!");
		}
	}
	
	else
	{
		pPlayer->BroadcastMessage("Missing required quest : Disrupt Their Reinforcements");
	}
	return true;
}


/*--------------------------------------------------------------------------------------------------------*/
//Arzeth's Demise

bool FuryoftheDreghoodElders(uint32 i, Spell* pSpell)
{
	
	Player *caster = pSpell->p_caster;
	if(caster == NULL)
		return true;

	if(!pSpell->GetUnitTarget()->IsCreature())
		return true;
		
	Creature* target = static_cast<Creature*>(pSpell->GetUnitTarget());
	uint32 entry = target->GetEntry();

	if(entry == 19354)
	{
		caster->BroadcastMessage("blaah");

		target->Despawn(0, 3*60*1000);
		sEAS.SpawnCreature(caster, 20680, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 5*60*1000);
		
		return true;
	}
	return true;
}



/*--------------------------------------------------------------------------------------------------------*/
// A Spirit Guide

bool ASpiritGuide(uint32 i, Spell* pSpell)
{
  	Player *pPlayer = (Player*)pSpell->u_caster;
  	if(!pPlayer)
	  	return true;

  	if(!pSpell->u_caster->IsPlayer())
    		return true;

  	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9410);
  	if(qle == NULL)
    		return true;
  
  	Creature *spiritwolf = sEAS.SpawnCreature(pPlayer, 17077, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
  	spiritwolf->Despawn(10*1000, 0);

  	pPlayer->CastSpell(pPlayer, 29938, false);
  	
  	return true;
}



/*--------------------------------------------------------------------------------------------------------*/

class HellfireDeadNPC : public CreatureAIScript
{
public:
  ADD_CREATURE_FACTORY_FUNCTION(HellfireDeadNPC);
  HellfireDeadNPC(Creature* pCreature) : CreatureAIScript(pCreature) {}

  void OnLoad()
  {
    _unit->SetStandState(7);
    _unit->setDeathState(CORPSE);
    _unit->GetAIInterface()->m_canMove = false;
  }
};

void SetupHellfirePeninsula(ScriptMgr * mgr)
{
	/*-------------------------------------------------------------------*/
	// Finished
	mgr->register_creature_script(16772, &FelOrcScavengersQAI::Create);
	mgr->register_creature_script(19701, &FelOrcScavengersQAI::Create);
	mgr->register_creature_script(16876, &FelOrcScavengersQAI::Create);
	mgr->register_creature_script(16925, &FelOrcScavengersQAI::Create);
	mgr->register_creature_script(18952, &FelOrcScavengersQAI::Create);

	mgr->register_creature_script(19411, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(19410, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(16867, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(16870, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(19413, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(19414, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(16878, &BurdenOfSoulsQAI::Create);
	mgr->register_creature_script(19415, &BurdenOfSoulsQAI::Create);
	mgr->register_dummy_spell(34665, &TestingTheAntidote);
	mgr->register_creature_script(16992, &Dreadtusk::Create);
	
	mgr->register_gameobject_script(184661, &ZethGorMustBurnAllianz::Create);
	mgr->register_dummy_spell(35724, &ZethGorMustBurnHorde);
	mgr->register_dummy_spell(32979, &LayingWasteToTheUnwantedAllianz);
	mgr->register_dummy_spell(33067, &BurnItUp);
	mgr->register_dummy_spell(29297, &CleansingtheWater);
	mgr->register_dummy_spell(30489, &TheSeersRelic);
	mgr->register_dummy_spell(34387, &DisruptTheirReinforcements);
	
	GossipScript * Prisoner1Gossip = (GossipScript*) new Prisoner1();
	mgr->register_gossip_script(20677, Prisoner1Gossip);
	GossipScript * Prisoner2Gossip = (GossipScript*) new Prisoner2();
	mgr->register_gossip_script(20678, Prisoner2Gossip);
	GossipScript * Prisoner3Gossip = (GossipScript*) new Prisoner3();
	mgr->register_gossip_script(20679, Prisoner3Gossip);
	
	/*-------------------------------------------------------------------*/
	// TODO
	//mgr->register_dummy_spell(35460, &FuryoftheDreghoodElders);
	//mgr->register_dummy_spell(29731, &ASpiritGuide);
	
	
	/*-------------------------------------------------------------------*/
	// NPC States
	mgr->register_creature_script(20677, &PrisonersDreghoodElders::Create);
	mgr->register_creature_script(20678, &PrisonersDreghoodElders::Create);
	mgr->register_creature_script(20679, &PrisonersDreghoodElders::Create);
	mgr->register_creature_script(17405, &HellfireDeadNPC::Create);
	mgr->register_creature_script(16852, &HellfireDeadNPC::Create);
	mgr->register_creature_script(20158, &HellfireDeadNPC::Create);
}
