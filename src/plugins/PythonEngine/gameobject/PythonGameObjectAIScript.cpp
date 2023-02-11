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

#include "gameobject/PythonGameObjectAIScript.hpp"

#include "gameobject/PythonGOEventTypes.hpp"

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
	for( int i = 0; i < PYTHON_GO_EVENT_COUNT; i++ )
	{
		this->functions.setFunction( i, functions.getFunction( i ) );
	}
}

void PythonGameObjectAIScript::OnCreate()
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_GO_EVENT_ON_CREATE;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 1 );
	args.setItemGameObject( 0, _gameobject );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonGameObjectAIScript::OnSpawn()
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_GO_EVENT_ON_SPAWN;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 1 );
	args.setItemGameObject( 0, _gameobject );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonGameObjectAIScript::OnDespawn()
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_GO_EVENT_ON_DESPAWN;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 1 );
	args.setItemGameObject( 0, _gameobject );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonGameObjectAIScript::OnLootTaken( Player* looter, ItemPrototype* itemProto )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_GO_EVENT_ON_LOOT_TAKEN;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 4 );
	args.setItemGameObject( 0, _gameobject );
	args.setItem( 1, PYTHON_GO_EVENT_ON_LOOT_TAKEN );
	args.setItemPlayer( 2, looter );
	args.setItem( 3, itemProto->ItemId );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonGameObjectAIScript::OnActivate( Player* player )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_GO_EVENT_ON_USE;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItemGameObject( 0, this->_gameobject );
	args.setItem( 1, PYTHON_GO_EVENT_ON_USE );
	args.setItemPlayer( 2, player );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonGameObjectAIScript::OnDamaged( uint32 damage )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_GO_EVENT_ON_DAMAGED;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 2 );
	args.setItemGameObject( 0, _gameobject );
	args.setItem( 1, damage );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonGameObjectAIScript::OnDestroyed()
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_GO_EVENT_ON_DESTROYED;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 1 );
	args.setItemGameObject( 0, _gameobject );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonGameObjectAIScript::AIUpdate()
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_GO_EVENT_ON_AIUPDATE;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 1 );
	args.setItemGameObject( 0, _gameobject );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

