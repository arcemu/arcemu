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

#ifndef GOSSIPFUNCTION_TUPLE_H
#define GOSSIPFUNCTION_TUPLE_H

#include "gossip/PythonGossipEventTypes.hpp"

/// Contains references to GameObject event handler functions
class GossipFunctionTuple
{
public:

	GossipFunctionTuple()
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
		for( int i = 0; i < PYTHON_GOSSIP_EVENT_COUNT; i++ )
		{
			functions[ i ] = NULL;
		}
	}

	///
	/// Tells if this tuple has a function for the specified event type
	///
	/// Parameters
	///   eventType  -  The event type of the function we're looking for
	///
	/// Return value
	///   Returns true if this tuple has a function for the specified event.
	///   Returns false otherwise.
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

private:
	/// Array that contains pointers to Python functions
	///
	/// Key: Gossip event Id. See PythonGossipEventTypes
	/// Value: A pointer to a Python function
	///
	void* functions[ PYTHON_GOSSIP_EVENT_COUNT ];
};

#endif
