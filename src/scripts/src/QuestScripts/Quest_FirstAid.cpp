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


#define SAY_DOC1 "I'm saved! Thank you, doctor!"
#define SAY_DOC2 "HOORAY! I AM SAVED!"
#define SAY_DOC3 "Sweet, sweet embrace... take me..."


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
			_unit->SetUInt32Value(UNIT_FIELD_BYTES_1, 7);

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



void SetupFirstAid(ScriptMgr* mgr)
{
	mgr->register_creature_script(12923, &InjuredSoldier::Create);
	mgr->register_creature_script(12924, &InjuredSoldier::Create);
	mgr->register_creature_script(12925, &InjuredSoldier::Create);
	mgr->register_creature_script(12936, &InjuredSoldier::Create);
	mgr->register_creature_script(12937, &InjuredSoldier::Create);
	mgr->register_creature_script(12938, &InjuredSoldier::Create);

}
