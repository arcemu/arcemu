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
		if ( sEAS.GetQuest( pPlayer, 11965 ) )
			sEAS.KillMobForQuest( pPlayer, 11965, 0 );
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
		if ( sEAS.GetQuest( pPlayer, 11900 ) )
			sEAS.KillMobForQuest( pPlayer, 11900, 0 );
	};

};

class ColdarraGeoMonitorSouth : public GameObjectAIScript
{
public:
	ADD_GAMEOBJECT_FACTORY_FUNCTION(ColdarraGeoMonitorSouth);
	ColdarraGeoMonitorSouth(GameObject* goinstance) : GameObjectAIScript(goinstance) {};

	void OnActivate(Player* pPlayer)
	{
		if ( sEAS.GetQuest( pPlayer, 11900 ) )
			sEAS.KillMobForQuest( pPlayer, 11900, 1 );
	};

};

class ColdarraGeoMonitorNorth : public GameObjectAIScript
{
public:
	ADD_GAMEOBJECT_FACTORY_FUNCTION(ColdarraGeoMonitorNorth);
	ColdarraGeoMonitorNorth(GameObject* goinstance) : GameObjectAIScript(goinstance) {};

	void OnActivate(Player* pPlayer)
	{
		if ( sEAS.GetQuest( pPlayer, 11900 ) )
			sEAS.KillMobForQuest( pPlayer, 11900, 2 );
	};

};

class ColdarraGeoMonitorWest : public GameObjectAIScript
{
public:
	ADD_GAMEOBJECT_FACTORY_FUNCTION(ColdarraGeoMonitorWest);
	ColdarraGeoMonitorWest(GameObject* goinstance) : GameObjectAIScript(goinstance) {};

	void OnActivate(Player* pPlayer)
	{
		if ( sEAS.GetQuest( pPlayer, 11900 ) )
			sEAS.KillMobForQuest( pPlayer, 11900, 3 );
	};

};

// Neutralizing the Cauldrons
#define CN_PURIFYING_TOTEM	25494

class PurifyingTotemAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(PurifyingTotemAI, MoonScriptCreatureAI);
    PurifyingTotemAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		SetCanEnterCombat( false );
		SetCanMove( false );
		Despawn(8000, 0);
	}

};

bool NeutralizingTheCauldrons(uint32 i, Spell* pSpell)
{
	if (pSpell == NULL || pSpell->p_caster == NULL || !pSpell->p_caster->IsInWorld())
		return true;

	Player* pPlayer = pSpell->p_caster;
	QuestLogEntry* pQuest = sEAS.GetQuest( pPlayer, 11647);
	if ( pQuest == NULL )
		return true;

	GameObject* pCauldron = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 187690 );
	if(pCauldron == NULL)
		return true;

	float posX = pCauldron->GetPositionX();

	if(posX == 3747.07f)
		sEAS.KillMobForQuest( pPlayer, pQuest, 0 );

	if(posX == 4023.5f)
		sEAS.KillMobForQuest( pPlayer, pQuest, 1 );

	if(posX == 4126.12f)
		sEAS.KillMobForQuest( pPlayer, pQuest, 2 );
	
	return true;
}

// Cutting Off the Source
class NerubarEggSac : public GameObjectAIScript
{
public:
	ADD_GAMEOBJECT_FACTORY_FUNCTION(NerubarEggSac);
	NerubarEggSac(GameObject* goinstance) : GameObjectAIScript(goinstance) {};

	void OnActivate(Player* pPlayer)
	{
		if( sEAS.GetQuest( pPlayer, 11602 ) )
			return;

		sEAS.KillMobForQuest( pPlayer, 11602, 0);
		_gameobject->SetState(1);
		_gameobject->SetState(0);	
		_gameobject->Despawn(500, 60000);
	};

};

// Stop the Plague
bool HighmessasCleansingSeeds(uint32 i, Spell * pSpell)
{
	if (pSpell == NULL || pSpell->p_caster == NULL || !pSpell->p_caster->IsInWorld())
		return true;

	Player*  pPlayer = pSpell->p_caster;
	QuestLogEntry* pQuest = sEAS.GetQuest( pPlayer, 11677 );
	if ( !pQuest )
		return true;

	sEAS.KillMobForQuest( pPlayer, pQuest, 0);
	return true;
}

