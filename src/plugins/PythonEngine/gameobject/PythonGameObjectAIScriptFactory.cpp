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

#include "gameobject/PythonGameObjectAIScriptFactory.hpp"

#include "gameobject/PythonGameObjectAIScript.hpp"

#include "engine/FunctionRegistry.hpp"


class FactoryManagedPythonGameObjectAIScript : public PythonGameObjectAIScript
{
public:
	FactoryManagedPythonGameObjectAIScript( GameObject* src, GOFunctionTuple &tuple ) : PythonGameObjectAIScript( src, tuple ){}
	
	~FactoryManagedPythonGameObjectAIScript()
	{
		PythonGameObjectAIScriptFactory::removeScript( this );
	}
};


HM_NAMESPACE::HM_HASH_SET< GameObjectAIScript* > PythonGameObjectAIScriptFactory::createdScripts;

Mutex PythonGameObjectAIScriptFactory::lock;

GameObjectAIScript* PythonGameObjectAIScriptFactory::createScript( GameObject* src )
{
	Guard g( lock );

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

	createdScripts.insert( script );

	return script;
}

void PythonGameObjectAIScriptFactory::onReload()
{
	Guard g( lock );

	HM_NAMESPACE::HM_HASH_SET< GameObjectAIScript* >::iterator itr = createdScripts.begin();
	while( itr != createdScripts.end() )
	{
		PythonGameObjectAIScript *script = (PythonGameObjectAIScript*)(*itr);

		GOFunctionTuple* tuple = FunctionRegistry::getGOEventFunctions( script->getGameObject()->GetInfo()->ID );
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
	Guard g( lock );

	createdScripts.clear();
}

void PythonGameObjectAIScriptFactory::removeScript( GameObjectAIScript *script )
{
	Guard g( lock );

	HM_NAMESPACE::HM_HASH_SET< GameObjectAIScript* >::iterator itr = createdScripts.find( script );
	if( itr != createdScripts.end() )
	{
		createdScripts.erase( itr );
	}
}

