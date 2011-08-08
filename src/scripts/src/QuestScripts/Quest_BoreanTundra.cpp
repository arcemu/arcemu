/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
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

// Call to Arms!
class BellRope : public GameObjectAIScript
{
	public:
		ADD_GAMEOBJECT_FACTORY_FUNCTION(BellRope);
		BellRope(GameObject* goinstance) : GameObjectAIScript(goinstance) {};

		void OnActivate(Player* pPlayer)
		{
			if(sEAS.GetQuest(pPlayer, 11965))
				sEAS.KillMobForQuest(pPlayer, 11965, 0);
		};

};

// Reading the Meters
class ColdarraGeoMonitorNexus : public GameObjectAIScript
{
	public:
		ADD_GAMEOBJECT_FACTORY_FUNCTION(ColdarraGeoMonitorNexus);
		ColdarraGeoMonitorNexus(GameObject* goinstance) : GameObjectAIScript(goinstance) {};

		void OnActivate(Player* pPlayer)
		{
			if(sEAS.GetQuest(pPlayer, 11900))
				sEAS.KillMobForQuest(pPlayer, 11900, 0);
		};

};

class ColdarraGeoMonitorSouth : public GameObjectAIScript
{
	public:
		ADD_GAMEOBJECT_FACTORY_FUNCTION(ColdarraGeoMonitorSouth);
		ColdarraGeoMonitorSouth(GameObject* goinstance) : GameObjectAIScript(goinstance) {};

		void OnActivate(Player* pPlayer)
		{
			if(sEAS.GetQuest(pPlayer, 11900))
				sEAS.KillMobForQuest(pPlayer, 11900, 1);
		};

};

class ColdarraGeoMonitorNorth : public GameObjectAIScript
{
	public:
		ADD_GAMEOBJECT_FACTORY_FUNCTION(ColdarraGeoMonitorNorth);
		ColdarraGeoMonitorNorth(GameObject* goinstance) : GameObjectAIScript(goinstance) {};

		void OnActivate(Player* pPlayer)
		{
			if(sEAS.GetQuest(pPlayer, 11900))
				sEAS.KillMobForQuest(pPlayer, 11900, 2);
		};

};

class ColdarraGeoMonitorWest : public GameObjectAIScript
{
	public:
		ADD_GAMEOBJECT_FACTORY_FUNCTION(ColdarraGeoMonitorWest);
		ColdarraGeoMonitorWest(GameObject* goinstance) : GameObjectAIScript(goinstance) {};

		void OnActivate(Player* pPlayer)
		{
			if(sEAS.GetQuest(pPlayer, 11900))
				sEAS.KillMobForQuest(pPlayer, 11900, 3);
		};

};

// Neutralizing the Cauldrons
#define CN_PURIFYING_TOTEM	25494

class PurifyingTotemAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(PurifyingTotemAI, MoonScriptCreatureAI);
		PurifyingTotemAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			SetCanEnterCombat(false);
			SetCanMove(false);
			Despawn(8000, 0);
		}

};



// Cutting Off the Source
class NerubarEggSac : public GameObjectAIScript
{
	public:
		ADD_GAMEOBJECT_FACTORY_FUNCTION(NerubarEggSac);
		NerubarEggSac(GameObject* goinstance) : GameObjectAIScript(goinstance) {};

		void OnActivate(Player* pPlayer)
		{
			if(sEAS.GetQuest(pPlayer, 11602))
				return;

			sEAS.KillMobForQuest(pPlayer, 11602, 0);
			_gameobject->SetState(1);
			_gameobject->SetState(0);
			_gameobject->Despawn(500, 60000);
		};

};


// Bury Those Cockroaches!
class SeaforiumDepthCharge : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(SeaforiumDepthCharge, MoonScriptCreatureAI);
		SeaforiumDepthCharge(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			SetCanMove(false);
			SetCanEnterCombat(false);
			_unit->SetFaction(21);
		}

		void OnLoad()
		{
			if(!_unit->IsSummon())
				return;

			Unit* summoner = TO< Summon* >(_unit)->GetOwner();

			if(summoner != NULL)
			{
				if(summoner->IsPlayer())
				{
					Player* p = TO_PLAYER(summoner);
					if(p->HasQuest(11608))
					{
						GameObject* pSinkhole = p->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(p->GetPositionX(), p->GetPositionY(), p->GetPositionZ(), 300171);
						if(pSinkhole != NULL)
						{
							_unit->CastSpell(_unit, 45502, true);

							float posX = pSinkhole->GetPositionX();
							if(posX == 2657.13f)
								sEAS.KillMobForQuest(p, 11608, 0);

							if(posX == 2716.02f)
								sEAS.KillMobForQuest(p, 11608, 1);

							if(posX == 2877.96f)
								sEAS.KillMobForQuest(p, 11608, 2);

							if(posX == 2962.16f)
								sEAS.KillMobForQuest(p, 11608, 3);

						}
					}
				}
			}
			_unit->Despawn(500, 0);
		}
};

// Hatching a Plan
class BlueDragonEgg : public GameObjectAIScript
{
	public:
		ADD_GAMEOBJECT_FACTORY_FUNCTION(BlueDragonEgg);
		BlueDragonEgg(GameObject* goinstance) : GameObjectAIScript(goinstance) {};

