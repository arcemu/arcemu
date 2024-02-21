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

#include "ArcPyGameObjectScript.hpp"
#include "ArcPyGameObject.hpp"

static PyObject* ArcPyGameObjectScript_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	PyObject *o = NULL;
	if( !PyArg_ParseTuple( args, "O", &o ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a GameObject parameter" );
		return NULL;
	}

	if( strcmp( Py_TYPE( o )->tp_name, "ArcPyGameObject" ) != 0 )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a GameObject parameter" );
		return NULL;
	}

	ArcPyGameObjectScript *self = (ArcPyGameObjectScript*)type->tp_alloc( type, 0 );
	Py_INCREF( o );
	self->gameobject = o;

	return (PyObject*)self;
}

static int ArcPyGameObjectScript_init( ArcPyGameObjectScript *self, PyObject *args, PyObject *keywords )
{
	PyObject *o = NULL;
	if( !PyArg_ParseTuple( args, "O", &o ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a GameObject parameter" );
		return -1;
	}

	if( strcmp( Py_TYPE( o )->tp_name, "ArcPyGameObject" ) != 0 )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a GameObject parameter" );
		return -1;
	}

	if( self->gameobject != NULL )
	{
		Py_DecRef( self->gameobject );
		self->gameobject = NULL;
	}

	Py_INCREF( o );
	self->gameobject = o;
	
	return 0;
}

static void ArcPyGameObjectScript_dealloc( ArcPyGameObjectScript* self )
{
	if( self->gameobject != NULL )
	{
		Py_DecRef( self->gameobject );
		self->gameobject = NULL;
	}

	Py_TYPE( self )->tp_free( (PyObject*)self );
}

////////////////////////////////////////// Event handler methods /////////////////////////////////////////////////////////////////////////////

static PyObject* ArcPyGameObjectScript_OnCreate( ArcPyGameObjectScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyGameObjectScript_OnSpawn( ArcPyGameObjectScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyGameObjectScript_OnDespawn( ArcPyGameObjectScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyGameObjectScript_OnLootTaken( ArcPyGameObjectScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyGameObjectScript_OnActivate( ArcPyGameObjectScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyGameObjectScript_OnDamaged( ArcPyGameObjectScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyGameObjectScript_OnDestroyed( ArcPyGameObjectScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyGameObjectScript_AIUpdate( ArcPyGameObjectScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static PyMethodDef ArcPyGameObjectScript_methods[] = 
{
	{ "OnCreate", (PyCFunction)ArcPyGameObjectScript_OnCreate, METH_NOARGS, "" },
	{ "OnSpawn", (PyCFunction)ArcPyGameObjectScript_OnSpawn, METH_NOARGS, "" },
	{ "OnDespawn", (PyCFunction)ArcPyGameObjectScript_OnDespawn, METH_NOARGS, "" },
	{ "OnLootTaken", (PyCFunction)ArcPyGameObjectScript_OnLootTaken, METH_VARARGS, "" },
	{ "OnActivate", (PyCFunction)ArcPyGameObjectScript_OnActivate, METH_VARARGS, "" },
	{ "OnDamaged", (PyCFunction)ArcPyGameObjectScript_OnDamaged, METH_VARARGS, "" },
	{ "OnDestroyed", (PyCFunction)ArcPyGameObjectScript_OnDestroyed, METH_NOARGS, "" },
	{ "AIUpdate", (PyCFunction)ArcPyGameObjectScript_AIUpdate, METH_NOARGS, "" },
	{NULL}
};

static PyMemberDef ArcPyGameObjectScript_members[] =
{
	{ "gameobject", T_OBJECT_EX, offsetof( ArcPyGameObjectScript, gameobject ), 0, "Reference to the gameobject that is controlled by this script" },
	{ NULL }
};

PyTypeObject ArcPyGameObjectScriptType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyGameObjectScript",		// tp_name
	sizeof( ArcPyGameObjectScript ),// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyGameObjectScript_dealloc,	// tp_dealloc
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
	"ArcPyGameObjectScript",					// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyGameObjectScript_methods,	// tp_methods
	ArcPyGameObjectScript_members,	// tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyGameObjectScript_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyGameObjectScript_new,		// tp_new
};

int registerArcPyGameObjectScript( PyObject *module )
{
	if( PyType_Ready( &ArcPyGameObjectScriptType ) < 0 )
	{
		return -1;
	}
	
	Py_INCREF( &ArcPyGameObjectScriptType );
	PyModule_AddObject( module, "GameObjectScript", (PyObject*)&ArcPyGameObjectScriptType);

	return 0;
}
