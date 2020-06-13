/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2008 WEmu Team
 * Copyright (C) 2009 WhyScripts Team <http://www.whydb.org/>
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

class Lady_Jaina : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			if(plr->GetQuestLogForEntry(558))
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7012, plr);
				Menu->AddItem(0, "Lady Jaina, this may sound like an odd request... but I have a young ward who is quite shy. You are a hero to him, and he asked me to get your autograph.", 1);
				Menu->SendTo(plr);
			}
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
		{
			Creature*  pCreature = (pObject->IsCreature()) ? (TO_CREATURE(pObject)) : NULL;
			if(pObject == NULL)
				return;

			switch(IntId)
			{
				case 0: // Return to start
					GossipHello(pCreature, plr);
					break;
				case 1: // Give Item
					{
						plr->CastSpell(plr, dbcSpell.LookupEntry(23122), true);
						plr->Gossip_Complete();
						break;
					}
					break;
			}
		}

};

class Cairne : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			if(plr->GetQuestLogForEntry(925))
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7013, plr);
				Menu->AddItem(0, "Give me hoofprint.", 1);
				Menu->SendTo(plr);
			}
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			Creature*  pCreature = (pObject->IsCreature()) ? (TO_CREATURE(pObject)) : NULL;
			if(pObject == NULL)
				return;

			switch(IntId)
			{
				case 0: // Return to start
					GossipHello(pCreature, plr);
					break;
				case 1: // Give Item
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7014, plr);
						Menu->SendTo(plr);
						plr->CastSpell(plr, dbcSpell.LookupEntry(23123), true);
						break;
					}
					break;
			}
		}

};

#define TELEPORT_SPELL 68328

class TeleportQ_Gossip : public GossipScript
{
public:
	void GossipHello( Object* pObject, Player* plr )
    {
        uint32 Text = objmgr.GetGossipTextForNpc(TO_CREATURE(pObject)->GetEntry());

        // check if there is a entry in the db
        if ( NpcTextStorage.LookupEntry(Text) == NULL ) { return; }

        Arcemu::Gossip::Menu menu(pObject->GetGUID(), Text, plr->GetSession()->language);
        sQuestMgr.FillQuestMenu(TO_CREATURE(pObject), plr, menu);

        // Requirements:
        // one of these quests: 12791, 12794, 12796
        // and item 39740: Kirin Tor Signet
		if ( ( plr->HasQuest(12791) || plr->HasQuest(12794) || plr->HasQuest(12796) ) && plr->HasItemCount(39740, 1, false) )
        {
            menu.AddItem(0, "Teleport me to Dalaran.", 1);
        }
        menu.Send(plr);
	}

	void GossipSelectOption(Object* pObject, Player*  plr, uint32 Id, uint32 IntId, const char* Code)
	{
		if( IntId == 1 )
		{
			plr->CastSpell(plr, TELEPORT_SPELL, true);
		}
		plr->Gossip_Complete();
	}
};

void SetupQuestGossip(ScriptMgr* mgr)
{
	GossipScript* LJ = new Lady_Jaina();
	GossipScript* CB = new Cairne();

	mgr->register_gossip_script(4968, LJ);
	mgr->register_gossip_script(3057, CB);

	// **** Dalaran quests start **** //
	GossipScript* TeleportQGossip = new TeleportQ_Gossip;

	// Horde
	mgr->register_gossip_script(26471, TeleportQGossip);
	mgr->register_gossip_script(29155, TeleportQGossip);
	mgr->register_gossip_script(29159, TeleportQGossip);
	mgr->register_gossip_script(29160, TeleportQGossip);
	mgr->register_gossip_script(29162, TeleportQGossip);
	// Alliance
	mgr->register_gossip_script(23729, TeleportQGossip);
	mgr->register_gossip_script(26673, TeleportQGossip);
	mgr->register_gossip_script(27158, TeleportQGossip);
	mgr->register_gossip_script(29158, TeleportQGossip);
	mgr->register_gossip_script(29161, TeleportQGossip);
	// Both
	mgr->register_gossip_script(29169, TeleportQGossip);
	// **** Dalaran Quests end **** //
}
