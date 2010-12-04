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

class ArchmageMalin_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 11469, plr);

		if(plr->GetQuestLogForEntry(11223))
        Menu->AddItem( 0, "Can you send me to Theramore? I have an urgent message for Lady Jaina from Highlord Bolvar.", 1);
        
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature* pCreature = (pObject->IsCreature())?(TO_CREATURE(pObject)):NULL;
		if(!pObject->IsCreature())
			return;
		
		switch(IntId)
        {
        case 1:
			{
				plr->Gossip_Complete();
				pCreature->CastSpell(plr, dbcSpell.LookupEntry(42711), true);
            }break;
		}
    }

};

/*********************************************
// Stormwind Harbor View Taxi Begin
**********************************************/

//This is when you talk to Thargold Ironwing...He will fly you through Stormwind Harbor to check it out.
class SCRIPT_DECL SWHarborFlyAround : public GossipScript
{
public:
    void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
    void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
    void GossipEnd(Object * pObject, Player* Plr);
};

void SWHarborFlyAround::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
    GossipMenu *Menu;
	objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 13454, Plr);
	Menu->AddItem( 0, "Yes, please.", 1 );
	Menu->AddItem( 0, "No, thank you.", 2 );
	if(AutoSend)
    Menu->SendTo(Plr);
}

void SWHarborFlyAround::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
	Creature * pCreature = (pObject->IsCreature())?TO_CREATURE(pObject):NULL;
	if(pCreature==NULL)
		return;

    switch(IntId)
    {
    case 1:{
	TaxiPath * taxipath = sTaxiMgr.GetTaxiPath(1041);
	Plr->TaxiStart(taxipath, 25679, 0);
	}break;
	
	case 2:
	{Plr->Gossip_Complete();}
	break;
    }
}

void SWHarborFlyAround::GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

void SetupStormwindGossip(ScriptMgr * mgr)
{
	GossipScript * ArchmageMalinGossip = new ArchmageMalin_Gossip;
	mgr->register_gossip_script(2708, ArchmageMalinGossip); // Archmage Malin
	GossipScript * SWHARFLY = new SWHarborFlyAround();
	mgr->register_gossip_script(29154, SWHARFLY);
}
