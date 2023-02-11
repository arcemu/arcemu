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
#include "instance/PythonInstanceScriptFactory.hpp"
#include "instance/PythonInstanceScript.hpp"
#include "engine/FunctionRegistry.hpp"

class FactoryCreatedPythonInstanceScript : public PythonInstanceScript
{
public:
	FactoryCreatedPythonInstanceScript( MapMgr* mgr, const InstanceFunctionTuple &tuple ) : PythonInstanceScript( mgr, tuple ){}
	
	~FactoryCreatedPythonInstanceScript()
	{
		PythonInstanceScriptFactory::removeScript( this );
	}
};

HM_NAMESPACE::HM_HASH_SET< InstanceScript* > PythonInstanceScriptFactory::createdScripts;

Mutex PythonInstanceScriptFactory::lock;

InstanceScript* PythonInstanceScriptFactory::createInstanceScript( MapMgr* mgr )
{
	Guard g( lock );

	InstanceFunctionTuple *functions = FunctionRegistry::getInstanceFunctions( mgr->GetMapId() );
	FactoryCreatedPythonInstanceScript *script = NULL;

	if( functions != NULL )
	{
		script = new FactoryCreatedPythonInstanceScript( mgr, *functions );
	}
	else
	{
		InstanceFunctionTuple empty;
		script = new FactoryCreatedPythonInstanceScript( mgr, empty );
	}

	createdScripts.insert( script );

	return script;
}

void PythonInstanceScriptFactory::onShutdown()
{
	Guard g( lock );

	createdScripts.clear();
}

void PythonInstanceScriptFactory::onReload()
{
	Guard g( lock );

	HM_NAMESPACE::HM_HASH_SET< InstanceScript* >::iterator itr = createdScripts.begin();
	while( itr != createdScripts.end() )
	{
		FactoryCreatedPythonInstanceScript *script = static_cast< FactoryCreatedPythonInstanceScript* >( *itr );

		InstanceFunctionTuple *tuple = FunctionRegistry::getInstanceFunctions( script->GetInstance()->GetMapId() );
		if( tuple != NULL )
		{
			script->setFunctions( *tuple );
		}
		else
		{
			script->clearFunctions();
		}

		++itr;
	}
}

void PythonInstanceScriptFactory::removeScript( InstanceScript* script )
{
	Guard g( lock );

	HM_NAMESPACE::HM_HASH_SET< InstanceScript* >::iterator itr = createdScripts.find( script );
	if( itr != createdScripts.end() )
	{
		createdScripts.erase( script );
	}
}
