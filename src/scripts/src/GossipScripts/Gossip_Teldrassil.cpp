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

class ErelasAmbersky_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2153, plr);
			
			Menu->AddItem( 0, "Tell me more about these hippogryphs.", 1);

        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		if(!pObject->IsCreature())
			return;

		GossipMenu * Menu;
        switch(IntId)
        {
        case 1:
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2154, plr);
				Menu->SendTo(plr);
            }break;
		}
    }

};

void SetupTeldrassilGossip(ScriptMgr * mgr)
{
	GossipScript * ErelasAmberskyGossip = new ErelasAmbersky_Gossip;
	
	mgr->register_gossip_script(7916, ErelasAmberskyGossip); // Erelas Ambersky
}
