/*
 * WEmu Scripts for WEmu MMORPG Server
 * Copyright (C) 2008 WEmu Team
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
#include "../EAS/EasyFunctions.h"

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, plr); \
	Menu->SendTo(plr);

// Fel Orc Scavengers Quest
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
// The Dreghood Elders

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


class SCRIPT_DECL WingCommanderBrack : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		if(Plr->GetQuestLogForEntry(10129))
		{
			Menu->AddItem( 0, "Put me on a wyvern", 1);
		}
		if(Plr->GetQuestLogForEntry(10242))
		{
			Menu->AddItem( 0, "Flight to Spinebreaker Post", 2);
		}
		if(Plr->GetQuestLogForEntry(10162))
		{
			Menu->AddItem( 0, "I'm ready to mission", 3);
		}
		Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature* creat = static_cast<Creature*>(pObject);
		switch(IntId)
		{
		case 1:
			creat->CastSpell(Plr, dbcSpell.LookupEntry(33659), true);
			break;
		case 2:
			creat->CastSpell(Plr, dbcSpell.LookupEntry(34578), true);
			break;
		case 3:
			creat->CastSpell(Plr, dbcSpell.LookupEntry(33825), true);
			break;
		}
	}
};

class SCRIPT_DECL ForwardCommanderKingston : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		if(Plr->GetQuestLogForEntry(10146))
		{
			Menu->AddItem( 0, "Put me on a gryphon", 1);
		}
		if(Plr->GetQuestLogForEntry(10340))
		{
			Menu->AddItem( 0, "Flight to Shatter Point", 2);
		}
		Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature* creat = static_cast<Creature*>(pObject);
		switch(IntId)
		{
		case 1:
			creat->CastSpell(Plr, dbcSpell.LookupEntry(33768), true);
			break;
		case 2:
			creat->CastSpell(Plr, dbcSpell.LookupEntry(35069), true);
			break;
		}
	}
};

class SCRIPT_DECL GryphoneerWindbellow : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		if(Plr->GetQuestLogForEntry(10163))
		{
			Menu->AddItem( 0, "I'm ready to mission.", 1);
		}
		Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature* creat = static_cast<Creature*>(pObject);
		switch(IntId)
		{
		case 1:
			creat->CastSpell(Plr, dbcSpell.LookupEntry(33899), true);
			break;
		}
	}
};

class SCRIPT_DECL VlaggaFreyfeather : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		if(Plr->GetQuestLogForEntry(10289))
		{
			Menu->AddItem( 0, "I'm ready to fly to Thrallmar", 1);
		}
		Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature* creat = static_cast<Creature*>(pObject);
		switch(IntId)
		{
		case 1:
			creat->CastSpell(Plr, dbcSpell.LookupEntry(34924), true);
			break;
		}
	}
};

class SCRIPT_DECL AmishWildhammer : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		if(Plr->GetQuestLogForEntry(10140))
		{
			Menu->AddItem( 0, "I'm ready to fly to Honor Hold", 1);
		}
		Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature* creat = static_cast<Creature*>(pObject);
		switch(IntId)
		{
		case 1:
			creat->CastSpell(Plr, dbcSpell.LookupEntry(34907), true);
			break;
		}
	}
};

void SetupHellfirePeninsula(ScriptMgr * mgr)
{
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

	// Fertig
	mgr->register_gameobject_script(184661, &ZethGorMustBurnAllianz::Create);
	mgr->register_dummy_spell(35724, &ZethGorMustBurnHorde);
	
	GossipScript * Prisoner1Gossip = (GossipScript*) new Prisoner1();
	mgr->register_gossip_script(20677, Prisoner1Gossip);
	GossipScript * Prisoner2Gossip = (GossipScript*) new Prisoner2();
	mgr->register_gossip_script(20678, Prisoner2Gossip);
	GossipScript * Prisoner3Gossip = (GossipScript*) new Prisoner3();
	mgr->register_gossip_script(20679, Prisoner3Gossip);
	

	// NPC States
	mgr->register_creature_script(20677, &PrisonersDreghoodElders::Create);
	mgr->register_creature_script(20678, &PrisonersDreghoodElders::Create);
	mgr->register_creature_script(20679, &PrisonersDreghoodElders::Create);
	mgr->register_creature_script(17405, &HellfireDeadNPC::Create);
	mgr->register_creature_script(16852, &HellfireDeadNPC::Create);
	mgr->register_creature_script(20158, &HellfireDeadNPC::Create);

	mgr->register_gossip_script(19401, CREATE_GOSSIPSCRIPT(WingCommanderBrack));
	mgr->register_gossip_script(19409, CREATE_GOSSIPSCRIPT(ForwardCommanderKingston));
	mgr->register_gossip_script(20235, CREATE_GOSSIPSCRIPT(GryphoneerWindbellow));
	mgr->register_gossip_script(18930, CREATE_GOSSIPSCRIPT(VlaggaFreyfeather));
	mgr->register_gossip_script(18931, CREATE_GOSSIPSCRIPT(AmishWildhammer));
}