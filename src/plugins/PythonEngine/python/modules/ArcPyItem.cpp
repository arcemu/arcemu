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

#include "ArcPyItem.hpp"

static PyObject* ArcPyItem_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyItem *self = (ArcPyItem*)type->tp_alloc( type, 0 );
	self->itemPtr = NULL;
	return (PyObject*)self;
}

static int ArcPyItem_init( ArcPyItem *self, PyObject *args, PyObject *keywords )
{
	self->itemPtr = NULL;
	return 0;
}

static void ArcPyItem_dealloc( ArcPyItem* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}


/// getName
///   Returns the name of this Item
///
/// Parameters
///   None
///
/// Return value
///   Returns a String that contains the name of this item.
///
/// Example
///   name = item.getName()
///
static PyObject* ArcPyItem_getName( ArcPyItem *self, PyObject *args )
{
	Item* item = self->itemPtr;
	PyObject *name = PyUnicode_FromString( item->GetProto()->Name1 );	
	return name;
}


/// getId
///   Returns the Id of this item
///
/// Parameters
///   None
///
/// Return value
///   Returns an integer that contains the Id of this Item
///
/// Example
///   id = item.getId()
///
static PyObject* ArcPyItem_getId( ArcPyItem* self, PyObject* args )
{
	Item* item = self->itemPtr;
	PyObject *id = PyLong_FromLong( item->GetProto()->ItemId );
	return id;
}

static PyMethodDef ArcPyItem_methods[] = 
{
	{ "getName", (PyCFunction)ArcPyItem_getName, METH_NOARGS, "Returns the name of the Item" },
	{ "getId", (PyCFunction)ArcPyItem_getId, METH_NOARGS, "Returns the Id of the Item" },
	{NULL}
};

static PyTypeObject ArcPyItemType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyItem",					// tp_name
	sizeof( ArcPyItem ),			// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyItem_dealloc,	// tp_dealloc
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
	"Arcemu Item",				// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyItem_methods,			// tp_methods
	0,								// tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyItem_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyItem_new,				// tp_new
};

int registerArcPyItem( PyObject *module )
{
	ArcPyItemType.tp_new = ArcPyItem_new;

	if( PyType_Ready( &ArcPyItemType ) < 0 )
	{
		return -1;
	}

	Py_INCREF( &ArcPyItemType );
	PyModule_AddObject( module, "Item", (PyObject*)&ArcPyItemType);

	return 0;
}

ArcPyItem* createArcPyItem()
{
	PyTypeObject *type = &ArcPyItemType;
	ArcPyItem* player = (ArcPyItem*)type->tp_alloc( type, 0 );
	return player;
}
