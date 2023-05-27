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

#include "engine/FunctionRegistry.hpp"

#include "gameobject/PythonGOEventTypes.hpp"
#include "gossip/PythonGossipEventTypes.hpp"

#include "Python.h"

HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* > FunctionRegistry::creatureGossipFunctions;
HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* > FunctionRegistry::itemGossipFunctions;
HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* > FunctionRegistry::goGossipFunctions;
HM_NAMESPACE::HM_HASH_MAP< unsigned int, GOFunctionTuple* > FunctionRegistry::goFunctions;
HM_NAMESPACE::HM_HASH_MAP< unsigned int, InstanceFunctionTuple* > FunctionRegistry::instanceFunctions;
HM_NAMESPACE::HM_HASH_MAP< unsigned int, CreatureFunctionTuple* > FunctionRegistry::creatureFunctions;
HM_NAMESPACE::HM_HASH_MAP< unsigned int, QuestFunctionTuple* > FunctionRegistry::questFunctions;
HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* > FunctionRegistry::dummySpellFunctions;
HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* > FunctionRegistry::scriptedEffectHandlerFunctions;
HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* > FunctionRegistry::dummyAuraFunctions;


void FunctionRegistry::registerCreatureGossipFunction( unsigned int creatureId, unsigned int gossipEvent, void* function )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* >::iterator itr = creatureGossipFunctions.find( creatureId );
	if( itr == creatureGossipFunctions.end() )
	{
		GossipFunctionTuple* tuple = new GossipFunctionTuple();
		itr = creatureGossipFunctions.insert( std::pair< unsigned int, GossipFunctionTuple* >( creatureId, tuple ) ).first;
	}

	GossipFunctionTuple *tuple = itr->second;
	tuple->setFunction( gossipEvent, function );
}

void FunctionRegistry::registerGOGossipFunction( unsigned int goId, unsigned int gossipEvent, void* function )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* >::iterator itr = goGossipFunctions.find( goId );
	if( itr == goGossipFunctions.end() )
	{
		GossipFunctionTuple* tuple = new GossipFunctionTuple();
		itr = goGossipFunctions.insert( std::pair< unsigned int, GossipFunctionTuple* >( goId, tuple ) ).first;
	}

	GossipFunctionTuple *tuple = itr->second;
	tuple->setFunction( gossipEvent, function );
}

void FunctionRegistry::registerItemGossipFunction( unsigned int itemId, unsigned int gossipEvent, void* function )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* >::iterator itr = itemGossipFunctions.find( itemId );
	if( itr == itemGossipFunctions.end() )
	{
		GossipFunctionTuple* tuple = new GossipFunctionTuple();
		itr = itemGossipFunctions.insert( std::pair< unsigned int, GossipFunctionTuple* >( itemId, tuple ) ).first;
	}

	GossipFunctionTuple *tuple = itr->second;
	tuple->setFunction( gossipEvent, function );
}

void FunctionRegistry::registerGOEventFunction( unsigned int goId, unsigned int goEvent, void* function )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GOFunctionTuple* >::iterator itr = goFunctions.find( goId );
	if( itr == goFunctions.end() )
	{
		GOFunctionTuple* tuple = new GOFunctionTuple();
		itr = goFunctions.insert( std::pair< unsigned int, GOFunctionTuple* >( goId, tuple ) ).first;
	}

	itr->second->setFunction( goEvent, function );
}

void FunctionRegistry::registerCreatureEventFunction( unsigned int creatureId, unsigned int creatureEvent, void* function )
{
	ARCEMU_ASSERT( creatureEvent < PYTHON_CREATURE_EVENT_COUNT );

	HM_NAMESPACE::HM_HASH_MAP< unsigned int, CreatureFunctionTuple* >::iterator itr = creatureFunctions.find( creatureId );
	if( itr == creatureFunctions.end() )
	{
		CreatureFunctionTuple* tuple = new CreatureFunctionTuple();
		itr = creatureFunctions.insert( std::pair< unsigned int, CreatureFunctionTuple* >( creatureId, tuple ) ).first;
	}

	itr->second->setFunction( creatureEvent, function );
}

