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
#include "quest/PythonQuestScriptFactory.hpp"
#include "engine/FunctionRegistry.hpp"

class FactoryCreatedPythonQuestScript : public PythonQuestScript
{
public:
	FactoryCreatedPythonQuestScript( const QuestFunctionTuple &tuple, unsigned int questId ) :
	PythonQuestScript( tuple )
	{
		this->questId = questId;
	}

	~FactoryCreatedPythonQuestScript()
	{
		PythonQuestScriptFactory::removeScript( this );
	}

	unsigned int getQuestId() const{ return questId; }

private:
	unsigned int questId;
};

HM_NAMESPACE::HM_HASH_SET< PythonQuestScript* > PythonQuestScriptFactory::createdScripts;

Mutex PythonQuestScriptFactory::lock;

PythonQuestScript* PythonQuestScriptFactory::createQuestScript( unsigned int questId )
{
	Guard g( lock );

	QuestFunctionTuple *tuple = FunctionRegistry::getQuestFunctions( questId );
	FactoryCreatedPythonQuestScript *script = new FactoryCreatedPythonQuestScript( *tuple, questId );
	createdScripts.insert( script );
	return script;
}

void PythonQuestScriptFactory::onReload()
{
	Guard g( lock );

	HM_NAMESPACE::HM_HASH_SET< PythonQuestScript* >::iterator itr = createdScripts.begin();
	while( itr != createdScripts.end() )
	{
		FactoryCreatedPythonQuestScript *script = static_cast< FactoryCreatedPythonQuestScript* >( *itr );
		QuestFunctionTuple *tuple = FunctionRegistry::getQuestFunctions( script->getQuestId() );

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

void PythonQuestScriptFactory::onShutDown()
{
	Guard g( lock );

	createdScripts.clear();
}

void PythonQuestScriptFactory::removeScript( PythonQuestScript* script )
{
	Guard g( lock );

	HM_NAMESPACE::HM_HASH_SET< PythonQuestScript* >::iterator itr = createdScripts.find( script );
	if( itr != createdScripts.end() )
	{
		createdScripts.erase( itr );
	}
}
