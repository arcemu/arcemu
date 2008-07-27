/*
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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

// Creating the Pendant Quest
bool Quest_CreatingThePendant(uint32 i, Spell * pSpell)
{
  if(!pSpell->u_caster->IsPlayer())
    return true;

  Player *plr = (Player*)pSpell->u_caster;
  QuestLogEntry *qle = plr->GetQuestLogForEntry(10567);
 
  if(qle == NULL)
    return true;

  uint32 entry = 21767;

  Creature *creat = sEAS.SpawnCreature(plr, entry, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 5*60*1000);

  return true;
}


void SetupBladeEdgeMountains(ScriptMgr * mgr)
{
	mgr->register_creature_script(19995, &BladespireQAI::Create);
	mgr->register_creature_script(21296, &BladespireQAI::Create);
	mgr->register_creature_script(20765, &BladespireQAI::Create);
	mgr->register_creature_script(20766, &BladespireQAI::Create);
	mgr->register_creature_script(19998, &BladespireQAI::Create);
	mgr->register_dummy_spell(37426, &Quest_CreatingThePendant);
}
