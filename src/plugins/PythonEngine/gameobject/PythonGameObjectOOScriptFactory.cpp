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
#include "PythonGameObjectOOScript.hpp"
#include "PythonGameObjectOOScriptFactory.hpp"

#include "engine/FunctionRegistry.hpp"

#include "python/ArcPython.hpp"

#include "python/modules/ArcPyGameObject.hpp"

/// A Python GameObject AI script that removes itself from the factory's list when being destructed
class FactoryManagedPythonGameObjectOOScript : public PythonGameObjectOOScript
{
public:
	FactoryManagedPythonGameObjectOOScript( GameObject* src, PyObject *obj ) : PythonGameObjectOOScript( src, obj ){}
	
	~FactoryManagedPythonGameObjectOOScript()
	{
		PythonGameObjectOOScriptFactory::removeScript( this );
	}
};

HM_NAMESPACE::HM_HASH_SET< GameObjectAIScript* > PythonGameObjectOOScriptFactory::createdScripts;

Mutex PythonGameObjectOOScriptFactory::lock;

GameObjectAIScript* PythonGameObjectOOScriptFactory::createScript( GameObject* src )
{
	Guard g( lock );

	Guard pg( ArcPython::getLock() );

	uint32 id = src->getProto()->ID;
	PythonGameObjectOOScript* script = NULL;
	
	void *factoryFunction = FunctionRegistry::getGameObjectScriptFactory( id );
	if( factoryFunction != NULL )
	{
		PyObject *instance = PyObject_CallFunction( (PyObject*)factoryFunction, "O", createArcPyGameObject( src ) );
		if( instance != NULL )
		{
			/// Check that it is a subclass, and is a subclass of Python GameObject script base class
			if( ( Py_TYPE( instance )->tp_base != NULL ) && ( strcmp( Py_TYPE( instance )->tp_base->tp_name, "ArcPyGameObjectScript" ) == 0 ) )
			{
				script = new FactoryManagedPythonGameObjectOOScript( src, instance );
				createdScripts.insert( script );
			}
			else
			{
				LOG_ERROR( "Failed to instantiate Python GameObject script for %u: The script must be a subclass of GameObjectScript", id );
			}
		}
		else
		{
			LOG_ERROR( "Failed to instantiate Python GameObject script for %u", id );
		}
	}

	return script;
}

void PythonGameObjectOOScriptFactory::onReload()
{
	Guard g( lock );

	Guard pg( ArcPython::getLock() );

	HM_NAMESPACE::HM_HASH_SET< GameObjectAIScript* >::iterator itr = createdScripts.begin();
	while( itr != createdScripts.end() )
	{
		PythonGameObjectOOScript *script = (PythonGameObjectOOScript*)(*itr);
		uint32 id = script->getGameObject()->getProto()->ID;

		void *factoryFunction = FunctionRegistry::getGameObjectScriptFactory( id );
		if( factoryFunction != NULL )
		{
			PyObject *instance = PyObject_CallFunction( (PyObject*)factoryFunction, "O", createArcPyGameObject( script->getGameObject() ) );
			if( instance != NULL )
			{
				/// Check that it is a subclass, and is a subclass of Python GameObject script base class
				if( ( Py_TYPE( instance )->tp_base != NULL ) && ( strcmp( Py_TYPE( instance )->tp_base->tp_name, "ArcPyGameObjectScript" ) == 0 ) )
				{
					script->setObject( instance );
				}
				else
				{
					LOG_ERROR( "Failed to instantiate Python creature script for %u: The script must be a subclass of GameObjectScript", id );
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

void PythonGameObjectOOScriptFactory::onShutdown()
{
	Guard g( lock );

	createdScripts.clear();
}

void PythonGameObjectOOScriptFactory::removeScript( GameObjectAIScript *script )
{
	Guard g( lock );

	HM_NAMESPACE::HM_HASH_SET< GameObjectAIScript* >::iterator itr = createdScripts.find( script );
	if( itr != createdScripts.end() )
	{
		createdScripts.erase( itr );
	}
}
