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

// Exarch Nasuun
#define GOSSIP_EXARCH_NASUUN_1    "Nasuun, do you know how long until we have an alchemy lab at the Sun's Reach Harbor?"
#define GOSSIP_EXARCH_NASUUN_2    "What do you know about the magical gates at the Sunwell Plateau being brought down?"
#define GOSSIP_EXARCH_NASUUN_3    "I have something else to ask you about."

//#define USE_THE_STATUS	// Decoment this is for the status

class ExarchNasuun_Gossip : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 12227);
#ifdef USE_THE_STATUS
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, GOSSIP_EXARCH_NASUUN_1, 1);   // this is the status
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, GOSSIP_EXARCH_NASUUN_2, 2);
#else
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, GOSSIP_EXARCH_NASUUN_2, 3);
#endif
			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			switch(Id)
			{
				case 0:
					OnHello(pObject, plr);
					break;
				case 1:
					Arcemu::Gossip::Menu::SendQuickMenu(pObject->GetGUID(), 12303, plr, 0, Arcemu::Gossip::ICON_CHAT, GOSSIP_EXARCH_NASUUN_3);
					break;
				case 2:
					Arcemu::Gossip::Menu::SendQuickMenu(pObject->GetGUID(), 12305, plr, 0, Arcemu::Gossip::ICON_CHAT, GOSSIP_EXARCH_NASUUN_3);
					break;
				case 3:
					Arcemu::Gossip::Menu::SendQuickMenu(pObject->GetGUID(), 12623, plr, 0, Arcemu::Gossip::ICON_CHAT, GOSSIP_EXARCH_NASUUN_3);
					break;
				default:
					break;
			}
		}

};

class ZephyrGossipScript : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* Plr)
		{
			Arcemu::Gossip::Menu::SendQuickMenu(pObject->GetGUID(), 1, Plr, 1, Arcemu::Gossip::ICON_CHAT, "Bring me to Caverns of Time!");
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* EnteredCode)
		{
			if(plr->GetStanding(989) >= 21000)
				//plr->SafeTeleport( 1, 0, -8170.441406f, -4751.321777f, 33.457771f, 5.136f);
				TO_CREATURE(pObject)->CastSpell(plr, dbcSpell.LookupEntry(37778), true);
			else
				plr->BroadcastMessage("You need to be Revered with the faction Keepers of Time!"); // Dunno what the correct text is ^^
			Arcemu::Gossip::Menu::Complete(plr);
		}
		void Destroy() { delete this; }

};

void SetupShattrathGossip(ScriptMgr* mgr)
{
	mgr->register_creature_gossip(25967, new ZephyrGossipScript);		// Zephyr
	mgr->register_creature_gossip(24932, new ExarchNasuun_Gossip); // Exarch Nasuun

}
