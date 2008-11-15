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

// Exarch Nasuun
#define GOSSIP_EXARCH_NASUUN_1    "Nasuun, do you know how long until we have an alchemy lab at the Sun's Reach Harbor?"
#define GOSSIP_EXARCH_NASUUN_2    "What do you know about the magical gates at the Sunwell Plateau being brought down?"
#define GOSSIP_EXARCH_NASUUN_3    "I have something else to ask you about."

class SCRIPT_DECL ExarchNasuun_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 12227, plr);
        Menu->AddItem( 0, GOSSIP_EXARCH_NASUUN_1, 1);  // this is the status
		Menu->AddItem( 0, GOSSIP_EXARCH_NASUUN_2, 2);
        
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
		case 0:
			GossipHello(pObject, plr, true);
			break;

        case 1:
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr); // Text of status [The Alchemy lab is not quite yet ready, $N. Mar'nah says she is (percentual) percent done with its assembly.$Bhowever.$B$BIf you wold like to help her with that, you will find her inside the inn at the Sun's Reach Harbor.]
				Menu->AddItem( 0, GOSSIP_EXARCH_NASUUN_3, 0);
				Menu->SendTo(plr);
            }break;
		case 2:
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 12305, plr);
				Menu->AddItem( 0, GOSSIP_EXARCH_NASUUN_3, 0);
				Menu->SendTo(plr);
            }break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};

void SetupShattrathGossip(ScriptMgr * mgr)
{
	GossipScript * ExarchNasuunGossip = (GossipScript*) new ExarchNasuun_Gossip;
	mgr->register_gossip_script(24932, ExarchNasuunGossip); // Exarch Nasuun

}
