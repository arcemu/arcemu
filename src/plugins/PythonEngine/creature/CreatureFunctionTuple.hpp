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

#ifndef CREATUREFUNCTION_TUPLE_H
#define CREATUREFUNCTION_TUPLE_H

#include "PythonCreatureEventTypes.hpp"

/// Stores Creature functions loaded from Python scripts
class CreatureFunctionTuple
{
public:
	/// Initializes this Tuple
	///
	/// Parameters
	///   None
	///
	/// Return value
	///   None
	///
	CreatureFunctionTuple()
	{
		for( int i = 0; i < PYTHON_CREATURE_EVENT_COUNT; i++ )
		{
			functions[ i ] = NULL;
		}
	}

	///
	/// Removes the function references from this tuple
	///
	/// Parameters
	///   None
	///
	/// Return value
	///   None
	///
	void clearFunctions()
	{
		for( int i = 0; i < PYTHON_CREATURE_EVENT_COUNT; i++ )
		{
			functions[ i ] = NULL;
		}
	}

	/// Tells if there's a function reference for this event type
	///
	/// Parameters
	///   unsigned long eventType  -  The event type. See PythonCreatureEventTypes
	///
	/// Return value
	///   Returns true if there's such a function, and false otherwise
	///
	bool hasFunction( unsigned long eventType ) const
	{
		if( functions[ eventType ] != NULL )
			return true;
		else
			return false;
	}

	///
	/// Adds or replaces a function for the specified event in this tuple
	///
	/// Parameters
	///   eventType  -  The event type of the function
	///   function   -  A pointer to the function
	///
	/// Return value
	///   None
	///
	void setFunction( unsigned long eventType, void* function )
	{
		functions[ eventType ] = function;
	}

	/// Returns the function for the event type
	///
	/// Parameters
	///   unsigned long eventType  -  The event type. See PythonCreatureEventTypes
	///
	/// Return value
	///   Returns a pointer to the function, or NULL if there's no such function
	///
	void* getFunction( unsigned long eventType ) const
	{
		return functions[ eventType ];
	}

private:
	void* functions[ PYTHON_CREATURE_EVENT_COUNT ];
};

#endif
