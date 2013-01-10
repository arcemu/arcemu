/*
 * Moon++ Scripts for Ascent MMORPG Server
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

class SkornWhitecloud_Gossip : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			//Send quests and gossip menu.
			uint32 Text = objmgr.GetGossipTextForNpc(pObject->GetEntry());

			if(NpcTextStorage.LookupEntry(Text) == NULL)
				Text = Arcemu::Gossip::DEFAULT_TXTINDEX;
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), Text, plr->GetSession()->language);

			// After killing Ghosthowl Skorn Whitecloud changes his Text
			if (plr->HasFinishedQuest(770)) {
				menu.setTextID(521);
			}
			
			sQuestMgr.FillQuestMenu(TO_CREATURE(pObject), plr, menu);

			// Localized chat menu for German
			if (plr->GetSession()->language == 1) {
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Erzaehlt mir eine Geschichte, Skorn", 0);
			}
			else {
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Tell me a story, Skorn.", 0);
			}

			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			if(!pObject->IsCreature())
				return;
			Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 523, plr);
		}

		void Destroy() { delete this; }
};

void SetupMulgoreGossip(ScriptMgr* mgr)
{
	mgr->register_creature_gossip(3052, new SkornWhitecloud_Gossip); // Skorn Whitecloud
}
