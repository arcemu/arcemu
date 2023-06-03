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

#ifndef PYTHON_CREATURE_EVENT_TYPES_H
#define PYTHON_CREATURE_EVENT_TYPES_H

/// Scripted event types of Creatures
enum PythonCreatureEventTypes
{
	PYTHON_CREATURE_EVENT_ON_ENTER_COMBAT		     = 1,
    PYTHON_CREATURE_EVENT_ON_LEAVE_COMBAT		     = 2,
    PYTHON_CREATURE_EVENT_ON_TARGET_DIED		     = 3,
    PYTHON_CREATURE_EVENT_ON_DIED		             = 4,
    PYTHON_CREATURE_EVENT_ON_TARGET_PARRIED		     = 5,
    PYTHON_CREATURE_EVENT_ON_TARGET_DODGED		     = 6,
    PYTHON_CREATURE_EVENT_ON_TARGET_BLOCKED		     = 7,
    PYTHON_CREATURE_EVENT_ON_TARGET_CRIT_HIT		 = 8,
    PYTHON_CREATURE_EVENT_ON_PARRY		             = 9,
    PYTHON_CREATURE_EVENT_ON_DODGED		             = 10,
    PYTHON_CREATURE_EVENT_ON_BLOCKED		         = 11,
    PYTHON_CREATURE_EVENT_ON_CRIT_HIT		         = 12,
    PYTHON_CREATURE_EVENT_ON_HIT		             = 13,
    PYTHON_CREATURE_EVENT_ON_ASSIST_TARGET_DIED		 = 14,
    PYTHON_CREATURE_EVENT_ON_FEAR		             = 15,
    PYTHON_CREATURE_EVENT_ON_FLEE		             = 16,
    PYTHON_CREATURE_EVENT_ON_CALL_FOR_HELP		     = 17,
    PYTHON_CREATURE_EVENT_ON_LOAD		             = 18,
    PYTHON_CREATURE_EVENT_ON_REACH_WP		         = 19,
    PYTHON_CREATURE_EVENT_ON_LOOT_TAKEN		         = 20,
    PYTHON_CREATURE_EVENT_ON_AIUPDATE		         = 21,
    PYTHON_CREATURE_EVENT_ON_EMOTE		             = 22,
    PYTHON_CREATURE_EVENT_ON_DAMAGE_TAKEN            = 23,
	PYTHON_CREATURE_EVENT_ON_ENTER_VEHICLE           = 24,
	PYTHON_CREATURE_EVENT_ON_EXIT_VEHICLE            = 25,
	PYTHON_CREATURE_EVENT_ON_FIRST_PASSENGER_ENTERED = 26,
	PYTHON_CREATURE_EVENT_ON_VEHICLE_FULL            = 27,
	PYTHON_CREATURE_EVENT_ON_LAST_PASSENGER_LEFT     = 28,
	PYTHON_CREATURE_EVENT_ON_CAST_SPELL              = 29,
	PYTHON_CREATURE_EVENT_ON_DESPAWN                 = 30,
	PYTHON_CREATURE_EVENT_ON_HEALED                  = 31,
	PYTHON_CREATURE_EVENT_ON_APPLY_AURA              = 32,
	PYTHON_CREATURE_EVENT_ON_ACTIVATE                = 33,
	PYTHON_CREATURE_EVENT_ON_DEACTIVATE              = 34,

	PYTHON_CREATURE_EVENT_COUNT
};

#endif

