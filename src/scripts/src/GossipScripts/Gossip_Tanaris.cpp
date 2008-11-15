/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
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

#include "StdAfx.h"
#include "Setup.h"

// Trenton Lighthammer
#define GOSSIP_TRENTON_LIGHTHAMMER    "Tell me more, Trenton."

class SCRIPT_DECL TrentonLighthammer_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1758, plr);
        Menu->AddItem( 0, GOSSIP_TRENTON_LIGHTHAMMER, 1);
        
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
		if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;
		
		GossipMenu * Menu;
        switch(IntId)
        {
        case 1:
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1759, plr);
				Menu->SendTo(plr);
            }break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};

void SetupTanarisGossip(ScriptMgr * mgr)
{
	GossipScript * TrentonLighthammerGossip = (GossipScript*) new TrentonLighthammer_Gossip;
	mgr->register_gossip_script(7804, TrentonLighthammerGossip); // Trenton Lighthammer

}
