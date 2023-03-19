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

#ifndef PYTHON_QUESTEVENT_TYPES_H
#define PYTHON_QUESTEVENT_TYPES_H

/// Quest scripted event types
enum PythonQuestEventTypes
{
	PYTHON_QUEST_EVENT_ON_ACCEPT            = 1,
	PYTHON_QUEST_EVENT_ON_COMPLETE          = 2,
	PYTHON_QUEST_EVENT_ON_CANCEL            = 3,
	PYTHON_QUEST_EVENT_GAMEOBJECT_ACTIVATE  = 4,
	PYTHON_QUEST_EVENT_ON_CREATURE_KILL     = 5,
	PYTHON_QUEST_EVENT_ON_EXPLORE_AREA      = 6,
	PYTHON_QUEST_EVENT_ON_PLAYER_ITEMPICKUP = 7,


	PYTHON_QUEST_EVENT_COUNT
};

#endif
