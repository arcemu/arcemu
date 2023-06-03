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
#include "ArcPyPlayer.hpp"

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

	/// Don't allow scripts to specify 0 as a delay since we usually want to despawn on the next update
	if( delay == 0 )
		delay = 1;

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


/// addVendorItem
///   Adds an item to the NPC vendor inventory
///
/// Parameters
///   id            -  The Id of the item
///   amount        -  The stack amount of the item (optional)
///   extendedCost  -  The extended cost Id (token) (optional)
///
/// Return value
///   None
///
/// Example
///   creature.addVendorItem( 123 )
///   creature.addVendorItem( 123, 5 )
///   creature.addVendorItem( 123, 5, 45 )
///
static PyObject* ArcPyCreature_addVendorItem( ArcPyCreature *self, PyObject *args )
{
	uint32 id;
	uint32 amount = 1;
	uint32 extendedCostId = 0;

	if( !PyArg_ParseTuple( args, "k|kk", &id, &amount, &extendedCostId ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires an item Id as parameter" );
		return NULL;
	}

	Creature *creature = self->creaturePtr;

	if( !creature->hasVendorComponent() )
	{
		creature->createVendorComponent();
	}

	Vendor *vendor = creature->getVendor();	

	ItemExtendedCostEntry *ec = NULL;
	if( extendedCostId != 0 )
	{
		ec = dbcItemExtendedCost.LookupEntryForced( extendedCostId );
	}

	vendor->addItem( id, amount, ec );

	Py_RETURN_NONE;
}

/// removeVendorItem
///   Removes an item from the NPC vendor inventory
///
/// Parameters
///   id            -  The Id of the item
///
/// Return value
///   None
///
/// Example
///   creature.removeVendorItem( 123 )
///
static PyObject* ArcPyCreature_removeVendorItem( ArcPyCreature *self, PyObject *args )
{
	uint32 id;

	if( !PyArg_ParseTuple( args, "k", &id ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires an item Id as parameter" );
		return NULL;
	}

	Creature *creature = self->creaturePtr;

	if( !creature->hasVendorComponent() )
	{
		creature->createVendorComponent();
	}

	Vendor *vendor = creature->getVendor();
	vendor->removeItem( id );

	Py_RETURN_NONE;
}

/// removeVendorItems
///   Removes all items from the NPC vendor inventory
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   creature.removeVendorItems()
///
static PyObject* ArcPyCreature_removeVendorItems( ArcPyCreature *self, PyObject *args )
{
	Creature *creature = self->creaturePtr;

	if( !creature->hasVendorComponent() )
	{
		creature->createVendorComponent();
	}

	Vendor *vendor = creature->getVendor();
	vendor->removeAllItems();

	Py_RETURN_NONE;
}

/// getId()
///   Returns the identifier of this Creature
///
/// Parameters
///   None
///
/// Return value
///   Returns the identifier of this Creature
///
/// Example
///   id = creature.getId()
///
static PyObject* ArcPyCreature_getId( ArcPyCreature *self, PyObject *args )
{
	Creature *creature = self->creaturePtr;
	return PyLong_FromUnsignedLong( creature->GetProto()->Id );
}


/// getPlayerOwner()
///   Returns the Player owner of this Creature
///
/// Parameters
///   None
///
/// Return value
///   Returns the Player owner of this Creature
///
/// Example
///   playerOwner = creature.getPlayerOwner()
///
static PyObject* ArcPyCreature_getPlayerOwner( ArcPyCreature *self, PyObject *args )
{
	Creature *creature = self->creaturePtr;
	if( creature->GetPlayerOwner() == NULL )
		Py_RETURN_NONE;

	return (PyObject*)createArcPyPlayer( static_cast< Player* >( creature->GetPlayerOwner() ) );
}


/// moveTo()
///   Makes the Creature move to the designated coordinates
///
/// Parameters
///   x,y,z - coordinates
///   o     - orientation
///
/// Return value
///   None
///
/// Example
///   creature.moveTo( 1.234, 2.3456, 3.4567, 0.2345 )
///
static PyObject* ArcPyCreature_moveTo( ArcPyCreature *self, PyObject *args )
{
	float x;
	float y;
	float z;
	float o;

	if( !PyArg_ParseTuple( args, "ffff", &x, &y, &z, &o ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires x,y,z coordinates and an orientation as parameter" );
		return NULL;
	}

	Creature *creature = self->creaturePtr;
	creature->GetAIInterface()->MoveTo( x, y, z, o );

	Py_RETURN_NONE;
}

/// getMostHated()
///   Retrieves the Unit that has the most threat on this Creature
///
/// Parameters
///   None
///
/// Return value
///   Retrieves the Unit that has the most threat on this Creature.
///   Returns None if there's no such Unit
///
/// Example
///   tank = creature.getMostHated()
///
static PyObject* ArcPyCreature_getMostHated( ArcPyCreature *self, PyObject *args )
{
	Creature *creature = self->creaturePtr;
	Unit *unit = creature->GetAIInterface()->GetMostHated();

	if( unit == NULL )
		Py_RETURN_NONE;
	else
		return (PyObject*)createArcPyUnit( unit );
}


/// getSecondMostHated()
///   Retrieves the Unit that has the second most threat on this Creature
///
/// Parameters
///   None
///
/// Return value
///   Retrieves the Unit that has the second most threat on this Creature.
///   Returns None if there's no such Unit
///
/// Example
///   tank = creature.getMostHated()
///
static PyObject* ArcPyCreature_getSecondMostHated( ArcPyCreature *self, PyObject *args )
{
	Creature *creature = self->creaturePtr;
	Unit *unit = creature->GetAIInterface()->GetSecondHated();

	if( unit == NULL )
		Py_RETURN_NONE;
	else
		return (PyObject*)createArcPyUnit( unit );
}

/// wipeTargetList()
///   Removes all AI threat targets from the Creature
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   creature.wipeTargetList()
///
static PyObject* ArcPyCreature_wipeTargetList( ArcPyCreature *self, PyObject *args )
{
	Creature *creature = self->creaturePtr;
	creature->GetAIInterface()->WipeTargetList();

	Py_RETURN_NONE;
}


/// setNextTarget
///   Sets the AI attack target for the Creature
///
/// Parameters
///   target    -   A Unit that should be targeted for attacks. None if we'd like to unset the target.
///
/// Return value
///   None
///
/// Example
///   creature.setNextTarget( target )
///
static PyObject* ArcPyCreature_setNextTarget( ArcPyCreature *self, PyObject *args )
{
	PyObject *obj;
	
	if( !PyArg_ParseTuple( args, "O", &obj ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a Unit or a None parameter" );
		return NULL;
	}

	Creature *creature = self->creaturePtr;

	if( isArcPyUnit( obj ) )
	{
		creature->GetAIInterface()->setNextTarget( ((ArcPyUnit*)obj)->unitPtr );
	}
	else
	if( strcmp( Py_TYPE( obj )->tp_name, "NoneType" ) == 0 )
	{
		creature->GetAIInterface()->setNextTarget( (Unit*)NULL );
	}
	else
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a Unit or a None parameter" );
		return NULL;
	}

	Py_RETURN_NONE;
}


/// activate()
///   Activates the Creature's scripted behavior.
///
/// Parameters
///   activator   -   The Unit which has activated this Creature
///
/// Return value
///   None
///
/// Example
///   creature.activate( unit )
///
static PyObject* ArcPyCreature_activate( ArcPyCreature *self, PyObject *args )
{
	PyObject *obj;
	
	if( !PyArg_ParseTuple( args, "O", &obj ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a Unit parameter" );
		return NULL;
	}

	Creature *creature = self->creaturePtr;

	if( isArcPyUnit( obj ) )
	{
		creature->activate( ((ArcPyUnit*)obj)->unitPtr );
	}

	Py_RETURN_NONE;
}


/// deactivate()
///   Deactivates the Creature's scripted behavior.
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   creature.deactivate()
///
static PyObject* ArcPyCreature_deactivate( ArcPyCreature *self, PyObject *args )
{
	Creature *creature = self->creaturePtr;
	creature->deactivate();
	Py_RETURN_NONE;
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
	{ "addVendorItem", (PyCFunction)ArcPyCreature_addVendorItem, METH_VARARGS, "Adds an item to the NPC's vendor inventory" },
	{ "removeVendorItem", (PyCFunction)ArcPyCreature_removeVendorItem, METH_VARARGS, "Removes an item from the NPC's vendor inventory" },
	{ "removeVendorItems", (PyCFunction)ArcPyCreature_removeVendorItems, METH_NOARGS, "Removes all items from the NPC's vendor inventory" },
	{ "getId", (PyCFunction)ArcPyCreature_getId, METH_NOARGS, "Returns the identifier of this Creature" },
	{ "getPlayerOwner", (PyCFunction)ArcPyCreature_getPlayerOwner, METH_NOARGS, "Returns the Player owner of this Creature" },
	{ "moveTo", (PyCFunction)ArcPyCreature_moveTo, METH_VARARGS, "Makes the Creature move to the designated coordinates" },
	{ "getMostHated", (PyCFunction)ArcPyCreature_getMostHated, METH_NOARGS, "Retrieves the Unit that has the most threat on this Creature" },
	{ "getSecondMostHated", (PyCFunction)ArcPyCreature_getSecondMostHated, METH_NOARGS, "Retrieves the Unit that has the second most threat on this Creature" },
	{ "wipeTargetList", (PyCFunction)ArcPyCreature_wipeTargetList, METH_NOARGS, "Wipes the Creature's AI threat target list" },
	{ "setNextTarget", (PyCFunction)ArcPyCreature_setNextTarget, METH_VARARGS, "Sets the AI attack target for the Creature" },
	{ "activate", (PyCFunction)ArcPyCreature_activate, METH_VARARGS, "Activates the Creature's scripted behavior" },
	{ "deactivate", (PyCFunction)ArcPyCreature_deactivate, METH_NOARGS, "Deactivates the Creature's scripted behavior" },
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
	creature->unit.Object.objectPtr = c;
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