void FunctionRegistry::registerQuestEventFunction( unsigned int questId, unsigned int questEvent, void* function )
{
	ARCEMU_ASSERT( questEvent < PYTHON_QUEST_EVENT_COUNT );

	HM_NAMESPACE::HM_HASH_MAP< unsigned int, QuestFunctionTuple* >::iterator itr = questFunctions.find( questId );
	if( itr == questFunctions.end() )
	{
		QuestFunctionTuple* tuple = new QuestFunctionTuple();
		itr = questFunctions.insert( std::pair< unsigned int, QuestFunctionTuple* >( questId, tuple ) ).first;
	}

	itr->second->setFunction( questEvent, function );
}

void FunctionRegistry::registerInstanceEventFunction( unsigned int mapId, unsigned int instanceEvent, void* function )
{
	ARCEMU_ASSERT( instanceEvent < PYTHON_INSTANCE_EVENT_COUNT );

	HM_NAMESPACE::HM_HASH_MAP< unsigned int, InstanceFunctionTuple* >::iterator itr = instanceFunctions.find( mapId );
	if( itr == instanceFunctions.end() )
	{
		InstanceFunctionTuple* tuple = new InstanceFunctionTuple();
		itr = instanceFunctions.insert( std::pair< unsigned int, InstanceFunctionTuple* >( mapId, tuple ) ).first;
	}

	itr->second->setFunction( instanceEvent, function );
}

void FunctionRegistry::registerDummySpellHandler( unsigned long spellId, void* function )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* >::iterator itr = dummySpellFunctions.find( spellId );
	if( itr == dummySpellFunctions.end() )
	{
		dummySpellFunctions.insert( std::pair< unsigned long, void* >( spellId, function ) );
	}
	else
	{
		void *oldFunction = itr->second;
		Py_DECREF( oldFunction );
		oldFunction = NULL;

		dummySpellFunctions[ spellId ] = function;
	}
}

void FunctionRegistry::registerScriptedEffectHandler( unsigned long spellId, void* function )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* >::iterator itr = scriptedEffectHandlerFunctions.find( spellId );
	if( itr == scriptedEffectHandlerFunctions.end() )
	{
		scriptedEffectHandlerFunctions.insert( std::pair< unsigned long, void* >( spellId, function ) );
	}
	else
	{
		void *oldFunction = itr->second;
		Py_DECREF( oldFunction );
		oldFunction = NULL;

		scriptedEffectHandlerFunctions[ spellId ] = function;
	}
}

void FunctionRegistry::registerDummyAuraHandler( unsigned long spellId, void* function )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* >::iterator itr = dummyAuraFunctions.find( spellId );
	if( itr == dummyAuraFunctions.end() )
	{
		dummyAuraFunctions.insert( std::pair< unsigned long, void* >( spellId, function ) );
	}
	else
	{
		void *oldFunction = itr->second;
		Py_DECREF( oldFunction );
		oldFunction = NULL;

		dummyAuraFunctions[ spellId ] = function;
	}
}

void* FunctionRegistry::getDummySpellHandler( unsigned long spellId )
{
	void* function = NULL;

	HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* >::iterator itr = dummySpellFunctions.find( spellId );
	if( itr != dummySpellFunctions.end() )
	{
		function = itr->second;
	}

	return function;
}



void* FunctionRegistry::getScriptedEffectHandler( unsigned long spellId )
{
	void* function = NULL;

	HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* >::iterator itr = scriptedEffectHandlerFunctions.find( spellId );
	if( itr != scriptedEffectHandlerFunctions.end() )
	{
		function = itr->second;
	}

	return function;
}

void* FunctionRegistry::getDummyAuraHandler( unsigned long spellId )
{
	void *function = NULL;
	HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* >::iterator itr = dummyAuraFunctions.find( spellId );
	if( itr != dummyAuraFunctions.end() )
	{
		function = itr->second;
	}	

	return function;
}

void FunctionRegistry::visitCreatureGossipFunctions( GossipFunctionTupleVisitor *visitor )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* >::iterator itr = creatureGossipFunctions.begin();
	while( itr != creatureGossipFunctions.end() )
	{
		visitor->visit( itr->first, *(itr->second) );
		++itr;
	}
}

void FunctionRegistry::visitGOGossipFunctions( GossipFunctionTupleVisitor *visitor )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* >::iterator itr = goGossipFunctions.begin();
	while( itr != goGossipFunctions.end() )
	{
		visitor->visit( itr->first, *(itr->second) );
		++itr;
	}
}

void FunctionRegistry::visitInstanceEventFunctions( InstanceFunctionTupleVisitor *visitor )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, InstanceFunctionTuple* >::iterator itr = instanceFunctions.begin();
	while( itr != instanceFunctions.end() )
	{
		visitor->visit( itr->first, *(itr->second) );
		++itr;
	}
}

