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

#include "PythonGameObjectAIScript.hpp"

#include "PythonGOEventTypes.hpp"

#include "Python.h"
#include "python/ArcPython.hpp"
#include "python/PythonCallable.hpp"
#include "python/ArcPyTuple.hpp"

PythonGameObjectAIScript::PythonGameObjectAIScript( GameObject* src, GOFunctionTuple &functions ) :
GameObjectAIScript( src )
{
	setFunctions( functions );
}

PythonGameObjectAIScript::~PythonGameObjectAIScript()
{
}

void PythonGameObjectAIScript::setFunctions( GOFunctionTuple &functions )
{
	this->functions.onAIUpdate = functions.onAIUpdate;
	this->functions.onCreate = functions.onCreate;
	this->functions.onDamaged = functions.onDamaged;
	this->functions.onDespawn = functions.onDespawn;
	this->functions.onDestroyed = functions.onDestroyed;
	this->functions.onLootTaken = functions.onLootTaken;
	this->functions.onSpawn = functions.onSpawn;
	this->functions.onUse = functions.onUse;
}

void PythonGameObjectAIScript::OnCreate()
{
	Guard g( ArcPython::getLock() );

	if( functions.onCreate == NULL )
		return;

	ArcPyTuple args( 1 );
	args.setItemGameObject( 0, _gameobject );

	PythonCallable callable( functions.onCreate );
	PythonValue value = callable.call( args );

	if( value.isEmpty() )
	{
		Python::printError();
	}
	else
	{
		value.decref();
	}
}

void PythonGameObjectAIScript::OnSpawn()
{
	Guard g( ArcPython::getLock() );

	if( functions.onSpawn == NULL )
		return;

	ArcPyTuple args( 1 );
	args.setItemGameObject( 0, _gameobject );

	PythonCallable callable( functions.onSpawn );
	PythonValue value = callable.call( args );

	if( value.isEmpty() )
	{
		Python::printError();
	}
	else
	{
		value.decref();
	}
}

void PythonGameObjectAIScript::OnDespawn()
{
	Guard g( ArcPython::getLock() );

	if( functions.onDespawn == NULL )
		return;

	ArcPyTuple args( 1 );
	args.setItemGameObject( 0, _gameobject );

	PythonCallable callable( functions.onDespawn );
	PythonValue value = callable.call( args );

	if( value.isEmpty() )
	{
		Python::printError();
	}
	else
	{
		value.decref();
	}
}

void PythonGameObjectAIScript::OnLootTaken( Player* looter, ItemPrototype* itemProto )
{
	Guard g( ArcPython::getLock() );

	if( functions.onLootTaken == NULL )
		return;

	ArcPyTuple args( 4 );
	args.setItemGameObject( 0, _gameobject );
	args.setItem( 1, PYTHON_GO_EVENT_ON_LOOT_TAKEN );
	args.setItemPlayer( 2, looter );
	args.setItem( 3, itemProto->ItemId );

	PythonCallable callable( functions.onLootTaken );
	PythonValue value = callable.call( args );

	if( value.isEmpty() )
	{
		Python::printError();
	}
	else
	{
		value.decref();
	}
}

void PythonGameObjectAIScript::OnActivate( Player* player )
{
	Guard g( ArcPython::getLock() );

	if( functions.onUse == NULL )
		return;

	ArcPyTuple args( 3 );
	args.setItemGameObject( 0, this->_gameobject );
	args.setItem( 1, PYTHON_GO_EVENT_ON_USE );
	args.setItemPlayer( 2, player );

	PythonCallable callable( functions.onUse );
	PythonValue value = callable.call( args );
	
	if( value.isEmpty() )
	{
		Python::printError();
	}
	else
	{
		value.decref();
	}
}

void PythonGameObjectAIScript::OnDamaged( uint32 damage )
{
	Guard g( ArcPython::getLock() );

	if( functions.onDamaged == NULL )
		return;

	ArcPyTuple args( 2 );
	args.setItemGameObject( 0, _gameobject );
	args.setItem( 1, damage );

	PythonCallable callable( functions.onDamaged );
	PythonValue value = callable.call( args );

	if( value.isEmpty() )
	{
		Python::printError();
	}
	else
	{
		value.decref();
	}
}

void PythonGameObjectAIScript::OnDestroyed()
{
	Guard g( ArcPython::getLock() );

	if( functions.onDestroyed == NULL )
		return;

	ArcPyTuple args( 1 );
	args.setItemGameObject( 0, _gameobject );

	PythonCallable callable( functions.onDestroyed );
	PythonValue value = callable.call( args );

	if( value.isEmpty() )
	{
		Python::printError();
	}
	else
	{
		value.decref();
	}
}

void PythonGameObjectAIScript::AIUpdate()
{
	Guard g( ArcPython::getLock() );

	if( functions.onAIUpdate == NULL )
		return;

	ArcPyTuple args( 1 );
	args.setItemGameObject( 0, _gameobject );

	PythonCallable callable( functions.onAIUpdate );
	PythonValue value = callable.call( args );

	if( value.isEmpty() )
	{
		Python::printError();
	}
	else
	{
		value.decref();
	}
}

