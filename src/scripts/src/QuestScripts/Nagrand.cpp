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

// Giselda The Crone Quest Horde
class GiseldaHordeQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GiseldaHordeQAI);
    GiseldaHordeQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit * mKiller)
	{
		if (mKiller->IsPlayer()) 
		{
			QuestLogEntry *en = ((Player*)mKiller)->GetQuestLogForEntry(9935);
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

// Giselda The Crone Quest Alliance
class GiseldaTheCroneAllianceQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GiseldaTheCroneAllianceQAI);
    GiseldaTheCroneAllianceQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit * mKiller)
	{
		if (mKiller->IsPlayer()) 
		{
			QuestLogEntry *en = ((Player*)mKiller)->GetQuestLogForEntry(9936);
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

void SetupNagrand(ScriptMgr * mgr)
{
	mgr->register_creature_script(17147, &GiseldaHordeQAI::Create);
	mgr->register_creature_script(17148, &GiseldaHordeQAI::Create);
	mgr->register_creature_script(18397, &GiseldaHordeQAI::Create);
	mgr->register_creature_script(18658, &GiseldaHordeQAI::Create);
	mgr->register_creature_script(17146, &GiseldaHordeQAI::Create);

	mgr->register_creature_script(17147, &GiseldaTheCroneAllianceQAI::Create);
	mgr->register_creature_script(17148, &GiseldaTheCroneAllianceQAI::Create);
	mgr->register_creature_script(18397, &GiseldaTheCroneAllianceQAI::Create);
	mgr->register_creature_script(18658, &GiseldaTheCroneAllianceQAI::Create);
	mgr->register_creature_script(17146, &GiseldaTheCroneAllianceQAI::Create);
}
