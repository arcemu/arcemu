/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
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

#include "Setup.h"

class Quest_JourneytoUndercity : public QuestScript // never extend std::tr1::enable_shared_from_this. EVER. :)
{
	public:

		void OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			Creature* creat = mTarget->GetMapMgr()->GetSqlIdCreature(19175); //Lady Sylvanas Windrunner - NCDB guid
			if(creat == NULL) // we still check for equality with == NULL. if we are assigning, we use = NULL/OBJECT/ETC
				return;

			sEAS.SpawnCreatureExtended(mTarget, 21628, 1295.222656f, 314.253998f, -57.320854f, 2.365611f, 35, 180000, true, true, true);
			sEAS.SpawnCreatureExtended(mTarget, 21628, 1293.403931f, 311.264465f, -57.320854f, 1.939140f, 35, 180000, true, true, true);
			sEAS.SpawnCreatureExtended(mTarget, 21628, 1286.532104f, 311.452423f, -57.320854f, 0.592182f, 35, 180000, true, true, true);
			sEAS.SpawnCreatureExtended(mTarget, 21628, 1284.536011f, 314.496338f, -57.320845f, 0.580401f, 35, 180000, true, true, true);

			creat->PlaySoundToSet(10896);
			creat->CastSpell(creat, dbcSpell.LookupEntry(36568), false);

			creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);

			// Players can't interact with Sylvanas for 180000 ms.
			// Cast creat to an object because the EventSetUInt32Value method is in Object class.
			sEventMgr.AddEvent(TO_OBJECT(creat), &Object::EventSetUInt32Value, (uint32)UNIT_NPC_FLAGS, (uint32)2, EVENT_SCRIPT_UPDATE_EVENT, 180000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}
};

void SetupUndercity(ScriptMgr* mgr)
{
	mgr->register_quest_script(9180, new Quest_JourneytoUndercity());
}
