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
#include "../EAS/EasyFunctions.h"


#include "StdAfx.h"
#include "Setup.h"
#include "../EAS/EasyFunctions.h"

class SCRIPT_DECL Lady_Jaina : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(plr->GetQuestLogForEntry(558))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7012, plr);
			Menu->AddItem(0, "I know this is rather silly but i have a young ward who is a bit shy and would like your autograph.", 1);
			Menu->SendTo(plr);
		}
	}

	void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
		if(pObject==NULL)
			return;

		switch(IntId)
		{
		case 0: // Return to start
			GossipHello(pCreature, plr, true);
			break;
		case 1: // Give Item
			{
				plr->CastSpell(plr, dbcSpell.LookupEntry(23122),true);
				plr->Gossip_Complete();
				break;
			}
			break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};

class SCRIPT_DECL Cairne : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(plr->GetQuestLogForEntry(925))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7013, plr);
			Menu->AddItem(0, "Give me hoofprint.", 1);
			Menu->SendTo(plr);
		}
	}

	void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
	{
		GossipMenu *Menu;
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
		if(pObject==NULL)
			return;

		switch(IntId)
		{
		case 0: // Return to start
			GossipHello(pCreature, plr, true);
			break;
		case 1: // Give Item
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7014, plr);
				Menu->SendTo(plr);
				plr->CastSpell(plr, dbcSpell.LookupEntry(23123),true);
				break;
			}
			break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};

void SetupQuestGossip(ScriptMgr * mgr)
{
	GossipScript * LJ = (GossipScript*) new Lady_Jaina();
	GossipScript * CB = (GossipScript*) new Cairne();

	mgr->register_gossip_script(4968, LJ);
	mgr->register_gossip_script(3057, CB);

}
