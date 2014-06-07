/*
 * EasyWoW MMORPG Server
 * Copyright (C) 2014 EasyWoW Team <http://github.com/EasyWoW>
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

class StabledArgentHippogryph_Gossip : public Arcemu::Gossip::Script
{
public:
    void OnHello(Object *pObject, Player *plr)
    {
        Arcemu::Gossip::Menu menu(pObject->GetGUID(), objmgr.GetGossipTextForNpc(pObject->GetEntry()));
        if (plr->HasQuest(14027))
            menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Mount the Hippogryph and prepare for battle!", 1);
        menu.Send(plr);
    }

    void OnSelectOption(Object *pObject, Player *plr, uint32 Id, const char *Code)
    {
        plr->TaxiStart(sTaxiMgr.GetTaxiPath(1268), 22471, 0);
    }
};

void SetupIcecrownQuests(ScriptMgr* mgr)
{
    mgr->register_creature_gossip(35117, new StabledArgentHippogryph_Gossip);
}

