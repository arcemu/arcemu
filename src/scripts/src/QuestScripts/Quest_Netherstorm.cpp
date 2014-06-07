/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2008 WEmu Team
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

// Quest: 10652
class Veronia : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), objmgr.GetGossipTextForNpc(pObject->GetEntry()), plr->GetSession()->language);
			if(plr->HasQuest(10652))
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "I'm ready", 0);
			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
            plr->Gossip_Complete();
			TO_CREATURE(pObject)->CastSpell(plr, 34905, true);
		}
};

// Quest: 10438
class ProtectorateNetherDrake_Gossip : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), objmgr.GetGossipTextForNpc(pObject->GetEntry()), plr->GetSession()->language);
			if(plr->HasQuest(10438))
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Fly me to Ultris", 0);
			menu.Send(plr);
		}
 
		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
            plr->Gossip_Complete();
			plr->TaxiStart(sTaxiMgr.GetTaxiPath(627), 0, 0);
		}
};

void SetupNetherstorm(ScriptMgr* mgr)
{
	mgr->register_creature_gossip(20162, new Veronia);
	mgr->register_creature_gossip(20903, new ProtectorateNetherDrake_Gossip);
}
