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

#include "PythonGOEventTypes.hpp"
#include "PythonGossipEventTypes.hpp"

#include "Python.h"

HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple > FunctionRegistry::creatureGossipFunctions;
HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple > FunctionRegistry::itemGossipFunctions;
HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple > FunctionRegistry::goGossipFunctions;
HM_NAMESPACE::HM_HASH_MAP< unsigned int, GOFunctionTuple* > FunctionRegistry::goFunctions;

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

void FunctionRegistry::registerGOGossipFunction( unsigned int goId, unsigned int gossipEvent, void* function )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple >::iterator itr = goGossipFunctions.find( goId );
	if( itr == goGossipFunctions.end() )
	{
		GossipFunctionTuple tuple;
		itr = goGossipFunctions.insert( std::pair< unsigned int, GossipFunctionTuple >( goId, tuple ) ).first;
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

void FunctionRegistry::registerItemGossipFunction( unsigned int itemId, unsigned int gossipEvent, void* function )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple >::iterator itr = itemGossipFunctions.find( itemId );
	if( itr == itemGossipFunctions.end() )
	{
		GossipFunctionTuple tuple;
		itr = itemGossipFunctions.insert( std::pair< unsigned int, GossipFunctionTuple >( itemId, tuple ) ).first;
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

void FunctionRegistry::registerGOEventFunction( unsigned int goId, unsigned int goEvent, void* function )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GOFunctionTuple* >::iterator itr = goFunctions.find( goId );
	if( itr == goFunctions.end() )
	{
		GOFunctionTuple* tuple = new GOFunctionTuple();
		itr = goFunctions.insert( std::pair< unsigned int, GOFunctionTuple* >( goId, tuple ) ).first;
	}

	switch( goEvent )
	{
	case PYTHON_GO_EVENT_ON_CREATE:
		{
			itr->second->onCreate = function;
			break;
		}

	case PYTHON_GO_EVENT_ON_SPAWN:
		{
			itr->second->onSpawn = function;
			break;
		}

	case PYTHON_GO_EVENT_ON_LOOT_TAKEN:
		{
			itr->second->onLootTaken = function;
			break;
		}

	case PYTHON_GO_EVENT_ON_USE:
		{
			itr->second->onUse = function;
			break;
		}

	case PYTHON_GO_EVENT_ON_AIUPDATE:
		{
			itr->second->onAIUpdate = function;
			break;
		}

	case PYTHON_GO_EVENT_ON_DESPAWN:
		{
			itr->second->onDespawn = function;
			break;
		}

	case PYTHON_GO_EVENT_ON_DAMAGED:
		{
			itr->second->onDamaged = function;
			break;
		}

	case PYTHON_GO_EVENT_ON_DESTROYED:
		{
			itr->second->onDestroyed = function;
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

void FunctionRegistry::visitGOGossipFunctions( GossipFunctionTupleVisitor *visitor )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple >::iterator itr = goGossipFunctions.begin();
	while( itr != goGossipFunctions.end() )
	{
		visitor->visit( itr->first, itr->second );
		++itr;
	}
}

void FunctionRegistry::visitItemGossipFunctions( GossipFunctionTupleVisitor *visitor )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple >::iterator itr = itemGossipFunctions.begin();
	while( itr != itemGossipFunctions.end() )
	{
		visitor->visit( itr->first, itr->second );
		++itr;
	}
}


void FunctionRegistry::visitGOEventFunctions( GOFunctionTupleVisitor *visitor )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GOFunctionTuple* >::iterator itr = goFunctions.begin();
	while( itr != goFunctions.end() )
	{
		visitor->visit( itr->first, *(itr->second) );
		++itr;
	}
}

void FunctionRegistry::releaseFunctions()
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple >::iterator itr;
	
	itr = creatureGossipFunctions.begin();
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

	itr = itemGossipFunctions.begin();
	while( itr != itemGossipFunctions.end() )
	{
		if( itr->second.onHelloFunction != NULL )
			Py_DecRef( (PyObject*)itr->second.onHelloFunction );

		if( itr->second.onSelectionFunction != NULL )
			Py_DecRef( (PyObject*)itr->second.onSelectionFunction );
		
		if( itr->second.onEndFunction != NULL )
			Py_DecRef( (PyObject*)itr->second.onEndFunction );

		++itr;
	}

	itemGossipFunctions.clear();

	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GOFunctionTuple* >::iterator goFunctionsItr = goFunctions.begin();
	while( goFunctionsItr != goFunctions.end() )
	{
		if( goFunctionsItr->second->onUse != NULL )
			Py_DecRef( (PyObject*)goFunctionsItr->second->onUse );

		delete goFunctionsItr->second;
		goFunctionsItr->second = NULL;

		++goFunctionsItr;
	}

	goFunctions.clear();
}

GOFunctionTuple* FunctionRegistry::getGOEventFunctions( unsigned int goId )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GOFunctionTuple* >::iterator itr = goFunctions.find( goId );
	if( itr == goFunctions.end() )
		return NULL;
	else
		return itr->second;
}
