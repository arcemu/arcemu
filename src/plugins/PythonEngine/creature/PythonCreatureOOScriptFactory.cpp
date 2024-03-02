/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2024 <http://www.ArcEmu.org/>
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
#include "Python.h"
#include "PythonCreatureOOScript.hpp"
#include "PythonCreatureOOScriptFactory.hpp"

#include "engine/ReferenceRegistry.hpp"

#include "python/ArcPython.hpp"

#include "python/modules/ArcPyCreature.hpp"


/// A Python Creature AI script that removes itself from the factory's list when being destructed
class FactoryManagedPythonCreatureOOScript : public PythonCreatureOOScript
{
public:
	FactoryManagedPythonCreatureOOScript( Creature* src, PyObject *obj ) : PythonCreatureOOScript( src, obj ){}
	
	~FactoryManagedPythonCreatureOOScript()
	{
		PythonCreatureOOScriptFactory::removeScript( this );
	}
};

HM_NAMESPACE::HM_HASH_SET< CreatureAIScript* > PythonCreatureOOScriptFactory::createdScripts;

Mutex PythonCreatureOOScriptFactory::lock;

CreatureAIScript* PythonCreatureOOScriptFactory::createScript( Creature* src )
{
	Guard g( lock );

	Guard pg( ArcPython::getLock() );

	uint32 id = src->GetProto()->Id;
	PythonCreatureOOScript* script = NULL;
	
	void *factoryFunction = ReferenceRegistry::getCreatureScriptFactory( id );
	if( factoryFunction != NULL )
	{
		PyObject *instance = PyObject_CallFunction( (PyObject*)factoryFunction, "O", createArcPyCreature( src ) );
		if( instance != NULL )
		{
			/// Check that it is a subclass, and is a subclass of Python creature script base class
			if( ( Py_TYPE( instance )->tp_base != NULL ) && ( strcmp( Py_TYPE( instance )->tp_base->tp_name, "ArcPyCreatureScript" ) == 0 ) )
			{
				script = new FactoryManagedPythonCreatureOOScript( src, instance );
				createdScripts.insert( script );
			}
			else
			{
				LOG_ERROR( "Failed to instantiate Python creature script for %u: The script must be a subclass of CreatureScript", id );
			}
		}
		else
		{
			LOG_ERROR( "Failed to instantiate Python creature script for %u", id );
		}
	}

	return script;
}

void PythonCreatureOOScriptFactory::onReload()
{
	Guard g( lock );

	Guard pg( ArcPython::getLock() );

	HM_NAMESPACE::HM_HASH_SET< CreatureAIScript* >::iterator itr = createdScripts.begin();
	while( itr != createdScripts.end() )
	{
		PythonCreatureOOScript *script = (PythonCreatureOOScript*)(*itr);
		uint32 id = script->GetUnit()->GetProto()->Id;

		void *factoryFunction = ReferenceRegistry::getCreatureScriptFactory( id );
		if( factoryFunction != NULL )
		{
			PyObject *instance = PyObject_CallFunction( (PyObject*)factoryFunction, "O", createArcPyCreature( script->GetUnit() ) );
			if( instance != NULL )
			{
				/// Check that it is a subclass, and is a subclass of Python creature script base class
				if( ( Py_TYPE( instance )->tp_base != NULL ) && ( strcmp( Py_TYPE( instance )->tp_base->tp_name, "ArcPyCreatureScript" ) == 0 ) )
				{
					script->setObject( instance );
				}
				else
				{
					LOG_ERROR( "Failed to instantiate Python creature script for %u: The script must be a subclass of CreatureScript", id );
				}
			}
			else
			{
				LOG_ERROR( "Failed to instantiate Python creature script for %u", id );
			}
		}
		else
		{
			script->setObject( NULL );
		}
		
		++itr;
	}
}

void PythonCreatureOOScriptFactory::onShutdown()
{
	Guard g( lock );

	createdScripts.clear();
}

void PythonCreatureOOScriptFactory::removeScript( CreatureAIScript *script )
{
	Guard g( lock );

	HM_NAMESPACE::HM_HASH_SET< CreatureAIScript* >::iterator itr = createdScripts.find( script );
	if( itr != createdScripts.end() )
	{
		createdScripts.erase( itr );
	}
}
