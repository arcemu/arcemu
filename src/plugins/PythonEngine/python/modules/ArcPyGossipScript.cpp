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

#include "Python.h"
#include "structmember.h"

#include "ArcPyGossipScript.hpp"

static PyObject* ArcPyGossipScript_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyGossipScript *self = (ArcPyGossipScript*)type->tp_alloc( type, 0 );
	
	return (PyObject*)self;
}

static int ArcPyGossipScript_init( ArcPyGossipScript *self, PyObject *args, PyObject *keywords )
{
	return 0;
}

static void ArcPyGossipScript_dealloc( ArcPyGossipScript* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}

////////////////////////////////////////// Event handler methods /////////////////////////////////////////////////////////////////////////////

static PyObject* ArcPyGossipScript_OnHello( ArcPyGossipScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyGossipScript_OnSelectOption( ArcPyGossipScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyGossipScript_OnEnd( ArcPyGossipScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static PyMethodDef ArcPyGossipScript_methods[] = 
{
	{ "OnHello", (PyCFunction)ArcPyGossipScript_OnHello, METH_VARARGS, "" },
	{ "OnSelectOption", (PyCFunction)ArcPyGossipScript_OnSelectOption, METH_VARARGS, "" },
	{ "OnEnd", (PyCFunction)ArcPyGossipScript_OnEnd, METH_VARARGS, "" },
	{NULL}
};

PyTypeObject ArcPyGossipScriptType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyGossipScript",		// tp_name
	sizeof( ArcPyGossipScript ),// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyGossipScript_dealloc,	// tp_dealloc
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
	"ArcPyGossipScript",					// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyGossipScript_methods,	// tp_methods
	0,                              // tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyGossipScript_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyGossipScript_new,		// tp_new
};

int registerArcPyGossipScript( PyObject *module )
{
	if( PyType_Ready( &ArcPyGossipScriptType ) < 0 )
	{
		return -1;
	}
	
	Py_INCREF( &ArcPyGossipScriptType );
	PyModule_AddObject( module, "GossipScript", (PyObject*)&ArcPyGossipScriptType);

	return 0;
}
