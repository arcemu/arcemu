/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2023 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef P_QUEST_SCRIPT_H
#define P_QUEST_SCRIPT_H

#include "quest/QuestFunctionTuple.hpp"

class PythonQuestScript : public QuestScript
{
public:
	PythonQuestScript( const QuestFunctionTuple &tuple );
	~PythonQuestScript();

	void setFunctions( const QuestFunctionTuple &tuple );
	void clearFunctions();

	void OnQuestStart( Player* target, QuestLogEntry* questLogEntry );
	void OnQuestComplete( Player* target, QuestLogEntry* questLogEntry );
	void OnQuestCancel( Player* target );
	void OnGameObjectActivate( uint32 entry, Player* target, QuestLogEntry* questLogEntry );
	void OnCreatureKill( uint32 entry, Player* target, QuestLogEntry* questLogEntry );
	void OnExploreArea( uint32 areaId, Player* target, QuestLogEntry* questLogEntry );
	void OnPlayerItemPickup( uint32 itemId, uint32 totalCount, Player* target, QuestLogEntry* questLogEntry );

private:
	QuestFunctionTuple functions;
};

#endif
