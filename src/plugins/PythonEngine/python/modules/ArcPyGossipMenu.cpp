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

#include "ArcPyObject.hpp"
#include "ArcPyItem.hpp"
#include "ArcPyUnit.hpp"
#include "ArcPyPlayer.hpp"

#include "ArcPyGossipMenu.hpp"

static PyObject* ArcPyGossipMenu_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyGossipMenu *self = (ArcPyGossipMenu*)type->tp_alloc( type, 0 );
	self->gossipMenuPtr = NULL;
	
	return (PyObject*)self;
}

/// GossipMenu
///   Creates a new GossipMenu
///
/// Parameters
///   textId    -  The text to show in the menu
///   unit      -  The GossipMenu's owner object. It's portrait will be shown in the menu.
///   autosend  -  Should the menu be automatically sent? It's not implemented here.
///
/// Return value
///   Returns the constructed GossipMenu
///
/// Example:
///   menu = GossipMenu( 1, unit, arcemu.GOSSIP_AUTOSEND_FALSE )
///
static int ArcPyGossipMenu_init( ArcPyGossipMenu *self, PyObject *args, PyObject *keywords )
{
	unsigned long textId = 0;
	PyObject* obj = NULL;
	unsigned long autoSend = 0;

	if( ! PyArg_ParseTuple( args, "kOk", &textId, &obj, &autoSend ) )
	{
		return -1;
	}

	if( isArcPyObject( obj ) )
	{
		Object *apo = ((ArcPyObject*)obj)->objectPtr;
		self->gossipMenuPtr = new Arcemu::Gossip::Menu( apo, textId );
	}
	else
	{
		PyErr_SetString( PyExc_TypeError, "Second argument should be an ArcPyObject subtype (GameObject, Item, Unit)" );
		return -1;
	}

	return 0;
}

static void ArcPyGossipMenu_dealloc( ArcPyGossipMenu* self )
{
	delete self->gossipMenuPtr;
	self->gossipMenuPtr = NULL;
	Py_TYPE( self )->tp_free( (PyObject*)self );
}

/// addItem
///   Adds a menu item to a GossipMenu
///
/// Parameters
///   icon        - Icon Id for the menu item. See the Icons enum for possible values.
///   text        - Text of the menu item
///   option      - Sequence number of this menu item
///   coded       - When True (1) selecting this option requires a code to be entered
///   boxMessage  - (optional) Text shown with the cost of selecting this item
///   boxMoney    - (optional) Cost (in copper) of selecting this item
///
/// Return value
///   None
///
/// Examples
///   menu.addItem( arcemu.ICON_CHAT, "Something0", 7, 0 )
///   menu.addItem( arcemu.ICON_VENDOR, "Something1", 8, 0, "Some message", 123456 )
///	  menu.addItem( arcemu.ICON_DOT, "Something2", 9, 1 )
///
static PyObject* ArcPyGossipMenu_addItem( ArcPyGossipMenu *self, PyObject *args )
{
	unsigned long icon = 0;
	const char* text = NULL;
	unsigned long optionId = 0;
	unsigned long coded = 0;

	const char *boxMessage = "";
	unsigned long boxMoney = 0;

	ARCEMU_ASSERT( self->gossipMenuPtr != NULL );

	if( ! PyArg_ParseTuple( args, "kskk|sk", &icon, &text, &optionId, &coded, &boxMessage, &boxMoney ) )
	{
		return NULL;
	}

	std::string strText = text;

	if( coded == 0 )
		self->gossipMenuPtr->AddItem( icon, text, optionId, boxMoney, boxMessage, false );
	else
		self->gossipMenuPtr->AddItem( icon, text, optionId, boxMoney, boxMessage, true );

	Py_RETURN_NONE;
}


