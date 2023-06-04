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

#include "ArcPyCreature.hpp"
#include "ArcPyUnit.hpp"
#include "ArcPyPlayer.hpp"
#include "ArcPyAura.hpp"

static PyObject* ArcPyUnit_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyUnit *self = (ArcPyUnit*)type->tp_alloc( type, 0 );
	self->unitPtr = NULL;
	self->Object.objectPtr = NULL;
	return (PyObject*)self;
}

static int ArcPyUnit_init( ArcPyUnit *self, PyObject *args, PyObject *keywords )
{
	self->unitPtr = NULL;
	self->Object.objectPtr = NULL;
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
///   delay       -  Optional delay before sending the chat message (integer)
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
	uint32 delay = 0;

	if( ! PyArg_ParseTuple( args, "kks|k", &type, &lang, &msg, &delay ) )
	{
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->SendChatMessage( (uint8)type, lang, msg, delay );

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
		ArcPyUnit *apu = createArcPyUnit(vehicleBase);
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

/// getFaction
///   Returns the Unit's faction Id
///
/// Parameters
///   None
///
/// Return value
///   Returns the Unit's faction Id
///
/// Example
///   unit.getFaction()
///
static PyObject* ArcPyUnit_getFaction( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	return PyLong_FromUnsignedLong( unit->GetFaction() );
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

	if( !isArcPyUnit( o ) )
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


/// setMount
///   Set the mount display Id of the Unit
///
/// Parameters
///   mountId   -   The mount display Id to set
///
/// Return value
///   None
///
/// Example
///   unit.setMount( 6080 )
///
static PyObject* ArcPyUnit_setMount( ArcPyUnit *self, PyObject *args )
{
	uint32 mountId;

	if( !PyArg_ParseTuple( args, "k", &mountId ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a mount Id parameter" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->SetMount( mountId );

	Py_RETURN_NONE;
}

/// equipWeapons
///   Equips weapons on the Unit
///
/// Parameters
///   meleeWeaponId    -   The melee weapon's id
///   offhandWeaponId  -   The offhand weapon's id (optional)
///   rangedWeaponId   -   The ranged weapon's id (optional)
///
/// Return value
///   None
///
/// Example
///   unit.equipWeapons( 25 )
///
static PyObject* ArcPyUnit_equipWeapons( ArcPyUnit *self, PyObject *args )
{
	uint32 meleeWeaponId;
	uint32 offhandWeaponId;
	uint32 rangedWeaponId;

	if( !PyArg_ParseTuple( args, "k|kk", &meleeWeaponId, &offhandWeaponId, &rangedWeaponId ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires at least a melee weapon Id parameter" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->SetEquippedItem( MELEE, meleeWeaponId );
	unit->SetEquippedItem( OFFHAND, offhandWeaponId );
	unit->SetEquippedItem( RANGED, rangedWeaponId );

	Py_RETURN_NONE;
}

/// setSheatState
///   Sets whether the equipped weapon is holstered or is in the hand(s) of the Unit
///
/// Parameters
///   sheatState    -   The new sheat state to set
///
/// Return value
///   None
///
/// Example
///   unit.setSheatState( 0 ) # Weapon will be in holster
///   unit.setSheatState( 1 ) # Weapon will be in hand(s)
///
static PyObject* ArcPyUnit_setSheatState( ArcPyUnit *self, PyObject *args )
{
	uint32 sheatState = 0;

	if( !PyArg_ParseTuple( args, "k", &sheatState ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a sheat state parameter ( 0 or 1 )" );
		return NULL;
	}

	if( sheatState > 1 )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a sheat state parameter ( 0 or 1 )" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->SetByte( UNIT_FIELD_BYTES_2, 0, sheatState );

	Py_RETURN_NONE;
}

/// setPvPFlag
///   Flags the Unit for PvP
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   unit.setPvPFlag()
///
static PyObject* ArcPyUnit_setPvPFlag( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	unit->SetPvPFlag();

	Py_RETURN_NONE;
}


/// removePvPFlag
///   Unflags the Unit for PvP
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   unit.removePvPFlag()
///
static PyObject* ArcPyUnit_removePvPFlag( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	unit->RemovePvPFlag();

	Py_RETURN_NONE;
}


/// isPvPFlagged
///   Tells if the Unit is flagged for PvP
///
/// Parameters
///   None
///
/// Return value
///   Returns True if the Unit is PvP flagged.
///   Returns False otherwise
///
/// Example
///   if unit.isPvPFlagged():
///        print( "Unit is flagged for PvP" )
///
static PyObject* ArcPyUnit_isPvPFlagged( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;

	if( unit->IsPvPFlagged() )
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}

/// hasAura( spellId )
///   Tells if the Unit has an Aura with the specified spellId
///
/// Parameters
///   spellId    -    The spellId of the Aura
///
/// Return value
///   Returns True if the Unit has such an Aura.
///   Returns False otherwise.
///
/// Example
///   if unit.hasAura( 1243 ):
///        print( "Unit has Aura 1243" )
///
static PyObject* ArcPyUnit_hasAura( ArcPyUnit *self, PyObject *args )
{
	uint32 spellId;

	if( !PyArg_ParseTuple( args, "k", &spellId ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a spellId parameter" );
		return NULL;
	}

	Unit *unit = self->unitPtr;

	if( unit->HasAura( spellId ) )
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}


/// getAuraBySpellId( spellId )
///   Finds and returns the first Aura with the specified spell Id
///
/// Parameters
///   spellId    -    The spellId of the Aura
///
/// Return value
///   Returns an ArcPyAura object if the Unit has such an Aura.
///   Returns None otherwise.
///
/// Example
///   aura = unit.findAuraBySpellId( 1243 ):
///
static PyObject* ArcPyUnit_getAuraBySpellId( ArcPyUnit *self, PyObject *args )
{
	uint32 spellId;

	if( !PyArg_ParseTuple( args, "k", &spellId ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a spellId parameter" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	Aura *aura = unit->FindAura( spellId );

	if( aura == NULL )
		Py_RETURN_NONE;

	
	ArcPyAura *apa = createArcPyAura();
	apa->auraPtr = aura;
	Py_INCREF( apa );
	
	return (PyObject*)apa;
}


/// getStandState
///   Returns the current standstate of the Unit
///
/// Parameters
///   None
///
/// Return value
///   Returns the current standstate of the Unit
///
/// Example
///   s = unit.getStandState()
///
static PyObject* ArcPyUnit_getStandState( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	PyObject *state = PyLong_FromLong( unit->GetStandState() );
	return state;
}


/// setStandState
///   Sets the standstate of the Unit
///
/// Parameters
///   standstate   -   The standstate to be set
///
/// Return value
///   None
///
/// Example
///   unit.setStandState( STANDSTATE_SIT )
///
static PyObject* ArcPyUnit_setStandState( ArcPyUnit *self, PyObject *args )
{
	uint32 state;

	if( !PyArg_ParseTuple( args, "k", &state ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a standstate parameter" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->SetStandState( static_cast< uint8 >( state ) );

	Py_RETURN_NONE;
}


/// getMaxHealth
///   Returns the maximum health of the Unit
///
/// Parameters
///   None
///
/// Return value
///   Returns the maximum health of the Unit
///
/// Example
///   maxHealth = unit.getMaxHealth()
///
static PyObject* ArcPyUnit_getMaxHealth( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	PyObject *maxHealth = PyLong_FromUnsignedLong( unit->GetMaxHealth() );
	return maxHealth;
}


/// setMaxHealth
///   Sets the maximum health of the Unit
///
/// Parameters
///   maxHealth   -  The new maximum health of the Unit
///
/// Return value
///   None
///
/// Example
///   unit.setMaxHealth( 1337 )
///
static PyObject* ArcPyUnit_setMaxHealth( ArcPyUnit *self, PyObject *args )
{
	uint32 maxHealth;

	if( !PyArg_ParseTuple( args, "k", &maxHealth ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a maxHealth parameter" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->SetMaxHealth( maxHealth );

	Py_RETURN_NONE;
}


/// getHealth
///   Returns the current health of the Unit
///
/// Parameters
///   None
///
/// Return value
///   Returns the current health of the Unit
///
/// Example
///   health = unit.getHealth()
///
static PyObject* ArcPyUnit_getHealth( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	PyObject *health = PyLong_FromUnsignedLong( unit->GetHealth() );
	return health;
}


/// setHealth
///   Sets the current health of the Unit
///
/// Parameters
///   health   -  The new current health of the Unit
///
/// Return value
///   None
///
/// Example
///   unit.setHealth( 1337 )
///
static PyObject* ArcPyUnit_setHealth( ArcPyUnit *self, PyObject *args )
{
	uint32 health;

	if( !PyArg_ParseTuple( args, "k", &health ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a health parameter" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->SetHealth( health );

	Py_RETURN_NONE;
}


/// isTagged
///   Tells if the Unit is tagged
///
/// Parameters
///   None
///
/// Return value
///   Returns True if the Unit is tagged.
///   Returns False otherwise.
///
/// Example
///   if unit.isTagged():
///       print( "tagged" )
///
static PyObject* ArcPyUnit_isTagged( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	if( unit->IsTagged() )
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}

/// getTaggerGuid
///   Returns the Unit's tagger's GUID
///
/// Parameters
///   None
///
/// Return value
///   Returns a GUID if the Unit is tagged
///   Returns 0 otherwise
///
/// Example
///   tagger = unit.getTaggerGuid()
///
static PyObject* ArcPyUnit_getTaggerGuid( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	uint64 tagger = unit->GetTaggerGUID();

	return PyLong_FromUnsignedLongLong( tagger );
}


/// tag
///   Tags the Unit with the specified GUID
///
/// Parameters
///   taggerGuid  -   The tagger's GUID
///
/// Return value
///   None
///
/// Example
///   unit.tag( guid )
///
static PyObject* ArcPyUnit_tag( ArcPyUnit *self, PyObject *args )
{
	uint64 guid;

	if( !PyArg_ParseTuple( args, "K", &guid ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a guid parameter" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->Tag( guid );

	Py_RETURN_NONE;
}


/// untag
///   Untags the Unit
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   unit.untag()
///
static PyObject* ArcPyUnit_untag( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	unit->UnTag();

	Py_RETURN_NONE;
}

/// isPlayer
///   Tells if this Unit is a Player
///
/// Parameters
///   None
///
/// Return value
///   Returns True if the Unit is a Player.
///   Returns False othersie
///
/// Example
///   if unit.isPlayer():
///     print( "Unit is a Player" )
///
static PyObject* ArcPyUnit_isPlayer( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	if( unit->IsPlayer() )
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}

/// toPlayer
///   Casts a Unit to a Player. Throws an error if the cast is not possible.
///
/// Parameters
///   None
///
/// Return value
///   Returns a Player object if the cast is possible.
///   Throws an error otherwise.
///
/// Example
///   player = unit.toPlayer()
///
static PyObject* ArcPyUnit_toPlayer( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	if( !unit->IsPlayer() )
	{
		PyErr_SetString( PyExc_TypeError, "This function requires a Unit that is a Player" );
		return NULL;
	}

	ArcPyPlayer *player = createArcPyPlayer( TO_PLAYER( unit ) );
	return (PyObject*)( player );
}

/// toCreature
///   Casts a Unit to a Creature. Throws an error if the cast is not possible.
///
/// Parameters
///   None
///
/// Return value
///   Returns a Creature object if the cast is possible.
///   Returns None otherwise.
///
/// Example
///   creature = unit.toCreature()
///
static PyObject* ArcPyUnit_toCreature( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	if( !unit->IsCreature() )
	{
		Py_RETURN_NONE;
	}

	ArcPyCreature *creature = createArcPyCreature( TO_CREATURE( unit ) );
	return (PyObject*)( creature );
}


/// emote
///   Make the Unit perform an emote
///
/// Parameters
///   emote   -  The emote to perform
///   time    -  Time to wait before the emote (optional)
///
/// Return value
///   None
///
/// Example
///   unit.emote( 2 )
///
static PyObject* ArcPyUnit_emote( ArcPyUnit *self, PyObject *args )
{
	uint32 emote;
	uint32 time = 0;

	if( !PyArg_ParseTuple( args, "k|k", &emote, &time ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires an emote parameter" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	if( time > 0 )
		unit->EventAddEmote( (EmoteType)emote, time );
	else
		unit->Emote( (EmoteType)emote );

	Py_RETURN_NONE;
}


/// setEmoteState()
///   Set the (permanent) emote state of the Unit
///
/// Parameters
///   emote   -  The emote state to set
///   delay   -  Delay before setting the emote state
///
/// Return value
///   None
///
/// Example
///   unit.setEmoteState( 2 )
///
static PyObject* ArcPyUnit_setEmoteState( ArcPyUnit *self, PyObject *args )
{
	uint32 emote;
	uint32 delay = 0;

	if( !PyArg_ParseTuple( args, "k|k", &emote, &delay ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires an emote parameter" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	if( delay > 0 )
	{
		sEventMgr.AddEvent( unit, &Unit::SetEmoteState, emote, EVENT_UNIT_EMOTE, delay, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
	}
	else
	{
		unit->SetEmoteState( emote );
	}

	Py_RETURN_NONE;
}

/// getEmoteState()
///   Returns the (permanent) emote state of the Unit
///
/// Parameters
///   None
///
/// Return value
///   Returns the emote state of the Unit
///
/// Example
///   emote = unit.getEmoteState()
///
static PyObject* ArcPyUnit_getEmoteState( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	return PyLong_FromUnsignedLong( unit->GetEmoteState() );
}

/// faceUnit
///   Make the Unit face another Unit
///
/// Parameters
///   target   -  The unit to face
///
/// Return value
///   None
///
/// Example
///   unit.faceUnit( target )
///
static PyObject* ArcPyUnit_faceUnit( ArcPyUnit *self, PyObject *args )
{
	PyObject *obj;

	if( !PyArg_ParseTuple( args, "O", &obj ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a Unit parameter" );
		return NULL;
	}

	if( strcmp( Py_TYPE( obj )->tp_name, "ArcPyUnit" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a Unit parameter" );
		return NULL;
	}

	ArcPyUnit *other = (ArcPyUnit*)obj;
	Unit *unit = self->unitPtr;
	Unit *target = other->unitPtr;

	unit->faceObject( target );

	Py_RETURN_NONE;
}

/// setFacing()
///   Set the Unit's orientation / facing
///
/// Parameters
///   facing   -  The orientation/facing to set
///
/// Return value
///   None
///
/// Example
///   unit.setFacing( 3.14 )
///
static PyObject* ArcPyUnit_setFacing( ArcPyUnit *self, PyObject *args )
{
	float facing;

	if( !PyArg_ParseTuple( args, "f", &facing ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a Unit parameter" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->SetFacing( facing );

	Py_RETURN_NONE;
}


/// castSpell
///   Makes the Unit cast a spell
///
/// Parameters
///   spellId   -  The spell's identfier
///   triggered -  Is this a triggered spell? (bool)
///   target    -  The unit the spell should be cast on (optional)
///
/// Return value
///   Returns the cast result as an integer
///
/// Example
///   unit.castSpell( 60424 )
///   unit.castSpell( 60424, False )
///   unit.castSpell( 2052, False, player )
///
static PyObject* ArcPyUnit_castSpell( ArcPyUnit *self, PyObject *args )
{
	uint32 spellId;
	int triggeredInt = 0;
	PyObject *target = NULL;

	if( !PyArg_ParseTuple( args, "k|pO", &spellId, &triggeredInt, &target ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a spellId" );
		return NULL;
	}

	bool triggered;
	if( triggeredInt != 0 )
		triggered = true;
	else
		triggered = false;

	Unit *targetUnit = NULL;

	if( target != NULL )
	{
		if( !isArcPyUnit( target ) )
		{
			PyErr_SetString( PyExc_TypeError, "This method requires an ArcPyUnit object to be passed if target is specified" );
			return NULL;
		}
		targetUnit = ((ArcPyUnit*)target)->unitPtr;
	}

	Unit *unit = self->unitPtr;
	if( targetUnit == NULL )
	{
		targetUnit = unit;
	}

	uint8 result = unit->CastSpell( targetUnit, spellId, triggered );
	return PyLong_FromUnsignedLong( result );
}


/// getDisplayId
///   Returns the display Id of the Unit
///
/// Parameters
///   None
///
/// Return value
///   Returns the display Id of the Unit
///
/// Example
///   displayId = unit.getDisplayId()
///
static PyObject* ArcPyUnit_getDisplayId( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	return PyLong_FromUnsignedLong( unit->GetDisplayId() );
}

/// setDisplayId
///   Sets the display Id of the Unit
///
/// Parameters
///   displayId  -  The display Id to be set for the Unit
///
/// Return value
///   None
///
/// Example
///   unit.setDisplayId( 1234 )
///
static PyObject* ArcPyUnit_setDisplayId( ArcPyUnit *self, PyObject *args )
{
	uint32 displayId;

	if( !PyArg_ParseTuple( args, "k", &displayId ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a display Id" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->SetDisplayId( displayId );

	Py_RETURN_NONE;
}

/// getNativeDisplayId
///   Returns the native display Id of the Unit
///
/// Parameters
///   None
///
/// Return value
///   Returns the native display Id of the Unit
///
/// Example
///   displayId = unit.getNativeDisplayId()
///
static PyObject* ArcPyUnit_getNativeDisplayId( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	return PyLong_FromUnsignedLong( unit->GetNativeDisplayId() );
}

/// setNativeDisplayId
///   Sets the native display Id of the Unit
///
/// Parameters
///   displayId  -  The native display Id to be set for the Unit
///
/// Return value
///   None
///
/// Example
///   unit.setNativeDisplayId( 1234 )
///
static PyObject* ArcPyUnit_setNativeDisplayId( ArcPyUnit *self, PyObject *args )
{
	uint32 displayId;

	if( !PyArg_ParseTuple( args, "k", &displayId ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a display Id" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->SetNativeDisplayId( displayId );

	Py_RETURN_NONE;
}

/// kill
///   Kiss the Unit
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   unit.kill()
///
static PyObject* ArcPyUnit_kill( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	unit->DealDamage( unit, unit->GetMaxHealth(), 0, 0, 0 );

	Py_RETURN_NONE;
}

/// ejectAllPassengers
///   Ejects all vehicle passengers
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   unit.ejectAllPassengers()
///
static PyObject* ArcPyUnit_ejectAllPassengers( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	Vehicle *vehicle = unit->GetVehicleComponent();
	if( vehicle != NULL )
	{
		vehicle->EjectAllPassengers();
	}

	Py_RETURN_NONE;
}

/// knockBack
///   Knocks the Unit back
///
/// Parameters
///   horizontal - Horizontal knockback
///   vertical   - Vertical knockback
///
/// Return value
///   None
///
/// Example
///   unit.knockBack( 1.0, 2.0 )
///
static PyObject* ArcPyUnit_knockBack( ArcPyUnit *self, PyObject *args )
{
	float horizontal;
	float vertical;

	if( !PyArg_ParseTuple( args, "ff", &horizontal, &vertical ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a horizontal and a vertical knockback parameter" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->HandleKnockback( unit, horizontal, vertical );

	Py_RETURN_NONE;
}

/// getVehicleController
///   Retrieves the vehicle's controller
///
/// Parameters
///   None
///
/// Return value
///   Returns the vehicle's controller, if there's one.
///   Returns None otherwise
///
/// Example
///   controller = vehicle.getVehicleController()
///
static PyObject* ArcPyUnit_getVehicleController( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	Vehicle *vehicle = unit->GetVehicleComponent();
	if( vehicle != NULL )
	{
		Unit *controller = vehicle->getController();
		if( controller != NULL )
		{
			return (PyObject*)createArcPyUnit( controller );
		}
	}

	Py_RETURN_NONE;
}

/// root()
///   Roots the Unit (Makes it unable to move)
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   unit.root()
///
static PyObject* ArcPyUnit_root( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	unit->Root();

	Py_RETURN_NONE;
}

/// unroot()
///   Unroots the Unit (Makes it able to move again)
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   unit.unroot()
///
static PyObject* ArcPyUnit_unroot( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	unit->Unroot();

	Py_RETURN_NONE;
}


/// setTargetGUID()
///   Sets the target GUID for this Unit
///
/// Parameters
///   guid    -   The target's GUID
///
/// Return value
///   None
///
/// Example
///   unit.setTargetGUID( guid )
///
static PyObject* ArcPyUnit_setTargetGUID( ArcPyUnit *self, PyObject *args )
{
	uint64 guid;

	if( !PyArg_ParseTuple( args, "K", &guid ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a guid parameter" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->SetTargetGUID( guid );

	Py_RETURN_NONE;
}

/// getTargetGUID()
///   Retrieves the target GUID for this Unit
///
/// Parameters
///   None
///
/// Return value
///   Returns the target GUID
///
/// Example
///   target = unit.getTargetGUID()
///
static PyObject* ArcPyUnit_getTargetGUID( ArcPyUnit *self, PyObject *args )
{
	Unit *unit = self->unitPtr;
	return PyLong_FromUnsignedLongLong( unit->GetTargetGUID() );
}

/// setChannelSpellTargetGUID()
///   Sets the channel spell target GUID for this Unit
///
/// Parameters
///   guid    -   The target's GUID
///
/// Return value
///   None
///
/// Example
///   unit.setChannelSpellTargetGUID( guid )
///
static PyObject* ArcPyUnit_setChannelSpellTargetGUID( ArcPyUnit *self, PyObject *args )
{
	uint64 guid;

	if( !PyArg_ParseTuple( args, "K", &guid ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a guid parameter" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->SetChannelSpellTargetGUID( guid );

	Py_RETURN_NONE;
}

/// setChannelSpellId()
///   Sets the channel spell Id
///
/// Parameters
///   spellId    -   The channel spell Id
///
/// Return value
///   None
///
/// Example
///   unit.setChannelSpellId( 12345 )
///
static PyObject* ArcPyUnit_setChannelSpellId( ArcPyUnit *self, PyObject *args )
{
	uint32 spellId;

	if( !PyArg_ParseTuple( args, "k", &spellId ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a spellId parameter" );
		return NULL;
	}

	Unit *unit = self->unitPtr;
	unit->SetChannelSpellId( spellId );

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
	{ "ejectAllPassengers", (PyCFunction)ArcPyUnit_ejectAllPassengers, METH_NOARGS, "Ejects all passengers from the vehicle" },
	{ "addVehiclePassenger", (PyCFunction)ArcPyUnit_addVehiclePassenger, METH_VARARGS, "Adds a passenger to the Vehicle" },
	{ "hasEmptyVehicleSeat", (PyCFunction)ArcPyUnit_hasEmptyVehicleSeat, METH_NOARGS, "Tells if the vehicle has an empty seat" },
	{ "enterVehicle", (PyCFunction)ArcPyUnit_enterVehicle, METH_VARARGS, "Makes the Unit enter a vehicle" },
	{ "exitVehicle", (PyCFunction)ArcPyUnit_exitVehicle, METH_NOARGS, "Makes the Unit exit a vehicle" },
	{ "getVehicleBase", (PyCFunction)ArcPyUnit_getVehicleBase, METH_NOARGS, "Returns the Vehicle the Unit is on" },
	{ "getVehicleController", (PyCFunction)ArcPyUnit_getVehicleController, METH_NOARGS, "Returns the Vehicle's controller Unit" },
	{ "playSoundToSet", (PyCFunction)ArcPyUnit_playSoundToSet, METH_VARARGS, "Plays a sound to nearby players" },
	{ "setFaction", (PyCFunction)ArcPyUnit_setFaction, METH_VARARGS, "Sets the faction Id of the Unit" },
	{ "getFaction", (PyCFunction)ArcPyUnit_getFaction, METH_NOARGS, "Returns the faction Id of the Unit" },
	{ "setUnitToFollow", (PyCFunction)ArcPyUnit_setUnitToFollow, METH_VARARGS, "Sets the Unit that this Unit will follow" },
	{ "stopFollowing", (PyCFunction)ArcPyUnit_stopFollowing, METH_NOARGS, "The Unit will stop following" },
	{ "setMount", (PyCFunction)ArcPyUnit_setMount, METH_VARARGS, "Sets the mount display Id of the Unit" },
	{ "equipWeapons", (PyCFunction)ArcPyUnit_equipWeapons, METH_VARARGS, "Equips weapons on the Unit" },
	{ "setSheatState", (PyCFunction)ArcPyUnit_setSheatState, METH_VARARGS, "Sets wheter the Unit will have it's equipeed weapon in hand(s) or in holster" },
	{ "setPvPFlag", (PyCFunction)ArcPyUnit_setPvPFlag, METH_NOARGS, "Flags the Unit for PvP" },
	{ "removePvPFlag", (PyCFunction)ArcPyUnit_removePvPFlag, METH_NOARGS, "Unflags the Unit for PvP" },
	{ "isPvPFlagged", (PyCFunction)ArcPyUnit_isPvPFlagged, METH_NOARGS, "Tells if the Unit is flagged for PvP" },
	{ "hasAura", (PyCFunction)ArcPyUnit_hasAura, METH_VARARGS, "Tells if the Unit has an Aura with the specified spellId" },
	{ "getAuraBySpellId", (PyCFunction)ArcPyUnit_getAuraBySpellId, METH_VARARGS, "Finds and returns the first Aura with the specified spell Id" },
	{ "getStandState", (PyCFunction)ArcPyUnit_getStandState, METH_NOARGS, "Returns the current standstate of the Unit" },
	{ "setStandState", (PyCFunction)ArcPyUnit_setStandState, METH_VARARGS, "Sets the standstate of the Unit" },
	{ "getMaxHealth", (PyCFunction)ArcPyUnit_getMaxHealth, METH_NOARGS, "Returns the maximum health of the Unit" },
	{ "setMaxHealth", (PyCFunction)ArcPyUnit_setMaxHealth, METH_VARARGS, "Sets the maximum health of the Unit" },
	{ "getHealth", (PyCFunction)ArcPyUnit_getHealth, METH_NOARGS, "Returns the current health of the Unit" },
	{ "setHealth", (PyCFunction)ArcPyUnit_setHealth, METH_VARARGS, "Sets the current health of the Unit" },
	{ "isTagged", (PyCFunction)ArcPyUnit_isTagged, METH_NOARGS, "Tells if the Unit is tagged" },
	{ "getTaggerGuid", (PyCFunction)ArcPyUnit_getTaggerGuid, METH_NOARGS, "Returns the GUID of the tagger" },
	{ "tag", (PyCFunction)ArcPyUnit_tag, METH_VARARGS, "Tags the Unit with the specified GUID" },
	{ "untag", (PyCFunction)ArcPyUnit_untag, METH_NOARGS, "Untags the Unit" },
	{ "isPlayer", (PyCFunction)ArcPyUnit_isPlayer, METH_NOARGS, "Tells if the Unit is a Player" },
	{ "toPlayer", (PyCFunction)ArcPyUnit_toPlayer, METH_NOARGS, "Casts the Unit to a Player if possible" },
	{ "toCreature", (PyCFunction)ArcPyUnit_toCreature, METH_NOARGS, "Casts the Unit to a Creature if possible" },
	{ "emote", (PyCFunction)ArcPyUnit_emote, METH_VARARGS, "Make the Unit perform an emote" },
	{ "setEmoteState", (PyCFunction)ArcPyUnit_setEmoteState, METH_VARARGS, "Sets the Unit's emote state" },
	{ "getEmoteState", (PyCFunction)ArcPyUnit_getEmoteState, METH_NOARGS, "Return the Unit's emote state" },
	{ "faceUnit", (PyCFunction)ArcPyUnit_faceUnit, METH_VARARGS, "Make the Unit face another Unit" },
	{ "setFacing", (PyCFunction)ArcPyUnit_setFacing, METH_VARARGS, "Sets the Unit's orientation / facing" },
	{ "castSpell", (PyCFunction)ArcPyUnit_castSpell, METH_VARARGS, "Makes the Unit cast a spell" },
	{ "getDisplayId", (PyCFunction)ArcPyUnit_getDisplayId, METH_NOARGS, "Returns the display Id of the Unit" },
	{ "setDisplayId", (PyCFunction)ArcPyUnit_setDisplayId, METH_VARARGS, "Sets the display Id of the Unit" },
	{ "getNativeDisplayId", (PyCFunction)ArcPyUnit_getNativeDisplayId, METH_NOARGS, "Returns the native display Id of the Unit" },
	{ "setNativeDisplayId", (PyCFunction)ArcPyUnit_setNativeDisplayId, METH_VARARGS, "Sets the native display Id of the Unit" },
	{ "kill", (PyCFunction)ArcPyUnit_kill, METH_NOARGS, "Kills the Unit" },
	{ "knockBack", (PyCFunction)ArcPyUnit_knockBack, METH_VARARGS, "Knocks the Unit back" },
	{ "root", (PyCFunction)ArcPyUnit_root, METH_NOARGS, "Roots the Unit" },
	{ "unroot", (PyCFunction)ArcPyUnit_unroot, METH_NOARGS, "Unroots the Unit" },
	{ "setTargetGUID", (PyCFunction)ArcPyUnit_setTargetGUID, METH_VARARGS, "Sets the target GUID for this Unit" },
	{ "getTargetGUID", (PyCFunction)ArcPyUnit_getTargetGUID, METH_NOARGS, "Returns the target GUID for this Unit" },
	{ "setChannelSpellTargetGUID", (PyCFunction)ArcPyUnit_setChannelSpellTargetGUID, METH_VARARGS, "Sets the channel spell target GUID for this Unit" },
	{ "setChannelSpellId", (PyCFunction)ArcPyUnit_setChannelSpellId, METH_VARARGS, "Sets the channel spell Id" },
	{NULL}
};

PyTypeObject ArcPyUnitType = {
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
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,				// tp_flags
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
	&ArcPyObjectType,				// tp_base
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

ArcPyUnit* createArcPyUnit( Unit* u )
{
	PyTypeObject *type = &ArcPyUnitType;
	ArcPyUnit* unit = (ArcPyUnit*)type->tp_alloc( type, 0 );
	unit->unitPtr = u;
	unit->Object.objectPtr = u;
	return unit;
}

bool isArcPyUnit( PyObject *obj )
{
	const char *typeName = Py_TYPE( obj )->tp_name;

	if( strcmp( typeName, "ArcPyUnit"  ) == 0 ||
		strcmp( typeName, "ArcPyCreature" ) == 0 ||
		strcmp( typeName, "ArcPyPlayer" ) == 0 )
	{
		return true;
	}

	return false;
}
