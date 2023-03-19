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

#ifndef PYTHON_GO_EVENT_TYPES_H
#define PYTHON_GO_EVENT_TYPES_H

/// GameObject scripted event types
enum PythonGOEventTypes
{
    PYTHON_GO_EVENT_ON_CREATE        = 1,
    PYTHON_GO_EVENT_ON_SPAWN         = 2,
    PYTHON_GO_EVENT_ON_LOOT_TAKEN    = 3,
    PYTHON_GO_EVENT_ON_USE           = 4,
    PYTHON_GO_EVENT_ON_AIUPDATE      = 5,
    PYTHON_GO_EVENT_ON_DESPAWN       = 6,
	PYTHON_GO_EVENT_ON_DAMAGED       = 7,
	PYTHON_GO_EVENT_ON_DESTROYED     = 8,

	PYTHON_GO_EVENT_COUNT
};

#endif
