/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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

#define GOSSIP_AK_ITEM_1 "Grant me your mark, wise ancient."
#define GOSSIP_AK_ITEM_2 "Grant me your mark, mighty ancient."

class AshyenAndKeleth_Gossip : public Arcemu::Gossip::Script
{
    public:
        void OnHello(Object* pObject, Player* plr)
        {
            Arcemu::Gossip::Menu menu(pObject->GetGUID(), objmgr.GetGossipTextForNpc(pObject->GetEntry()), plr->GetSession()->language);
            if (plr->HasQuest(9785) || plr->HasFinishedQuest(9785))
                menu.AddItem(Arcemu::Gossip::ICON_CHAT, pObject->GetEntry() == 17900 ? GOSSIP_AK_ITEM_1 : GOSSIP_AK_ITEM_2, 1);
            menu.Send(plr);
        }

        void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
        {
            switch(plr->GetStandingRank(942))
            {
                case STANDING_FRIENDLY: TO_CREATURE(pObject)->CastSpell(plr, pObject->GetEntry() == 17900 ? 31808 : 31807 , true); break;
                case STANDING_HONORED: TO_CREATURE(pObject)->CastSpell(plr, pObject->GetEntry() == 17900 ? 31810 : 31812, true); break;
                case STANDING_REVERED: TO_CREATURE(pObject)->CastSpell(plr, pObject->GetEntry() == 17900 ? 31811 : 31813, true); break;
                case STANDING_EXALTED: TO_CREATURE(pObject)->CastSpell(plr, pObject->GetEntry() == 17900 ? 31815 : 31814, true); break;
                default:
                    break;
            }
        }
};

// Quest: 9803
#define ElderKuruti_OPTION_MAIN "Greetings, elder. It is time for your people to end their hostility toward the draenei and their allies."
#define ElderKuruti_OPTION_0 "I did not mean to deceive you, elder. The draenei of Telredor thought to approach you in a way that would be familiar to you."
#define ElderKuruti_OPTION_1 "I will tell them. Farewell, elder."
class ElderKuruti : public Arcemu::Gossip::Script
{
	public:
        void OnHello(Object* pObject, Player* plr)
		{
            Arcemu::Gossip::Menu menu(pObject->GetGUID(), 9226, plr->GetSession()->language);
            if(!plr->HasQuest(9803) && !plr->HasFinishedQuest(9803))
                menu.AddItem(Arcemu::Gossip::ICON_CHAT, ElderKuruti_OPTION_MAIN, 0);
            menu.Send(plr);
		}

        void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
            switch(Id)
			{
                case 0:
                    Arcemu::Gossip::Menu::SendQuickMenu(pObject->GetGUID(), 9227, plr, 1, Arcemu::Gossip::ICON_CHAT, ElderKuruti_OPTION_0);
					break;
                case 1:
                    Arcemu::Gossip::Menu::SendQuickMenu(pObject->GetGUID(), 9229, plr, 2, Arcemu::Gossip::ICON_CHAT, ElderKuruti_OPTION_1);
					break;
                case 2:
                    if(!plr->GetItemInterface()->GetItemCount(24573, true))
                        sEAS.AddItem(24573, plr);

                    Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 9231, plr);
					break;
			}
		}
};

void SetupZangarmarsh(ScriptMgr* mgr)
{
    mgr->register_creature_gossip(17900, new AshyenAndKeleth_Gossip);
    mgr->register_creature_gossip(17901, new AshyenAndKeleth_Gossip);
    mgr->register_creature_gossip(18197, new ElderKuruti);
}
