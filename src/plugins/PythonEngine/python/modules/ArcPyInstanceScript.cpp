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

#include "ArcPyInstanceScript.hpp"

static PyObject* ArcPyInstanceScript_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyInstanceScript *self = (ArcPyInstanceScript*)type->tp_alloc( type, 0 );
	
	return (PyObject*)self;
}

static int ArcPyInstanceScript_init( ArcPyInstanceScript *self, PyObject *args, PyObject *keywords )
{
	return 0;
}

static void ArcPyInstanceScript_dealloc( ArcPyInstanceScript* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}

////////////////////////////////////////// Event handler methods /////////////////////////////////////////////////////////////////////////////

static PyObject* ArcPyInstanceScript_OnPlayerDeath( ArcPyInstanceScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyInstanceScript_OnPlayerEnter( ArcPyInstanceScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyInstanceScript_OnAreaTrigger( ArcPyInstanceScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyInstanceScript_OnZoneChange( ArcPyInstanceScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyInstanceScript_OnCreatureDeath( ArcPyInstanceScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyInstanceScript_OnCreaturePush( ArcPyInstanceScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyInstanceScript_OnGOActivate( ArcPyInstanceScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyInstanceScript_OnGOPush( ArcPyInstanceScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyInstanceScript_OnLoad( ArcPyInstanceScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyInstanceScript_OnDestroy( ArcPyInstanceScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static PyMethodDef ArcPyInstanceScript_methods[] = 
{
	{ "OnPlayerDeath", (PyCFunction)ArcPyInstanceScript_OnPlayerDeath, METH_VARARGS, "" },
	{ "OnPlayerEnter", (PyCFunction)ArcPyInstanceScript_OnPlayerEnter, METH_VARARGS, "" },
	{ "OnAreaTrigger", (PyCFunction)ArcPyInstanceScript_OnAreaTrigger, METH_VARARGS, "" },
	{ "OnZoneChange", (PyCFunction)ArcPyInstanceScript_OnZoneChange, METH_VARARGS, "" },
	{ "OnCreatureDeath", (PyCFunction)ArcPyInstanceScript_OnCreatureDeath, METH_VARARGS, "" },
	{ "OnCreaturePush", (PyCFunction)ArcPyInstanceScript_OnCreaturePush, METH_VARARGS, "" },
	{ "OnGameObjectActivate", (PyCFunction)ArcPyInstanceScript_OnGOActivate, METH_VARARGS, "" },
	{ "OnGameObjectPush", (PyCFunction)ArcPyInstanceScript_OnGOPush, METH_VARARGS, "" },
	{ "OnLoad", (PyCFunction)ArcPyInstanceScript_OnLoad, METH_VARARGS, "" },
	{ "OnDestroy", (PyCFunction)ArcPyInstanceScript_OnDestroy, METH_VARARGS, "" },
	{NULL}
};

PyTypeObject ArcPyInstanceScriptType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyInstanceScript",		// tp_name
	sizeof( ArcPyInstanceScript ),// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyInstanceScript_dealloc,	// tp_dealloc
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
	"ArcPyInstanceScript",					// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyInstanceScript_methods,	// tp_methods
	0,                              // tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyInstanceScript_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyInstanceScript_new,		// tp_new
};

int registerArcPyInstanceScript( PyObject *module )
{
	if( PyType_Ready( &ArcPyInstanceScriptType ) < 0 )
	{
		return -1;
	}
	
	Py_INCREF( &ArcPyInstanceScriptType );
	PyModule_AddObject( module, "InstanceScript", (PyObject*)&ArcPyInstanceScriptType);

	return 0;
}
