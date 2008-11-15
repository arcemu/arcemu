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

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, plr); \
    Menu->SendTo(plr);

class SCRIPT_DECL BeatenCorpse : public GossipScript
{
public:
  void GossipHello(Object * pObject, Player * plr, bool AutoSend)
  {
    GossipMenu *Menu;
    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3557, plr);

    if(plr->GetQuestLogForEntry(4921) != NULL)
      Menu->AddItem(0, "I inspect the body further.", 1);
    
    if(AutoSend)
      Menu->SendTo(plr);
  }

  void GossipSelectOption(Object * pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
  {      
    GossipMenu *Menu;

    switch(IntId)	
		{
			case 1:
      {
         SendQuickMenu(3558);

         QuestLogEntry *qle = plr->GetQuestLogForEntry(4921);
         if(qle == NULL)
           return;
           
         if(qle->GetMobCount(0) != 0)
           return;

         qle->SetMobCount(0, 1);
         qle->SendUpdateAddKill(0);
         qle->UpdatePlayerFields();
		  }break;
    }
  }
};

void SetupBarrens(ScriptMgr *mgr)
{
  GossipScript *gos = (GossipScript*) new BeatenCorpse;
  mgr->register_gossip_script(10668, gos);
}
