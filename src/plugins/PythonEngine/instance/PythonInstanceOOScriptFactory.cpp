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

#include "engine/ReferenceRegistry.hpp"

#include "PythonInstanceOOScriptFactory.hpp"

#include "Python.h"

#include "PythonInstanceOOScript.hpp"

#include "python/ArcPython.hpp"

/// A Python GameObject AI script that removes itself from the factory's list when being destructed
class FactoryManagedPythonInstanceOOScript : public PythonInstanceOOScript
{
public:
	FactoryManagedPythonInstanceOOScript( MapMgr *mgr, PyObject *obj ) : PythonInstanceOOScript( mgr, obj )
	{
	}
	
	~FactoryManagedPythonInstanceOOScript()
	{
		PythonInstanceOOScriptFactory::removeScript( this );
	}
};

HM_NAMESPACE::HM_HASH_SET< InstanceScript* > PythonInstanceOOScriptFactory::createdScripts;

Mutex PythonInstanceOOScriptFactory::lock;

InstanceScript* PythonInstanceOOScriptFactory::createInstanceScript( MapMgr *mgr )
{
	Guard g( lock );

	Guard pg( ArcPython::getLock() );

	PythonInstanceOOScript* script = NULL;

	uint32 id = mgr->GetMapId();
	
	void *factoryFunction = ReferenceRegistry::getInstanceScriptFactory( id );
	if( factoryFunction != NULL )
	{
		PyObject *instance = PyObject_CallFunction( (PyObject*)factoryFunction, NULL );
		if( instance != NULL )
		{
			/// Check that it is a subclass, and is a subclass of Python Instance script base class
			if( ( Py_TYPE( instance )->tp_base != NULL ) && ( strcmp( Py_TYPE( instance )->tp_base->tp_name, "ArcPyInstanceScript" ) == 0 ) )
			{
				script = new FactoryManagedPythonInstanceOOScript( mgr, instance );
				createdScripts.insert( script );
			}
			else
			{
				LOG_ERROR( "Failed to instantiate Python Instance script for %u: The script must be a subclass of InstanceScript", id );
			}
		}
		else
		{
			LOG_ERROR( "Failed to instantiate Python Instance script for %u", id );
		}
	}

	return script;
}

void PythonInstanceOOScriptFactory::onReload()
{
	Guard g( lock );

	Guard pg( ArcPython::getLock() );

	HM_NAMESPACE::HM_HASH_SET< InstanceScript* >::iterator itr = createdScripts.begin();
	while( itr != createdScripts.end() )
	{
		PythonInstanceOOScript *script = (PythonInstanceOOScript*)(*itr);
		uint32 id = script->GetInstance()->GetMapId();

		void *factoryFunction = ReferenceRegistry::getGameObjectScriptFactory( id );
		if( factoryFunction != NULL )
		{
			PyObject *instance = PyObject_CallFunction( (PyObject*)factoryFunction, NULL );
			if( instance != NULL )
			{
				/// Check that it is a subclass, and is a subclass of Python Instance script base class
				if( ( Py_TYPE( instance )->tp_base != NULL ) && ( strcmp( Py_TYPE( instance )->tp_base->tp_name, "ArcPyInstanceScript" ) == 0 ) )
				{
					script->setObject( instance );
				}
				else
				{
					LOG_ERROR( "Failed to instantiate Python instance script for %u: The script must be a subclass of InstanceScript", id );
				}
			}
			else
			{
				LOG_ERROR( "Failed to instantiate Python instance script for %u", id );
			}
		}
		else
		{
			script->setObject( NULL );
		}
		
		++itr;
	}
}

void PythonInstanceOOScriptFactory::onShutdown()
{
	Guard g( lock );

	createdScripts.clear();
}

void PythonInstanceOOScriptFactory::removeScript( InstanceScript *script )
{
	Guard g( lock );

	HM_NAMESPACE::HM_HASH_SET< InstanceScript* >::iterator itr = createdScripts.find( script );
	if( itr != createdScripts.end() )
	{
		createdScripts.erase( itr );
	}
}
