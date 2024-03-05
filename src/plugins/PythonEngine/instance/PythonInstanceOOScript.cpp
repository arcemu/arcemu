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

#include "PythonInstanceOOScript.hpp"

#include "python/ArcPython.hpp"

#include "python/modules/ArcPyGameObject.hpp"
#include "python/modules/ArcPyCreature.hpp"
#include "python/modules/ArcPyPlayer.hpp"

PythonInstanceOOScript::PythonInstanceOOScript( MapMgr *mgr, PyObject *pyObject ) :
InstanceScript( mgr )
{
	this->pyObject = NULL;
	setObject( pyObject );
}

PythonInstanceOOScript::~PythonInstanceOOScript()
{
	setObject( NULL );
}

void PythonInstanceOOScript::setObject( PyObject *pyObject )
{
	if( this->pyObject != NULL )
	{
		Py_DECREF( this->pyObject );
		this->pyObject = NULL;
	}

	if( pyObject != NULL )
	{
		Py_INCREF( pyObject );
	}

	this->pyObject = pyObject;
}

void PythonInstanceOOScript::OnPlayerDeath( Player* victim, Unit* killer )
{
	Guard g( ArcPython::getLock() );

	if( pyObject == NULL )
	{
		return;
	}

	PyObject_CallMethod( pyObject, "OnPlayerDeath", "OO", createArcPyPlayer( victim ), createArcPyUnit( killer ) );
}

void PythonInstanceOOScript::OnPlayerEnter( Player* player )
{
	Guard g( ArcPython::getLock() );

	if( pyObject == NULL )
	{
		return;
	}

	PyObject_CallMethod( pyObject, "OnPlayerEnter", "O", createArcPyPlayer( player ) );
}

void PythonInstanceOOScript::OnAreaTrigger( Player* player, uint32 areaId )
{
	Guard g( ArcPython::getLock() );

	if( pyObject == NULL )
	{
		return;
	}

	PyObject_CallMethod( pyObject, "OnAreaTrigger", "OI", createArcPyPlayer( player ), areaId );
}

void PythonInstanceOOScript::OnZoneChange( Player* player, uint32 newZone, uint32 oldZone )
{
	Guard g( ArcPython::getLock() );

	if( pyObject == NULL )
	{
		return;
	}

	PyObject_CallMethod( pyObject, "OnZoneChange", "OII", createArcPyPlayer( player ), newZone, oldZone );
}

void PythonInstanceOOScript::OnCreatureDeath( Creature* victim, Unit* killer )
{
	Guard g( ArcPython::getLock() );

	if( pyObject == NULL )
	{
		return;
	}

	PyObject_CallMethod( pyObject, "OnCreatureDeath", "OO", createArcPyCreature( victim ), createArcPyUnit( killer ) );
}

void PythonInstanceOOScript::OnCreaturePushToWorld( Creature* creature )
{
	Guard g( ArcPython::getLock() );

	if( pyObject == NULL )
	{
		return;
	}

	PyObject_CallMethod( pyObject, "OnCreaturePush", "O", createArcPyCreature( creature ) );
}

void PythonInstanceOOScript::OnGameObjectActivate( GameObject* gameObject, Player* player )
{
	Guard g( ArcPython::getLock() );

	if( pyObject == NULL )
	{
		return;
	}

	PyObject_CallMethod( pyObject, "OnGameObjectActivate", "OO", createArcPyGameObject( gameObject ), createArcPyPlayer( player ) );
}

void PythonInstanceOOScript::OnGameObjectPushToWorld( GameObject* gameObject )
{
	Guard g( ArcPython::getLock() );

	if( pyObject == NULL )
	{
		return;
	}

	PyObject_CallMethod( pyObject, "OnGameObjectPush", "O", createArcPyGameObject( gameObject ) );
}

void PythonInstanceOOScript::OnLoad()
{
	Guard g( ArcPython::getLock() );

	if( pyObject == NULL )
	{
		return;
	}

	PyObject_CallMethod( pyObject, "OnLoad", NULL );
}

void PythonInstanceOOScript::Destroy()
{
	Guard g( ArcPython::getLock() );

	if( pyObject == NULL )
	{
		return;
	}

	PyObject_CallMethod( pyObject, "OnDestroy", NULL );
}

