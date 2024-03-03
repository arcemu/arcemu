/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2024 <http://www.ArcEmu.org/>
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

#ifndef A_PQOOS_H_
#define A_PQOOS_H_

/// A QuestScript that calls Python functions to handle scripted events
class PythonQuestOOScript : public QuestScript
{
private:
	PyObject *obj;

public:
	PythonQuestOOScript( PyObject *obj );
	~PythonQuestOOScript();
	void setObject( PyObject *obj );

	void OnQuestStart( Player* target, QuestLogEntry* questLogEntry );
	void OnQuestComplete( Player* target, QuestLogEntry* questLogEntry );
	void OnQuestCancel( Player* target );
	void OnGameObjectActivate( uint32 entry, Player* target, QuestLogEntry* questLogEntry );
	void OnCreatureKill( uint32 entry, Player* target, QuestLogEntry* questLogEntry );
	void OnExploreArea( uint32 areaId, Player* target, QuestLogEntry* questLogEntry );
	void OnPlayerItemPickup( uint32 itemId, uint32 totalCount, Player* target, QuestLogEntry* questLogEntry );
};

#endif
