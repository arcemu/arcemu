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

class CassaCrimsonwing_Gossip : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 11224, plr);

			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 11224);
			if(plr->GetQuestLogForEntry(11142) != NULL)
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Lady Jaina told me to speak to you about using a gryphon to survey Alcaz Island.", 1);

			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			plr->GetQuestLogForEntry(11142)->SendQuestComplete();
			plr->TaxiStart(sTaxiMgr.GetTaxiPath(724), 1147, 0);     // Gryph
		}

		void Destroy() { delete this; }

};

class CaptainGarranVimes_Gossip : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			//Send quests and gossip menu.
			uint32 Text = objmgr.GetGossipTextForNpc(pObject->GetEntry());
			if(NpcTextStorage.LookupEntry(Text) == NULL)
				Text = Arcemu::Gossip::DEFAULT_TXTINDEX;
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), Text, plr->GetSession()->language);
			sQuestMgr.FillQuestMenu(TO_CREATURE(pObject), plr, menu);
			if((plr->GetQuestLogForEntry(11123) != NULL) || (plr->GetQuestRewardStatus(11123) == 0)) 
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "What have you heard of the Shady Rest Inn?", 0);
			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 1794, plr);
		}

		void Destroy() { delete this; }

};

void SetupTheramoreGossip(ScriptMgr* mgr)
{
	mgr->register_creature_gossip(23704, new CassaCrimsonwing_Gossip); // Cassa Crimsonwing
	mgr->register_creature_gossip(4944, new CaptainGarranVimes_Gossip); // Captain Garran Vimes
}
