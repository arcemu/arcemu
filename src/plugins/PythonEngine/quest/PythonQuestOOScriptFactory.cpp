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

#include "Python.h"
#include "PythonQuestOOScriptFactory.hpp"

class FactoryCreatedPythonOOQuestScript : public PythonQuestOOScript
{
public:
	FactoryCreatedPythonOOQuestScript( PyObject *obj, unsigned int questId ) :
	PythonQuestOOScript( obj )
	{
		this->questId = questId;
	}

	~FactoryCreatedPythonOOQuestScript()
	{
		PythonQuestOOScriptFactory::removeScript( this );
	}

	unsigned int getQuestId() const{ return questId; }

private:
	unsigned int questId;
};

HM_NAMESPACE::HM_HASH_SET< PythonQuestOOScript* > PythonQuestOOScriptFactory::createdScripts;

Mutex PythonQuestOOScriptFactory::lock;

PythonQuestOOScript* PythonQuestOOScriptFactory::createQuestScript( unsigned int questId )
{
	Guard g( lock );

	PyObject *pyScript = static_cast< PyObject* >( ReferenceRegistry::getQuestScript( questId ) );

	FactoryCreatedPythonOOQuestScript *script = new FactoryCreatedPythonOOQuestScript( pyScript, questId );
	createdScripts.insert( script );
	return script;
}

void PythonQuestOOScriptFactory::onReload()
{
	Guard g( lock );

	HM_NAMESPACE::HM_HASH_SET< PythonQuestOOScript* >::iterator itr = createdScripts.begin();
	while( itr != createdScripts.end() )
	{
		FactoryCreatedPythonOOQuestScript *script = static_cast< FactoryCreatedPythonOOQuestScript* >( *itr );
		PyObject *pyScript = static_cast< PyObject* >( ReferenceRegistry::getQuestScript( script->getQuestId() ) );

		script->setObject( pyScript );

		++itr;
	}
}

void PythonQuestOOScriptFactory::onShutDown()
{
	Guard g( lock );

	createdScripts.clear();
}

void PythonQuestOOScriptFactory::removeScript( PythonQuestOOScript* script )
{
	Guard g( lock );

	HM_NAMESPACE::HM_HASH_SET< PythonQuestOOScript* >::iterator itr = createdScripts.find( script );
	if( itr != createdScripts.end() )
	{
		createdScripts.erase( itr );
	}
}
