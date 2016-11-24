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

class TabletOfTheSeven : public QuestScript
{
	public:
		void OnGameObjectActivate(uint32 entry, Player* mTarget, QuestLogEntry* qLogEntry)
		{
			if(mTarget == NULL || qLogEntry == NULL || entry != 169294)
				return;

			if(mTarget->GetItemInterface()->GetItemCount(11470) < qLogEntry->GetQuest()->required_itemcount[0])
				mTarget->CastSpell(mTarget, 15065, false);
		}
};

// Quest: 4224
#define RaggedJohn_GOSSIP_MAIN "Official buisness, John. I need some information about Marsha Windsor. Tell me about the last time you saw him."
#define RaggedJohn_GOSSIP_0 "So what did you do?"
#define RaggedJohn_GOSSIP_1 "Start making sense, dwarf. I don't want to have anything to do with your cracker, your pappy, or any sort of 'discreditin'."
#define RaggedJohn_GOSSIP_2 "Ironfoe?"
#define RaggedJphn_GOSSIP_3 "Interesting... continue John."
#define RaggedJohn_GOSSIP_4 "So that's how Windsor died..."
#define RaggedJohn_GOSSIP_5 "So how did he die?"
#define RaggedJohn_GOSSIP_6 "Ok so where the hell is he? Wait a minute! Are you drunk?"
#define RaggedJohn_GOSSIP_7 "WHY is he in Blackrock Depths?"
#define RaggedJohn_GOSSIP_8 "300? So the Dark Irons killed him and dragged him into the Depths?"
#define RaggedJohn_GOSSIP_9 "Ahh... Ironfoe"
#define RaggedJohn_GOSSIP_10 "Thanks, Ragged John. Your story was very uplifting and informative"

class RaggedJohnGossip : public Arcemu::Gossip::Script
{
    public:
        void OnHello(Object *pObject, Player *Plr)
        {
            Arcemu::Gossip::Menu menu(pObject->GetGUID(), objmgr.GetGossipTextForNpc(pObject->GetEntry()), Plr->GetSession()->language);
            sQuestMgr.FillQuestMenu(TO_CREATURE(pObject), Plr, menu);
            if(Plr->HasQuest(4224) && !Plr->HasFinishedQuest(4224))
                menu.AddItem(Arcemu::Gossip::ICON_CHAT, RaggedJohn_GOSSIP_MAIN, 0);

            menu.Send(Plr);
        }

        void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* Code)
        {
            switch(Id)
            {
                case 0:  Arcemu::Gossip::Menu::SendQuickMenu(Plr->GetGUID(), 2714, Plr, 1, Arcemu::Gossip::ICON_CHAT, RaggedJohn_GOSSIP_0); break;
                case 1:  Arcemu::Gossip::Menu::SendQuickMenu(Plr->GetGUID(), 2715, Plr, 2, Arcemu::Gossip::ICON_CHAT, RaggedJohn_GOSSIP_1); break;
                case 2:  Arcemu::Gossip::Menu::SendQuickMenu(Plr->GetGUID(), 2716, Plr, 3, Arcemu::Gossip::ICON_CHAT, RaggedJohn_GOSSIP_2); break;
                case 3:  Arcemu::Gossip::Menu::SendQuickMenu(Plr->GetGUID(), 2717, Plr, 4, Arcemu::Gossip::ICON_CHAT, RaggedJphn_GOSSIP_3); break;
                case 4:  Arcemu::Gossip::Menu::SendQuickMenu(Plr->GetGUID(), 2718, Plr, 5, Arcemu::Gossip::ICON_CHAT, RaggedJohn_GOSSIP_4); break;
                case 5:  Arcemu::Gossip::Menu::SendQuickMenu(Plr->GetGUID(), 2719, Plr, 6, Arcemu::Gossip::ICON_CHAT, RaggedJohn_GOSSIP_5); break;
                case 6:  Arcemu::Gossip::Menu::SendQuickMenu(Plr->GetGUID(), 2720, Plr, 7, Arcemu::Gossip::ICON_CHAT, RaggedJohn_GOSSIP_6); break;
                case 7:  Arcemu::Gossip::Menu::SendQuickMenu(Plr->GetGUID(), 2721, Plr, 8, Arcemu::Gossip::ICON_CHAT, RaggedJohn_GOSSIP_7); break;
                case 8:  Arcemu::Gossip::Menu::SendQuickMenu(Plr->GetGUID(), 2722, Plr, 9, Arcemu::Gossip::ICON_CHAT, RaggedJohn_GOSSIP_8); break;
                case 9:  Arcemu::Gossip::Menu::SendQuickMenu(Plr->GetGUID(), 2723, Plr, 10, Arcemu::Gossip::ICON_CHAT, RaggedJohn_GOSSIP_9); break;
                case 10: Arcemu::Gossip::Menu::SendQuickMenu(Plr->GetGUID(), 2725, Plr, 11, Arcemu::Gossip::ICON_CHAT, RaggedJohn_GOSSIP_10); break;
                case 11:
                {
                    QuestLogEntry* pQuest = Plr->GetQuestLogForEntry(4224);
                    if(!pQuest)
                        return;

                    pQuest->Complete();
                    pQuest->SendQuestComplete();
                    pQuest->UpdatePlayerFields();
                    Arcemu::Gossip::Menu::Complete(Plr);
                }
            }
        }
};

void SetupBurningSteppes(ScriptMgr* mgr)
{
    mgr->register_creature_gossip(26596, new RaggedJohnGossip);

    QuestScript* tablet = new TabletOfTheSeven;
    mgr->register_quest_script(4296, tablet);
}
