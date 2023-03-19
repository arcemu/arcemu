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

#ifndef PYTHON_INSTANCE_EVENT_TYPES_H
#define PYTHON_INSTANCE_EVENT_TYPES_H

/// Scripted Instance event types
enum PythonInstanceEventTypes
{
	PYTHON_INSTANCE_EVENT_ON_PLAYER_DEATH   = 1,
	PYTHON_INSTANCE_EVENT_ON_PLAYER_ENTER   = 2,
	PYTHON_INSTANCE_EVENT_ON_AREA_TRIGGER   = 3,
	PYTHON_INSTANCE_EVENT_ON_ZONE_CHANGE    = 4,
	PYTHON_INSTANCE_EVENT_ON_CREATURE_DEATH = 5,
	PYTHON_INSTANCE_EVENT_ON_CREATURE_PUSH  = 6,
	PYTHON_INSTANCE_EVENT_ON_GO_ACTIVATE    = 7,
	PYTHON_INSTANCE_EVENT_ON_GO_PUSH        = 8,
	PYTHON_INSTANCE_EVENT_ONLOAD            = 9,
	PYTHON_INSTANCE_EVENT_DESTROY           = 10,
	
	PYTHON_INSTANCE_EVENT_COUNT
};

#endif
