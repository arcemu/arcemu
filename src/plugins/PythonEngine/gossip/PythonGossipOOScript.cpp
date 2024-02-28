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
#include "PythonGossipOOScript.hpp"

#include "python/ArcPython.hpp"

#include "python/modules/ArcPyPlayer.hpp"

PythonGossipOOScript::PythonGossipOOScript( PyObject *obj )
{
	this->obj = NULL;
	setObject( obj );
}

PythonGossipOOScript::~PythonGossipOOScript()
{
	setObject( NULL );
}

void PythonGossipOOScript::setObject( PyObject *obj )
{
	if( this->obj != NULL )
	{
		Py_DECREF( this->obj );
		this->obj = NULL;
	}

	if( obj != NULL )
	{
		Py_INCREF( obj );
	}

	this->obj = obj;
}

void PythonGossipOOScript::OnHello( Object* object, Player* player )
{
	Guard g( ArcPython::getLock() );

	if( obj == NULL )
	{
		return;
	}

	PyObject_CallMethod( obj, "OnHello", "OO", createArcPyObject( object ), createArcPyPlayer( player ) );
}

void PythonGossipOOScript::OnSelectOption( Object* object, Player* player, uint32 id, const char* enteredCode )
{
	Guard g( ArcPython::getLock() );

	if( obj == NULL )
	{
		return;
	}

	PyObject_CallMethod( obj, "OnSelectOption", "OOIs", createArcPyObject( object ), createArcPyPlayer( player ), id, enteredCode );
}

void PythonGossipOOScript::OnEnd( Object* object, Player* player )
{
	Guard g( ArcPython::getLock() );

	if( obj == NULL )
	{
		return;
	}

	PyObject_CallMethod( obj, "OnEnd", "OO", createArcPyObject( object ), createArcPyPlayer( player ) );
}
