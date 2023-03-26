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
#include "instance/PythonInstanceScript.hpp"

#include "Python.h"

#include "python/ArcPython.hpp"
#include "python/ArcPyTuple.hpp"
#include "python/PythonValue.hpp"
#include "python/PythonCallable.hpp"

PythonInstanceScript::PythonInstanceScript( MapMgr* mgr, const InstanceFunctionTuple &tuple ) :
InstanceScript( mgr )
{
	setFunctions( tuple );
}

PythonInstanceScript::~PythonInstanceScript()
{
}

void PythonInstanceScript::setFunctions( const InstanceFunctionTuple &tuple )
{
	for( int i = 0; i < PYTHON_INSTANCE_EVENT_COUNT; i++ )
	{
		this->functions.setFunction( i, tuple.getFunction( i ) );
	}
}

void PythonInstanceScript::clearFunctions()
{
	this->functions.clearFunctions();
}


void PythonInstanceScript::OnPlayerDeath( Player* victim, Unit* killer )
{
	Guard g( ArcPython::getLock() );

	unsigned long eventType = PYTHON_INSTANCE_EVENT_ON_PLAYER_DEATH;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItem( 0, GetInstance()->GetInstanceID() );
	args.setItemPlayer( 1, victim );
	args.setItemUnit( 2, killer );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonInstanceScript::OnPlayerEnter( Player* player )
{
	Guard g( ArcPython::getLock() );

	unsigned long eventType = PYTHON_INSTANCE_EVENT_ON_PLAYER_ENTER;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 2 );
	args.setItem( 0, GetInstance()->GetInstanceID() );
	args.setItemPlayer( 1, player );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonInstanceScript::OnAreaTrigger( Player* player, uint32 areaId )
{
	Guard g( ArcPython::getLock() );

	unsigned long eventType = PYTHON_INSTANCE_EVENT_ON_AREA_TRIGGER;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItem( 0, GetInstance()->GetInstanceID() );
	args.setItemPlayer( 1, player );
	args.setItem( 2, areaId );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonInstanceScript::OnZoneChange( Player* player, uint32 newZone, uint32 oldZone )
{
	Guard g( ArcPython::getLock() );

	unsigned long eventType = PYTHON_INSTANCE_EVENT_ON_ZONE_CHANGE;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 4 );
	args.setItem( 0, GetInstance()->GetInstanceID() );
	args.setItemPlayer( 1, player );
	args.setItem( 2, newZone );
	args.setItem( 3, oldZone );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}


void PythonInstanceScript::OnCreatureDeath( Creature* victim, Unit* killer )
{
	Guard g( ArcPython::getLock() );

	unsigned long eventType = PYTHON_INSTANCE_EVENT_ON_CREATURE_DEATH;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItem( 0, GetInstance()->GetInstanceID() );
	args.setItemUnit( 1, victim );
	args.setItemUnit( 2, killer );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonInstanceScript::OnCreaturePushToWorld( Creature* creature )
{
	Guard g( ArcPython::getLock() );

	unsigned long eventType = PYTHON_INSTANCE_EVENT_ON_CREATURE_PUSH;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 2 );
	args.setItem( 0, GetInstance()->GetInstanceID() );
	args.setItemUnit( 1, creature );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonInstanceScript::OnGameObjectActivate( GameObject* gameObject, Player* player )
{
	Guard g( ArcPython::getLock() );

	unsigned long eventType = PYTHON_INSTANCE_EVENT_ON_GO_ACTIVATE;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItem( 0, GetInstance()->GetInstanceID() );
	args.setItemGameObject( 1, gameObject );
	args.setItemPlayer( 2, player );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonInstanceScript::OnGameObjectPushToWorld( GameObject* gameObject )
{
	Guard g( ArcPython::getLock() );

	unsigned long eventType = PYTHON_INSTANCE_EVENT_ON_GO_PUSH;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 2 );
	args.setItem( 0, GetInstance()->GetInstanceID() );
	args.setItemGameObject( 1, gameObject );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}


void PythonInstanceScript::OnLoad()
{
	Guard g( ArcPython::getLock() );

	unsigned long eventType = PYTHON_INSTANCE_EVENT_ONLOAD;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 1 );
	args.setItem( 0, GetInstance()->GetInstanceID() );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonInstanceScript::Destroy()
{
	Guard g( ArcPython::getLock() );

	unsigned long eventType = PYTHON_INSTANCE_EVENT_DESTROY;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 1 );
	args.setItem( 0, GetInstance()->GetInstanceID() );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );

	delete this;
}

