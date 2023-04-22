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

#include "ArcPyUnit.hpp"
#include "ArcPyCreature.hpp"

static PyObject* ArcPyCreature_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyCreature *self = (ArcPyCreature*)type->tp_alloc( type, 0 );
	self->creaturePtr = NULL;
	self->unit.unitPtr = NULL;
	self->unit.Object.objectPtr = NULL;
	return (PyObject*)self;
}

static int ArcPyCreature_init( ArcPyCreature *self, PyObject *args, PyObject *keywords )
{
	self->creaturePtr = NULL;
	self->unit.unitPtr = NULL;
	self->unit.Object.objectPtr = NULL;
	return 0;
}

static void ArcPyCreature_dealloc( ArcPyCreature* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}


/// destroyCustomWayPoints
///   Destroys the custom waypoints of the Creature
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   creature.destroyCustomWayPoints()
///
static PyObject* ArcPyCreature_destroyCustomWaypoints( ArcPyCreature *self, PyObject *args )
{
	Creature *creature = self->creaturePtr;
	creature->DestroyCustomWaypointMap();

	Py_RETURN_NONE;
}


/// createCustomWaypoint
///   Creates and adds a custom waypoint to the Creature
///
/// Parameters
///   x       -  X coordinate
///   y       -  Y coordinate
///   z       -  Z coordinate
///   o       -  Orientation
///   wait    -  How long the Creature will wait before moving on
///   flags   -  The movement flags used for the movement between waypoints
///   model   -  The displayid that should be used at the waypoint. 0 means no change.
///
/// Return value
///   None
///
/// Example
///   creature.createWaypoint( 123.0, 12.34, 56.23, 2.39, 1000, 0, 0 )
///
static PyObject* ArcPyCreature_createCustomWaypoint( ArcPyCreature *self, PyObject *args )
{
	float x;
	float y;
	float z;
	float o;
	uint32 wait;
	uint32 flags;
	uint32 model;

	if( !PyArg_ParseTuple( args, "ffffkkk", &x, &y, &z, &o, &wait, &flags, &model ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires x,y,z,o,wait,flags,model parameters" );
		return NULL;
	}

	Creature *creature = self->creaturePtr;

	if( ! creature->hasCustomWayPoints() )
	{
		creature->setCustomWayPoints( new WayPointMap );
		creature->GetAIInterface()->SetWaypointMap( creature->getCustomWayPoints() );
	}

	WayPointMap *waypoints = creature->getCustomWayPoints();
	if( model == 0 )
		model = creature->GetDisplayId();

	WayPoint* wp = new WayPoint;
	wp->id = (uint32)waypoints->size();
	if( wp->id == 0 )
		wp->id = 1;

	wp->x = x;
	wp->y = y;
	wp->z = z;
	wp->o = o;
	wp->flags = flags;
	wp->backwardskinid = model;
	wp->forwardskinid = model;
	wp->backwardemoteid = wp->forwardemoteid = 0;
	wp->backwardemoteoneshot = wp->forwardemoteoneshot = false;
	wp->waittime = wait;

	if( !creature->GetAIInterface()->addWayPointUnsafe( wp ) )
	{
		std::stringstream ss;
		ss << "Failed to add waypoint " << wp->id <<  " to AIInterface";
		PyErr_SetString( PyExc_BaseException, ss.str().c_str() );
		delete wp;
	}

	Py_RETURN_NONE;
}


/// setMovementType
///   Sets the AI movement type of the Creature
///
/// Parameters
///   type       -  The movement type
///
/// Return value
///   None
///
/// Example
///   creature.setMovementType( arcemu.MOVEMENTTYPE_FORWARDTHENSTOP )
///
static PyObject* ArcPyCreature_setMovementType( ArcPyCreature *self, PyObject *args )
{
	uint32 type;
	if( !PyArg_ParseTuple( args, "k", &type ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a type parameter" );
		return NULL;
	}

	Creature *creature = self->creaturePtr;
	creature->GetAIInterface()->setMoveType( type );

	Py_RETURN_NONE;
}


/// resetWaypoint
///   Resets the current waypoint of the creature to 0
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   creature.resetWaypoint()
///
static PyObject* ArcPyCreature_resetWaypoint( ArcPyCreature *self, PyObject *args )
{
	Creature *creature = self->creaturePtr;
	creature->GetAIInterface()->setWaypointToMove( 0 );

	Py_RETURN_NONE;
}

/// setCanRegenerateHP
///   Sets whether the creature can regenerate it's HP.
///
/// Parameters
///   canRegenerate   -  0 means false, >0 means true
///
/// Return value
///   None
///
/// Example
///   creature.setCanRegenerateHP( 0 ) # Turns off HP regeneration
///
static PyObject* ArcPyCreature_setCanRegenerateHP( ArcPyCreature *self, PyObject *args )
{
	uint32 canRegenerate;
	if( !PyArg_ParseTuple( args, "k", &canRegenerate ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires an integer parameter" );
		return NULL;
	}

	Creature *creature = self->creaturePtr;
	if( canRegenerate > 0 )
		creature->setCanRegenerateHp( true );
	else
		creature->setCanRegenerateHp( false );

	Py_RETURN_NONE;
}

/// stopMovement
///   Make the Unit stop moving for the specified duration
///
/// Parameters
///   time   -  The duration the Unit should stop moving for
///
/// Return value
///   None
///
/// Example
///   creature.stopMovement( 2000 )
///
static PyObject* ArcPyCreature_stopMovement( ArcPyCreature *self, PyObject *args )
{
	uint32 time = 0;

	if( !PyArg_ParseTuple( args, "k", &time ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a time parameter" );
		return NULL;
	}

	Creature *creature = self->creaturePtr;
	creature->GetAIInterface()->StopMovement( time );

	Py_RETURN_NONE;
}

/// despawn
///   Removes the creature from the world
///
/// Parameters
///   delay         -   Milliseconds to wait before removing the creature
///   respawnTime   -  Milliseconds to wait before respawning
///
/// Return value
///   None
///
/// Example
///   creature.despawn( 1500, 5000 ) # Despawns in 1.5 s, and respawns in 5 s after.
///
static PyObject* ArcPyCreature_despawn( ArcPyCreature *self, PyObject *args )
{
	uint32 delay;
	uint32 respawnTime;

	if( !PyArg_ParseTuple( args, "kk", &delay, &respawnTime ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a delay and a respawntime parameter" );
		return NULL;
	}

	Creature *creature = self->creaturePtr;
	creature->Despawn( delay, respawnTime );

	Py_RETURN_NONE;
}

/// addNpcFlag
///   Adds the specified NPC flag(s)
///
/// Parameters
///   flag(s)   -  The flag(s) to be added to the Creature's NPC flags field
///
/// Return value
///   None
///
/// Example
///   creature.addNpcFlag( 2 )
///
static PyObject* ArcPyCreature_addNpcFlag( ArcPyCreature *self, PyObject *args )
{
	uint32 flag;

	if( !PyArg_ParseTuple( args, "k", &flag ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a flag as a parameter" );
		return NULL;
	}

	Creature *creature = self->creaturePtr;
	creature->SetFlag( UNIT_NPC_FLAGS, flag );

	Py_RETURN_NONE;
}

/// removeNpcFlag
///   Removes the specified NPC flag(s)
///
/// Parameters
///   flag(s)   -  The flag(s) to be removed from the Creature's NPC flags field
///
/// Return value
///   None
///
/// Example
///   creature.removeNpcFlag( 2 )
///
static PyObject* ArcPyCreature_removeNpcFlag( ArcPyCreature *self, PyObject *args )
{
	uint32 flag;

	if( !PyArg_ParseTuple( args, "k", &flag ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a flag as a parameter" );
		return NULL;
	}

	Creature *creature = self->creaturePtr;
	creature->RemoveFlag( UNIT_NPC_FLAGS, flag );

	Py_RETURN_NONE;
}


/// hasNpcFlag
///   Tells if the Creature has the specified NPC flags
///
/// Parameters
///   flag(s)   -  The flag(s) to be queried in the Creature's NPC flags field
///
/// Return value
///   None
///
/// Example
///   creature.hasNpcFlag( 2 )
///
static PyObject* ArcPyCreature_hasNpcFlag( ArcPyCreature *self, PyObject *args )
{
	uint32 flag;

	if( !PyArg_ParseTuple( args, "k", &flag ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a flag as a parameter" );
		return NULL;
	}

	Creature *creature = self->creaturePtr;
	if( creature->HasFlag( UNIT_NPC_FLAGS, flag ) )
	{
		Py_RETURN_TRUE;
	}
	else
	{
		Py_RETURN_FALSE;
	}
}

static PyMethodDef ArcPyCreature_methods[] = 
{
	{ "destroyCustomWaypoints", (PyCFunction)ArcPyCreature_destroyCustomWaypoints, METH_NOARGS, "Destroys the custom waypoints of the Creature" },
	{ "createCustomWaypoint", (PyCFunction)ArcPyCreature_createCustomWaypoint, METH_VARARGS, "Creates and adds a custom waypoint to the creature" },
	{ "setMovementType", (PyCFunction)ArcPyCreature_setMovementType, METH_VARARGS, "Sets the AI movement type of the creature" },
	{ "resetWaypoint", (PyCFunction)ArcPyCreature_resetWaypoint, METH_NOARGS, "Resets the current waypoint of the creature to 0" },
	{ "setCanRegenerateHP", (PyCFunction)ArcPyCreature_setCanRegenerateHP, METH_VARARGS, "Sets whether the creature can regenerate it's HP" },
	{ "stopMovement", (PyCFunction)ArcPyCreature_stopMovement, METH_VARARGS, "Stop moving for the specified duration" },
	{ "despawn", (PyCFunction)ArcPyCreature_despawn, METH_VARARGS, "Removes the creature from the world" },
	{ "addNpcFlag", (PyCFunction)ArcPyCreature_addNpcFlag, METH_VARARGS, "Adds the specified NPC flag(s) to the Creature" },
	{ "removeNpcFlag", (PyCFunction)ArcPyCreature_removeNpcFlag, METH_VARARGS, "Removes the specified NPC flag(s) from the Creature" },
	{ "hasNpcFlag", (PyCFunction)ArcPyCreature_hasNpcFlag, METH_VARARGS, "Tells if the Creature has the specified NPC flag(s)" },
	{NULL}
};

static PyTypeObject ArcPyCreatureType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	ARCPYCREATURE_TYPE_NAME,			// tp_name
	sizeof( ArcPyCreature ),			// tp_basicsize
	0,								    // tp_itemsize
	(destructor)ArcPyCreature_dealloc,	// tp_dealloc
	0,								    // tp_print
	0,								    // tp_getattr
	0,								    // tp_setattr
	0,								    // tp_as_async
	0,								    // tp_repr
	0,								    // tp_as_number
	0,								    // tp_as_sequence
	0,								    // tp_as_mapping
	0,								    // tp_hash
	0,								    // tp_call
	0,								    // tp_str
	0,								    // tp_getattro
	0,								    // tp_setattro
	0,								    // tp_as_buffer
	Py_TPFLAGS_DEFAULT,				    // tp_flags
	"Arcemu Creature",				    // tp_doc
	0,								    // tp_traverse
	0,								    // tp_clear
	0,								    // tp_richcompare
	0,								    // tp_weaklistoffset
	0,								    // tp_iter
	0,								    // tp_iternext
	ArcPyCreature_methods,			    // tp_methods
	0,								    // tp_members
	0,								    // tp_getset
	&ArcPyUnitType,					    // tp_base
	0,								    // tp_dict
	0,								    // tp_descr_get
	0,								    // tp_descr_set
	0,								    // tp_dictoffset
	(initproc)ArcPyCreature_init,	    // tp_tp_init
	0,								    // tp_alloc
	ArcPyCreature_new,				    // tp_new
};

static PyModuleDef ArcPyCreatureModule = {
	PyModuleDef_HEAD_INIT,
	"creature", NULL, -1, NULL,
	NULL, NULL, NULL, NULL
};

int registerArcPyCreature( PyObject *module )
{
	if( PyType_Ready( &ArcPyCreatureType ) < 0 )
	{
		return -1;
	}

	Py_INCREF( &ArcPyCreatureType );
	PyModule_AddObject( module, "Creature", (PyObject*)&ArcPyCreatureType);

	return 0;
}

ArcPyCreature* createArcPyCreature( Creature* c )
{
	PyTypeObject *type = &ArcPyCreatureType;
	
	ArcPyCreature* creature = (ArcPyCreature*)type->tp_alloc( type, 0 );
	creature->creaturePtr = c;
	creature->unit.unitPtr = c;
	creature->unit.Object.objectPtr = NULL;
	return creature;
}

bool isArcPyCreature( PyObject *obj )
{
	if( strcmp( Py_TYPE( obj )->tp_name, ARCPYCREATURE_TYPE_NAME ) == 0 )
	{
		return true;
	}

	return false;
}
