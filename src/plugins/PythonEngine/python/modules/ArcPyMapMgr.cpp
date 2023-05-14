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

#include "ArcPyMapMgr.hpp"
#include "ArcPyCreature.hpp"

#include "StdAfx.h"

static PyObject* ArcPyMapMgr_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyMapMgr *self = (ArcPyMapMgr*)type->tp_alloc( type, 0 );
	self->ptr = NULL;
	return (PyObject*)self;
}

static int ArcPyMapMgr_init( ArcPyMapMgr *self, PyObject *args, PyObject *keywords )
{
	self->ptr = NULL;
	return 0;
}

static void ArcPyMapMgr_dealloc( ArcPyMapMgr* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}

/// getMapId
///   Returns the map Id of this MapMgr
///
/// Parameters
///   None
///
/// Return value
///   Returns the map Id of this MapMgr
///
/// Example
///   id = mapMgr.getMapId()
///
static PyObject* ArcPyMapMgr_getMapId( ArcPyMapMgr *self, PyObject *args )
{
	MapMgr *mapMgr = self->ptr;
	return PyLong_FromUnsignedLong( mapMgr->GetMapId() );
}

/// spawnCreature
///   Spawns a Creature on this map
///
/// Parameters
///   id    -   Creature Id
///   x     -   X coordinate of the spawn point
///   y     -   X coordinate of the spawn point
///   z     -   X coordinate of the spawn point
///
/// Return value
///   Returns a reference to the spawned Creature on success.
///   Returns None on failure.
///
/// Example
///   creature = mapMgr.spawnCreature( 68, 1.234, 2.345, 3.456 )
///
static PyObject* ArcPyMapMgr_spawnCreature( ArcPyMapMgr *self, PyObject *args )
{
	uint32 id;
	float x;
	float y;
	float z;

	if( !PyArg_ParseTuple( args, "kfff", &id, &x, &y, &z ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires id,x,y,z parameters" );
		return NULL;
	}

	CreatureInfo *info = CreatureNameStorage.LookupEntry( id );
	if( info == NULL )
	{
		Py_RETURN_NONE;
	}

	CreatureProto *proto = CreatureProtoStorage.LookupEntry( id );
	if( proto == NULL )
	{
		Py_RETURN_NONE;
	}
	
	MapMgr *mapMgr = self->ptr;
	Creature *creature = mapMgr->CreateCreature( id );
	if( creature == NULL )
		Py_RETURN_NONE;

	creature->Load( proto, x, y, z );
	creature->m_noRespawn = true;
	creature->AddToWorld( mapMgr );

	return (PyObject*)createArcPyCreature( creature );
}

static PyMethodDef ArcPyMapMgr_methods[] = 
{
	{ "getMapId", (PyCFunction)ArcPyMapMgr_getMapId, METH_NOARGS, "Returns the map Id of this MapMgr" },
	{ "spawnCreature", (PyCFunction)ArcPyMapMgr_spawnCreature, METH_VARARGS, "Spawns a Creature on this Map" },
	{NULL}
};

PyTypeObject ArcPyMapMgrType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyMapMgr",					// tp_name
	sizeof( ArcPyMapMgr ),			// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyMapMgr_dealloc,	// tp_dealloc
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
	"Arcemu MapMgr",					// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyMapMgr_methods,				// tp_methods
	0,								// tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyMapMgr_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyMapMgr_new,					// tp_new
};

int registerArcPyMapMgr( PyObject *module )
{
	if( PyType_Ready( &ArcPyMapMgrType ) < 0 )
	{
		return -1;
	}
	
	Py_INCREF( &ArcPyMapMgrType );
	PyModule_AddObject( module, "MapMgr", (PyObject*)&ArcPyMapMgrType);

	return 0;
}

ArcPyMapMgr* createArcPyMapMgr( MapMgr *mgr )
{
	PyTypeObject *type = &ArcPyMapMgrType;
	ArcPyMapMgr* mapMgr = (ArcPyMapMgr*)type->tp_alloc( type, 0 );
	mapMgr->ptr = mgr;
	return mapMgr;
}

bool isArcPyMapMgr( PyObject *obj )
{
	const char *typeName = Py_TYPE( obj )->tp_name;

	if( strcmp( typeName, "ArcPyMapMgr" ) == 0 )
	{
		return true;
	}

	return false;
}
