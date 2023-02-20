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

#include "ArcPyGuild.hpp"

static PyObject* ArcPyGuild_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyGuild *self = (ArcPyGuild*)type->tp_alloc( type, 0 );
	self->guildPtr = NULL;
	return (PyObject*)self;
}

static int ArcPyGuild_init( ArcPyGuild *self, PyObject *args, PyObject *keywords )
{
	self->guildPtr = NULL;
	return 0;
}

static void ArcPyGuild_dealloc( ArcPyGuild* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}


/// getGuildName
///   Returns the name of this Guild
///
/// Parameters
///   None
///
/// Return value
///   Returns a string that contains the name of this Guild
///
/// Example
///   name = g.getGuildName()
///
static PyObject* ArcPyGuild_getGuildName( ArcPyGuild *self, PyObject *args )
{
	Guild* guild = self->guildPtr;
	PyObject *name = PyUnicode_FromString(  guild->GetGuildName() );	
	return name;
}


/// getGuildId
///   Returns the Id of this Guild
///
/// Parameters
///   None
///
/// Return value
///   Returns an integer that contains the Id of this Guild
///
/// Example
///   id = guild.getGuildId()
///
static PyObject* ArcPyGuild_getGuildId( ArcPyGuild* self, PyObject* args )
{
	Guild* guild = self->guildPtr;
	PyObject *id = PyLong_FromLong( guild->GetGuildId() );
	return id;
}

static PyMethodDef ArcPyGuild_methods[] = 
{
	{ "getGuildName", (PyCFunction)ArcPyGuild_getGuildName, METH_NOARGS, "Returns the name of the guild" },
	{ "getGuildId", (PyCFunction)ArcPyGuild_getGuildId, METH_NOARGS, "Returns the Id of the guild" },
	{NULL}
};

static PyTypeObject ArcPyGuildType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyGuild",					// tp_name
	sizeof( ArcPyGuild ),			// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyGuild_dealloc,	// tp_dealloc
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
	"Arcemu Guild",				// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyGuild_methods,			// tp_methods
	0,								// tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyGuild_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyGuild_new,				// tp_new
};

int registerArcPyGuild( PyObject *module )
{
	ArcPyGuildType.tp_new = ArcPyGuild_new;

	if( PyType_Ready( &ArcPyGuildType ) < 0 )
	{
		return -1;
	}

	Py_INCREF( &ArcPyGuildType );
	PyModule_AddObject( module, "Guild", (PyObject*)&ArcPyGuildType);

	return 0;
}

ArcPyGuild* createArcPyGuild()
{
	PyTypeObject *type = &ArcPyGuildType;
	ArcPyGuild* player = (ArcPyGuild*)type->tp_alloc( type, 0 );
	return player;
}
