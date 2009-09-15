/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
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

class LogRide_Gossip : public GossipScript

{
	public:
	void GossipHello(Object* pObject, Player * plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 40002, plr);
		Menu->AddItem( 0, "Get On The Log", 1 );
		if(AutoSend)
			Menu->SendTo(plr);
	}
	void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
		if( pCreature == NULL )
			return;
		switch(IntId)
		{
			case 1:
			{
				if(pCreature->GetEntry() == 27414)
				{
					TaxiPath * path = sTaxiMgr.GetTaxiPath(869);
					plr->TaxiStart( path, 26609, 0 );
				}
				else
				{
					TaxiPath * path = sTaxiMgr.GetTaxiPath(877);
					plr->TaxiStart( path, 26610, 0 );
				}
			}break;
		}
 	}

	void Destroy()
	{
		delete this;
	}
 };

void SetupGrizzlyHills(ScriptMgr * mgr)
{
	GossipScript * LogRide = (GossipScript*) new LogRide_Gossip();
	mgr->register_gossip_script(27414, LogRide);
	mgr->register_gossip_script(27425, LogRide);
}
