/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
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

#include "Setup.h"

bool BaronVardusAllowSpawn = true;

class WantedBaronVardus : public QuestScript
{
	public:

		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			if(BaronVardusAllowSpawn == true)
			{
				uint32 rand = RandomUInt(3);
				switch(rand)
				{
					case 0:
						mTarget->GetMapMgr()->GetInterface()->SpawnCreature(2306, 692.64f, -904.74f, 157.79f, 0, true, false, 0, 0)->Despawn(1800000, 0);
					case 1:
						mTarget->GetMapMgr()->GetInterface()->SpawnCreature(2306, 939.0f, -852.46f, 114.644f, 0, true, false, 0, 0)->Despawn(1800000, 0);
					case 2:
						mTarget->GetMapMgr()->GetInterface()->SpawnCreature(2306, 1184.07f, -553.43f, 71.3346f, 0, true, false, 0, 0)->Despawn(1800000, 0);
					case 3:
						mTarget->GetMapMgr()->GetInterface()->SpawnCreature(2306, 1001.20f, -793.93f, 108.65f, 0, true, false, 0, 0)->Despawn(1800000, 0);
				}
				BaronVardusAllowSpawn = false;
			}
		}
};

class Baron_Vardus : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Baron_Vardus);
		Baron_Vardus(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnDied(Unit* mKiller)
		{
			BaronVardusAllowSpawn = true;
		}
};

// Quest: 533
class HelcularRevengeFlames : public GameObjectAIScript
{
    public:
        ADD_GAMEOBJECT_FACTORY_FUNCTION(HelcularRevengeFlames)
        HelcularRevengeFlames(GameObject *g) : GameObjectAIScript(g)
        {
            entry = g->GetEntry();
        }

        void OnActivate(Player *pPlayer)
        {
            QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry(553);
            if(!pQuest)
                return;

            int32 go = -1;
            switch(entry)
            {
                case 1768: go = 0; break;
                case 1769: go = 1; break;
                case 1770: go = 2; break;
            }

            if(go != -1 && pQuest->GetMobCount(go) < pQuest->GetQuest()->required_mobcount[go])
            {
                pQuest->SetMobCount(go, pQuest->GetMobCount(go)+1);
                pQuest->SendUpdateAddKill(go);
                pQuest->UpdatePlayerFields();
            }
        }

    private:
        uint32 entry;
};

void SetupHillsbradFoothills(ScriptMgr* mgr)
{
	mgr->register_creature_script(2306, &Baron_Vardus::Create);
    mgr->register_quest_script(566, new WantedBaronVardus);
    mgr->register_gameobject_script(1768, &HelcularRevengeFlames::Create);
    mgr->register_gameobject_script(1769, &HelcularRevengeFlames::Create);
    mgr->register_gameobject_script(1770, &HelcularRevengeFlames::Create);
}