		void OnActivate(Player* pPlayer)
		{
			if(!sEAS.GetQuest(pPlayer, 11936))
				return;

			sEAS.KillMobForQuest(pPlayer, 11936, 0);
			_gameobject->SetState(1);
			_gameobject->SetState(0);
			_gameobject->Despawn(500, 60000);
		}
};




enum eFizzcrank
{
    NPC_FIZZCRANK               = 25590,

    GOSSIP_TEXTID_FIZZCRANK1    = 12456,
    GOSSIP_TEXTID_FIZZCRANK2    = 12457,
    GOSSIP_TEXTID_FIZZCRANK3    = 12458,
    GOSSIP_TEXTID_FIZZCRANK4    = 12459,
    GOSSIP_TEXTID_FIZZCRANK5    = 12460,
    GOSSIP_TEXTID_FIZZCRANK6    = 12461,
    GOSSIP_TEXTID_FIZZCRANK7    = 12462,
    GOSSIP_TEXTID_FIZZCRANK8    = 12463,
    GOSSIP_TEXTID_FIZZCRANK9    = 12464,

    QUEST_THE_MECHAGNOMES       = 11708
};

#define GOSSIP_ITEM_GO_ON   "Go on."
#define GOSSIP_ITEM_TELL_ME "Tell me what's going on out here, Fizzcrank."

class FizzcrankGossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* pPlayer)
		{
			GossipMenu* Menu;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, pPlayer);

			if(sEAS.GetQuest(pPlayer, QUEST_THE_MECHAGNOMES))
				Menu->AddItem(0, GOSSIP_ITEM_TELL_ME, 1);

			Menu->SendTo(pPlayer);
		}

		void GossipSelectOption(Object* pObject, Player*  pPlayer, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 1:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK1, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 2);
					Menu->SendTo(pPlayer);
					break;
				case 2:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK2, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 3);
					Menu->SendTo(pPlayer);
					break;
				case 3:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK3, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 4);
					Menu->SendTo(pPlayer);
					break;
				case 4:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK4, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 5);
					Menu->SendTo(pPlayer);
					break;
				case 5:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK5, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 6);
					Menu->SendTo(pPlayer);
					break;
				case 6:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK6, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 7);
					Menu->SendTo(pPlayer);
					break;
				case 7:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK7, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 8);
					Menu->SendTo(pPlayer);
					break;
				case 8:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK8, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 9);
					Menu->SendTo(pPlayer);
					break;
				case 9:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK9, pPlayer);
					Menu->SendTo(pPlayer);
					break;
			}
		}

};

#define GOSSIP_ITEM_FREE_FLIGHT "I'd like passage to the Transitus Shield."
#define GOSSIP_ITEM_FLIGHT      "May I use a drake to fly elsewhere?"

enum eSurristrasz
{
    NPC_SURRISTRASZ             = 24795,

    SPELL_ABMER_TO_COLDARRA     = 46064
};

class SurristraszGossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* pPlayer)
		{
			GossipMenu* Menu;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, pPlayer);

			Menu->AddItem(0, GOSSIP_ITEM_FREE_FLIGHT, 1);
			Menu->AddItem(3, GOSSIP_ITEM_FLIGHT, 2);

			Menu->SendTo(pPlayer);
		};

		void GossipSelectOption(Object* pObject, Player*  pPlayer, uint32 Id, uint32 IntId, const char* Code)
		{
			if(!pObject->IsCreature())
				return;

			switch(IntId)
			{
				case 1:
					pPlayer->Gossip_Complete();
					pPlayer->CastSpell(pPlayer, SPELL_ABMER_TO_COLDARRA, true);
					break;
				case 2:
					pPlayer->GetSession()->SendTaxiList(TO_CREATURE(pObject));
					break;
			};
		};
};

void SetupBoreanTundra(ScriptMgr* mgr)
{
	// Call to Arms!
	mgr->register_gameobject_script(188163, &BellRope::Create);
	// Reading the Meters
	mgr->register_gameobject_script(188100, &ColdarraGeoMonitorNexus::Create);
	mgr->register_gameobject_script(188101, &ColdarraGeoMonitorSouth::Create);
	mgr->register_gameobject_script(188102, &ColdarraGeoMonitorNorth::Create);
	mgr->register_gameobject_script(188103, &ColdarraGeoMonitorWest::Create);

	// Cutting Off the Source
	mgr->register_gameobject_script(187655, &NerubarEggSac::Create);
	// Bury Those Cockroaches!
	mgr->register_creature_script(25401, &SeaforiumDepthCharge::Create);
	// Hatching a Plan
	mgr->register_gameobject_script(188133, &BlueDragonEgg::Create);

	// Neutralizing the Cauldrons
	mgr->register_creature_script(CN_PURIFYING_TOTEM, &PurifyingTotemAI::Create);

	// Mechagnomes
	// Fizzcrank Fullthrottle
	GossipScript* fGossip = new FizzcrankGossip;
	mgr->register_gossip_script(NPC_FIZZCRANK, fGossip);

	// Surristrasz
	GossipScript* sGossip = new SurristraszGossip;
	mgr->register_gossip_script(NPC_SURRISTRASZ, sGossip);
}
