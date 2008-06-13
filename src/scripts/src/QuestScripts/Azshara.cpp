/*
 * WEmu Scripts for WEmu MMORPG Server
 * Copyright (C) 2008 WEmu Team
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

/************************************************************************/
/*										  Arximag Teleport  Azshara													*/
/************************************************************************/

class SCRIPT_DECL ArximagTeleportDown : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1994, Plr);

		Menu->AddItem(0, "[i am, ready]", 1);

		if(AutoSend)
			Menu->SendTo(Plr);
	}
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
	if(pCreature==NULL)
	{
		return;
		}
		GossipMenu * Menu;
		switch(IntId)
		{
		case 0:
			GossipHello(pObject, Plr, true);
			break;
		case 1:
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1994, Plr);
				 Plr->EventTeleport(1, 3649.567627, -4705.155762, 120.966309);
			}break;
		}
	}
};

class SCRIPT_DECL ArximagTeleportUp : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);

		Menu->AddItem(0, "[i am, ready]", 1);


		if(AutoSend)
			Menu->SendTo(Plr);
	}
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
	if(pCreature==NULL)
	{
		return;
		}

		GossipMenu * Menu;
		switch(IntId)
		{
		case 0:
			GossipHello(pObject, Plr, true);
			break;
		case 1:
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
				 Plr->EventTeleport(1, 3778.438721, -4612.299805, 227.253922);
			}break;
		}
	}
};

void SetupAzshara(ScriptMgr * mgr)
{
	GossipScript * axtd = (GossipScript*) new ArximagTeleportDown();
	GossipScript * axtu = (GossipScript*) new ArximagTeleportUp();

	mgr->register_gossip_script(8399, axtd); 
	mgr->register_gossip_script(8395, axtu); 

}
