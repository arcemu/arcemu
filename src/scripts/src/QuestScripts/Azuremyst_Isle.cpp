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
#include "EAS/EasyFunctions.h"

class SCRIPT_DECL DraeneiFishingNet : public GossipScript
{
public:
  void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
  {
    Item *item = (Item*)pObject;
    uint32 spellid = item->GetProto()->Spells[0].Id;
    Plr->CastSpell(Plr, dbcSpell.LookupEntry(spellid), false);

    QuestLogEntry *qle = Plr->GetQuestLogForEntry(9452);
    if(qle == NULL)
      return;

    uint8 chance = RandomUInt(10);
    if(chance <= 3)
    {
      sEAS.SpawnCreature(Plr, 17102, Plr->GetPositionX(), Plr->GetPositionY(), Plr->GetPositionZ(), Plr->GetOrientation(), 3*60*1000);
      return;
    }

    sEAS.AddItem(23614, Plr);

    qle->SendUpdateAddKill(1);
    qle->UpdatePlayerFields();

    Plr->Gossip_Complete();
  }
};

void SetupAzuremystIsle(ScriptMgr *mgr)
{
  mgr->register_item_gossip_script(23654, CREATE_GOSSIPSCRIPT(DraeneiFishingNet));
}
