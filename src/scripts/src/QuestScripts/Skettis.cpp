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

// Hungry Nether Rays Quest 11093
class HungryNetherRay : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HungryNetherRay);
    HungryNetherRay(Creature* pCreature) : CreatureAIScript(pCreature)  {}

    void OnDied(Unit * mKiller)
    {
        if (mKiller->IsPlayer())
        {    
            Player*pPlayer = (Player*)mKiller;
            QuestLogEntry *en = ((Player*)mKiller)->GetQuestLogForEntry(11093);
            if (en == NULL)
            return;
            float SSX = _unit->GetPositionX();
            float SSY = _unit->GetPositionY();
            float SSZ = _unit->GetPositionZ();
            Creature*NetherRay = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX,SSY,SSZ,23439);
            if (NetherRay == NULL)
                return;
            if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
            {
                pPlayer->BroadcastMessage("|cffff8040Hungry Nether Ray feeds on the freshy-killed warp chaser");
                NetherRay->GetAIInterface()->MoveTo(SSX,SSY,SSZ,0.0f);
                uint32 newcount = en->GetMobCount(1) + 1;
                en->SetMobCount(1, newcount);
                en->SendUpdateAddKill(1);
                en->UpdatePlayerFields();
                return;
            }
        }
    }
};

void SetupSkettis(ScriptMgr * mgr)
{
	mgr->register_creature_script(23219, &HungryNetherRay::Create);
}