/// addQuests
///   Adds quests of the specified creature to the Gossip menu
///
/// Parameters
///   creature  -  The quest giver creature
///   player    -  The player we're adding quests for
///
/// Return value
///   None
///
/// Example
///   menu.addQuests( creature, player )
///
static PyObject* ArcPyGossipMenu_addQuests( ArcPyGossipMenu *self, PyObject *args )
{
	PyObject *argUnit = NULL;
	PyObject *argPlayer = NULL;

	if( ! PyArg_ParseTuple( args, "OO", &argUnit, &argPlayer ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a Unit and a Player parameter" );
		return NULL;
	}

	if( !isArcPyUnit( argUnit ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a Unit and a Player parameter" );
		return NULL;
	}

	if( !isArcPyPlayer( argPlayer ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires a Unit and a Player parameter" );
		return NULL;
	}

	Unit *unit = ((ArcPyUnit*)argUnit)->unitPtr;
	Player *player = ((ArcPyPlayer*)argPlayer)->playerPtr;

	if( ! unit->IsCreature() )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires the passed Unit to be a Creature" );
		return NULL;
	}

	Creature *creature = static_cast< Creature* >( unit );
	Arcemu::Gossip::Menu &menu = *self->gossipMenuPtr;

	sQuestMgr.FillQuestMenu( creature, player, menu );

	Py_RETURN_NONE;
}

/// sendToPlayer
///   Sends the menu to a Player
///
/// Parameters
///   player  -  The Player we will send this menu to
///
/// Return value
///   None
///
/// Example
///   menu.sendToPlayer( player )
///
static PyObject* ArcPyGossipMenu_sendToPlayer( ArcPyGossipMenu* self, PyObject* args )
{
	PyObject* playerObj = NULL;
	if( ! PyArg_ParseTuple( args, "O", &playerObj ) )
	{
		return NULL;
	}

	if( strcmp( Py_TYPE( playerObj )->tp_name, "ArcPyPlayer" ) != 0 )
	{
		return NULL;
	}

	Player *player = ((ArcPyPlayer*)playerObj)->playerPtr;
	self->gossipMenuPtr->Send( player );
	
	Py_RETURN_NONE;
}


/// complete
///   Close the gossip menu for the Player
///
/// Parameters
///   player  -  The Player we will close the menu for
///
/// Return value
///   None
///
/// Example
///   GossipMenu.complete( player )
///
static PyObject* ArcPyGossipMenu_complete( ArcPyGossipMenu* /*self*/, PyObject* args )
{
	PyObject* playerObj = NULL;
	if( ! PyArg_ParseTuple( args, "O", &playerObj ) )
	{
		return NULL;
	}

	if( strcmp( Py_TYPE( playerObj )->tp_name, "ArcPyPlayer" ) != 0 )
	{
		return NULL;
	}

	Player *player = ((ArcPyPlayer*)playerObj)->playerPtr;
	Arcemu::Gossip::Menu::Complete( player );

	Py_RETURN_NONE;
}

static PyMethodDef ArcPyGossipMenu_methods[] = 
{
	{ "addItem", (PyCFunction)ArcPyGossipMenu_addItem, METH_VARARGS, "Adds a new menu item to the Gossip menu" },
	{ "addQuests", (PyCFunction)ArcPyGossipMenu_addQuests, METH_VARARGS, "Adds quests of the specified creature to the Gossip menu" },
	{ "sendToPlayer", (PyCFunction)ArcPyGossipMenu_sendToPlayer, METH_VARARGS, "Sends the menu to a Player" },
	{ "complete", (PyCFunction)ArcPyGossipMenu_complete, METH_VARARGS | METH_STATIC, "Completes the player's gossip menu" },
	{NULL}
};

static PyTypeObject ArcPyGossipMenuType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyGossipMenu",					// tp_name
	sizeof( ArcPyGossipMenu ),			// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyGossipMenu_dealloc,	// tp_dealloc
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
	"Arcemu Gossip Menu",				// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyGossipMenu_methods,			// tp_methods
	0,								// tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyGossipMenu_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyGossipMenu_new,				// tp_new
};

int registerArcPyGossipMenu( PyObject *module )
{
	ArcPyGossipMenuType.tp_new = ArcPyGossipMenu_new;

	if( PyType_Ready( &ArcPyGossipMenuType ) < 0 )
	{
		return -1;
	}

	Py_INCREF( &ArcPyGossipMenuType );
	PyModule_AddObject( module, "GossipMenu", (PyObject*)&ArcPyGossipMenuType);

	return 0;
}

ArcPyGossipMenu* createArcPyGossipMenu()
{
	PyTypeObject *type = &ArcPyGossipMenuType;
	ArcPyGossipMenu* menu = (ArcPyGossipMenu*)type->tp_alloc( type, 0 );
	return menu;
}
