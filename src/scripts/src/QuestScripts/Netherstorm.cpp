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

class SCRIPT_DECL Veronia : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(Plr->GetQuestLogForEntry(10652))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			Menu->AddItem( 0, "I'm ready", 1);
			Menu->SendTo(Plr);
		}
	}

	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature* creat = static_cast<Creature*>(pObject);
		switch(IntId)
		{
		case 1:
			creat->CastSpell(Plr, dbcSpell.LookupEntry(34905), true);
			break;
		}
	}
}; 

void SetupNetherstorm(ScriptMgr* mgr)
{
	mgr->register_gossip_script(20162, CREATE_GOSSIPSCRIPT(Veronia));
}