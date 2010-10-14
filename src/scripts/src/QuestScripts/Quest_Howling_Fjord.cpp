/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
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

// War is Hell
bool WarIsHell(uint32 i, Spell* pSpell)
{
	if( !pSpell->u_caster->IsPlayer() )
		return true;

	Player* plr = TO_PLAYER(pSpell->u_caster);
	if( plr == NULL )
		return true;

	Creature* target = TO_CREATURE(plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( plr->GetPositionX(), plr->GetPositionY() , plr->GetPositionZ(), 24008 ));
	if( target == NULL )
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(11270);
  
	if( qle == NULL )
		return true;
 
	GameObject* obj = NULL;

	sEAS.KillMobForQuest( plr, qle, 0 );

	obj = sEAS.SpawnGameobject(plr, 183816, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 1, 0, 0, 0, 0);
	sEAS.GameobjectDelete(obj, 1*30*1000);

	target->Despawn( 2000, 60*1000 );
	plr->UpdateNearbyGameObjects();
	qle->UpdatePlayerFields();
	return true;
}

// A Lesson in Fear
bool PlantForsakenBanner(uint32 i, Spell* pSpell)
{
	if(pSpell->u_caster->IsPlayer() == false)
		return true;

	Player* pPlayer = TO_PLAYER(pSpell->u_caster);
	if( pPlayer == NULL )
		return true;

	QuestLogEntry* pQuest = pPlayer->GetQuestLogForEntry(11282);
	if( pQuest == NULL )
		return true;
	
	Creature* target = static_cast<Creature*>(pSpell->GetUnitTarget());
	if ( target == NULL || target->isAlive() )
		return true;

	uint32 cit = target->GetEntry();
	switch(cit)
	{
	case 24161:
		sEAS.KillMobForQuest( pPlayer, 11282, 0 );
		target->Despawn(0, 3*60*1000);
		break;
	case 24016:
		sEAS.KillMobForQuest( pPlayer, 11282, 1 );
		target->Despawn(0, 3*60*1000);
		break;
	case 24162:
		sEAS.KillMobForQuest( pPlayer, 11282, 2 );
		target->Despawn(0, 3*60*1000);
		break;
	}
	return true;
}
class NorthFleet : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(NorthFleet);
    NorthFleet(Creature* pCreature) : CreatureAIScript(pCreature)  {}

   void OnDied(Unit * mKiller)
   {
	      if (mKiller->IsPlayer())
      {
         QuestLogEntry *qle = TO_PLAYER(mKiller)->GetQuestLogForEntry(11230);
         if( qle != NULL )
         {
            if( qle->GetMobCount( 0 ) < qle->GetQuest()->required_mobcount[ 0 ] )
            {
               uint32 newcount = qle->GetMobCount( 0 ) + 1;
               qle->SetMobCount( 0, newcount );
               qle->SendUpdateAddKill( 0 );
               qle->UpdatePlayerFields();
               return;
            }
         }
      }
   }
};
class ChillmereScourge : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(ChillmereScourge);
    ChillmereScourge(Creature* pCreature) : CreatureAIScript(pCreature)  {}

   void OnDied(Unit * mKiller)
   {
      if (mKiller->IsPlayer())
      {
         QuestLogEntry *qle = TO_PLAYER(mKiller)->GetQuestLogForEntry(11397);
         if( qle != NULL )
         {
            if( qle->GetMobCount( 0 ) < qle->GetQuest()->required_mobcount[ 0 ] )
            {
               uint32 newcount = qle->GetMobCount( 0 ) + 1;
               qle->SetMobCount( 0, newcount );
               qle->SendUpdateAddKill( 0 );
               qle->UpdatePlayerFields();
               return;
            }
         }
      }
   }
};
class Baleheim : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Baleheim);
    Baleheim(Creature* pCreature) : CreatureAIScript(pCreature)  {}

   void OnDied(Unit * mKiller)
   {
      if (mKiller->IsPlayer())
      {
         QuestLogEntry *qle = TO_PLAYER(mKiller)->GetQuestLogForEntry(11283);
         if( qle != NULL )
         {
            if( qle->GetMobCount( 0 ) < qle->GetQuest()->required_mobcount[ 0 ] )
            {
               uint32 newcount = qle->GetMobCount( 0 ) + 1;
               qle->SetMobCount( 0, newcount );
               qle->SendUpdateAddKill( 0 );
               qle->UpdatePlayerFields();
               return;
            }
         }
      }
   }
};

class Plaguethis_Gossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {
		GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 40002, plr);
        Menu->AddItem( 0, "Where would you like to fly too ?", 2); 
        if( plr->GetQuestLogForEntry(11332)!= NULL)
			Menu->AddItem( 0, "Greer, i need a gryphon to ride and some bombs to drop on New Agamand!", 1 );
       

        if(AutoSend)
            Menu->SendTo(plr);
    }

	void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?TO_CREATURE(pObject):NULL;
		if( pCreature == NULL )
			return;

		switch(IntId)
		{
		case 1:
		{
			Item *item;
			item = objmgr.CreateItem( 33634 , plr);
			item->SetStackCount(10);

			if(!plr->GetItemInterface()->AddItemToFreeSlot(item))
			{
				plr->GetSession()->SendNotification("No free slots were found in your inventory!");
				item->DeleteMe();
			}
			else
			{
				plr->SendItemPushResult( false, true, false, true, plr->GetItemInterface()->LastSearchResult()->ContainerSlot,
				   plr->GetItemInterface()->LastSearchResult()->Slot, 1, item->GetEntry(), item->GetItemRandomSuffixFactor(),
				   item->GetItemRandomPropertyId(), item->GetStackCount() );		
				
			}

            if(pCreature->GetEntry() == 23859)
            {
				TaxiPath * path = sTaxiMgr.GetTaxiPath(745);
				plr->TaxiStart( path, 17759, 0 );
			}
            break;
		}
		case 2:
		{
			plr->GetSession()->SendTaxiList(pCreature);
			break;
		}
		}
	}

};


void SetupHowlingFjord(ScriptMgr * mgr)
{
	GossipScript * Plague = new Plaguethis_Gossip(); // thx  Dzjhenghiz
	mgr->register_gossip_script(23859, Plague);

	mgr->register_dummy_spell(42793, &WarIsHell);
	mgr->register_dummy_spell(43178, &PlantForsakenBanner);
	mgr->register_creature_script(23643, &ChillmereScourge::Create);
	mgr->register_creature_script(23645, &ChillmereScourge::Create);
	mgr->register_creature_script(23644, &ChillmereScourge::Create);
	mgr->register_creature_script(24540, &ChillmereScourge::Create);
	mgr->register_creature_script(24485, &ChillmereScourge::Create);
	mgr->register_creature_script(23653, &Baleheim::Create);
	mgr->register_creature_script(23655, &Baleheim::Create);
	mgr->register_creature_script(24015, &Baleheim::Create);
	mgr->register_creature_script(23866, &NorthFleet::Create);
	mgr->register_creature_script(23934, &NorthFleet::Create);
	mgr->register_creature_script(23946, &NorthFleet::Create);
	mgr->register_creature_script(23794, &NorthFleet::Create);
	mgr->register_creature_script(23793, &NorthFleet::Create);
}
