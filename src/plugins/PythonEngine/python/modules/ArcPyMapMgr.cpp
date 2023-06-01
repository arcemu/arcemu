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
#include "ArcPyGameObject.hpp"

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

/// spawnGameObject
///   Spawns a GameObject on this map
///
/// Parameters
///   id    -   GameObject Id
///   x     -   X coordinate of the spawn point
///   y     -   X coordinate of the spawn point
///   z     -   X coordinate of the spawn point
///
/// Return value
///   Returns a reference to the spawned GameObject on success.
///   Returns None on failure.
///
/// Example
///   gameObject = mapMgr.spawGameObject( 68, 1.234, 2.345, 3.456 )
///
static PyObject* ArcPyMapMgr_spawnGameObject( ArcPyMapMgr *self, PyObject *args )
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

	GameObjectInfo *info = GameObjectNameStorage.LookupEntry( id );
	if( info == NULL )
	{
		Py_RETURN_NONE;
	}

	MapMgr *mapMgr = self->ptr;

	GameObject *go = mapMgr->CreateGameObject( id );
	go->CreateFromProto( id, mapMgr->GetMapId(), x, y, z, 0.0f );
	go->AddToWorld( mapMgr );

	return (PyObject*)createArcPyGameObject( go );
}


/// getObject
///   Looks up and returns an Object from this map by Guid
///
/// Parameters
///   guid    -   The Object's GUID
///
/// Return value
///   Returns a reference to the Object on success.
///   Returns None on failure.
///
/// Example
///   obj = mapMgr.getObject( guid )
///
static PyObject* ArcPyMapMgr_getObject( ArcPyMapMgr *self, PyObject *args )
{
	uint64 guid;

	if( !PyArg_ParseTuple( args, "K", &guid ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a guid parameter" );
		return NULL;
	}

	MapMgr *mapMgr = self->ptr;
	Object *object = mapMgr->GetObject( guid );

	if( object != NULL )
		return (PyObject*)createArcPyObject( object );
	else
		Py_RETURN_NONE;
}


/// getUnit
///   Looks up and returns a Unit from this map by Guid
///
/// Parameters
///   guid    -   The Unit's GUID
///
/// Return value
///   Returns a reference to the Unit on success.
///   Returns None on failure.
///
/// Example
///   unit = mapMgr.getUnit( guid )
///
static PyObject* ArcPyMapMgr_getUnit( ArcPyMapMgr *self, PyObject *args )
{
	uint64 guid;

	if( !PyArg_ParseTuple( args, "K", &guid ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a guid parameter" );
		return NULL;
	}

	MapMgr *mapMgr = self->ptr;
	Unit *unit = mapMgr->GetUnit( guid );

	if( unit != NULL )
		return (PyObject*)createArcPyUnit( unit );
	else
		Py_RETURN_NONE;
}

/// getWorldState
///  Retrieves the value of a WorldState field
///
/// Parameters
///   zone      -   The zone in which the worldstate should be updated
///   field     -   The worldstate field to update
///
/// Return value
///   Returns the value of the specified worldstate in the specified zone
///
/// Example
///   value = mapMgr.getWorldState( 1234, 2345 )
///
static PyObject* ArcPyMapMgr_getWorldState( ArcPyMapMgr *self, PyObject *args )
{
	uint32 zoneId;
	uint32 field;

	if( !PyArg_ParseTuple( args, "kk", &zoneId, &field ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires zone, field parameters" );
		return NULL;
	}

	MapMgr *mapMgr = self->ptr;
	WorldStatesHandler &worldStatesHandler = mapMgr->GetWorldStatesHandler();

	uint32 value = worldStatesHandler.GetWorldStateForZone( zoneId, field );

	return PyLong_FromUnsignedLong( value );
}

/// updateWorldState
///   Updates a WorldState field for a zone
///
/// Parameters
///   zone      -   The zone in which the worldstate should be updated
///   field     -   The worldstate field to update
///   value     -   The new value for the Worldstate field
///
/// Return value
///   None
///
/// Example
///   mapMgr.updateWorldState( 1234, 2345, 12 )
///
static PyObject* ArcPyMapMgr_updateWorldState( ArcPyMapMgr *self, PyObject *args )
{
	uint32 zoneId;
	uint32 field;
	uint32 value;

	if( !PyArg_ParseTuple( args, "kkk", &zoneId, &field, &value ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires zone, field, value parameters" );
		return NULL;
	}

	MapMgr *mapMgr = self->ptr;
	WorldStatesHandler &worldStatesHandler = mapMgr->GetWorldStatesHandler();
	worldStatesHandler.SetWorldStateForZone( zoneId, field, value );

	Py_RETURN_NONE;
}


/// getCreatureNearestCoords
///   Returns the Creature with the specified Id, that is nearest to the specified coordinates
///
/// Parameters
///   x      -   The X coordinate of the Creature
///   y      -   The Y coordinate of the Creature
///   z      -   The Z coordinate of the Creature
///   id     -   Id of the Creature
///
/// Return value
///   Returns the Creature if there's such a Creature near the coordinates specified.
///   Returns None on failure.
///
/// Example
///   creature = mapMgr.getCreatureNearestCoords( 1, 2, 3, 123 )
///
static PyObject* ArcPyMapMgr_getCreatureNearestCoords( ArcPyMapMgr *self, PyObject *args )
{
	float x;
	float y;
	float z;
	uint32 id;

	if( !PyArg_ParseTuple( args, "fffk", &x, &y, &z, &id ) ) 
	{
		PyErr_SetString( PyExc_ValueError, "This method requires x,y,z and entry parameters" );
		return NULL;
	}

	MapMgr *mapMgr = self->ptr;
	Creature *creature = mapMgr->GetInterface()->GetCreatureNearestCoords( x, y, z, id );

	if( creature == NULL )
	{
		Py_RETURN_NONE;
	}
	else
	{
		return (PyObject*)createArcPyCreature( creature );
	}
}

/// getGameObjectNearestCoords
///   Returns the GameObject with the specified Id, that is nearest to the specified coordinates
///
/// Parameters
///   x      -   The X coordinate of the Creature
///   y      -   The Y coordinate of the Creature
///   z      -   The Z coordinate of the Creature
///   id     -   Id of the GameObject
///
/// Return value
///   Returns the GameObject if there's such a GameObject near the coordinates specified.
///   Returns None on failure.
///
/// Example
///   go = mapMgr.getGameObjectNearestCoords( 1, 2, 3, 123 )
///
static PyObject* ArcPyMapMgr_getGameObjectNearestCoords( ArcPyMapMgr *self, PyObject *args )
{
	float x;
	float y;
	float z;
	uint32 id;

	if( !PyArg_ParseTuple( args, "fffk", &x, &y, &z, &id ) ) 
	{
		PyErr_SetString( PyExc_ValueError, "This method requires x,y,z and entry parameters" );
		return NULL;
	}

	MapMgr *mapMgr = self->ptr;
	GameObject *go = mapMgr->GetInterface()->GetGameObjectNearestCoords( x, y, z, id );

	if( go == NULL )
	{
		Py_RETURN_NONE;
	}
	else
	{
		return (PyObject*)createArcPyGameObject( go );
	}
}

static PyMethodDef ArcPyMapMgr_methods[] = 
{
	{ "getMapId", (PyCFunction)ArcPyMapMgr_getMapId, METH_NOARGS, "Returns the map Id of this MapMgr" },
	{ "spawnCreature", (PyCFunction)ArcPyMapMgr_spawnCreature, METH_VARARGS, "Spawns a Creature on this Map" },
	{ "spawnGameObject", (PyCFunction)ArcPyMapMgr_spawnGameObject, METH_VARARGS, "Spawns a GameObject on this Map" },
	{ "getObject", (PyCFunction)ArcPyMapMgr_getObject, METH_VARARGS, "Looks up and returns an Object from this map by Guid" },
	{ "getUnit", (PyCFunction)ArcPyMapMgr_getUnit, METH_VARARGS, "Looks up and returns a Unit from this map by Guid" },
	{ "getWorldState", (PyCFunction)ArcPyMapMgr_getWorldState, METH_VARARGS, "Returns the value of a worldstate field in a zone" },
	{ "updateWorldState", (PyCFunction)ArcPyMapMgr_updateWorldState, METH_VARARGS, "Update worldstate for a zone" },
	{ "getCreatureNearestCoords", (PyCFunction)ArcPyMapMgr_getCreatureNearestCoords, METH_VARARGS, "Returns the Creature with the specified Id, that is nearest to the specified coordinates" },
	{ "getGameObjectNearestCoords", (PyCFunction)ArcPyMapMgr_getGameObjectNearestCoords, METH_VARARGS, "Returns the GameObject with the specified Id, that is nearest to the specified coordinates" },
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
