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

#include "ArcPyQuest.hpp"

static PyObject* ArcPyQuest_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyQuest* self = (ArcPyQuest*)type->tp_alloc( type, 0 );
	self->questPtr = NULL;
	return (PyObject*)self;
}

static int ArcPyQuest_init( ArcPyQuest *self, PyObject *args, PyObject *keywords )
{
	self->questPtr = NULL;
	return 0;
}

static void ArcPyQuest_dealloc( ArcPyQuest* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}

/// getTitle
///   Returns the title of this Quest
///
/// Parameters
///   None
///
/// Return value
///   Returns a string that contains the title of this Quest
///
/// Example
///   title = quest.getTitle()
///
static PyObject* ArcPyQuest_getTitle( ArcPyQuest *self, PyObject *args )
{
	Quest* quest = self->questPtr;
	PyObject *name = PyUnicode_FromString(  quest->title );	
	return name;
}


/// getId
///   Returns the Id of this Quest
///
/// Parameters
///   None
///
/// Return value
///   Returns an integer that contains the Id of this Quest
///
/// Example
///   id = quest.getId()
///
static PyObject* ArcPyQuest_getId( ArcPyQuest* self, PyObject* args )
{
	Quest* quest = self->questPtr;
	PyObject *id = PyLong_FromLong( quest->id );
	return id;
}

static PyMethodDef ArcPyQuest_methods[] = 
{
	{ "getTitle", (PyCFunction)ArcPyQuest_getTitle, METH_NOARGS, "Returns the title of the quest" },
	{ "getId", (PyCFunction)ArcPyQuest_getId, METH_NOARGS, "Returns the Id of the quest" },
	{NULL}
};

static PyTypeObject ArcPyQuestType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyQuest",					// tp_name
	sizeof( ArcPyQuest ),			// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyQuest_dealloc,	// tp_dealloc
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
	"Arcemu Quest",				// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyQuest_methods,			// tp_methods
	0,								// tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyQuest_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyQuest_new,				// tp_new
};

int registerArcPyQuest( PyObject *module )
{
	ArcPyQuestType.tp_new = ArcPyQuest_new;

	if( PyType_Ready( &ArcPyQuestType ) < 0 )
	{
		return -1;
	}

	Py_INCREF( &ArcPyQuestType );
	PyModule_AddObject( module, "Quest", (PyObject*)&ArcPyQuestType);

	return 0;
}

ArcPyQuest* createArcPyQuest()
{
	PyTypeObject *type = &ArcPyQuestType;
	ArcPyQuest* quest = (ArcPyQuest*)type->tp_alloc( type, 0 );
	return quest;
}
