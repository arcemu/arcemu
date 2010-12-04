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

class SilvaFilnaveth_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;
        if(plr->getClass() == DRUID && plr->getRace() == RACE_NIGHTELF){
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4914, plr);
			Menu->AddItem( 0, "I'd like to fly to Rut'theran Village.", 1);
		}else if(plr->getClass() == DRUID && plr->getRace() == RACE_TAUREN){
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4915, plr);
		}else{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4913, plr);
        };
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature* pCreature = (pObject->IsCreature())?(TO_CREATURE(pObject)):NULL;
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

};

class BunthenPlainswind_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;
		if(plr->getClass() == DRUID && plr->getRace() == RACE_TAUREN){
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4918, plr);
			Menu->AddItem( 0, "I'd like to fly to Thunder Bluff.", 1);
		}else if(plr->getClass() == DRUID && plr->getRace() == RACE_NIGHTELF){
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4917, plr);
		}else{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4916, plr);
        };
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature* pCreature = (pObject->IsCreature())?(TO_CREATURE(pObject)):NULL;
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

};

void SetupMoongladeGossip(ScriptMgr * mgr)
{
	GossipScript * SilvaFilnavethGossip = new SilvaFilnaveth_Gossip;
	GossipScript * BunthenPlainswindGossip = new BunthenPlainswind_Gossip;

	mgr->register_gossip_script(11800, SilvaFilnavethGossip); // Silva Fil'naveth
	mgr->register_gossip_script(11798, BunthenPlainswindGossip); // Bunthen Plainswind

}
