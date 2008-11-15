/*
 * ArcScript SCcripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.info/>
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
#define GOSSIP_SILVA_FILNAVETH    "I'd like to fly to Rut'theran Village."

class SilvaFilnaveth_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
        if(plr->getClass() == DRUID && plr->getRace() == RACE_NIGHTELF){
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4914, plr);
			Menu->AddItem( 0, GOSSIP_SILVA_FILNAVETH, 1);
		}else if(plr->getClass() == DRUID && plr->getRace() == RACE_TAUREN){
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4915, plr);
		}else{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4913, plr);
        };
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
		if(pCreature==NULL)
			return;

        switch(IntId)
        {
        case 1:
			{
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 315 ); // Flight Path
				plr->TaxiStart( path, 479, 0 ); // Hippogryph
            }break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};

// Bunthen Plainswind
#define GOSSIP_BUNTHEN_PLAINSWIND    "I'd like to fly to Thunder Bluff."

class BunthenPlainswind_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
		if(plr->getClass() == DRUID && plr->getRace() == RACE_TAUREN){
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4918, plr);
			Menu->AddItem( 0, GOSSIP_BUNTHEN_PLAINSWIND, 1);
		}else if(plr->getClass() == DRUID && plr->getRace() == RACE_NIGHTELF){
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4917, plr);
		}else{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4916, plr);
        };
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
		if(pCreature==NULL)
			return;

        switch(IntId)
        {
        case 1:
			{
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 316 ); // Flight Path
				plr->TaxiStart( path, 295, 0 ); // Wyvern
            }break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};

void SetupMoongladeScript(ScriptMgr * mgr)
{
	GossipScript * SilvaFilnavethGossip = (GossipScript*) new SilvaFilnaveth_Gossip;
	GossipScript * BunthenPlainswindGossip = (GossipScript*) new BunthenPlainswind_Gossip;

	mgr->register_gossip_script(11800, SilvaFilnavethGossip); // Silva Fil'naveth
	mgr->register_gossip_script(11798, BunthenPlainswindGossip); // Bunthen Plainswind

}