// There's Something Going On In Those Caves
bool BixiesInhibitingPowder(uint32 i, Spell * pSpell)
{
	if (pSpell == NULL || pSpell->p_caster == NULL || !pSpell->p_caster->IsInWorld())
		return true;

	Player* pPlayer = pSpell->p_caster;
	QuestLogEntry* pQuest = sEAS.GetQuest( pPlayer, 11694 );
	if ( !pQuest )
		return true;

	sEAS.KillMobForQuest( pPlayer, 11694, 0);
	return true;
}

// Bury Those Cockroaches!
class SeaforiumDepthCharge : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SeaforiumDepthCharge, MoonScriptCreatureAI);
    SeaforiumDepthCharge(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		SetCanMove( false );
		SetCanEnterCombat( false );
		_unit->SetFaction(21);
		RegisterAIUpdateEvent(3000);
	}

	void AIUpdate()
	{
		_unit->CastSpell(_unit, 45502, true);
		_unit->Despawn(500, 0);
		ParentClass::AIUpdate();
	}

};

bool PlantingSeaforiumDepthCharge(uint32 i, Spell * pSpell)
{
	if(pSpell == NULL || pSpell->p_caster == NULL || !pSpell->p_caster->IsInWorld())
		return true;

	
	Player* pPlayer = pSpell->p_caster;
	QuestLogEntry* pQuest = sEAS.GetQuest( pPlayer, 11608);
	if( !pQuest )
		return true;

	GameObject* pSinkhole = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 300171);
	if( pSinkhole == NULL )
		return true;

	pPlayer->GetMapMgr()->GetInterface()->SpawnCreature( 25401, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), false, false, NULL, NULL, 1);
	
	float posX = pSinkhole->GetPositionX();
	if(posX == 2657.13f)
		sEAS.KillMobForQuest( pPlayer, 11608, 0);

	if(posX == 2716.02f)
		sEAS.KillMobForQuest( pPlayer, 11608, 1);

	if(posX == 2877.96f)
		sEAS.KillMobForQuest( pPlayer, 11608, 2);

	if(posX == 2962.16f)
		sEAS.KillMobForQuest( pPlayer, 11608, 3);

	return true;
}

// Hatching a Plan
class BlueDragonEgg : public GameObjectAIScript
{
public:
	ADD_GAMEOBJECT_FACTORY_FUNCTION(BlueDragonEgg);
	BlueDragonEgg(GameObject* goinstance) : GameObjectAIScript(goinstance) {};

	void OnActivate(Player* pPlayer)
	{
		if ( !sEAS.GetQuest( pPlayer, 11936) )
			return;

		sEAS.KillMobForQuest( pPlayer, 11936, 0);
		_gameobject->SetState(1);
		_gameobject->SetState(0);
		_gameobject->Despawn(500, 60000);
	}
};

