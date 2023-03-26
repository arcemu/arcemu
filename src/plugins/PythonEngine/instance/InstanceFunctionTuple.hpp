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

#ifndef INSTANCEFUNCTION_TUPLE_H
#define INSTANCEFUNCTION_TUPLE_H

#include "PythonInstanceEventTypes.hpp"

/// Contains Instance event handler function references
class InstanceFunctionTuple
{
public:
	InstanceFunctionTuple()
	{
		clearFunctions();
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
		for( int i = 0; i < PYTHON_INSTANCE_EVENT_COUNT; i++ )
		{
			functions[ i ] = NULL;
		}
	}

	///
	/// Tells if this tuple contains a function for the specified event
	///
	/// Parameters
	///   eventType  -  The type of event that function that we're looking for handles
	///
	/// Return value
	///   Returns true if the tuple contains a function for the specified event type.
	///   Return false otherwise.
	///
	bool hasFunction( unsigned long eventType ) const
	{
		if( functions[ eventType ] != NULL )
			return true;
		else
			return false;
	}


	///
	/// Retrieves the function for the specified event type
	///
	/// Parameters
	///   eventType  -  The event type of the function we're looking for
	///
	/// Return value
	///   Returns a pointer to a function if there's such a function in this tuple
	///   Returns NULL otherwise.
	///
	void* getFunction( unsigned long eventType ) const
	{
		return functions[ eventType ];
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


private:

	///
	/// Instance event handler function references
	///
	/// Key:   Instance event type. See PythonInstanceEventTypes
	/// Value: A pointer to a Python function
	///
	void* functions[ PYTHON_INSTANCE_EVENT_COUNT ];
};

#endif
