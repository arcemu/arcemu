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

// The Bladespire Threat Quest
class BladespireQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BladespireQAI);
    BladespireQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit * mKiller)
	{
		if (mKiller->IsPlayer()) 
		{
			QuestLogEntry *en = ((Player*)mKiller)->GetQuestLogForEntry(10503);
			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				uint32 newcount = en->GetMobCount(0) + 1;
				en->SetMobCount(0, newcount);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();
				return;
			}
		}
	}
};

void SetupBladeEdgeMountains(ScriptMgr * mgr)
{
	mgr->register_creature_script(19995, &BladespireQAI::Create);
	mgr->register_creature_script(21296, &BladespireQAI::Create);
	mgr->register_creature_script(20765, &BladespireQAI::Create);
	mgr->register_creature_script(20766, &BladespireQAI::Create);
	mgr->register_creature_script(19998, &BladespireQAI::Create);
}
