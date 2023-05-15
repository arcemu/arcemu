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

#include <Python.h>
#include <cstdio>

#include "StdAfx.h"

#include "ArcPyGameObject.hpp"

static PyObject* ArcPyGameObject_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyGameObject *self = (ArcPyGameObject*)type->tp_alloc( type, 0 );
	self->gameObjectPtr = NULL;
	self->Object.objectPtr = NULL;
	return (PyObject*)self;
}

static int ArcPyGameObject_init( ArcPyGameObject *self, PyObject *args, PyObject *keywords )
{
	self->gameObjectPtr = NULL;
	self->Object.objectPtr = NULL;
	return 0;
}

static void ArcPyGameObject_dealloc( ArcPyGameObject* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}

/// getName
///   Returns the name of the GameObject
///
/// Parameters
///   No parameters
///
/// Return value
///   Returns a string that contains the name of the GameObject
///
/// Example:
///   n = go.getName()
///
static PyObject* ArcPyGameObject_getName( ArcPyGameObject *self, PyObject *args )
{
	GameObject* gameObject = self->gameObjectPtr;
	PyObject *name = PyUnicode_FromString(  gameObject->GetInfo()->Name );	
	return name;
}


/// getId
///   Returns the Id of the GameObject
///
/// Parameters
///   No parameters
///
/// Return value
///   Returns an integer that contains the Id of the GameObject
///
/// Example:
///   id = go.getId()
///
static PyObject* ArcPyGameObject_getId( ArcPyGameObject* self, PyObject* args )
{
	GameObject *gameObject = self->gameObjectPtr;
	PyObject *id = PyLong_FromLong( gameObject->GetInfo()->ID );
	return id;
}

/// RegisterAIUpdateEvent
///   Registers periodical updates for the GameObject
///
/// Parameters:
///   interval   -  Update interval in milliseconds
///
/// Return value
///   No return value
///
/// Example:
///   go.RegisterAIUpdateEvent( 1000 )
///
static PyObject* ArcPyGameObject_RegisterAIUpdateEvent( ArcPyGameObject* self, PyObject* args )
{
	GameObject *gameObject = self->gameObjectPtr;

	uint32 interval;

	if( !PyArg_ParseTuple( args, "k", &interval ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires an interval parameter" );
		return NULL;
	}

	if( sEventMgr.HasEvent( gameObject, EVENT_SCRIPT_UPDATE_EVENT ) )
	{
		Py_RETURN_NONE;
	}

	sEventMgr.AddEvent( gameObject, &GameObject::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, interval, 0, 0 );

	Py_RETURN_NONE;
}


/// ModifyAIUpdateEvent
///   Modifies the AI update interval of the GameObject
///
/// Parameters:
///   interval   -  Update interval in milliseconds
///
/// Return value
///   No return value
///
/// Example:
///   go.ModifyAIUpdateEvent( 2500 )
///
static PyObject* ArcPyGameObject_ModifyAIUpdateEvent( ArcPyGameObject* self, PyObject* args )
{
	GameObject *gameObject = self->gameObjectPtr;

	uint32 interval;

	if( !PyArg_ParseTuple( args, "k", &interval ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires an interval parameter" );
		return NULL;
	}

	sEventMgr.ModifyEventTimeAndTimeLeft(gameObject, EVENT_SCRIPT_UPDATE_EVENT, interval);

	Py_RETURN_NONE;
}

/// RemoveAIUpdateEvent
///   Stops AI updates of the GameObject
///
/// Parameters:
///   None
///
/// Return value
///   No return value
///
/// Example:
///   go.RemoveAIUpdateEvent()
///
static PyObject* ArcPyGameObject_RemoveAIUpdateEvent( ArcPyGameObject* self, PyObject* args )
{
	GameObject *gameObject = self->gameObjectPtr;

	sEventMgr.RemoveEvents(gameObject, EVENT_SCRIPT_UPDATE_EVENT);

	Py_RETURN_NONE;
}

/// despawn
///   Despawns the GameObject
///
/// Parameters:
///   delay          -  Time interval until despawning (0 - immediately)
///   respawnTime    -  Time interval until respawning (0 - no respawn)
///
/// Return value
///   No return value
///
/// Example:
///   go.RemoveAIUpdateEvent()
///
static PyObject* ArcPyGameObject_despawn( ArcPyGameObject* self, PyObject* args )
{
	uint32 delay;
	uint32 respawnTime;

	if( !PyArg_ParseTuple( args, "kk", &delay, &respawnTime ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a delay, and a respawn time parameter" );
		return NULL;
	}

	GameObject *go = self->gameObjectPtr;
	go->Despawn( delay, respawnTime );

	Py_RETURN_NONE;
}

static PyMethodDef ArcPyGameObject_methods[] = 
{
	{ "getName", (PyCFunction)ArcPyGameObject_getName, METH_NOARGS, "Returns the name of the GameObject" },
	{ "getId", (PyCFunction)ArcPyGameObject_getId, METH_NOARGS, "Returns the Id of the GameObject" },
	{ "RegisterAIUpdateEvent", (PyCFunction)ArcPyGameObject_RegisterAIUpdateEvent, METH_VARARGS, "Registers AI Updates for the GameObject" },
	{ "ModifyAIUpdateEvent", (PyCFunction)ArcPyGameObject_ModifyAIUpdateEvent, METH_VARARGS, "Modifies the interval of the AI Updates of the GameObject" },
	{ "RemoveAIUpdateEvent", (PyCFunction)ArcPyGameObject_RemoveAIUpdateEvent, METH_VARARGS, "Stops AI Updates of the GameObject" },
	{ "despawn", (PyCFunction)ArcPyGameObject_despawn, METH_VARARGS, "Despawns the GameObject" },
	{NULL}
};

static PyTypeObject ArcPyGameObjectType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyGameObject",					// tp_name
	sizeof( ArcPyGameObject ),			// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyGameObject_dealloc,	// tp_dealloc
	0,								// tp_print
	0,								// tp_getattr
	0,								// tp_setattr
	0,								// tp_as_async
	0,								// tp_repr
	0,								// tp_as_number
	0,								// tp_as_sequence
	0,								// tp_as_mapping
	0,								// tp_hash
	0,								// tp_call
	0,								// tp_str
	0,								// tp_getattro
	0,								// tp_setattro
	0,								// tp_as_buffer
	Py_TPFLAGS_DEFAULT,				// tp_flags
	"Arcemu GameObject",				// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyGameObject_methods,			// tp_methods
	0,								// tp_members
	0,								// tp_getset
	&ArcPyObjectType,			    // tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyGameObject_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyGameObject_new,				// tp_new
};

int registerArcPyGameObject( PyObject *module )
{
	ArcPyGameObjectType.tp_new = ArcPyGameObject_new;

	if( PyType_Ready( &ArcPyGameObjectType ) < 0 )
	{
		return -1;
	}

	Py_INCREF( &ArcPyGameObjectType );
	PyModule_AddObject( module, "GameObject", (PyObject*)&ArcPyGameObjectType);

	return 0;
}

ArcPyGameObject* createArcPyGameObject( GameObject *go )
{
	PyTypeObject *type = &ArcPyGameObjectType;
	ArcPyGameObject* apgo = (ArcPyGameObject*)type->tp_alloc( type, 0 );
	apgo->gameObjectPtr = go;
	apgo->Object.objectPtr = go;
	return apgo;
}