// Leading the Ancestors Home
bool CompleteAncestorRitual(uint32 i, Spell * pSpell)
{
	if(pSpell == NULL || pSpell->p_caster == NULL || !pSpell->p_caster->IsInWorld())
		return true;

	Player* pPlayer = pSpell->p_caster;
	QuestLogEntry* pQuest = sEAS.GetQuest( pPlayer, 11610 );
	if( !pQuest )
		return true;

	GameObject* pElderObj;
	pElderObj = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 191088);
	if(pElderObj != NULL && pPlayer->GetDistance2dSq(pElderObj) < 8.0f)
	{
		sEAS.KillMobForQuest( pPlayer, 11610, 0);
		return true;
	}

	pElderObj = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 191089);
	if(pElderObj != NULL && pPlayer->GetDistance2dSq(pElderObj) < 8.0f)
	{
		sEAS.KillMobForQuest( pPlayer, 11610, 1);
		return true;
	}

	pElderObj = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 191090);
	if(pElderObj != NULL && pPlayer->GetDistance2dSq(pElderObj) < 8.0f)
	{
		sEAS.KillMobForQuest( pPlayer, 11610, 2);
		return true;
	}

	return true;
}


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
	void GossipHello(Object* pObject, Player*  pPlayer, bool AutoSend)
	{
		GossipMenu *Menu;
        
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, pPlayer);

        if( sEAS.GetQuest( pPlayer, QUEST_THE_MECHAGNOMES ) )
		    Menu->AddItem( 0, GOSSIP_ITEM_TELL_ME, 1 );

		if(AutoSend)
			Menu->SendTo(pPlayer);
	}

	void GossipSelectOption(Object* pObject, Player*  pPlayer, uint32 Id, uint32 IntId, const char * Code)
	{
        GossipMenu *Menu;
		switch (IntId)
		{
			case 1:
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK1, pPlayer);
                Menu->AddItem( 0, GOSSIP_ITEM_GO_ON, 2 );
                Menu->SendTo(pPlayer);
                break;
            case 2:
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK2, pPlayer);
                Menu->AddItem( 0, GOSSIP_ITEM_GO_ON, 3 );
                Menu->SendTo(pPlayer);
                break;
            case 3:
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK3, pPlayer);
                Menu->AddItem( 0, GOSSIP_ITEM_GO_ON, 4 );
                Menu->SendTo(pPlayer);
                break;
            case 4:
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK4, pPlayer);
                Menu->AddItem( 0, GOSSIP_ITEM_GO_ON, 5 );
                Menu->SendTo(pPlayer);
                break;
            case 5:
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK5, pPlayer);
                Menu->AddItem( 0, GOSSIP_ITEM_GO_ON, 6 );
                Menu->SendTo(pPlayer);
                break;
            case 6:
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK6, pPlayer);
                Menu->AddItem( 0, GOSSIP_ITEM_GO_ON, 7 );
                Menu->SendTo(pPlayer);
                break;
            case 7:
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK7, pPlayer);
                Menu->AddItem( 0, GOSSIP_ITEM_GO_ON, 8 );
                Menu->SendTo(pPlayer);
                break;
            case 8:
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK8, pPlayer);
                Menu->AddItem( 0, GOSSIP_ITEM_GO_ON, 9 );
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
	void GossipHello(Object* pObject, Player*  pPlayer, bool AutoSend)
	{
		GossipMenu *Menu;
        
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, pPlayer);

	    Menu->AddItem( 0, GOSSIP_ITEM_FREE_FLIGHT, 1 ); 
        Menu->AddItem( 3, GOSSIP_ITEM_FLIGHT, 2 );

		if(AutoSend)
			Menu->SendTo(pPlayer);
	};

    void GossipSelectOption(Object* pObject, Player*  pPlayer, uint32 Id, uint32 IntId, const char * Code)
	{
        if( !pObject->IsCreature() )
            return;

		switch (IntId)
		{
        case 1:
            pPlayer->Gossip_Complete();
            pPlayer->CastSpell( pPlayer, SPELL_ABMER_TO_COLDARRA, true );
            break;
        case 2:
            pPlayer->GetSession()->SendTaxiList( TO_CREATURE( pObject ) );
            break;
        };
    };
};

void SetupBoreanTundra(ScriptMgr * mgr)
{
	// Call to Arms!
	mgr->register_gameobject_script(188163, &BellRope::Create);
	// Reading the Meters
	mgr->register_gameobject_script(188100, &ColdarraGeoMonitorNexus::Create);
	mgr->register_gameobject_script(188101, &ColdarraGeoMonitorSouth::Create);
	mgr->register_gameobject_script(188102, &ColdarraGeoMonitorNorth::Create);
	mgr->register_gameobject_script(188103, &ColdarraGeoMonitorWest::Create);
	// Neutralizing the Cauldrons
	mgr->register_creature_script(CN_PURIFYING_TOTEM, &PurifyingTotemAI::Create);
	mgr->register_dummy_spell(45653, &NeutralizingTheCauldrons);
	// Cutting Off the Source
	mgr->register_gameobject_script(187655, &NerubarEggSac::Create);
	// Stop the Plague
	mgr->register_dummy_spell(45834, &HighmessasCleansingSeeds);
	// There's Something Going On In Those Caves
	mgr->register_dummy_spell(45835, &BixiesInhibitingPowder);
	// Bury Those Cockroaches!
	mgr->register_creature_script(25401, &SeaforiumDepthCharge::Create);
	mgr->register_dummy_spell(45503, &PlantingSeaforiumDepthCharge);
	// Hatching a Plan
	mgr->register_gameobject_script(188133, &BlueDragonEgg::Create);
	// Leading the Ancestors Home
	mgr->register_dummy_spell(45536, &CompleteAncestorRitual);
    // Mechagnomes
    // Fizzcrank Fullthrottle
    GossipScript * fGossip = new FizzcrankGossip;
    mgr->register_gossip_script( NPC_FIZZCRANK, fGossip );
    
    // Surristrasz 
    GossipScript * sGossip = new SurristraszGossip;
    mgr->register_gossip_script( NPC_SURRISTRASZ, sGossip );
}
