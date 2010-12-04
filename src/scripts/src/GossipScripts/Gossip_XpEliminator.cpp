/*
 * Gossip scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team <http://www.moonplusplus.info/>
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

// XpEleminatorGossip
//  GossipScript subclass for turning on/off Player experience gain
class XpEliminatorGossip : public GossipScript{

public:

	XpEliminatorGossip(){
		GOSSIP_DISABLE_XP_GAIN = "I no longer wish to gain experience.";
		GOSSIP_ENABLE_XP_GAIN = "I wish to start gaining experience again";

		GOSSIP_BOXMSG_DISABLE_XP_GAIN = "Are you certain you wish to stop gaining experience?";
		GOSSIP_BOXMSG_ENABLE_XP_GAIN = "Are you certain you wish to start gaining experience again?";
	}

	void GossipHello(Object* pObject, Player* plr, bool AutoSend){

        GossipMenu *Menu;

        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 14736, plr);

		
		if( plr->CanGainXp() )
			Menu->AddMenuItem( 0, GOSSIP_DISABLE_XP_GAIN, 0, 1, const_cast< char*>( GOSSIP_BOXMSG_DISABLE_XP_GAIN ), 100000, false );
		else
			Menu->AddMenuItem( 0, GOSSIP_ENABLE_XP_GAIN, 0, 1, const_cast< char* >( GOSSIP_BOXMSG_ENABLE_XP_GAIN ), 100000, false );
        
        if(AutoSend)
            Menu->SendTo(plr);
    }

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code){
		if(!pObject->IsCreature())
			return;
		
		if( IntId == 1 ){

			// turning xp gains on/off costs 10g each time
			if( plr->HasGold( 100000 ) ){
				plr->ModGold( -100000 );
				plr->ToggleXpGain();
			}
		}

		plr->Gossip_Complete();
    }
	
private:
	const char *GOSSIP_DISABLE_XP_GAIN;
	const char *GOSSIP_ENABLE_XP_GAIN;
	
	const char *GOSSIP_BOXMSG_DISABLE_XP_GAIN;
	const char *GOSSIP_BOXMSG_ENABLE_XP_GAIN;

};

void SetupXpEliminatorGossip(ScriptMgr * mgr)
{

	GossipScript *xegs = new XpEliminatorGossip();

	mgr->register_gossip_script( 35364, xegs );  // Slahtzt the Horde NPC
	mgr->register_gossip_script( 35365, xegs );  // Besten the Alliance NPC
}
