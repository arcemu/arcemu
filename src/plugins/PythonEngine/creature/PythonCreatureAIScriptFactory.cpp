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

CreatureAIScript* PythonCreatureAIScriptFactory::createScript( Creature* src )
{
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
	createdScripts.clear();
}

void PythonCreatureAIScriptFactory::removeScript( CreatureAIScript *script )
{
	HM_NAMESPACE::HM_HASH_SET< CreatureAIScript* >::iterator itr = createdScripts.find( script );
	if( itr != createdScripts.end() )
	{
		createdScripts.erase( itr );
	}
}
