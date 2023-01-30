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

#include "PythonGameObjectAIScriptFactory.hpp"

#include "PythonGameObjectAIScript.hpp"

#include "FunctionRegistry.hpp"


class FactoryManagedPythonGameObjectAIScript : public PythonGameObjectAIScript
{
public:
	FactoryManagedPythonGameObjectAIScript( GameObject* src, GOFunctionTuple &tuple ) : PythonGameObjectAIScript( src, tuple ){}
	
	~FactoryManagedPythonGameObjectAIScript()
	{
		PythonGameObjectAIScriptFactory::removeScript( _gameobject->GetInfo()->ID );
	}
};


HM_NAMESPACE::HM_HASH_MAP< uint32, GameObjectAIScript* > PythonGameObjectAIScriptFactory::createdScripts;

GameObjectAIScript* PythonGameObjectAIScriptFactory::createScript( GameObject* src )
{
	uint32 id = src->GetInfo()->ID;
	PythonGameObjectAIScript* script = NULL;
	
	GOFunctionTuple* tuple = FunctionRegistry::getGOEventFunctions( id );
	if( tuple != NULL )
	{
		script = new FactoryManagedPythonGameObjectAIScript( src, *tuple );
	}
	else
	{
		/// This shouldn't happen
		GOFunctionTuple empty;
		script = new FactoryManagedPythonGameObjectAIScript( src, empty );
	}

	createdScripts.insert( std::pair< uint32, GameObjectAIScript* >( id, script ) );

	return script;
}

void PythonGameObjectAIScriptFactory::onReload()
{
	HM_NAMESPACE::HM_HASH_MAP< uint32, GameObjectAIScript* >::iterator itr = createdScripts.begin();
	while( itr != createdScripts.end() )
	{
		PythonGameObjectAIScript *script = (PythonGameObjectAIScript*)itr->second;

		GOFunctionTuple* tuple = FunctionRegistry::getGOEventFunctions( itr->first );
		if( tuple != NULL )
		{
			script->setFunctions( *tuple );
		}
		else
		{
			/// Removed script from the script files
			GOFunctionTuple empty;
			script->setFunctions( empty );
		}
		++itr;
	}
}

void PythonGameObjectAIScriptFactory::onShutdown()
{
	createdScripts.clear();
}

void PythonGameObjectAIScriptFactory::removeScript( uint32 goId )
{
	HM_NAMESPACE::HM_HASH_MAP< uint32, GameObjectAIScript* >::iterator itr = createdScripts.find( goId );
	if( itr != createdScripts.end() )
	{
		createdScripts.erase( itr );
	}
}

