/*
 * WEmu Scripts for WEmu MMORPG Server
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


#include "StdAfx.h"
#include "Setup.h"
#include "../EAS/EasyFunctions.h"


#define SAY_DOC1 "I'm saved! Thank you, doctor!"
#define SAY_DOC2 "HOORAY! I AM SAVED!"
#define SAY_DOC3 "Sweet, sweet embrace... take me..."


struct Location
{
	float x, y, z, o;
};

static Location AllianceCoords[]=
{
	{-3742.2626, -4536.9624, 14.2759, 3.6178}, // Far Left Bed, Behind
	{-3750.0925, -4540.1699, 14.2712, 3.5404}, // Far Left Bed
	{-3754.2653, -4539.0634, 14.1620, 5.1827}, // Left Bed
	{-3757.5134, -4532.7343, 14.1667, 5.1497}, // Right Bed
	{-3755.9040, -4529.0385, 14.0517, 0.4200}, // Far Right Bed
	{-3749.5649, -4526.9609, 14.0764, 5.1297}  // Far Right Bed, Behind
};

static Location HordeCoords[]=
{
	{-1013.75, -3492.59, 62.62, 4.34}, // Left, Behind
	{-1017.72, -3490.92, 62.62, 4.34}, // Right, Behind
	{-1015.77, -3497.15, 62.82, 4.34}, // Left, Mid
	{-1019.51, -3495.49, 62.82, 4.34}, // Right, Mid
	{-1017.25, -3500.85, 62.98, 4.34}, // Left, front
	{-1020.95, -3499.21, 62.98, 4.34}  // Right, Front
};

#define A_RUNTOX -3742.96
#define A_RUNTOY -4531.52
#define A_RUNTOZ 11.91

#define H_RUNTOX -1016.44
#define H_RUNTOY -3508.48
#define H_RUNTOZ 62.96


const uint32 allianceSoldierId[3] =
{
	12938, // 12938 Injured Alliance Soldier
	12936, // 12936 Badly injured Alliance Soldier
	12937  // 12937 Critically injured Alliance Soldier
};

const uint32 hordeSoldierId[3] =
{
	12923, //12923 Injured Soldier
	12924, //12924 Badly injured Soldier
	12925  //12925 Critically injured Soldier
};

class InjuredSoldier : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(InjuredSoldier);
	InjuredSoldier(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_0, 16777472);
		_unit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_COMBAT);
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_1,7);

		uint32 sid = _unit->GetEntry();

		switch(sid)
		{
		case 12923:
		case 12938:
			_unit->SetHealthPct(75);
			break;
		case 12924:
		case 12936:
			_unit->SetHealthPct(50);
			break;
		case 12925:
		case 12937:
			_unit->SetHealthPct(25);
			break;
		}

	}
};

bool Triage(uint32 i, Spell *pSpell)
{
	// Unit* target = pSpell->GetUnitTarget();
	// if(!pSpell->p_caster || !target || target->GetTypeId() != TYPEID_UNIT) return true;
	if(!pSpell->p_caster || pSpell->GetUnitTarget() == NULL) return true;
		pSpell->p_caster->CastSpell(pSpell->GetUnitTarget(), dbcSpell.LookupEntry(746), true);
		QuestLogEntry *en = pSpell->p_caster->GetQuestLogForEntry(6624);

			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
		{
		uint32 newcount = en->GetMobCount(0) + 1;
		en->SetMobCount(0, newcount);
		en->SendUpdateAddKill(0);
		en->UpdatePlayerFields();
		}
	// Creature* c_target = (Creature*)target;
	// if(!c_target) return true;

	// uint32 creatureID = c_target->GetEntry();

	// if(creatureID == 12937)
		// sQuestMgr.OnPlayerKill(pSpell->p_caster, c_target);

	return true;
}

void SetupFirstAid(ScriptMgr * mgr)
{
	mgr->register_creature_script(12923, &InjuredSoldier::Create);
	mgr->register_creature_script(12924, &InjuredSoldier::Create);
	mgr->register_creature_script(12925, &InjuredSoldier::Create);
	mgr->register_creature_script(12936, &InjuredSoldier::Create);
	mgr->register_creature_script(12937, &InjuredSoldier::Create);
	mgr->register_creature_script(12938, &InjuredSoldier::Create);
	mgr->register_dummy_spell(20804, &Triage);
}