void FunctionRegistry::visitItemGossipFunctions( GossipFunctionTupleVisitor *visitor )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* >::iterator itr = itemGossipFunctions.begin();
	while( itr != itemGossipFunctions.end() )
	{
		visitor->visit( itr->first, *(itr->second) );
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

void FunctionRegistry::visitCreatureEventFunctions( CreatureFunctionTupleVisitor *visitor )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, CreatureFunctionTuple* >::iterator itr = creatureFunctions.begin();
	while( itr != creatureFunctions.end() )
	{
		visitor->visit( itr->first, *(itr->second) );
		++itr;
	}
}

void FunctionRegistry::visitQuestEventFunctions( QuestFunctionTupleVisitor *visitor )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, QuestFunctionTuple* >::iterator itr = questFunctions.begin();
	while( itr != questFunctions.end() )
	{
		visitor->visit( itr->first, *(itr->second) );
		++itr;
	}
}

void FunctionRegistry::visitDummySpellHandlerFunctions( DummySpellHandlerVisitor *visitor )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* >::iterator itr = dummySpellFunctions.begin();
	while( itr != dummySpellFunctions.end() )
	{
		visitor->visit( itr->first, itr->second );
		++itr;
	}
}

void FunctionRegistry::visitScriptedEffectHandlerFunctions( ScriptedEffectHandlerVisitor *visitor )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* >::iterator itr = scriptedEffectHandlerFunctions.begin();
	while( itr != scriptedEffectHandlerFunctions.end() )
	{
		visitor->visit( itr->first, itr->second );
		++itr;
	}
}

void FunctionRegistry::visitDummyAuraHandlerFunctions( DummyAuraHandlerVisitor *visitor )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* >::iterator itr = dummyAuraFunctions.begin();
	while( itr != dummyAuraFunctions.end() )
	{
		visitor->visit( itr->first, itr->second );
		++itr;
	}
}

