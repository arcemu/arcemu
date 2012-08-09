/*
 * Gossip scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2011 Arcemu Team <http://www.moonplusplus.info/>
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

// XpEleminatorGossip
//  GossipScript subclass for turning on/off Player experience gain
#define	GOSSIP_DISABLE_XP_GAIN 240
#define	GOSSIP_ENABLE_XP_GAIN 241
#define	GOSSIP_BOXMSG_DISABLE_XP_GAIN 242
#define	GOSSIP_BOXMSG_ENABLE_XP_GAIN 243

class XpEliminatorGossip : public Arcemu::Gossip::Script
{
	void OnHello(Object* pObject, Player* plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 14736);
			if(plr->CanGainXp())
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, plr->GetSession()->LocalizedGossipTexts(GOSSIP_DISABLE_XP_GAIN), 1, 100000, plr->GetSession()->LocalizedGossipTexts(GOSSIP_BOXMSG_DISABLE_XP_GAIN));
			else
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, plr->GetSession()->LocalizedGossipTexts(GOSSIP_ENABLE_XP_GAIN), 1, 100000, plr->GetSession()->LocalizedGossipTexts(GOSSIP_BOXMSG_ENABLE_XP_GAIN));

			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			// turning xp gains on/off costs 10g each time
			if(plr->HasGold(100000))
			{
				plr->ModGold(-100000);
				plr->ToggleXpGain();
			}
			Arcemu::Gossip::Menu::Complete(plr);
		}
};

void SetupXpEliminatorGossip(ScriptMgr* mgr)
{

	Arcemu::Gossip::Script* xegs = new XpEliminatorGossip();

	mgr->register_creature_gossip(35364, xegs);    // Slahtzt the Horde NPC
	mgr->register_creature_gossip(35365, xegs);    // Besten the Alliance NPC
}
