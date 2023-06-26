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

#include "ArcPyObjectLocator.hpp"

#include "ArcPyObject.hpp"
#include "ArcPyUnit.hpp"
#include "ArcPyPlayer.hpp"

static PyObject* ArcPyObjectLocator_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyObjectLocator *self = (ArcPyObjectLocator*)type->tp_alloc( type, 0 );
	self->ptr = NULL;
	return (PyObject*)self;
}

static int ArcPyObjectLocator_init( ArcPyObjectLocator *self, PyObject *args, PyObject *keywords )
{
	self->ptr = NULL;

	PyObject* obj = NULL;

	if( !PyArg_ParseTuple( args, "O", &obj ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires an Object parameter" );
		return NULL;
	}

	if( !isArcPyObject( obj ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires an Object parameter" );
		return NULL;
	}
	
	Object *apo = ((ArcPyObject*)obj)->objectPtr;
	self->ptr = new ObjectLocator( apo );

	return 0;
}

static void ArcPyObjectLocator_dealloc( ArcPyObjectLocator* self )
{
	if( self->ptr != NULL )
	{
		delete self->ptr;
		self->ptr = NULL;
	}

	Py_TYPE( self )->tp_free( (PyObject*)self );
}



/// findClosestFriendly
///   Returns the closest friendly Unit or returns None
///
/// Parameters
///   None
///
/// Return value
///   Returns the closest friendly Unit or returns None
///
/// Example
///   u = locator.findClosestFriendly()
///

static PyObject* ArcPyObjectLocator_findClosestFriendly( ArcPyObjectLocator *self, PyObject *args )
{
	ObjectLocator *locator = self->ptr;
	Unit *unit = locator->findClosestFriendly();

	if( unit == NULL )
	{
		Py_RETURN_NONE;
	}

	return (PyObject*)createArcPyUnit( unit );
}


/// getRandomFriendly
///   Returns a random friendly Unit or returns None
///
/// Parameters
///   None
///
/// Return value
///   Returns a random friendly Unit or returns None
///
/// Example
///   u = locator.getRandomFriendly()
///

static PyObject* ArcPyObjectLocator_getRandomFriendly( ArcPyObjectLocator *self, PyObject *args )
{
	ObjectLocator *locator = self->ptr;
	Unit *unit = locator->getRandomFriendly();

	if( unit == NULL )
	{
		Py_RETURN_NONE;
	}

	return (PyObject*)createArcPyUnit( unit );
}


/// findClosestEnemy
///   Returns the closest enemy Unit or returns None
///
/// Parameters
///   None
///
/// Return value
///   Returns the closest enemy Unit or returns None
///
/// Example
///   u = locator.findClosestEnemy()
///

static PyObject* ArcPyObjectLocator_findClosestEnemy( ArcPyObjectLocator *self, PyObject *args )
{
	ObjectLocator *locator = self->ptr;
	Unit *unit = locator->findClosestEnemy();

	if( unit == NULL )
	{
		Py_RETURN_NONE;
	}

	return (PyObject*)createArcPyUnit( unit );
}

/// getRandomEnemy
///   Returns a random enemy Unit or returns None
///
/// Parameters
///   None
///
/// Return value
///   Returns a random enemy Unit or returns None
///
/// Example
///   u = locator.getRandomEnemy()
///

static PyObject* ArcPyObjectLocator_getRandomEnemy( ArcPyObjectLocator *self, PyObject *args )
{
	ObjectLocator *locator = self->ptr;
	Unit *unit = locator->getRandomEnemy();

	if( unit == NULL )
	{
		Py_RETURN_NONE;
	}

	return (PyObject*)createArcPyUnit( unit );
}


/// findClosestPlayer
///   Returns the closest Player or returns None
///
/// Parameters
///   None
///
/// Return value
///   Returns the closest Player or returns None
///
/// Example
///   p = locator.findClosestPlayer()
///

static PyObject* ArcPyObjectLocator_findClosestPlayer( ArcPyObjectLocator *self, PyObject *args )
{
	ObjectLocator *locator = self->ptr;
	Player *player = locator->findClosestPlayer();

	if( player == NULL )
	{
		Py_RETURN_NONE;
	}

	return (PyObject*)createArcPyPlayer( player );
}

static PyMethodDef ArcPyObjectLocator_methods[] = 
{
	{ "findClosestFriendly", (PyCFunction)ArcPyObjectLocator_findClosestFriendly, METH_NOARGS, "Returns the closest friendly Unit" },
	{ "getRandomFriendly", (PyCFunction)ArcPyObjectLocator_getRandomFriendly, METH_NOARGS, "Returns a random friendly Unit" },
	{ "findClosestEnemy", (PyCFunction)ArcPyObjectLocator_findClosestEnemy, METH_NOARGS, "Returns the closest enemy Unit" },
	{ "getRandomEnemy", (PyCFunction)ArcPyObjectLocator_getRandomEnemy, METH_NOARGS, "Returns a random enemy Unit" },
	{ "findClosestPlayer", (PyCFunction)ArcPyObjectLocator_findClosestPlayer, METH_NOARGS, "Returns the closest Player" },
	{ NULL }
};

PyTypeObject ArcPyObjectLocatorType = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	"ArcPyObjectLocator",					  // tp_name
	sizeof( ArcPyObjectLocator ),			  // tp_basicsize
	0,								          // tp_itemsize
	(destructor)ArcPyObjectLocator_dealloc,	  // tp_dealloc
	0,								          // tp_print
	0,								          // tp_getattr
	0,								          // tp_setattr
	0,								          // tp_as_async
	0,								          // tp_repr
	0,								          // tp_as_number
	0,								          // tp_as_sequence
	0,								          // tp_as_mapping
	0,								          // tp_hash
	0,								          // tp_call
	0,								          // tp_str
	0,								          // tp_getattro
	0,								          // tp_setattro
	0,								          // tp_as_buffer
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // tp_flags
	"Arcemu ObjectLocator",					  // tp_doc
	0,								          // tp_traverse
	0,								          // tp_clear
	0,								          // tp_richcompare
	0,								          // tp_weaklistoffset
	0,								          // tp_iter
	0,								          // tp_iternext
	ArcPyObjectLocator_methods,				  // tp_methods
	0,								          // tp_members
	0,								          // tp_getset
	0,								          // tp_base
	0,								          // tp_dict
	0,								          // tp_descr_get
	0,								          // tp_descr_set
	0,								          // tp_dictoffset
	(initproc)ArcPyObjectLocator_init,		  // tp_tp_init
	0,								          // tp_alloc
	ArcPyObjectLocator_new,					  // tp_new
};

int registerArcPyObjectLocator( PyObject *module )
{
	if( PyType_Ready( &ArcPyObjectLocatorType ) < 0 )
	{
		return -1;
	}
	
	Py_INCREF( &ArcPyObjectLocatorType );
	PyModule_AddObject( module, "ObjectLocator", (PyObject*)&ArcPyObjectLocatorType);

	return 0;
}

