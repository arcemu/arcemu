/*
 * WEmu Scripts for WEmu MMORPG Server
 * Copyright (C) 2008 WEmu Team
 * Based on Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team
 * Copyright (C) 2007-2008 Moon++ Team
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
#include "../EAS/EasyFunctions.h"

bool CenarionMoondust(uint32 i, Spell* pSpell) // Body And Heart (Alliance)
{
  const float pos[] = {6348.540039f, 128.124176f, 22.024008f, 4.172032f}; // x, y, z, o
  Player *p_caster = pSpell->p_caster;

  Creature *lunaclaw = p_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pos[0], pos[1], pos[2], 12138);
  if(lunaclaw != NULL)
  {
	if(!lunaclaw->isAlive())
	  lunaclaw->Delete();
	else
	  return true;
  }

  lunaclaw = sEAS.SpawnCreature(p_caster, 12138, pos[0], pos[1], pos[2], pos[3], 0);
  lunaclaw->GetAIInterface()->SetNextTarget(p_caster);

  sEAS.MoveToPlayer(p_caster, lunaclaw);

  return true;
}

bool CenarionLunardust(uint32 i, Spell* pSpell) // Body And Heart (Horde)
{
  const float pos[] = {-2449.117920f, -1627.319824f, 91.801430f, 0}; // x, y, z, o
  Player *p_caster = pSpell->p_caster;

  Creature *lunaclaw = p_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pos[0], pos[1], pos[2], 12138);
  if(lunaclaw != NULL)
  {
	if(!lunaclaw->isAlive())
	  lunaclaw->Delete();
	else
	  return true;
  }

  lunaclaw = sEAS.SpawnCreature(p_caster, 12138, pos[0], pos[1], pos[2], pos[3], 0);
  lunaclaw->GetAIInterface()->SetNextTarget(p_caster);

  sEAS.MoveToPlayer(p_caster, lunaclaw);

  return true;
}

class Lunaclaw : public CreatureAIScript
{
public:
  ADD_CREATURE_FACTORY_FUNCTION(Lunaclaw);

  Lunaclaw(Creature* pCreature) : CreatureAIScript(pCreature) {}

  void OnDied(Unit *mKiller)
  {
	if(!mKiller->IsPlayer())
	  return;

	Player *plr = static_cast<Player*>(mKiller);
	const uint32 quests[] = {6001, 6002};
	QuestLogEntry *qle;
	bool questOk = false;

	for(int i = 0; i<2; i++)
	{
	  if(plr->GetQuestLogForEntry(quests[i]) != NULL)
	  {
		questOk = true;
		qle = plr->GetQuestLogForEntry(quests[i]);

		break;
	  }
	}

	if(!questOk)
	  return;

			if(plr == NULL || plr->GetMapMgr() == NULL || plr->GetMapMgr()->GetInterface() == NULL)
				return;
	sEAS.SpawnCreature(plr, 12144, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, 1*60*1000);

	qle->SendQuestComplete();
	qle->UpdatePlayerFields();
  }
};

class MoongladeQuest : public QuestScript
{	
public:
  void OnQuestStart(Player * mTarget, QuestLogEntry * qLogEntry)
  {
	if(!mTarget->HasSpell(19027))
	  mTarget->CastSpell(mTarget, dbcSpell.LookupEntry(19027), true);
  }
};

bool CurativeAnimalSalve(uint32 i, Spell* pSpell) // Curing the Sick
{
	Player *caster = pSpell->p_caster;
	if(caster == NULL)
		return true;

	if(!pSpell->GetUnitTarget()->IsCreature())
		return true;

	Creature* target = static_cast<Creature*>(pSpell->GetUnitTarget());
	uint32 entry = target->GetEntry();

	if(entry == 12296 || entry == 12298)
	{
		QuestLogEntry *qle = caster->GetQuestLogForEntry(6129);
		if(qle == NULL)
		{
			qle = caster->GetQuestLogForEntry(6124);
			if(qle == NULL)
				return true;
		}

		if(qle->GetQuest()->required_mobcount[0] == qle->GetMobCount(0))
			return true;

		target->Despawn(0, 3*60*1000);

		if(entry == 12298) // Sickly Deer
			sEAS.SpawnCreature(caster, 12298, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 2*60*1000); // Cured Deer
		else // Sickly Gazelle
			sEAS.SpawnCreature(caster, 12297, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 2*60*1000); // Cured Gazelle

		qle->SetMobCount(0, qle->GetMobCount(0)+1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();

		return true;
	}
	return true;
}

void SetupDruid(ScriptMgr * mgr)
{
  QuestScript *Moonglade = (QuestScript*) new MoongladeQuest();
  mgr->register_quest_script(5921, Moonglade);
  mgr->register_quest_script(5922, Moonglade);
  mgr->register_creature_script(12138, &Lunaclaw::Create);
  mgr->register_dummy_spell(19138, &CenarionLunardust);
  mgr->register_dummy_spell(18974, &CenarionMoondust);
  mgr->register_dummy_spell(19512, &CurativeAnimalSalve);
}
