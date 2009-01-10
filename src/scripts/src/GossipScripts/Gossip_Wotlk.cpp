/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
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

#ifdef WIN32
#pragma warning(disable:4305)		// warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

#define UNKOWN_NPC_TEXT 2593 //tempolary
#define BACK_BUTTON
#ifdef BACK_BUTTON
#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
			Menu->SendTo(Plr);
#else

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
		Menu->AddItem(0, "I was looking for something else.", 0); \
		Menu->SendTo(Plr);
#endif

class SCRIPT_DECL CoTChromie : public GossipScript
{
public:
    void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
    void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
    void GossipEnd(Object * pObject, Player* Plr);
	void Destroy()
	{
		delete this;
	}
};

void CoTChromie::GossipHello(Object* pObject, Player * Plr, bool AutoSend) {
	GossipMenu *Menu;
	objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), UNKOWN_NPC_TEXT, Plr);
	Menu->AddItem(0, "Why have I been sent back to this particular place and time?", 1);
    if(AutoSend)
		Menu->SendTo(Plr);
}

void CoTChromie::GossipEnd(Object * pObject, Player* Plr) {
    GossipScript::GossipEnd(pObject, Plr);
}

void CoTChromie::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
	/*
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
	if(pCreature==NULL)
		return;

    switch(IntId) {
		case 1: { 
			// Chromie case 1, goto this page display page text and next page gossip
		}
	}
	*/
}

/*	TODO!
	- Teleportation NPCs from Dalaran -> CoT 
	- CoT Gossip scripts, Wotlk gossips :D
*/

void SetupWotlkgossips(ScriptMgr * mgr)
{	// Define them here, sorta :P
	GossipScript * cotnpc = (GossipScript*) new CoTChromie();
	// List npcs here
	mgr->register_gossip_script(26527,  cotnpc);	// Chromie 
	mgr->register_gossip_script(27915,  cotnpc);	// Chromie again..wtf BLIZZARD FAGS!</3
}