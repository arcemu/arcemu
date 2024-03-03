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

#include "python/ArcPython.hpp"

#include "PythonQuestOOScript.hpp"

#include "python/modules/ArcPyPlayer.hpp"

PythonQuestOOScript::PythonQuestOOScript( PyObject *obj )
{
	this->obj = NULL;
	setObject( obj );
}

PythonQuestOOScript::~PythonQuestOOScript()
{
	setObject( NULL );
}

void PythonQuestOOScript::setObject( PyObject *obj )
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

void PythonQuestOOScript::OnQuestStart( Player* target, QuestLogEntry* questLogEntry )
{
	Guard g( ArcPython::getLock() );

	if( obj == NULL )
	{
		return;
	}

	PyObject_CallMethod( obj, "OnQuestStart", "OI", createArcPyPlayer( target ), questLogEntry->GetQuest()->id );
}

void PythonQuestOOScript::OnQuestComplete( Player* target, QuestLogEntry* questLogEntry )
{
	Guard g( ArcPython::getLock() );

	if( obj == NULL )
	{
		return;
	}

	PyObject_CallMethod( obj, "OnQuestComplete", "OI", createArcPyPlayer( target ), questLogEntry->GetQuest()->id );
}

void PythonQuestOOScript::OnQuestCancel( Player* target )
{
	Guard g( ArcPython::getLock() );

	if( obj == NULL )
	{
		return;
	}

	PyObject_CallMethod( obj, "OnQuestCancel", "O", createArcPyPlayer( target ) );
}

void PythonQuestOOScript::OnGameObjectActivate( uint32 entry, Player* target, QuestLogEntry* questLogEntry )
{
	Guard g( ArcPython::getLock() );

	if( obj == NULL )
	{
		return;
	}

	PyObject_CallMethod( obj, "OnGameObjectActivate", "IOI", entry, createArcPyPlayer( target ), questLogEntry->GetQuest()->id );
}

void PythonQuestOOScript::OnCreatureKill( uint32 entry, Player* target, QuestLogEntry* questLogEntry )
{
	Guard g( ArcPython::getLock() );

	if( obj == NULL )
	{
		return;
	}

	PyObject_CallMethod( obj, "OnCreatureKill", "IOI", entry, createArcPyPlayer( target ), questLogEntry->GetQuest()->id );
}

void PythonQuestOOScript::OnExploreArea( uint32 areaId, Player* target, QuestLogEntry* questLogEntry )
{
	Guard g( ArcPython::getLock() );

	if( obj == NULL )
	{
		return;
	}

	PyObject_CallMethod( obj, "OnExploreArea", "IOI", areaId, createArcPyPlayer( target ), questLogEntry->GetQuest()->id );
}

void PythonQuestOOScript::OnPlayerItemPickup( uint32 itemId, uint32 totalCount, Player* target, QuestLogEntry* questLogEntry )
{
	Guard g( ArcPython::getLock() );

	if( obj == NULL )
	{
		return;
	}

	PyObject_CallMethod( obj, "OnPlayerItemPickup", "IIOI", itemId, totalCount, createArcPyPlayer( target ), questLogEntry->GetQuest()->id );
}
