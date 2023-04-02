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

#include "ArcPyUnit.hpp"

static PyObject* ArcPyUnit_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyUnit *self = (ArcPyUnit*)type->tp_alloc( type, 0 );
	self->unitPtr = NULL;
	return (PyObject*)self;
}

static int ArcPyUnit_init( ArcPyUnit *self, PyObject *args, PyObject *keywords )
{
	self->unitPtr = NULL;
	return 0;
}

static void ArcPyUnit_dealloc( ArcPyUnit* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}


/// getName
///   Returns the name of this Unit
///
/// Parameters
///   None
///
/// Return value
///   Returns a String that contains the name of this Unit
///
/// Example
///   name = unit.getName()
///
static PyObject* ArcPyUnit_getName( ArcPyUnit *self, PyObject *args )
{
	PyObject *name = NULL;

	Unit *unit = self->unitPtr;
	if( unit->IsCreature() )
		name = PyUnicode_FromString( TO_CREATURE( unit )->GetCreatureInfo()->Name );	
	else
		name = PyUnicode_FromString( TO_PLAYER( unit )->GetName() );	
	return name;
}


/// sendChatMessage
///   Sends a chat message for this Unit (Creature)
///
/// Parameters
///   type        -  Chat message type (integer). See constants.
///   lang        -  The language of this chat message (integer). See constants.
///   msg         -  The message (string)
///
/// Return value
///   None
///
/// Example
///   unit.sendChatMessage( arcemu.CHAT_MSG_MONSTER_SAY, arcemu.LANG_UNIVERSAL, 'Hello, I am a creature' )
///
static PyObject* ArcPyUnit_sendChatMessage( ArcPyUnit *self, PyObject *args )
{
	unsigned long type = 0;
	unsigned long lang = 0;
	const char *msg = NULL;

	if( ! PyArg_ParseTuple( args, "kks", &type, &lang, &msg ) )
	{
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->SendChatMessage( (uint8)type, lang, msg );

	Py_RETURN_NONE;
}

/// RegisterAIUpdateEvent
///   Initiates regular updates for this Unit (Creature)
///
/// Parameters
///   period        -  The time that must pass between updates in milliseconds (integer)
///
/// Return value
///   None
///
/// Example
///   unit.RegisterAIUpdateEvent( 1000 )
///
static PyObject* ArcPyUnit_RegisterAIUpdateEvent( ArcPyUnit *self, PyObject *args )
{
	uint32 interval;

	if( !PyArg_ParseTuple( args, "k", &interval ) )
	{
		return NULL;
	}

	if( !self->unitPtr->IsCreature() )
	{
		PyErr_SetString( PyExc_TypeError, "This method can only be called on Creatures" );
		return NULL;
	}

	sEventMgr.AddEvent( TO_CREATURE( self->unitPtr ), &Creature::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, interval, 0, 0 );

	Py_RETURN_NONE;
}

/// ModifyAIUpdateEvent
///   Modifies the AI update interval of the Unit (Creature)
///
/// Parameters:
///   interval   -  Update interval in milliseconds
///
/// Return value
///   No return value
///
/// Example:
///   unit.ModifyAIUpdateEvent( 2500 )
///
static PyObject* ArcPyUnit_ModifyAIUpdateEvent( ArcPyUnit* self, PyObject* args )
{
	Unit *unit = self->unitPtr;;

	uint32 interval;

	if( !PyArg_ParseTuple( args, "k", &interval ) )
	{
		return NULL;
	}

	sEventMgr.ModifyEventTimeAndTimeLeft(unit, EVENT_SCRIPT_UPDATE_EVENT, interval);

	Py_RETURN_NONE;
}

/// RemoveAIUpdateEvent
///   Stops AI updates of the Unit ( Creature )
///
/// Parameters:
///   None
///
/// Return value
///   No return value
///
/// Example:
///   unit.RemoveAIUpdateEvent()
///
static PyObject* ArcPyUnit_RemoveAIUpdateEvent( ArcPyUnit* self, PyObject* args )
{
	Unit *unit = self->unitPtr;

	sEventMgr.RemoveEvents(unit, EVENT_SCRIPT_UPDATE_EVENT);

	Py_RETURN_NONE;
}

/// isOnVehicle
///   Tells if the Unit is on a vehicle
///
/// Parameters
///   None
///
/// Return value
///   Returns True if the Unit is on a vehicle. Returns False otherwise.
///
/// Example
///   if unit.isOnVehicle():
///       print( "The unit is on a vehicle" )
///
static PyObject* ArcPyUnit_isOnVehicle( ArcPyUnit *self, PyObject *args )
{
	Unit* ptr = self->unitPtr;
	if( ( ptr->GetCurrentVehicle() != NULL ) || ( ptr->IsPlayer() && ptr->IsVehicle() ) )
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}


/// dismissVehicle
///   Dismisses the vehicle that the Unit is on
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   unit.dismissVehicle()
///
static PyObject* ArcPyUnit_dismissVehicle( ArcPyUnit *self, PyObject *args )
{
	Unit* ptr = self->unitPtr;

	Vehicle *v = NULL;
	
	if( ptr->GetCurrentVehicle() != NULL )
		v = ptr->GetCurrentVehicle();
	else
	if( ptr->IsPlayer() && ( ptr->GetVehicleComponent() != NULL ) )
		v = ptr->GetVehicleComponent();
	
	if( v == NULL )
		Py_RETURN_NONE;
	
	v->EjectAllPassengers();
	Unit *o = v->GetOwner();
	
	if( o->IsPlayer() )
		o->RemoveAllAuraType( SPELL_AURA_MOUNTED );
	else
		o->Delete();

	Py_RETURN_NONE;
}

/// addVehiclePassenger
///   Spawns a creature and makes it enter the vehicle
///
/// Parameters
///   creatureId   -  The Id of the creature to spawn and add to the vehicle
///
/// Return value
///   None
///
/// Example
///   vehicle.addVehiclePassenger( 31111 )
///
static PyObject* ArcPyUnit_addVehiclePassenger( ArcPyUnit *self, PyObject *args )
{
	unsigned long creatureId;

	if( !PyArg_ParseTuple( args, "k", &creatureId ) )
	{
		PyErr_SetString( PyExc_ValueError, "The command requires a creatureId." );
		return NULL;
	}

	Unit *ptr = self->unitPtr;

	Vehicle *v = NULL;
	
	if( ptr->GetCurrentVehicle() != NULL )
		v = ptr->GetCurrentVehicle();
	else
	if( ptr->IsPlayer() && ( ptr->GetVehicleComponent() != NULL ) )
		v = ptr->GetVehicleComponent();
	
	if( v == NULL )
		Py_RETURN_NONE;
	
	if( !v->HasEmptySeat() )
		Py_RETURN_NONE;
	
	CreatureInfo  *ci = CreatureNameStorage.LookupEntry( creatureId );
	CreatureProto *cp = CreatureProtoStorage.LookupEntry( creatureId );
	
	if( ( ci == NULL ) || ( cp == NULL ) )
	{
		PyErr_SetString( PyExc_ValueError, "The command requires a creature that exists." );
		return NULL;
	}
	
	Unit *u = v->GetOwner();
	
	Creature *c = u->GetMapMgr()->CreateCreature( creatureId );
	c->Load( cp, u->GetPositionX(), u->GetPositionY(), u->GetPositionZ(), u->GetOrientation() );
	c->PushToWorld( u->GetMapMgr() );
	c->EnterVehicle( u->GetGUID(), 1 );

	Py_RETURN_NONE;
}

/// hasEmptyVehicleSeat
///   Tells if the vehicle has an empty seat
///
/// Parameters
///   None
///
/// Return value
///   Returns True if the Vehicle has an empty seat. Otherwise returns False.
///
/// Example
///   if vehicle.hasEmptyVehicleSeat():
///       print( "Vehicle has an empty seat" )
///
static PyObject* ArcPyUnit_hasEmptyVehicleSeat( ArcPyUnit *self, PyObject *args )
{
	Unit* ptr = self->unitPtr;

	Vehicle *v = NULL;

	if( ptr->GetCurrentVehicle() != NULL )
		v = ptr->GetCurrentVehicle();
	else
	if( ptr->IsPlayer() && ( ptr->GetVehicleComponent() != NULL ) )
		v = ptr->GetVehicleComponent();

	if( v == NULL )
	{
		PyErr_SetString( PyExc_ValueError, "The command requires a vehicle." );
		return NULL;
	}
	
	if( v->HasEmptySeat() )
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}


/// enterVehicle
///   Makes the Unit enter the vehicle after the specified delay
///
/// Parameters
///   guid    -   The GUID of the Vehicle (integer)
///   delay   -   The time to wait in milliseconds before entering the vehicle (integer)
///
/// Return value
///   None
///
/// Example
///   unit.enterVehicle( guid, 1000 )
///
static PyObject* ArcPyUnit_enterVehicle( ArcPyUnit *self, PyObject *args )
{
	uint64 guid;
	uint32 delay;

	if( !PyArg_ParseTuple( args, "Kk", &guid, &delay ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a guid and a delay parameter" );
		return NULL;
	}

	self->unitPtr->EnterVehicle( guid, delay );

	Py_RETURN_NONE;
}


/// exitVehicle
///   Makes the Unit exit it's current vehicle
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   unit.exitVehicle()
///
static PyObject* ArcPyUnit_exitVehicle( ArcPyUnit *self, PyObject *args )
{
	Unit* ptr = self->unitPtr;

	if( ptr->GetCurrentVehicle() != NULL )
		ptr->GetCurrentVehicle()->EjectPassenger( ptr );
	else
	if( ptr->IsPlayer() && ( ptr->GetVehicleComponent() != NULL ) )
		ptr->RemoveAllAuraType( SPELL_AURA_MOUNTED );

	Py_RETURN_NONE;
}

/// getVehicleBase
///   Returns the Vehicle of the Unit
///
/// Parameters
///   None
///
/// Return value
///   Returns the vehicle (unit) of the unit, or return None
///
/// Example
///   vehicle = unit.getVehicleBase()
///
static PyObject* ArcPyUnit_getVehicleBase( ArcPyUnit *self, PyObject *args )
{
	Unit *vehicleBase = self->unitPtr->GetVehicleBase();

	if( vehicleBase == NULL )
		Py_RETURN_NONE;
	else
	{
		ArcPyUnit *apu = createArcPyUnit();
		apu->unitPtr = vehicleBase;
		return (PyObject*)apu;
	}
}

/// setSpeeds
///   Sets the walk, run, and fly speeds of the Unit
///
/// Parameters
///   speed   -  The speed we'd like to set (float)
///
/// Return value
///   None
///
/// Example
///   unit.setSpeeds( 10.0 )
///
static PyObject* ArcPyUnit_setSpeeds( ArcPyUnit *self, PyObject *args )
{
	float speed;

	if( !PyArg_ParseTuple( args, "f", &speed ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a speed parameter" );
		return NULL;
	}

	Unit* ptr = self->unitPtr;

	ptr->SetSpeeds( WALK, speed );
	ptr->SetSpeeds( RUN, speed );
	ptr->SetSpeeds( FLY, speed );

	Py_RETURN_NONE;
}

/// getGuid
///   Returns the GUID of this Unit
///
/// Parameters
///   None
///
/// Return value
///   Returns an integer that contains the GUID of the Unit
///
/// Example
///   guid = unit.getGUID()
///
static PyObject* ArcPyUnit_getGUID( ArcPyUnit *self, PyObject *args )
{
	return PyLong_FromUnsignedLongLong( self->unitPtr->GetGUID() );
}


/// playSoundToSet
///   Plays a sound to nearby players
///
/// Parameters
///   soundId   -  The numeric Id of the sound clip to be played
///
/// Return value
///   None
///
/// Example
///   unit.playSoundToSet( 11803 )
///
static PyObject* ArcPyUnit_playSoundToSet( ArcPyUnit *self, PyObject *args )
{
	uint32 soundId;

	if( !PyArg_ParseTuple( args, "k", &soundId ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a sound Id parameter" );
		return NULL;
	}

	Unit* ptr = self->unitPtr;

	ptr->PlaySoundToSet( soundId );

	Py_RETURN_NONE;
}

/// setFaction
///   Sets the Unit's faction Id
///
/// Parameters
///   faction   -  The identifier of the faction to be set
///
/// Return value
///   None
///
/// Example
///   unit.setFaction( 168 ) # Unit will be hated by all
///
static PyObject* ArcPyUnit_setFaction( ArcPyUnit *self, PyObject *args )
{
	uint32 faction;

	if( !PyArg_ParseTuple( args, "k", &faction ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a faction Id parameter" );
		return NULL;
	}

	Unit* ptr = self->unitPtr;

	ptr->SetFaction( faction );

	Py_RETURN_NONE;
}

/// setUnitToFollow
///   Sets the unit which should be followed
///
/// Parameters
///   unit           -  The Unit that should be followed
///   followDistance -  The follow distance (optional, 2.0 is the default)
///   followAngle    -  The follow angle in radians (optional, 0.0 is the default)
///
/// Return value
///   None
///
/// Example
///   unit.setUnitToFollow( other )
///   unit.setUnitToFollow( other, 10.0 )
///   unit.setUnitToFollow( other, 10.0, 3.14 )
///
static PyObject* ArcPyUnit_setUnitToFollow( ArcPyUnit *self, PyObject *args )
{
	PyObject *o;
	float followDistance = 2.0f;
	float followAngle = 0.0f;

	if( !PyArg_ParseTuple( args, "O|ff", &o, &followDistance, &followAngle ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a Unit parameter" );
		return NULL;
	}

	if( strcmp( Py_TYPE( o )->tp_name, "ArcPyUnit" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a Unit parameter" );
		return NULL;
	}

	ArcPyUnit *otherUnit = reinterpret_cast< ArcPyUnit* >( o );
	Unit* other = otherUnit->unitPtr;

	Unit *unit = self->unitPtr;
	unit->GetAIInterface()->SetUnitToFollow( other );
	unit->GetAIInterface()->SetFollowDistance( followDistance );
	unit->GetAIInterface()->SetUnitToFollowAngle( followAngle );

	Py_RETURN_NONE;
}


/// stopFollowing
///   Stop following the currently followed Unit.
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   unit.stopFollowing()
///
static PyObject* ArcPyUnit_stopFollowing( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	unit->GetAIInterface()->SetUnitToFollow( (Unit*)NULL );

	Py_RETURN_NONE;
}

static PyMethodDef ArcPyUnit_methods[] = 
{
	{ "getName", (PyCFunction)ArcPyUnit_getName, METH_NOARGS, "Returns the name of the Unit" },
	{ "setSpeeds", (PyCFunction)ArcPyUnit_setSpeeds, METH_VARARGS, "Sets walk, run, and floy speeds of the Unit" },
	{ "getGUID", (PyCFunction)ArcPyUnit_getGUID, METH_NOARGS, "Returns the GUID of this Unit" },
	{ "sendChatMessage", (PyCFunction)ArcPyUnit_sendChatMessage, METH_VARARGS, "Sends a chat message from the Unit" },	
	{ "RegisterAIUpdateEvent", (PyCFunction)ArcPyUnit_RegisterAIUpdateEvent, METH_VARARGS, "Registers regular AI updates for the Unit" },
	{ "ModifyAIUpdateEvent", (PyCFunction)ArcPyUnit_ModifyAIUpdateEvent, METH_VARARGS, "Modifies the update interval of AI updates for the Unit" },
	{ "RemoveAIUpdateEvent", (PyCFunction)ArcPyUnit_RemoveAIUpdateEvent, METH_NOARGS, "Removes regular AI updates from the Unit" },
	{ "isOnVehicle", (PyCFunction)ArcPyUnit_isOnVehicle, METH_NOARGS, "Tells if the Unit is on a Vehicle" },
	{ "dismissVehicle", (PyCFunction)ArcPyUnit_dismissVehicle, METH_NOARGS, "Dismisses the Unit's vehicle" },
	{ "addVehiclePassenger", (PyCFunction)ArcPyUnit_addVehiclePassenger, METH_VARARGS, "Adds a passenger to the Vehicle" },
	{ "hasEmptyVehicleSeat", (PyCFunction)ArcPyUnit_hasEmptyVehicleSeat, METH_NOARGS, "Tells if the vehicle has an empty seat" },
	{ "enterVehicle", (PyCFunction)ArcPyUnit_enterVehicle, METH_VARARGS, "Makes the Unit enter a vehicle" },
	{ "exitVehicle", (PyCFunction)ArcPyUnit_exitVehicle, METH_NOARGS, "Makes the Unit exit a vehicle" },
	{ "getVehicleBase", (PyCFunction)ArcPyUnit_getVehicleBase, METH_NOARGS, "Returns the Vehicle the Unit is on" },
	{ "playSoundToSet", (PyCFunction)ArcPyUnit_playSoundToSet, METH_VARARGS, "Plays a sound to nearby players" },
	{ "setFaction", (PyCFunction)ArcPyUnit_setFaction, METH_VARARGS, "Sets the faction Id of the Unit" },

	{ "setUnitToFollow", (PyCFunction)ArcPyUnit_setUnitToFollow, METH_VARARGS, "Sets the Unit that this Unit will follow" },
	{ "stopFollowing", (PyCFunction)ArcPyUnit_stopFollowing, METH_NOARGS, "The Unit will stop following" },
	{NULL}
};

static PyTypeObject ArcPyUnitType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyUnit",					// tp_name
	sizeof( ArcPyUnit ),			// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyUnit_dealloc,	// tp_dealloc
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
	"Arcemu Unit",					// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyUnit_methods,				// tp_methods
	0,								// tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyUnit_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyUnit_new,					// tp_new
};

int registerArcPyUnit( PyObject *module )
{
	ArcPyUnitType.tp_new = ArcPyUnit_new;

	if( PyType_Ready( &ArcPyUnitType ) < 0 )
	{
		return -1;
	}
	
	Py_INCREF( &ArcPyUnitType );
	PyModule_AddObject( module, "Unit", (PyObject*)&ArcPyUnitType);

	return 0;
}

ArcPyUnit* createArcPyUnit()
{
	PyTypeObject *type = &ArcPyUnitType;
	ArcPyUnit* unit = (ArcPyUnit*)type->tp_alloc( type, 0 );
	return unit;
}

