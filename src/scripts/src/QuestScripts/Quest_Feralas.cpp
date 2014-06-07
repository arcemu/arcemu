/*
 * EasyWoW for ArcEmu MMORPG Server
 * Copyright (C) 2014 EasyWoW Team <http://github.com/EasyWoW>
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

// Quest: 3520
class ScreecherSpirit_Gossip : public Arcemu::Gossip::Script
{
    public:
        void OnHello(Object* pObject, Player* plr)
        {
            Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), objmgr.GetGossipTextForNpc(pObject->GetEntry()), plr);
            QuestLogEntry* quest = plr->GetQuestLogForEntry(3520);

            if(quest && quest->GetMobCount(0) < quest->GetQuest()->required_mobcount[0])
            {
                quest->SetMobCount(0, quest->GetMobCount(0) + 1);
                quest->SendUpdateAddKill(0);
                quest->UpdatePlayerFields();
            }
            TO_CREATURE(pObject)->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }
};

void SetupFeralasQuests(ScriptMgr* mgr)
{
    mgr->register_creature_gossip(8612, new ScreecherSpirit_Gossip);
}
