/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2022 <http://www.ArcEmu.org/>
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

#include "unit_module.h"

static PyObject* ArcPyUnit_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyUnit *self = (ArcPyUnit*)type->tp_alloc( type, 0 );
	self->unitPtr = NULL;
	return (PyObject*)self;
}

static int ArcPyUnit_init( ArcPyUnit *self, PyObject *args, PyObject *keywords )
{
	self->unitPtr = NULL;
	return 0;
}

static void ArcPyUnit_dealloc( ArcPyUnit* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}

static PyObject* ArcPyUnit_getName( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	PyObject *name = PyUnicode_FromString( TO_CREATURE( unit )->GetCreatureInfo()->Name );	
	return name;
}

static PyObject* ArcPyUnit_sendChatMessage( ArcPyUnit *self, PyObject *args )
{
	unsigned long type = 0;
	unsigned long lang = 0;
	const char *msg = NULL;

	if( ! PyArg_ParseTuple( args, "kks", &type, &lang, &msg ) )
	{
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->SendChatMessage( (uint8)type, lang, msg );

	Py_RETURN_NONE;
}

static PyMethodDef ArcPyUnit_methods[] = 
{
	{ "getName", (PyCFunction)ArcPyUnit_getName, METH_NOARGS, "Returns the name of the Unit" },
	{ "sendChatMessage", (PyCFunction)ArcPyUnit_sendChatMessage, METH_VARARGS, "Sends a chat message from the Unit" },
	{NULL}
};

static PyTypeObject ArcPyUnitType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyUnit",					// tp_name
	sizeof( ArcPyUnit ),			// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyUnit_dealloc,	// tp_dealloc
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
	"Arcemu Unit",					// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyUnit_methods,				// tp_methods
	0,								// tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyUnit_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyUnit_new,					// tp_new
};

static PyModuleDef ArcPyUnitModule = {
	PyModuleDef_HEAD_INIT,
	"unit", NULL, -1, NULL,
	NULL, NULL, NULL, NULL
};

PyObject* PyInit_ArcPyUnit(void)
{
	PyObject *module = NULL;
	ArcPyUnitType.tp_new = ArcPyUnit_new;

	if( PyType_Ready( &ArcPyUnitType ) < 0 )
	{
		return NULL;
	}

	module = PyModule_Create( &ArcPyUnitModule );
	Py_INCREF( &ArcPyUnitType );
	PyModule_AddObject( module, "Unit", (PyObject*)&ArcPyUnitType);
	return module;
}

ArcPyUnit* createArcPyUnit()
{
	PyTypeObject *type = &ArcPyUnitType;
	ArcPyUnit* unit = (ArcPyUnit*)type->tp_alloc( type, 0 );
	return unit;
}

