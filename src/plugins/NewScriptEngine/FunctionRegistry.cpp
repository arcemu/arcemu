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

#include "StdAfx.h"

#include "FunctionRegistry.hpp"

#include "PythonGossipEventTypes.hpp"

#include "Python.h"

HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple > FunctionRegistry::creatureGossipFunctions;

void FunctionRegistry::registerCreatureGossipFunction( unsigned int creatureId, unsigned int gossipEvent, void* function )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple >::iterator itr = creatureGossipFunctions.find( creatureId );
	if( itr == creatureGossipFunctions.end() )
	{
		GossipFunctionTuple tuple;
		itr = creatureGossipFunctions.insert( std::pair< unsigned int, GossipFunctionTuple >( creatureId, tuple ) ).first;
	}

	switch( gossipEvent )
	{
	case PYTHON_GOSSIP_EVENT_HELLO:
		{
			itr->second.onHelloFunction = function;
			break;
		}

	case PYTHON_GOSSIP_EVENT_SELECT:
		{
			itr->second.onSelectionFunction = function;
			break;
		}

	case PYTHON_GOSSIP_EVENT_END:
		{
			itr->second.onEndFunction = function;
			break;
		}
	}
}

void FunctionRegistry::visitCreatureGossipFunctions( GossipFunctionTupleVisitor *visitor )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple >::iterator itr = creatureGossipFunctions.begin();
	while( itr != creatureGossipFunctions.end() )
	{
		visitor->visit( itr->first, itr->second );
		++itr;
	}
}

void FunctionRegistry::releaseFunctions()
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple >::iterator itr = creatureGossipFunctions.begin();
	while( itr != creatureGossipFunctions.end() )
	{
		if( itr->second.onHelloFunction != NULL )
			Py_DecRef( (PyObject*)itr->second.onHelloFunction );

		if( itr->second.onSelectionFunction != NULL )
			Py_DecRef( (PyObject*)itr->second.onSelectionFunction );
		
		if( itr->second.onEndFunction != NULL )
			Py_DecRef( (PyObject*)itr->second.onEndFunction );

		++itr;
	}

	creatureGossipFunctions.clear();
}

