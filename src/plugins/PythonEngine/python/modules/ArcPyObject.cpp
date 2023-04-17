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

#include "ArcPyObject.hpp"

static PyObject* ArcPyObject_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyObject *self = (ArcPyObject*)type->tp_alloc( type, 0 );
	self->objectPtr = NULL;
	return (PyObject*)self;
}

static int ArcPyObject_init( ArcPyObject *self, PyObject *args, PyObject *keywords )
{
	self->objectPtr = NULL;
	return 0;
}

static void ArcPyObject_dealloc( ArcPyObject* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}


/// getPositionX
///   Returns the X component of the Object's position
///
/// Parameters
///   None
///
/// Return value
///   Returns the X component of the Object's position
///
/// Example
///   x = object.getPositionX()
///
static PyObject* ArcPyObject_getPositionX( ArcPyObject *self, PyObject *args )
{
	Object *object = self->objectPtr;
	return PyFloat_FromDouble( object->GetPositionX() );
}

/// getPositionY
///   Returns the Y component of the Object's position
///
/// Parameters
///   None
///
/// Return value
///   Returns the Y component of the Object's position
///
/// Example
///   y = object.getPositionY()
///
static PyObject* ArcPyObject_getPositionY( ArcPyObject *self, PyObject *args )
{
	Object *object = self->objectPtr;
	return PyFloat_FromDouble( object->GetPositionY() );
}


/// getPositionZ
///   Returns the Z component of the Object's position
///
/// Parameters
///   None
///
/// Return value
///   Returns the Z component of the Object's position
///
/// Example
///   z = object.getPositionZ()
///
static PyObject* ArcPyObject_getPositionZ( ArcPyObject *self, PyObject *args )
{
	Object *object = self->objectPtr;
	return PyFloat_FromDouble( object->GetPositionZ() );
}

static PyMethodDef ArcPyObject_methods[] = 
{
	{ "getPositionX", (PyCFunction)ArcPyObject_getPositionX, METH_VARARGS, "Returns the X coordinate of the Object" },
	{ "getPositionY", (PyCFunction)ArcPyObject_getPositionY, METH_VARARGS, "Returns the Y coordinate of the Object" },
	{ "getPositionZ", (PyCFunction)ArcPyObject_getPositionZ, METH_VARARGS, "Returns the Z coordinate of the Object" },
	{NULL}
};

PyTypeObject ArcPyObjectType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyObject",					// tp_name
	sizeof( ArcPyObject ),			// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyObject_dealloc,	// tp_dealloc
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
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,	// tp_flags
	"Arcemu Object",					// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyObject_methods,				// tp_methods
	0,								// tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyObject_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyObject_new,					// tp_new
};

int registerArcPyObject( PyObject *module )
{
	if( PyType_Ready( &ArcPyObjectType ) < 0 )
	{
		return -1;
	}
	
	Py_INCREF( &ArcPyObjectType );
	PyModule_AddObject( module, "Object", (PyObject*)&ArcPyObjectType);

	return 0;
}

ArcPyObject* createArcPyObject( Object *o )
{
	PyTypeObject *type = &ArcPyObjectType;
	ArcPyObject* object = (ArcPyObject*)type->tp_alloc( type, 0 );
	object->objectPtr = o;
	return object;
}

bool isArcPyObject( PyObject *obj )
{
	const char *typeName = Py_TYPE( obj )->tp_name;

	if( strcmp( typeName, "ArcPyObject" ) == 0 ||
		strcmp( typeName, "ArcPyUnit"  ) == 0 ||
		strcmp( typeName, "ArcPyPlayer" ) == 0 )
	{
		return true;
	}

	return false;
}
