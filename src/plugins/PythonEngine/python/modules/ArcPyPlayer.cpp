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
	self->unit.unitPtr = NULL;
	return (PyObject*)self;
}

static int ArcPyPlayer_init( ArcPyPlayer *self, PyObject *args, PyObject *keywords )
{
	self->playerPtr = NULL;
	self->unit.unitPtr = NULL;
	return 0;
}

static void ArcPyPlayer_dealloc( ArcPyPlayer* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}

/// getName
///   Returns the name of this Player
///
/// Parameters
///   None
///
/// Return value
///   Returns a string that contains the name of this Player
///
/// Example
///   name = player.getName()
///
static PyObject* ArcPyPlayer_getName( ArcPyPlayer *self, PyObject *args )
{
	Player *player = self->playerPtr;
	PyObject *name = PyUnicode_FromString(  player->GetName() );	
	return name;
}


/// sendChatMessage
///   Sends a chat message for this Player
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
///   player.sendChatMessage( arcemu.CHAT_MSG_SAY, arcemu.LANG_UNIVERSAL, "Well, hello there!" )
///
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

/// toUnit
///   Converts this Player to a Unit
///
/// Parameters
///   None
///
/// Return value
///   Returns a Unit object that contains a reference to this Player
///
/// Example
///   unit = player.toUnit()
///
static PyObject* ArcPyPlayer_toUnit( ArcPyPlayer *self, PyObject *args )
{
	ArcPyUnit *apu = createArcPyUnit( self->playerPtr );
	return (PyObject*)apu;
}

/// spawnAndEnterVehicle
///   Spawns a vehicle creature and makes the Player enter it after a delay
///
/// Parameters
///   creatureId      -  The Id of the creature that will serve as vehicle (integer)
///   delay           -  The number of milliseconds to wait before entering the vehicle (integer)
///
/// Return value
///   None
///
/// Example
///   player.spawnAndEnterVehicle( 28605, 1000 )
///
static PyObject* ArcPyPlayer_spawnAndEnterVehicle( ArcPyPlayer *self, PyObject *args )
{
	unsigned long creatureId;
	unsigned long delay;

	if( ! PyArg_ParseTuple( args, "kk", &creatureId, &delay ) )
	{
		PyErr_SetString( PyExc_ValueError, "The command requires a creatureId, and a delay." );
		return NULL;
	}

	Player *ptr = self->playerPtr;

	if( delay < 1 * 1000 )
		delay = 1 * 1000;
	
	if( creatureId == 0 )
	{
		PyErr_SetString( PyExc_ValueError, "The the creature Id must not be " );
		return NULL;
	}
	
	if( ( ptr->GetCurrentVehicle() != NULL ) && ( !ptr->IsPlayer() || !ptr->IsVehicle() ) )
		Py_RETURN_NONE;
	
	CreatureInfo *ci = CreatureNameStorage.LookupEntry( creatureId );
	if( ci == NULL )
	{
		PyErr_SetString( PyExc_ValueError, "The creature must exist" );
		return NULL;
	}
	
	CreatureProto *cp = CreatureProtoStorage.LookupEntry( creatureId );
	if( cp == NULL )
	{
		PyErr_SetString( PyExc_ValueError, "The creature must exist" );
		return NULL;
	}
	
	if( ( cp->vehicleid == 0 )  )
	{
		PyErr_SetString( PyExc_ValueError, "Creature must have vehicle data" );
		return NULL;
	}
	
	LocationVector v( ptr->GetPosition() );
	Creature *c = ptr->GetMapMgr()->CreateCreature( cp->Id );
	c->Load( cp, v.x, v.y, v.z, v.o );
	c->RemoveFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK );
	c->PushToWorld( ptr->GetMapMgr() );
	
	// Need to delay this a bit since first the client needs to see the vehicle
	ptr->EnterVehicle( c->GetGUID(), delay );

	Py_RETURN_NONE;
}


/// teleport
///   does this
///
/// Parameters
///   mapId    -   The map we would like to teleport to (integer)
///   x        -   X teleport destination coordinate (float)
///   y        -   Y teleport destination coordinate (float)
///   z        -   Z destination coordinate (float)
///   o        -   (optional) The destination orientation of the Player (float)
///
/// Return value
///   None
///
/// Example
///   player.teleport( 0, -9459.524414, 76.275986, 56.855736 )
///
static PyObject* ArcPyPlayer_teleport( ArcPyPlayer *self, PyObject *args )
{
	uint32 map;
	float x;
	float y;
	float z;
	float orientation = 0.0f;

	if( ! PyArg_ParseTuple( args, "kfff|f", &map, &x, &y, &z, &orientation ) )
	{
		PyErr_SetString( PyExc_ValueError, "The command requires a map Id, and x,y,x coordinates." );
		return NULL;
	}

	Player *player = self->playerPtr;
	player->SafeTeleport( map, 0, x, y, z, orientation );

	Py_RETURN_NONE;
}


