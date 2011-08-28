
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

class Quest_Grimoire_Business : public QuestScript
{
	public:
		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			sEAS.SpawnCreature(mTarget, 22911, 3279.67f, 4640.77f, 216.526f, 1.3516f, 2 * 60 * 1000);
		}

};

class Quest_Maggocs_Treasure_Chest : public QuestScript
{
	public:
		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			mTarget->GetMapMgr()->GetInterface()->SpawnCreature(20600, 2001.76f, 5164.77f, 265.19f, 5.5148f, true, false, 0, 0);
		}

};

class Quest_Grulloc_Has_Two_Skulls : public QuestScript
{
	public:
		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			sEAS.SpawnCreature(mTarget, 20216, 2687.46f, 5541.14f, -1.93669f, 3.52847f, 2 * 60 * 1000);
		}

};

class Quest_Zuluhed_the_Whacked : public QuestScript
{
	public:
		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			sEAS.SpawnCreature(mTarget, 11980, -4177.39f, 376.289f, 117.78f, 2.7381f, 2 * 60 * 1000);
		}

};

#define QUEST_CLUCK		 3861
#define ITEM_CHICKEN_FEED   11109

class Chicken : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Chicken);
		Chicken(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnLoad()
		{
			_unit->SetFaction(12);
			_unit->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
			RegisterAIUpdateEvent(120000);
		}

		void AIUpdate()
		{
			if(_unit->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
				OnLoad();
		}
};

class Kaliri : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Kaliri);
		Kaliri(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnLoad()
		{
			_unit->SetFaction(35);
		}
};

void SetupUnsorted(ScriptMgr* mgr)
{
	mgr->register_quest_script(10998, new Quest_Grimoire_Business());
	mgr->register_quest_script(10996, new Quest_Maggocs_Treasure_Chest());
	mgr->register_quest_script(10995, new Quest_Grulloc_Has_Two_Skulls());
	mgr->register_quest_script(10866, new Quest_Zuluhed_the_Whacked());
	mgr->register_creature_script(620, &Chicken::Create);
	mgr->register_creature_script(21468, &Kaliri::Create);
}
