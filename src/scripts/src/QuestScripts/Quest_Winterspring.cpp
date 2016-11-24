/*
 * Why Scripts for Arcemu MMORPG Server <http://www.arcemu.org/>
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

// Quest: 5126
class Lorax_Gossip : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), objmgr.GetGossipTextForNpc(pObject->GetEntry()), plr->GetSession()->language);
            sQuestMgr.FillQuestMenu(TO_CREATURE(pObject), plr, menu);
            if(plr->HasQuest(5126) && !plr->HasFinishedQuest(5126))
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Talk to me", 0);
			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			switch(Id)
			{
				case 0: Arcemu::Gossip::Menu::SendQuickMenu(pObject->GetGUID(), 3759, plr, 1, Arcemu::Gossip::ICON_CHAT, "What do you do here?"); break;
				case 1: Arcemu::Gossip::Menu::SendQuickMenu(pObject->GetGUID(), 3760, plr, 2, Arcemu::Gossip::ICON_CHAT, "I can help you"); break;
				case 2: Arcemu::Gossip::Menu::SendQuickMenu(pObject->GetGUID(), 3761, plr, 3, Arcemu::Gossip::ICON_CHAT, "What deal?"); break;
				case 3: Arcemu::Gossip::Menu::SendQuickMenu(pObject->GetGUID(), 3762, plr, 4, Arcemu::Gossip::ICON_CHAT, "Then what happened?"); break;
				case 4: Arcemu::Gossip::Menu::SendQuickMenu(pObject->GetGUID(), 3763, plr, 5, Arcemu::Gossip::ICON_CHAT, "He is not safe, i'll make sure of that."); break;
				case 5:
				{
                    QuestLogEntry* pQuest = plr->GetQuestLogForEntry(5126);
                    if (pQuest)
                    {
                        pQuest->Complete();
                        pQuest->UpdatePlayerFields();
                        pQuest->SendQuestComplete();
                    }
					plr->Gossip_Complete();
				}break;
			}
		}
};

void SetupWinterspring(ScriptMgr* mgr)
{
    mgr->register_creature_gossip(10918, new Lorax_Gossip);
}
