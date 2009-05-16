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
#include "EAS/EasyFunctions.h"

// Hidden Enemies Quest 5727
class SCRIPT_DECL Neeru : public GossipScript
{
public:
        void GossipHello(Object* pObject, Player* pPlayer, bool AutoSend)
        {
        if(!pPlayer)
            return;
        GossipMenu *Menu;
        Creature *Neeru = (Creature*)(pObject);
        if (Neeru == NULL)
            return;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4513, pPlayer);
            QuestLogEntry *en = pPlayer->GetQuestLogForEntry(5727);
        if (en && en->GetExploredAreas(0)==0)
            Menu->AddItem( 0, "You may speak frankly, Neeru...", 1);
        if(AutoSend)
            Menu->SendTo(pPlayer);
        }
        void GossipSelectOption(Object* pObject, Player* pPlayer, uint32 Id, uint32 IntId, const char * EnteredCode)
        {
        if(!pPlayer)
            return;
        Creature *Neeru = (Creature*)(pObject);
        if (Neeru == NULL)
            return;
        switch (IntId)
        {
            case 0:
                GossipHello(pObject, pPlayer, true);
                break;
            case 1:
            {
            GossipMenu *Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4533, pPlayer);
            Menu->AddItem( 0, "It is good to see the Burning Blade is taking over where the Shadow Council once failed.", 2);
            Menu->SendTo(pPlayer);
                return;
            }break;
            case 2:
            {
            GossipMenu *Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4534, pPlayer);
            Menu->AddItem( 0, "So the Searing Blade is expendable?", 3);
            Menu->SendTo(pPlayer);
                return;
            }break;
            case 3:
            {
            GossipMenu *Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4535, pPlayer);
            Menu->AddItem( 0, "Is there anything you would have of me?", 4);
            Menu->SendTo(pPlayer);
                return;
            }break;
            case 4:
            {
            QuestLogEntry *en = pPlayer->GetQuestLogForEntry(5727);
            if(en && en->GetExploredAreas(0)==0)
                en->SetTrigger(0);
                en->UpdatePlayerFields();
                en->SendQuestComplete();
                GossipMenu *Menu;
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4536, pPlayer);
                Menu->SendTo(pPlayer);
                return;
            }break;
        }
        }
        void Destroy()
        {
            delete this;
        }
};

void SetupOrgrimmar(ScriptMgr * mgr)
{
	GossipScript * NeeruGossip = (GossipScript*) new Neeru();
    mgr->register_gossip_script(3216, NeeruGossip);
}
