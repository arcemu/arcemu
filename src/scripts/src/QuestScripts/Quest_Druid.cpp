/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2008 WEmu Team
 * Copyright (C) 2005-2008 Ascent Team
 * Copyright (C) 2007-2008 Moon++ Team
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
/**********************
Edits by : FenixGman
**********************/
#include "Setup.h"



class Lunaclaw : public CreatureAIScript
{
public:
  ADD_CREATURE_FACTORY_FUNCTION(Lunaclaw);

  Lunaclaw(Creature* pCreature) : CreatureAIScript(pCreature) {}

  void OnDied(Unit* mKiller)
  {
	if(!mKiller->IsPlayer())
	  return;

	Player* plr = TO_PLAYER(mKiller);

	if(plr->GetMapMgr() == NULL || plr->GetMapMgr()->GetInterface() == NULL)
		return;
	sEAS.SpawnCreature(plr, 12144, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, 1*60*1000);
  }
};

// Lunaclaw ghost gossip
#define GOSSIP_GHOST_MOONKIN    "You have fought well, spirit. I ask you to grand me the strenght of your body and the strenght of your heart."

class SCRIPT_DECL MoonkinGhost_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4714, plr);

		if(plr->GetQuestLogForEntry(6002) != NULL)
		{
			Menu->AddItem( 0, GOSSIP_GHOST_MOONKIN, 1); //Horde
		}
		else if(plr->GetQuestLogForEntry(6001) != NULL)
		{
			Menu->AddItem( 0, GOSSIP_GHOST_MOONKIN, 2); //Ally
		}
        
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature*  pCreature = (pObject->IsCreature())?(TO_CREATURE(pObject)):NULL;
		if(!pObject->IsCreature())
			return;
		
		GossipMenu * Menu;
        switch(IntId)
        {
		case 0: // Return to start
			GossipHello(pCreature, plr, true);
			break;

        case 1: //Horde
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4715, plr);
				Menu->SendTo(plr);

				QuestLogEntry *qle = plr->GetQuestLogForEntry(6002);
				if(qle == NULL)
				return;

				if(qle->GetMobCount(0) != 0)
				return;

				qle->SetMobCount(0, 1);
				qle->SendUpdateAddKill(0);
				qle->UpdatePlayerFields();

				pCreature->Emote(EMOTE_ONESHOT_WAVE);
				pCreature->Despawn(240000,0);
			}
			break;

		case 2: //Ally
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4715, plr);
				Menu->SendTo(plr);

				QuestLogEntry *qle = plr->GetQuestLogForEntry(6001);
				if(qle == NULL)
				return;

				if(qle->GetMobCount(0) != 0)
				return;

				qle->SetMobCount(0, 1);
				qle->SendUpdateAddKill(0);
				qle->UpdatePlayerFields();

				pCreature->Emote(EMOTE_ONESHOT_WAVE);
				pCreature->Despawn(240000,0);
			}
			break;

		}
	}
};

// bear ghost gossip
#define GOSSIP_GHOST_BEAR_A    "What do you represent, spirit?"
#define GOSSIP_GHOST_BEAR_B    "I seek to understand the importance of strength of the body."
#define GOSSIP_GHOST_BEAR_C    "I seek to understand the importance of strength of the heart."
#define GOSSIP_GHOST_BEAR_D    "I have heard your words, Great Bear Spirit, and I understand. I now seek your blessings to fully learn the way of the Claw."

class SCRIPT_DECL BearGhost_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4719, plr);

		if(plr->GetQuestLogForEntry(5930) != NULL) // horde
		{
			Menu->AddItem( 0, GOSSIP_GHOST_BEAR_A, 1);
		}
		else if(plr->GetQuestLogForEntry(5929) != NULL) // ally
		{
			Menu->AddItem( 0, GOSSIP_GHOST_BEAR_A, 5);
		}

        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature*  pCreature = (pObject->IsCreature())?(TO_CREATURE(pObject)):NULL;
		if(!pObject->IsCreature())
			return;
		
		GossipMenu * Menu;
        switch(IntId)
        {
		case 0: // Return to start
			GossipHello(pCreature, plr, true);
			break;
        case 1:
			{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4721, plr);
			Menu->AddItem( 0, GOSSIP_GHOST_BEAR_B, 2);
			Menu->SendTo(plr);
			break;
			}
				case 2:
				{
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4733, plr);
					Menu->AddItem( 0, GOSSIP_GHOST_BEAR_C, 3);
					Menu->SendTo(plr);
					break;
				}
				case 3:
				{
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4734, plr);
					Menu->AddItem( 0, GOSSIP_GHOST_BEAR_D, 4);
					Menu->SendTo(plr);
					break;
				}
			case 4:
			{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4735, plr);
			
			QuestLogEntry *qle = plr->GetQuestLogForEntry(5930);
			if(qle == NULL)
			return;

			if(qle->GetMobCount(0) != 0)
			return;

			qle->SetMobCount(0, 1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();

			Menu->SendTo(plr);
			break;
			}
		case 5:
			{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4721, plr);
			Menu->AddItem( 0, GOSSIP_GHOST_BEAR_B, 6);
			Menu->SendTo(plr);
			break;
			}
				case 6:
				{
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4733, plr);
					Menu->AddItem( 0, GOSSIP_GHOST_BEAR_C, 7);
					Menu->SendTo(plr);
					break;
				}
				case 7:
				{
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4734, plr);
					Menu->AddItem( 0, GOSSIP_GHOST_BEAR_D, 8);
					Menu->SendTo(plr);
					break;
				}
			case 8:
			{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4735, plr);
			
			QuestLogEntry *qle = plr->GetQuestLogForEntry(5929);
			if(qle == NULL)
			return;

			if(qle->GetMobCount(0) != 0)
			return;

			qle->SetMobCount(0, 1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();

			Menu->SendTo(plr);
			break;
			}
		}
	}
};

class MoongladeQuest : public QuestScript
{	
public:
  void OnQuestStart(Player* mTarget, QuestLogEntry * qLogEntry)
  {
	if(!mTarget->HasSpell(19027))
	  mTarget->CastSpell(mTarget, dbcSpell.LookupEntry(19027), true);
  }
};



void SetupDruid(ScriptMgr * mgr)
{
	
  GossipScript * MoonkinGhostGossip = new MoonkinGhost_Gossip;
  GossipScript * BearGhostGossip = new BearGhost_Gossip;
  QuestScript *Moonglade = new MoongladeQuest();
  mgr->register_quest_script(5921, Moonglade);
  mgr->register_quest_script(5922, Moonglade);
  mgr->register_creature_script(12138, &Lunaclaw::Create);

  //Register gossip scripts
  mgr->register_gossip_script(12144, MoonkinGhostGossip); // Ghost of Lunaclaw
  mgr->register_gossip_script(11956, BearGhostGossip); // Great Bear Spirit
  
}

