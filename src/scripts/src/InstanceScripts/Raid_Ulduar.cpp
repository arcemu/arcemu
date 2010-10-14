/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
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
#include "Raid_Ulduar.h"


// Ulduar Teleporter
void UlduarTeleporter::GossipHello(Object*  pObject, Player* Plr, bool AutoSend)
{
	GossipMenu *Menu;
    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 0, Plr);

    Menu->AddItem(0, "Teleport to the Expedition Base Camp.", 0);
    Menu->AddItem(0, "Teleport to the Formation Grounds.", 1);
    Menu->AddItem(0, "Teleport to the Colossal Forge.", 2);

    if(AutoSend)
		Menu->SendTo(Plr);
};

void UlduarTeleporter::GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char *Code)
{
	switch(IntId)
	{
	case 0:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), -735.864075f, -93.616364f, 429.841797f, 0.079723f);
		break;
	case 1:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), 130.710297f, -35.272095f, 409.804901f, 6.276515f);
		break;
	case 2:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), 539.894897f, -11.009894f, 409.804749f, 0.021830f);
		break;
	};
};

void SetupUlduar(ScriptMgr* mgr)
{
	GossipScript * UlduarTele = new UlduarTeleporter();
	mgr->register_go_gossip_script(194569, UlduarTele);
};