/// sendGossipPOI
///   Sends a gossip POI (Point Of Interest) to the Player. Which means a location is marked on the player's map
///
/// Parameters
///   x        -   X coordinate of the marked location
///   y        -   Y coordinate of the marked location
///   icon     -   The icon that will show up on the map
///   flags    -   ???
///   data     -   ???
///   name     -   The name of the location marked
///
/// Return value
///   None
///
/// Example
///   player.sendGossipPOI( -8811.46, 667.46, 7, 6, 0, "Stormwind Auction House" )
///
static PyObject* ArcPyPlayer_sendGossipPOI( ArcPyPlayer *self, PyObject *args )
{
	float x = 0.0f;
	float y = 0.0f;
	uint32 icon = 0;
	uint32 flags = 0;
	uint32 data = 0;
	const char *name = NULL;

	if( !PyArg_ParseTuple( args, "ffkkks", &x, &y, &icon, &flags, &data, &name ) )
	{
		return NULL;
	}

	Player *player = self->playerPtr;

	Messenger::SendGossipPOI( player, x, y, icon, flags, data, name );

	Py_RETURN_NONE;
}


/// markQuestObjectiveAsComplete
///   Marks a quest objective as complete in the player's quest log
///
/// Parameters
///   quest      - The identifier of the quest
///   objective  - The sequence number of the objective
///
/// Return value
///   None
///
/// Example
///   player.markQuestObjectiveAsComplete( 1234, 1 )
///
static PyObject* ArcPyPlayer_markQuestObjectiveAsComplete( ArcPyPlayer *self, PyObject *args )
{
	uint32 quest;
	uint32 objective;
	uint64 guid = 0;

	if( !PyArg_ParseTuple( args, "kk|K", &quest, &objective, &guid ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a quest, and an objective to be specified" );
		return NULL;
	}

	Player *player = self->playerPtr;

	QuestLogEntry *qle = player->GetQuestLogForEntry( quest );
	if( qle != NULL )
	{
		qle->MarkObjectiveComplete( objective, guid );
	}

	Py_RETURN_NONE;
}

/// addQuestKill
///   Adds a quest kill / credit to the specified quest's specified objective
///
/// Parameters
///   quest      - The identifier of the quest
///   objective  - The sequence number of the objective
///
/// Return value
///   None
///
/// Example
///   player.addQuestKill( 1234, 1 )
///
static PyObject* ArcPyPlayer_addQuestKill( ArcPyPlayer *self, PyObject *args )
{
	uint32 quest;
	uint32 objective;
	uint64 guid = 0;

	if( !PyArg_ParseTuple( args, "kk|K", &quest, &objective, &guid ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a quest, and an objective to be specified" );
		return NULL;
	}

	Player *player = self->playerPtr;
	player->AddQuestKill( quest, (uint8)objective, 0, guid );

	Py_RETURN_NONE;
}

/// sendMovie
///   Sends / shows a movie to the Player
///
/// Parameters
///   movieID    - The identifier of the movie
///
/// Return value
///   None
///
/// Example
///   player.sendMovie( 16 ) # Shows the Fall of the Lich King movie
///
static PyObject* ArcPyPlayer_sendMovie( ArcPyPlayer *self, PyObject *args )
{
	uint32 movieId;

	if( !PyArg_ParseTuple( args, "k", &movieId ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a movie id to be specified" );
		return NULL;
	}

	Player *player = self->playerPtr;
	Messenger::SendTriggerMovie( player, movieId );

	Py_RETURN_NONE;
}


static PyMethodDef ArcPyPlayer_methods[] = 
{
	{ "getName", (PyCFunction)ArcPyPlayer_getName, METH_NOARGS, "Returns the name of the Player" },
	{ "sendChatMessage", (PyCFunction)ArcPyPlayer_sendChatMessage, METH_VARARGS, "Sends a chat message from the Player" },
	{ "toUnit", (PyCFunction)ArcPyPlayer_toUnit, METH_NOARGS, "Returns the Player object as a Unit" },
	{ "spawnAndEnterVehicle", (PyCFunction)ArcPyPlayer_spawnAndEnterVehicle, METH_VARARGS, "Spawns a vehicle and makes the player enter it" },
	{ "teleport", (PyCFunction)ArcPyPlayer_teleport, METH_VARARGS, "Teleports the player to the given map's given coordinates" },
	{ "sendGossipPOI", (PyCFunction)ArcPyPlayer_sendGossipPOI, METH_VARARGS, "Sends a gossip POI (Point Of Interest) to the Player, marking it on the map" },
	{ "markQuestObjectiveAsComplete", (PyCFunction)ArcPyPlayer_markQuestObjectiveAsComplete, METH_VARARGS, "Marks a quest objective as complete in the player's quest log" },
	{ "addQuestKill", (PyCFunction)ArcPyPlayer_addQuestKill, METH_VARARGS, "Adds a quest kill credit for a quest's objective" },
	{ "sendMovie", (PyCFunction)ArcPyPlayer_sendMovie, METH_VARARGS, "Sends / shows a movie to the Player" },
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
	&ArcPyUnitType,					// tp_base
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

ArcPyPlayer* createArcPyPlayer( Player* p )
{
	PyTypeObject *type = &ArcPyPlayerType;
	
	ArcPyPlayer* player = (ArcPyPlayer*)type->tp_alloc( type, 0 );
	player->playerPtr = p;
	player->unit.unitPtr = p;

	return player;
}
