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
	return (PyObject*)self;
}

static int ArcPyGameObject_init( ArcPyGameObject *self, PyObject *args, PyObject *keywords )
{
	self->gameObjectPtr = NULL;
	return 0;
}

static void ArcPyGameObject_dealloc( ArcPyGameObject* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}

static PyObject* ArcPyGameObject_getName( ArcPyGameObject *self, PyObject *args )
{
	GameObject* gameObject = self->gameObjectPtr;
	PyObject *name = PyUnicode_FromString(  gameObject->GetInfo()->Name );	
	return name;
}

static PyObject* ArcPyGameObject_getId( ArcPyGameObject* self, PyObject* args )
{
	GameObject *gameObject = self->gameObjectPtr;
	PyObject *id = PyLong_FromLong( gameObject->GetInfo()->ID );
	return id;
}

static PyObject* ArcPyGameObject_RegisterAIUpdateEvent( ArcPyGameObject* self, PyObject* args )
{
	GameObject *gameObject = self->gameObjectPtr;

	uint32 interval;

	if( !PyArg_ParseTuple( args, "k", &interval ) )
	{
		return NULL;
	}

	sEventMgr.AddEvent(gameObject, &GameObject::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, interval, 0, 0);

	Py_RETURN_NONE;
}

static PyMethodDef ArcPyGameObject_methods[] = 
{
	{ "getName", (PyCFunction)ArcPyGameObject_getName, METH_NOARGS, "Returns the name of the GameObject" },
	{ "getId", (PyCFunction)ArcPyGameObject_getId, METH_NOARGS, "Returns the Id of the GameObject" },
	{ "RegisterAIUpdateEvent", (PyCFunction)ArcPyGameObject_RegisterAIUpdateEvent, METH_VARARGS, "Registers AI Updates for the GameObject" },
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
	0,								// tp_base
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

ArcPyGameObject* createArcPyGameObject()
{
	PyTypeObject *type = &ArcPyGameObjectType;
	ArcPyGameObject* player = (ArcPyGameObject*)type->tp_alloc( type, 0 );
	return player;
}
