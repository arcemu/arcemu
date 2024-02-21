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

#include "python/ArcPython.hpp"
#include "python/modules/ArcPyPlayer.hpp"

PythonGameObjectOOScript::PythonGameObjectOOScript( GameObject* src, PyObject *obj ) :
GameObjectAIScript( src )
{
	this->obj = NULL;
	setObject( obj );
}

PythonGameObjectOOScript::~PythonGameObjectOOScript()
{
	if( obj != NULL )
	{
		Py_DECREF( obj );
		obj = NULL;
	}
}

void PythonGameObjectOOScript::setObject( PyObject *obj )
{
	if( this->obj != NULL )
	{
		Py_DECREF( this->obj );
		this->obj = NULL;
	}

	this->obj = obj;
	Py_INCREF( this->obj );
}

void PythonGameObjectOOScript::OnCreate()
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "OnCreate", NULL );
}

void PythonGameObjectOOScript::OnSpawn()
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "OnSpawn", NULL );
}

void PythonGameObjectOOScript::OnDespawn()
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "OnDespawn", NULL );
}

void PythonGameObjectOOScript::OnLootTaken( Player *looter, ItemPrototype *itemInfo )
{
	Guard g( ArcPython::getLock() );

	PyObject *app = (PyObject*)createArcPyPlayer( looter );
	PyObject_CallMethod( obj, "OnLootTaken", "OI", app, itemInfo->ItemId );
}

void PythonGameObjectOOScript::OnActivate( Player *player )
{
	Guard g( ArcPython::getLock() );

	PyObject *app = (PyObject*)createArcPyPlayer( player );
	PyObject_CallMethod( obj, "OnActivate", "O", app );
}

void PythonGameObjectOOScript::OnDamaged( uint32 damage )
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "OnDamaged", "I", damage );
}

void PythonGameObjectOOScript::OnDestroyed()
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "OnDestroyed", NULL );
}

void PythonGameObjectOOScript::AIUpdate()
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "AIUpdate", NULL );
}