void FunctionRegistry::releaseFunctions()
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* >::iterator itr;
	
	itr = creatureGossipFunctions.begin();
	while( itr != creatureGossipFunctions.end() )
	{
		for( int i = 0; i < PYTHON_GOSSIP_EVENT_COUNT; i++ )
		{
			void *f = itr->second->getFunction( i );
			if( f != NULL )
			{
				Py_DecRef( (PyObject*)f );
			}
		}

		itr->second->clearFunctions();

		delete itr->second;
		itr->second = NULL;

		++itr;
	}

	creatureGossipFunctions.clear();

	itr = itemGossipFunctions.begin();
	while( itr != itemGossipFunctions.end() )
	{
		for( int i = 0; i < PYTHON_GOSSIP_EVENT_COUNT; i++ )
		{
			void *f = itr->second->getFunction( i );
			if( f != NULL )
			{
				Py_DecRef( (PyObject*)f );
			}
		}

		itr->second->clearFunctions();

		delete itr->second;
		itr->second = NULL;

		++itr;
	}

	itemGossipFunctions.clear();

	itr = goGossipFunctions.begin();
	while( itr != goGossipFunctions.end() )
	{
		for( int i = 0; i < PYTHON_GOSSIP_EVENT_COUNT; i++ )
		{
			void *f = itr->second->getFunction( i );
			if( f != NULL )
			{
				Py_DecRef( (PyObject*)f );
			}
		}

		itr->second->clearFunctions();

		delete itr->second;
		itr->second = NULL;

		++itr;
	}

	goGossipFunctions.clear();

	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GOFunctionTuple* >::iterator goFunctionsItr = goFunctions.begin();
	while( goFunctionsItr != goFunctions.end() )
	{
		for( int i = 0; i < PYTHON_GO_EVENT_COUNT; i++ )
		{
			void *f = goFunctionsItr->second->getFunction( i );
			if( f != NULL )
			{
				Py_DecRef( (PyObject*)f );
			}
		}		
		
		goFunctionsItr->second->clearFunctions();

		delete goFunctionsItr->second;
		goFunctionsItr->second = NULL;

		++goFunctionsItr;
	}

	goFunctions.clear();

	HM_NAMESPACE::HM_HASH_MAP< unsigned int, CreatureFunctionTuple* >::iterator creatureFunctionsItr = creatureFunctions.begin();
	while( creatureFunctionsItr != creatureFunctions.end() )
	{
		CreatureFunctionTuple* tuple = creatureFunctionsItr->second;

		for( int i = 0; i < PYTHON_CREATURE_EVENT_COUNT; i++ )
		{
			void *f = tuple->getFunction( i );
			if( f != NULL )
			{
				Py_DecRef( (PyObject*) f );
			}
		}

		tuple->clearFunctions();

		delete creatureFunctionsItr->second;
		creatureFunctionsItr->second = NULL;

		++creatureFunctionsItr;
	}

	creatureFunctions.clear();

	HM_NAMESPACE::HM_HASH_MAP< unsigned int, QuestFunctionTuple* >::iterator questFunctionsItr = questFunctions.begin();
	while( questFunctionsItr != questFunctions.end() )
	{
		QuestFunctionTuple* tuple = questFunctionsItr->second;

		for( int i = 0; i < PYTHON_QUEST_EVENT_COUNT; i++ )
		{
			void *f = tuple->getFunction( i );
			if( f != NULL )
			{
				Py_DecRef( (PyObject*) f );
			}
		}

		tuple->clearFunctions();

		delete questFunctionsItr->second;
		questFunctionsItr->second = NULL;

		++questFunctionsItr;
	}

	questFunctions.clear();



	HM_NAMESPACE::HM_HASH_MAP< unsigned int, InstanceFunctionTuple* >::iterator instanceFunctionsItr = instanceFunctions.begin();
	while( instanceFunctionsItr != instanceFunctions.end() )
	{
		InstanceFunctionTuple* tuple = instanceFunctionsItr->second;

		for( int i = 0; i < PYTHON_INSTANCE_EVENT_COUNT; i++ )
		{
			void *f = tuple->getFunction( i );
			if( f != NULL )
			{
				Py_DecRef( (PyObject*) f );
			}
		}

		tuple->clearFunctions();

		delete instanceFunctionsItr->second;
		instanceFunctionsItr->second = NULL;

		++instanceFunctionsItr;
	}

	instanceFunctions.clear();

	HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* >::iterator dummySpellIterator = dummySpellFunctions.begin();
	while( dummySpellIterator != dummySpellFunctions.end() )
	{
		Py_DecRef( (PyObject*)dummySpellIterator->second );
		dummySpellIterator->second = NULL;

		++dummySpellIterator;
	}

	dummySpellFunctions.clear();

	HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* >::iterator scriptedEffectsIterator = scriptedEffectHandlerFunctions.begin();
	while( scriptedEffectsIterator != scriptedEffectHandlerFunctions.end() )
	{
		Py_DecRef( (PyObject*)scriptedEffectsIterator->second );
		scriptedEffectsIterator->second = NULL;
		++scriptedEffectsIterator;
	}

	scriptedEffectHandlerFunctions.clear();

	HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* >::iterator dummyAurasIterator = dummyAuraFunctions.begin();
	while( dummyAurasIterator != dummyAuraFunctions.end() )
	{
		Py_DECREF( (PyObject*)dummyAurasIterator->second );
		dummyAurasIterator->second = NULL;
		++dummyAurasIterator;
	}

	dummyAuraFunctions.clear();
}

GOFunctionTuple* FunctionRegistry::getGOEventFunctions( unsigned int goId )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, GOFunctionTuple* >::iterator itr = goFunctions.find( goId );
	if( itr == goFunctions.end() )
		return NULL;
	else
		return itr->second;
}

CreatureFunctionTuple* FunctionRegistry::getCreatureEventFunctions( unsigned int creatureId )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, CreatureFunctionTuple* >::iterator itr = creatureFunctions.find( creatureId );
	if( itr == creatureFunctions.end() )
		return NULL;
	else
		return itr->second;
}

QuestFunctionTuple* FunctionRegistry::getQuestFunctions( unsigned int questId )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, QuestFunctionTuple* >::iterator itr = questFunctions.find( questId );
	if( itr == questFunctions.end() )
		return NULL;
	else
		return itr->second;
}

InstanceFunctionTuple* FunctionRegistry::getInstanceFunctions( unsigned int mapId )
{
	HM_NAMESPACE::HM_HASH_MAP< unsigned int, InstanceFunctionTuple* >::iterator itr = instanceFunctions.find( mapId );
	if( itr == instanceFunctions.end() )
		return NULL;
	else
		return itr->second;
}
