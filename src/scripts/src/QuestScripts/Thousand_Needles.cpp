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

// Test of Faith Quest 1149
class SCRIPT_DECL TestofFaith : public QuestScript
{
public:
  void OnQuestStart(Player* pPlayer, QuestLogEntry *qLogEntry)
  {
    if(pPlayer == NULL || pPlayer->GetMapMgr() == NULL || pPlayer->GetMapMgr()->GetSqlIdCreature(359538) == NULL)
    return;
    Creature *Dorn = pPlayer->GetMapMgr()->GetSqlIdCreature(359538);
    Dorn->CastSpell(pPlayer,6714,false);
    Dorn->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Until we meet again, brave one");
    TimedEvent *t1 = TimedEvent::Allocate(this, new CallbackP1<TestofFaith, Player*>(this, &TestofFaith::SpinningEffect, pPlayer), 0, 4000, 1);  
    pPlayer->event_AddEvent(t1);
    TimedEvent *t2 = TimedEvent::Allocate(this, new CallbackP1<TestofFaith, Player*>(this, &TestofFaith::QuestComplete, pPlayer), 0, 1000, 0);  
    pPlayer->event_AddEvent(t2);
  }
   void SpinningEffect(Player * pPlayer)
   {
       pPlayer->SafeTeleport(1,0,-4884.568,-1596.381,101.201,3.140);
   }
   void QuestComplete(Player * pPlayer)
        {
            if(pPlayer == NULL)
            {
                sEventMgr.RemoveEvents(pPlayer);
                return;
            }
            QuestLogEntry *en = pPlayer->GetQuestLogForEntry(1149);
            if (en == NULL)
            {
                sEventMgr.RemoveEvents(pPlayer);
                return;
            }
            if(en->CanBeFinished())
            {
                pPlayer->SafeTeleport(1,400,-5187.037,-2805.358,-8.313,5.861);
                sEventMgr.RemoveEvents(pPlayer);
            }
        }
};

void SetupThousandNeedles(ScriptMgr * mgr)
{
	mgr->register_quest_script(1149, CREATE_QUESTSCRIPT(TestofFaith));
}
