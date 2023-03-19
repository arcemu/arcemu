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

#include "PythonCreatureAIScriptFactory.hpp"

#include "PythonCreatureAIScript.hpp"

#include "engine/FunctionRegistry.hpp"


/// A Python Creature AI script that removes itself from the factory's list when being destructed
class FactoryManagedPythonCreatureAIScript : public PythonCreatureAIScript
{
public:
	FactoryManagedPythonCreatureAIScript( Creature* src, CreatureFunctionTuple &tuple ) : PythonCreatureAIScript( src, tuple ){}
	
	~FactoryManagedPythonCreatureAIScript()
	{
		PythonCreatureAIScriptFactory::removeScript( this );
	}
};


HM_NAMESPACE::HM_HASH_SET< CreatureAIScript* > PythonCreatureAIScriptFactory::createdScripts;

Mutex PythonCreatureAIScriptFactory::lock;

CreatureAIScript* PythonCreatureAIScriptFactory::createScript( Creature* src )
{
	Guard g( lock );

	uint32 id = src->GetProto()->Id;
	PythonCreatureAIScript* script = NULL;
	
	CreatureFunctionTuple* tuple = FunctionRegistry::getCreatureEventFunctions( id );
	if( tuple != NULL )
	{
		script = new FactoryManagedPythonCreatureAIScript( src, *tuple );
	}
	else
	{
		/// This shouldn't happen
		CreatureFunctionTuple empty;
		script = new FactoryManagedPythonCreatureAIScript( src, empty );
	}

	createdScripts.insert( script );

	return script;
}

void PythonCreatureAIScriptFactory::onReload()
{
	Guard g( lock );

	/// Reload the functions in each script when the script engine is reloaded
	HM_NAMESPACE::HM_HASH_SET< CreatureAIScript* >::iterator itr = createdScripts.begin();
	while( itr != createdScripts.end() )
	{
		PythonCreatureAIScript *script = (PythonCreatureAIScript*)(*itr);

		CreatureFunctionTuple* tuple = FunctionRegistry::getCreatureEventFunctions( script->GetUnit()->GetProto()->Id );
		if( tuple != NULL )
		{
			script->setFunctions( *tuple );
		}
		else
		{
			/// Removed script from the script files
			CreatureFunctionTuple empty;
			script->setFunctions( empty );
		}
		++itr;
	}
}

void PythonCreatureAIScriptFactory::onShutdown()
{
	Guard g( lock );

	createdScripts.clear();
}

void PythonCreatureAIScriptFactory::removeScript( CreatureAIScript *script )
{
	Guard g( lock );

	HM_NAMESPACE::HM_HASH_SET< CreatureAIScript* >::iterator itr = createdScripts.find( script );
	if( itr != createdScripts.end() )
	{
		createdScripts.erase( itr );
	}
}
