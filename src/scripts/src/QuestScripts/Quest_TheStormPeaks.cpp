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

// The Gifts of Loken
class LokensFury : public GameObjectAIScript
{
public:
	ADD_GAMEOBJECT_FACTORY_FUNCTION(LokensFury);
	LokensFury(GameObject* goinstance) : GameObjectAIScript(goinstance) {};

	void OnActivate(Player* pPlayer)
	{
		if ( sEAS.GetQuest( pPlayer, 12965) )
			sEAS.KillMobForQuest( pPlayer, 12965, 0 );
	};

};

class LokensPower : public GameObjectAIScript
{
public:
	ADD_GAMEOBJECT_FACTORY_FUNCTION(LokensPower);
	LokensPower(GameObject* goinstance) : GameObjectAIScript(goinstance) {};

	void OnActivate(Player* pPlayer)
	{
		if ( sEAS.GetQuest( pPlayer, 12965) )
			sEAS.KillMobForQuest( pPlayer, 12965, 1 );
	};

};

class LokensFavor : public GameObjectAIScript
{
public:
	ADD_GAMEOBJECT_FACTORY_FUNCTION(LokensFavor);
	LokensFavor(GameObject* goinstance) : GameObjectAIScript(goinstance) {};

	void OnActivate(Player* pPlayer)
	{
		if ( sEAS.GetQuest( pPlayer, 12965) )
			sEAS.KillMobForQuest( pPlayer, 12965, 2 );
	};

};

#define GOSSIP_SCOUTMENU1 "Are you okay? I've come to take you back to Frosthold if you can stand."
#define GOSSIP_SCOUTMENU2 "I'm sorry that I didn't get here sooner. What happened?"
#define GOSSIP_SCOUTMENU3 "I'll go get some help. Hang in there."

class SCRIPT_DECL MissingScout_Gossip : public GossipScript
{
public:
       void GossipHello(Object* pObject, Player* plr, bool AutoSend)
       {
               GossipMenu *Menu;
               objmgr.CreateGossipMenuForPlayer( &Menu, pObject->GetGUID(), 13611, plr );
               if( plr->HasQuest(12864) )
                       Menu->AddItem( 0, GOSSIP_SCOUTMENU1, 1 );
       
               if( AutoSend )
                       Menu->SendTo(plr);
       }
       void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
       {
               Creature* pCreature = (pObject->IsCreature()) ? (TO_CREATURE(pObject)) : NULL;
               if(pCreature == NULL)
                       return;

               GossipMenu *Menu;
               switch(IntId)
               {
                       case 1:
                       {
                               objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 13612, plr);
                               Menu->AddItem( 0, GOSSIP_SCOUTMENU2, 2);
                               Menu->SendTo(plr);
                       }break;
                       case 2:
                       {
                               objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 13613, plr);
                               Menu->AddItem( 0, GOSSIP_SCOUTMENU3, 3);
                               Menu->SendTo(plr);
                       }break;
                       case 3:
                       {
                               objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 13614, plr);
                               Menu->SendTo(plr);

                               QuestLogEntry *qle = plr->GetQuestLogForEntry(12864);
                               if(qle == NULL || qle->GetMobCount(0) != 0)
                                       return;

                               qle->SetMobCount(0, 1);
                               qle->SendUpdateAddKill(0);
                               qle->UpdatePlayerFields();
                       }break;
               }
       }

};

 void SetupTheStormPeaks(ScriptMgr * mgr)
 {
        // The Gifts of Loken
        mgr->register_gameobject_script(192120, &LokensFury::Create);
        mgr->register_gameobject_script(192121, &LokensPower::Create);
        mgr->register_gameobject_script(192122, &LokensFavor::Create);
       GossipScript * MissingScoutGossip = new MissingScout_Gossip;
       mgr->register_gossip_script(29811, MissingScoutGossip);
 }