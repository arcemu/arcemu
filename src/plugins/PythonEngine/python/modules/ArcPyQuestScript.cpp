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

#include "ArcPyQuestScript.hpp"

static PyObject* ArcPyQuestScript_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyQuestScript *self = (ArcPyQuestScript*)type->tp_alloc( type, 0 );
	
	return (PyObject*)self;
}

static int ArcPyQuestScript_init( ArcPyQuestScript *self, PyObject *args, PyObject *keywords )
{
	return 0;
}

static void ArcPyQuestScript_dealloc( ArcPyQuestScript* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}

////////////////////////////////////////// Event handler methods /////////////////////////////////////////////////////////////////////////////

static PyObject* ArcPyQuestScript_OnQuestStart( ArcPyQuestScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyQuestScript_OnQuestComplete( ArcPyQuestScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyQuestScript_OnQuestCancel( ArcPyQuestScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyQuestScript_OnGameObjectActivate( ArcPyQuestScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyQuestScript_OnCreatureKill( ArcPyQuestScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyQuestScript_OnExploreArea( ArcPyQuestScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyQuestScript_OnPlayerItemPickup( ArcPyQuestScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static PyMethodDef ArcPyQuestScript_methods[] = 
{
	{ "OnQuestStart", (PyCFunction)ArcPyQuestScript_OnQuestStart, METH_VARARGS, "" },
	{ "OnQuestComplete", (PyCFunction)ArcPyQuestScript_OnQuestComplete, METH_VARARGS, "" },
	{ "OnQuestCancel", (PyCFunction)ArcPyQuestScript_OnQuestCancel, METH_VARARGS, "" },	
	{ "OnGameObjectActivate", (PyCFunction)ArcPyQuestScript_OnGameObjectActivate, METH_VARARGS, "" },
	{ "OnCreatureKill", (PyCFunction)ArcPyQuestScript_OnCreatureKill, METH_VARARGS, "" },
	{ "OnExploreArea", (PyCFunction)ArcPyQuestScript_OnExploreArea, METH_VARARGS, "" },
	{ "OnPlayerItemPickup", (PyCFunction)ArcPyQuestScript_OnPlayerItemPickup, METH_VARARGS, "" },
	{NULL}
};

PyTypeObject ArcPyQuestScriptType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyQuestScript",		// tp_name
	sizeof( ArcPyQuestScript ),// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyQuestScript_dealloc,	// tp_dealloc
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
	"ArcPyQuestScript",					// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyQuestScript_methods,	// tp_methods
	0,                              // tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyQuestScript_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyQuestScript_new,		// tp_new
};

int registerArcPyQuestScript( PyObject *module )
{
	if( PyType_Ready( &ArcPyQuestScriptType ) < 0 )
	{
		return -1;
	}
	
	Py_INCREF( &ArcPyQuestScriptType );
	PyModule_AddObject( module, "QuestScript", (PyObject*)&ArcPyQuestScriptType);

	return 0;
}
