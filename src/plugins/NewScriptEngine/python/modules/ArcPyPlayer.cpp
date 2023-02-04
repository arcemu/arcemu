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

#include "ArcPyPlayer.hpp"
#include "ArcPyUnit.hpp"

static PyObject* ArcPyPlayer_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyPlayer *self = (ArcPyPlayer*)type->tp_alloc( type, 0 );
	self->playerPtr = NULL;
	return (PyObject*)self;
}

static int ArcPyPlayer_init( ArcPyPlayer *self, PyObject *args, PyObject *keywords )
{
	self->playerPtr = NULL;
	return 0;
}

static void ArcPyPlayer_dealloc( ArcPyPlayer* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}

static PyObject* ArcPyPlayer_getName( ArcPyPlayer *self, PyObject *args )
{
	Player *player = self->playerPtr;
	PyObject *name = PyUnicode_FromString(  player->GetName() );	
	return name;
}

static PyObject* ArcPyPlayer_sendChatMessage( ArcPyPlayer *self, PyObject *args )
{
	unsigned long type = 0;
	unsigned long lang = 0;
	const char *msg = NULL;

	if( ! PyArg_ParseTuple( args, "kks", &type, &lang, &msg ) )
	{
		return NULL;
	}

	Player *player = self->playerPtr;
	player->SendChatMessage( (uint8)type, lang, msg );

	Py_RETURN_NONE;
}

static PyObject* ArcPyPlayer_toUnit( ArcPyPlayer *self, PyObject *args )
{
	ArcPyUnit *apu = createArcPyUnit();
	apu->unitPtr = self->playerPtr;
	return (PyObject*)apu;
}

static PyMethodDef ArcPyPlayer_methods[] = 
{
	{ "getName", (PyCFunction)ArcPyPlayer_getName, METH_NOARGS, "Returns the name of the Player" },
	{ "sendChatMessage", (PyCFunction)ArcPyPlayer_sendChatMessage, METH_VARARGS, "Sends a chat message from the Player" },
	{ "toUnit", (PyCFunction)ArcPyPlayer_toUnit, METH_NOARGS, "Returns the Player object as a Unit" },
	{NULL}
};

static PyTypeObject ArcPyPlayerType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyPlayer",					// tp_name
	sizeof( ArcPyPlayer ),			// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyPlayer_dealloc,	// tp_dealloc
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
	"Arcemu Player",				// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyPlayer_methods,			// tp_methods
	0,								// tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyPlayer_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyPlayer_new,				// tp_new
};

static PyModuleDef ArcPyPlayerModule = {
	PyModuleDef_HEAD_INIT,
	"player", NULL, -1, NULL,
	NULL, NULL, NULL, NULL
};

int registerArcPyPlayer( PyObject *module )
{
	ArcPyPlayerType.tp_new = ArcPyPlayer_new;

	if( PyType_Ready( &ArcPyPlayerType ) < 0 )
	{
		return -1;
	}

	Py_INCREF( &ArcPyPlayerType );
	PyModule_AddObject( module, "Player", (PyObject*)&ArcPyPlayerType);

	return 0;
}

ArcPyPlayer* createArcPyPlayer()
{
	PyTypeObject *type = &ArcPyPlayerType;
	ArcPyPlayer* player = (ArcPyPlayer*)type->tp_alloc( type, 0 );
	return player;
}
