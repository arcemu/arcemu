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

#include "ArcPyWorldSession.hpp"
#include "ArcPyCreature.hpp"

static PyObject* ArcPyWorldSession_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyWorldSession *self = (ArcPyWorldSession*)type->tp_alloc( type, 0 );
	self->worldSessionPtr = NULL;
	return (PyObject*)self;
}

static int ArcPyWorldSession_init( ArcPyWorldSession *self, PyObject *args, PyObject *keywords )
{
	self->worldSessionPtr = NULL;
	return 0;
}

static void ArcPyWorldSession_dealloc( ArcPyWorldSession* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}

/// getAccountName
///   Returns the account name of the user who initiated this WorldSession
///
/// Parameters
///   None
///
/// Return value
///   Returns a String that contains the account name of the player who initiated this WorldSession
///
/// Example
///   account = ws.getAccountName()
///
static PyObject* ArcPyWorldSession_getAccountName( ArcPyWorldSession *self, PyObject *args )
{
	WorldSession *session = self->worldSessionPtr;
	PyObject *name = PyUnicode_FromString(  session->GetAccountName().c_str() );	
	return name;
}

/// getAccountId
///   Returns the account Id of the user who initiated this WorldSession
///
/// Parameters
///   None
///
/// Return value
///   Returns an integer that contains the account Id of the player who initiated this WorldSession
///
/// Example
///   accountId = ws.getAccountId()
///
static PyObject* ArcPyWorldSession_getAccountId( ArcPyWorldSession* self, PyObject* args )
{
	WorldSession *session = self->worldSessionPtr;
	PyObject *id = PyLong_FromLong( session->GetAccountId() );
	return id;
}


/// sendInventoryList
///   Sends the vendor inventory of the specified Creature
///
/// Parameters
///   creature   -   The vendor npc
///
/// Return value
///   None
///
/// Example
///   session.sendInventoryList( npc )
///
static PyObject* ArcPyWorldSession_sendInventoryList( ArcPyWorldSession* self, PyObject* args )
{
	PyObject *po;

	if( !PyArg_ParseTuple( args, "O", &po ) )
	{
		PyErr_SetString( PyExc_ValueError, "The command requires a Creature parameter." );
		return NULL;
	}

	if( strcmp( Py_TYPE( po )->tp_name, ARCPYCREATURE_TYPE_NAME ) != 0 )
	{
		PyErr_SetString( PyExc_ValueError, "The command requires a Creature parameter." );
		return NULL;
	}

	ArcPyCreature *apc = (ArcPyCreature*)po;
	
	WorldSession *session = self->worldSessionPtr;
	session->SendInventoryList( apc->creaturePtr );

	Py_RETURN_NONE;
}

/// sendTrainerList
///   Sends the trainer spells of the specified Creature
///
/// Parameters
///   creature   -   The trainer npc
///
/// Return value
///   None
///
/// Example
///   session.sendTrainerList( npc )
///
static PyObject* ArcPyWorldSession_sendTrainerList( ArcPyWorldSession* self, PyObject* args )
{
	PyObject *po;

	if( !PyArg_ParseTuple( args, "O", &po ) )
	{
		PyErr_SetString( PyExc_ValueError, "The command requires a Creature parameter." );
		return NULL;
	}

	if( strcmp( Py_TYPE( po )->tp_name, ARCPYCREATURE_TYPE_NAME ) != 0 )
	{
		PyErr_SetString( PyExc_ValueError, "The command requires a Creature parameter." );
		return NULL;
	}

	ArcPyCreature *apc = (ArcPyCreature*)po;
	
	WorldSession *session = self->worldSessionPtr;
	session->SendTrainerList( apc->creaturePtr );

	Py_RETURN_NONE;
}

static PyMethodDef ArcPyWorldSession_methods[] = 
{
	{ "getAccountName", (PyCFunction)ArcPyWorldSession_getAccountName, METH_NOARGS, "Returns the name of the account" },
	{ "getAccountId", (PyCFunction)ArcPyWorldSession_getAccountId, METH_NOARGS, "Returns the Id of the account" },
	{ "sendInventoryList", (PyCFunction)ArcPyWorldSession_sendInventoryList, METH_VARARGS, "Sends a vendor's inventory list to the player" },
	{ "sendTrainerList", (PyCFunction)ArcPyWorldSession_sendTrainerList, METH_VARARGS, "Sends a trainer's spell list to the player" },
	{NULL}
};

static PyTypeObject ArcPyWorldSessionType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyWorldSession",					// tp_name
	sizeof( ArcPyWorldSession ),			// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyWorldSession_dealloc,	// tp_dealloc
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
	"Arcemu WorldSession",				// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyWorldSession_methods,			// tp_methods
	0,								// tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyWorldSession_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyWorldSession_new,				// tp_new
};

int registerArcPyWorldSession( PyObject *module )
{
	ArcPyWorldSessionType.tp_new = ArcPyWorldSession_new;

	if( PyType_Ready( &ArcPyWorldSessionType ) < 0 )
	{
		return -1;
	}

	Py_INCREF( &ArcPyWorldSessionType );
	PyModule_AddObject( module, "WorldSession", (PyObject*)&ArcPyWorldSessionType);

	return 0;
}

ArcPyWorldSession* createArcPyWorldSession( WorldSession *session )
{
	PyTypeObject *type = &ArcPyWorldSessionType;
	ArcPyWorldSession* apws = (ArcPyWorldSession*)type->tp_alloc( type, 0 );
	apws->worldSessionPtr = session;
	return apws;
}
