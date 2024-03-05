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

#include "StdAfx.h"

#include <Python.h>
#include "serverhooks/ServerHookRegistry.hpp"
#include "engine/ReferenceRegistry.hpp"

#include "ArcPyObject.hpp"
#include "ArcPyUnit.hpp"
#include "ArcPyCreature.hpp"
#include "ArcPyGameObject.hpp"
#include "ArcPyItem.hpp"


/// These are all in their own files
extern void registerArcemuConstants( PyObject *module );
extern int registerArcPyAura( PyObject *module );
extern int registerArcPyCreature( PyObject *module );
extern int registerArcPyGameObject( PyObject *module );
extern int registerArcPyGossipMenu( PyObject *module );
extern int registerArcPyGuild( PyObject *module );
extern int registerArcPyItem( PyObject *module );
extern int registerArcPyMapMgr( PyObject *module );
extern int registerArcPyObject( PyObject *module );
extern int registerArcPyObjectLocator( PyObject *module );
extern int registerArcPyUnit( PyObject *module );
extern int registerArcPyPlayer( PyObject *module );
extern int registerArcPyQuest( PyObject *module );
extern int registerArcPySpell( PyObject *module );
extern int registerArcPyWorldSession( PyObject *module );

extern int registerArcPyCreatureScript( PyObject *module );
extern int registerArcPyGameObjectScript( PyObject *module );
extern int registerArcPyGossipScript( PyObject *module );
extern int registerArcPyQuestScript( PyObject *module );
extern int registerArcPyInstanceScript( PyObject *module );

/// RegisterServerHook
///   Registers a server event handler
///
/// Parameters:
///   eventType  -  The server event we're registering handler for
///   function   -  The Python function's name that will handle the event
///
/// Example:
///  RegisterServerHook( arcemu.SERVER_HOOK_EVENT_ON_PRE_DIE, onPreDie )
///
static PyObject* arcemu_RegisterServerHook( PyObject *self, PyObject *args )
{
    uint32 serverEvent = 0;
	PyObject *callback = NULL;

	if( !PyArg_ParseTuple( args, "IO", &serverEvent, &callback ) )
	{
		return NULL;
	}

	if( strcmp( Py_TYPE( callback )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Second argument should be a function!" );
		return NULL;
	}

	Py_IncRef( callback );
	ServerHookRegistry::addHook( serverEvent, callback );
	
	//sLog.Debug( "APE", "Registered handler for server hook event %u", serverEvent );

	Py_RETURN_NONE;
}


/// RegisterUnitGossipEvent
///   Registers a Unit (Creature) gossip event handler
///
/// Parameters:
///   goId         - The numerical identifier of the Unit (Creature)
///   eventType    - The type of the gossip event handler we're registering
///   function     - The Python function's name that will handle this event
///
/// Example:
///   RegisterUnitGossipEvent( 31111, arcemu.GOSSIP_EVENT_SELECT, mohawk_onSelectOption )
///
static PyObject* arcemu_RegisterUnitGossipEvent( PyObject *self, PyObject *args )
{
    uint32 creatureId = 0;
    uint32 gossipEvent = 0;
	PyObject *callback = NULL;

	if( !PyArg_ParseTuple( args, "IIO", &creatureId, &gossipEvent, &callback ) )
	{
		return NULL;
	}

	if( strcmp( Py_TYPE( callback )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Third argument should be a function!" );
		return NULL;
	}

	Py_IncRef( callback );
	ReferenceRegistry::registerCreatureGossipFunction( creatureId, gossipEvent, callback );

	Py_RETURN_NONE;
}


/// RegisterItemGossipEvent
///   Registers an Item gossip event handler
///
/// Parameters:
///   goId         - The numerical identifier of the item
///   eventType    - The type of the gossip event handler we're registering
///   function     - The Python function's name that will handle this event
///
/// Example:
///   RegisterItemGossipEvent( 8051, arcemu.GOSSIP_EVENT_HELLO, item_onHello )
///
static PyObject* arcemu_RegisterItemGossipEvent( PyObject *self, PyObject *args )
{
    uint32 itemId = 0;
    uint32 gossipEvent = 0;
	PyObject *callback = NULL;

	if( !PyArg_ParseTuple( args, "IIO", &itemId, &gossipEvent, &callback ) )
	{
		return NULL;
	}

	if( strcmp( Py_TYPE( callback )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Third argument should be a function!" );
		return NULL;
	}

	Py_IncRef( callback );
	ReferenceRegistry::registerItemGossipFunction( itemId, gossipEvent, callback );

	Py_RETURN_NONE;
}

/// RegisterGOGossipEvent
///   Registers a GameObject gossip event handler
///
/// Parameters:
///   goId         - The numerical identifier of the gameobject
///   eventType    - The type of the gossip event handler we're registering
///   function     - The Python function's name that will handle this event
///
/// Example:
///   RegisterGOGossipEvent( 202242, arcemu.GOSSIP_EVENT_SELECT, go_onSelectOption )
///
static PyObject* arcemu_RegisterGOGossipEvent( PyObject *self, PyObject *args )
{
    uint32 goId = 0;
    uint32 gossipEvent = 0;
	PyObject *callback = NULL;

	if( !PyArg_ParseTuple( args, "IIO", &goId, &gossipEvent, &callback ) )
	{
		return NULL;
	}

	if( strcmp( Py_TYPE( callback )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Third argument should be a function!" );
		return NULL;
	}

	Py_IncRef( callback );
	ReferenceRegistry::registerGOGossipFunction( goId, gossipEvent, callback );

	Py_RETURN_NONE;
}


/// RegisterGameObjectEvent
///   Registers a GameObject event handler
///
/// Parameters:
///   goId         - The numerical identifier of the gameobject
///   eventType    - The type of the event handler we're registering
///   function     - The Python function's name that will handle this event
///
/// Example:
///   RegisterGameObjectEvent( 190375, arcemu.GO_EVENT_ON_DAMAGED, fortressDoor_onDamaged )
///
static PyObject* arcemu_RegisterGameObjectEvent( PyObject *self, PyObject *args )
{
    uint32 goId = 0;
    uint32 goEvent = 0;
	PyObject *callback = NULL;

	if( !PyArg_ParseTuple( args, "IIO", &goId, &goEvent, &callback ) )
	{
		return NULL;
	}

	if( strcmp( Py_TYPE( callback )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Third argument should be a function!" );
		return NULL;
	}

	Py_IncRef( callback );
	ReferenceRegistry::registerGOEventFunction( goId, goEvent, callback );

	Py_RETURN_NONE;
}


/// RegisterUnitEvent
///   Registers a Unit (Creature) event handler
///
/// Parameters:
///   creatureId   - The numerical identifier of the creature
///   eventType    - The type of the event handler we're registering
///   function     - The Python function's name that will handle this event
///
/// Example:
///   RegisterUnitEvent( 113, arcemu.CREATURE_EVENT_ON_CALL_FOR_HELP, npc_onCallForHelp )
///
static PyObject* arcemu_RegisterUnitEvent( PyObject *self, PyObject *args )
{
    uint32 creatureId = 0;
    uint32 creatureEvent = 0;
	PyObject *callback = NULL;

	if( !PyArg_ParseTuple( args, "IIO", &creatureId, &creatureEvent, &callback ) )
	{
		return NULL;
	}

	if( strcmp( Py_TYPE( callback )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Third argument should be a function!" );
		return NULL;
	}

	Py_IncRef( callback );
	ReferenceRegistry::registerCreatureEventFunction( creatureId, creatureEvent, callback );

	Py_RETURN_NONE;
}

/// RegisterQuestEvent function.
///   Registers a Quest event handler
///
/// Parameters:
///   questId   - The numerical identifier of the quest
///   eventType - The type of the event handler we're registering
///   function  - The Python function's name that will handle this event
///
/// Example:
///  RegisterQuestEvent( 7, arcemu.QUEST_EVENT_ON_CREATURE_KILL, quest_onCreatureKill )
///
static PyObject* arcemu_RegisterQuestEvent( PyObject *self, PyObject *args )
{
    uint32 questId = 0;
    uint32 questEvent = 0;
	PyObject *callback = NULL;


	if( !PyArg_ParseTuple( args, "IIO", &questId, &questEvent, &callback ) )
	{
		return NULL;
	}

	if( strcmp( Py_TYPE( callback )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Third argument should be a function!" );
		return NULL;
	}

	Py_IncRef( callback );
	ReferenceRegistry::registerQuestEventFunction( questId, questEvent, callback );

	Py_RETURN_NONE;
}

/// RegisterInstanceEvent
///   Registers an Instance event handler
///
/// Parameters:
///   mapId     - The numerical identifier of the map
///   eventType - The type of the event handler we're registering
///   function  - The Python function's name that will handle this event
///
/// Example:
///   RegisterInstanceEvent( 34, arcemu.INSTANCE_EVENT_ON_GO_ACTIVATE, instance_onGOActivated ) 
///
static PyObject* arcemu_RegisterInstanceEvent( PyObject *self, PyObject *args )
{
    uint32 mapId = 0;
    uint32 instanceEvent = 0;
	PyObject *callback = NULL;


	if( !PyArg_ParseTuple( args, "IIO", &mapId, &instanceEvent, &callback ) )
	{
		return NULL;
	}

	if( strcmp( Py_TYPE( callback )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Third argument should be a function!" );
		return NULL;
	}

	Py_IncRef( callback );
	ReferenceRegistry::registerInstanceEventFunction( mapId, instanceEvent, callback );

	Py_RETURN_NONE;
}


/// RegisterDummySpellHandler
///   Registers a dummy spell effect handler
///
/// Parameters:
///   spellId   - The numerical identifier of the spell
///   function  - The Python function's name that will handle the dummy effect
///
/// Example:
///   RegisterDummySpell( 1234, spellname_handleDummyEffect )
///
static PyObject* arcemu_RegisterDummySpellHandler( PyObject *self, PyObject *args )
{
    uint32 spellId;
	PyObject *callback;

	if( !PyArg_ParseTuple( args, "IO", &spellId, &callback ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a spell Id and a callback function be specified" );
		return NULL;
	}

	if( strcmp( Py_TYPE( callback )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Second argument should be a function!" );
		return NULL;
	}

	Py_IncRef( callback );
	ReferenceRegistry::registerDummySpellHandler( spellId, callback );

	Py_RETURN_NONE;
}


/// RegisterScriptedEffectHandler
///   Registers a scripted spell effect handler
///
/// Parameters:
///   spellId   - The numerical identifier of the spell
///   function  - The Python function's name that will handle the scripted effect
///
/// Example:
///   RegisterScriptedEffectHandler( 1234, spellname_handleDummyEffect )
///
static PyObject* arcemu_RegisterScriptedEffectHandler( PyObject *self, PyObject *args )
{
    uint32 spellId;
	PyObject *callback;

	if( !PyArg_ParseTuple( args, "IO", &spellId, &callback ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a spell Id and a callback function be specified" );
		return NULL;
	}

	if( strcmp( Py_TYPE( callback )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Second argument should be a function!" );
		return NULL;
	}

	Py_IncRef( callback );
	ReferenceRegistry::registerScriptedEffectHandler( spellId, callback );

	Py_RETURN_NONE;
}

/// RegisterDummyAuraHandler
///   Registers a dummy aura handler
///
/// Parameters:
///   spellId   - The numerical identifier of the spell that applies the aura
///   function  - The Python function's name that will handle the aura effect
///
/// Example:
///   RegisterDummyAura( 1234, spellname_handleAura )
///
static PyObject* arcemu_RegisterDummyAuraHandler( PyObject *self, PyObject *args )
{
    uint32 spellId;
	PyObject *callback;

    if( !PyArg_ParseTuple( args, "IO", &spellId, &callback ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a spell Id and a callback function be specified" );
		return NULL;
	}

	if( strcmp( Py_TYPE( callback )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Second argument should be a function!" );
		return NULL;
	}

	Py_IncRef( callback );
	ReferenceRegistry::registerDummyAuraHandler( spellId, callback );

	Py_RETURN_NONE;
}


/// RegisterCreatureScriptFactory
///   Registers a Python Creature Script factory function
///
/// Parameters:
///   creatured   - The numerical identifier of the creature
///   function    - The factory function
///
/// Example:
///   RegisterCreatureScriptFactory( 1234, ValamiScript.create )
///
static PyObject* arcemu_RegisterCreatureScriptFactory( PyObject *self, PyObject *args )
{
	uint32 creatureId;
	PyObject *factoryFunction;

	if( !PyArg_ParseTuple( args, "IO", &creatureId, &factoryFunction ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a creature Id and a factory function to be specified" );
		return NULL;
	}

	if( strcmp( Py_TYPE( factoryFunction )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Second argument should be a function!" );
		return NULL;
	}

	Py_IncRef( factoryFunction );
	ReferenceRegistry::registerCreatureScriptFactory( creatureId, factoryFunction );

	Py_RETURN_NONE;
}

/// RegisterGameObjectScriptFactory
///   Registers a Python GameObject Script factory function
///
/// Parameters:
///   goId        - The numerical identifier of the gameobject
///   function    - The factory function
///
/// Example:
///   RegisterGameObjectScriptFactory( 1234, ValamiScript.create )
///
static PyObject* arcemu_RegisterGameObjectScriptFactory( PyObject *self, PyObject *args )
{
	uint32 goId;
	PyObject *factoryFunction;

	if( !PyArg_ParseTuple( args, "IO", &goId, &factoryFunction ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a gameobject Id and a factory function to be specified" );
		return NULL;
	}

	if( strcmp( Py_TYPE( factoryFunction )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Second argument should be a function!" );
		return NULL;
	}

	Py_IncRef( factoryFunction );
	ReferenceRegistry::registerGameObjectScriptFactory( goId, factoryFunction );

	Py_RETURN_NONE;
}


/// RegisterInstanceScriptFactory
///   Registers a Python Instance Script factory function
///
/// Parameters:
///   mapId       - The id of the map that uses the instance script
///   function    - The factory function
///
/// Example:
///   RegisterInstanceScriptFactory( 530, ValamiScript.create )
///
static PyObject* arcemu_RegisterInstanceScriptFactory( PyObject *self, PyObject *args )
{
	uint32 mapId;
	PyObject *factoryFunction;

	if( !PyArg_ParseTuple( args, "IO", &mapId, &factoryFunction ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a gameobject Id and a factory function to be specified" );
		return NULL;
	}

	if( strcmp( Py_TYPE( factoryFunction )->tp_name, "function" ) != 0 )
	{
		PyErr_SetString( PyExc_TypeError, "Second argument should be a function!" );
		return NULL;
	}

	Py_IncRef( factoryFunction );
	ReferenceRegistry::registerInstanceScriptFactory( mapId, factoryFunction );

	Py_RETURN_NONE;
}


/// RegisterGameObjectGossipScript
///   Registers a Python gossip script for a gameobject
///
/// Parameters:
///   goId        - The numerical identifier of the gameobject
///   script      - The python gossip script
///
/// Example:
///   RegisterGameObjectGossip( 1234, ValamiGossip )
///
static PyObject* arcemu_RegisterGameObjectGossipScript( PyObject *self, PyObject *args )
{
	uint32 goId;
	PyObject *gossipScript;

	if( !PyArg_ParseTuple( args, "IO", &goId, &gossipScript ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a gameobject Id and gossip script" );
		return NULL;
	}

	if( ( Py_TYPE( gossipScript )->tp_base == NULL ) || ( strcmp( Py_TYPE( gossipScript )->tp_base->tp_name, "ArcPyGossipScript" ) != 0 ) )
	{
		PyErr_SetString( PyExc_TypeError, "Second argument should be a class instance object that is a subclass of GossipScript!" );
		return NULL;
	}

	Py_IncRef( gossipScript );
	ReferenceRegistry::registerGOGossipScript( goId, gossipScript );

	Py_RETURN_NONE;
}

/// RegisterItemGossipScript
///   Registers a Python gossip script for an item
///
/// Parameters:
///   itemId      - The numerical identifier of the item
///   script      - The python gossip script
///
/// Example:
///   RegisterItemGossipScript( 1234, ValamiGossip )
///
static PyObject* arcemu_RegisterItemGossipScript( PyObject *self, PyObject *args )
{
	uint32 itemId;
	PyObject *gossipScript;

	if( !PyArg_ParseTuple( args, "IO", &itemId, &gossipScript ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a item Id and gossip script" );
		return NULL;
	}

	if( ( Py_TYPE( gossipScript )->tp_base == NULL ) || ( strcmp( Py_TYPE( gossipScript )->tp_base->tp_name, "ArcPyGossipScript" ) != 0 ) )
	{
		PyErr_SetString( PyExc_TypeError, "Second argument should be a class instance object that is a subclass of GossipScript!" );
		return NULL;
	}

	Py_IncRef( gossipScript );
	ReferenceRegistry::registerItemGossipScript( itemId, gossipScript );

	Py_RETURN_NONE;
}


/// RegisterCreatureGossipScript
///   Registers a Python gossip script for a creature
///
/// Parameters:
///   creatureId  - The numerical identifier of the creature
///   script      - The python gossip script
///
/// Example:
///   RegisterCreatureGossipScript( 1234, ValamiGossip )
///
static PyObject* arcemu_RegisterCreatureGossipScript( PyObject *self, PyObject *args )
{
	uint32 creatureId;
	PyObject *gossipScript;

	if( !PyArg_ParseTuple( args, "IO", &creatureId, &gossipScript ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a creature Id and gossip script" );
		return NULL;
	}

	if( ( Py_TYPE( gossipScript )->tp_base == NULL ) || ( strcmp( Py_TYPE( gossipScript )->tp_base->tp_name, "ArcPyGossipScript" ) != 0 ) )
	{
		PyErr_SetString( PyExc_TypeError, "Second argument should be a class instance object that is a subclass of GossipScript!" );
		return NULL;
	}

	Py_IncRef( gossipScript );
	ReferenceRegistry::registerCreatureGossipScript( creatureId, gossipScript );

	Py_RETURN_NONE;
}


/// RegisterQuestScript
///   Registers a Python quest script
///
/// Parameters:
///   questId     - The numerical identifier of the quest
///   script      - The python quest script
///
/// Example:
///   RegisterQuestScript( 1234, ValamiQuestScript )
///
static PyObject* arcemu_RegisterQuestScript( PyObject *self, PyObject *args )
{
	uint32 questId;
	PyObject *questScript;

	if( !PyArg_ParseTuple( args, "IO", &questId, &questScript ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a quest Id and quest script" );
		return NULL;
	}

	if( ( Py_TYPE( questScript )->tp_base == NULL ) || ( strcmp( Py_TYPE( questScript )->tp_base->tp_name, "ArcPyQuestScript" ) != 0 ) )
	{
		PyErr_SetString( PyExc_TypeError, "Second argument should be a class instance object that is a subclass of QuestScript!" );
		return NULL;
	}

	Py_IncRef( questScript );
	ReferenceRegistry::registerQuestScript( questId, questScript );

	Py_RETURN_NONE;
}


/// toUnit
///   Casts the ArcPyObject parameter to an ArcPyUnit
///
/// Parameters:
///   object     -  An ArcPyObject
///
/// Example:
///   unit = arcemu.toUnit( object )
///
static PyObject* arcemu_toUnit( PyObject *self, PyObject *args )
{
	PyObject *po;

	if( !PyArg_ParseTuple( args, "O", &po ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires an ArcPyObject parameter" );
		return NULL;
	}

	if( ! isArcPyObject( po ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires an ArcPyObject parameter" );
		return NULL;
	}

	ArcPyObject *apo = (ArcPyObject*)po;
	if( apo->objectPtr->IsUnit() )
	{
		return (PyObject*)createArcPyUnit( static_cast< Unit* >( apo->objectPtr ) );
	}
	else
	{
		Py_RETURN_NONE;
	}
}

/// toCreature
///   Casts the ArcPyObject parameter to an ArcPyCreature
///
/// Parameters:
///   object     -  An ArcPyObject
///
/// Example:
///   unit = arcemu.toUnit( object )
///
static PyObject* arcemu_toCreature( PyObject *self, PyObject *args )
{
	PyObject *po;

	if( !PyArg_ParseTuple( args, "O", &po ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires an ArcPyObject parameter" );
		return NULL;
	}

	if( ! isArcPyObject( po ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires an ArcPyObject parameter" );
		return NULL;
	}

	ArcPyObject *apo = (ArcPyObject*)po;
	if( apo->objectPtr->IsCreature() )
	{
		return (PyObject*)createArcPyCreature( static_cast< Creature* >( apo->objectPtr ) );
	}
	else
	{
		Py_RETURN_NONE;
	}
}

/// toGameObject
///   Casts the ArcPyObject parameter to an ArcPyGameObject
///
/// Parameters:
///   object     -  An ArcPyObject
///
/// Example:
///   go = arcemu.toGameObject( object )
///
static PyObject* arcemu_toGameObject( PyObject *self, PyObject *args )
{
	PyObject *po;

	if( !PyArg_ParseTuple( args, "O", &po ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires an ArcPyObject parameter" );
		return NULL;
	}

	if( !isArcPyObject( po ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires an ArcPyObject parameter" );
		return NULL;
	}

	ArcPyObject *apo = (ArcPyObject*)po;
	if( apo->objectPtr->IsGameObject() )
	{
		return (PyObject*)createArcPyGameObject( static_cast< GameObject* >( apo->objectPtr ) );
	}
	else
	{
		Py_RETURN_NONE;
	}
}

/// toItem
///   Casts the ArcPyObject parameter to an ArcPyItem
///
/// Parameters:
///   object     -  An ArcPyObject
///
/// Example:
///   item = arcemu.toItem( object )
///
static PyObject* arcemu_toItem( PyObject *self, PyObject *args )
{
	PyObject *po;

	if( !PyArg_ParseTuple( args, "O", &po ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires an ArcPyObject parameter" );
		return NULL;
	}

	if( !isArcPyObject( po ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires an ArcPyObject parameter" );
		return NULL;
	}

	ArcPyObject *apo = (ArcPyObject*)po;
	if( apo->objectPtr->IsItem() )
	{
		return (PyObject*)createArcPyItem( static_cast< Item* >( apo->objectPtr ) );
	}
	else
	{
		Py_RETURN_NONE;
	}
}


/// getGameTime
///   Returns the seconds elapsed since the Unix epoch (1970-01-01 00:00:00)
///
/// Parameters:
///   None
///
/// Return value:
///   Returns the seconds elapsed since the Unix epoch (1970-01-01 00:00:00)
///
/// Example:
///   time = arcemu.getGameTime()
///
static PyObject* arcemu_getGameTime( PyObject *self, PyObject *args )
{
	return PyLong_FromUnsignedLongLong( UNIXTIME );
}

/// This is where we assign the Arcemu Python module's function names to functions, and properties
static PyMethodDef ArcemuMethods[] = {
	{ "RegisterServerHook", arcemu_RegisterServerHook, METH_VARARGS, "Registers a server hook function" },
	{ "RegisterUnitGossipEvent", arcemu_RegisterUnitGossipEvent, METH_VARARGS, "Registers a Unit gossip event" },
	{ "RegisterItemGossipEvent", arcemu_RegisterItemGossipEvent, METH_VARARGS, "Registers an Item gossip event" },
	{ "RegisterGOGossipEvent", arcemu_RegisterGOGossipEvent, METH_VARARGS, "Registers a GO gossip event" },
	{ "RegisterGameObjectEvent", arcemu_RegisterGameObjectEvent, METH_VARARGS, "Registers a GameObject event" },
	{ "RegisterUnitEvent", arcemu_RegisterUnitEvent, METH_VARARGS, "Registers a Unit event" },
	{ "RegisterQuestEvent", arcemu_RegisterQuestEvent, METH_VARARGS, "Registers a Quest event" },
	{ "RegisterInstanceEvent", arcemu_RegisterInstanceEvent, METH_VARARGS, "Registers an Instance script event" },
	{ "RegisterDummySpellHandler", arcemu_RegisterDummySpellHandler, METH_VARARGS, "Registers a dummy spell effect handler function" },
	{ "RegisterScriptedEffectHandler", arcemu_RegisterScriptedEffectHandler, METH_VARARGS, "Registers a scripted spell effect handler function" },
	{ "RegisterDummyAuraHandler", arcemu_RegisterDummyAuraHandler, METH_VARARGS, "Registers a dummy aura handler function" },
	{ "RegisterCreatureScript", arcemu_RegisterCreatureScriptFactory, METH_VARARGS, "Registers a creature script factory function" },
	{ "RegisterGameObjectScript", arcemu_RegisterGameObjectScriptFactory, METH_VARARGS, "Registers a gameobject script factory function" },
	{ "RegisterGameObjectGossipScript", arcemu_RegisterGameObjectGossipScript, METH_VARARGS, "Registers a gameobject gossip script object" },	
	{ "RegisterItemGossipScript", arcemu_RegisterItemGossipScript, METH_VARARGS, "Registers an item gossip script object" },
	{ "RegisterCreatureGossipScript", arcemu_RegisterCreatureGossipScript, METH_VARARGS, "Registers a creature gossip script object" },
	{ "RegisterQuestScript", arcemu_RegisterQuestScript, METH_VARARGS, "Registers a quest script object" },
	{ "RegisterInstanceScript", arcemu_RegisterInstanceScriptFactory, METH_VARARGS, "Registers an instance script factory function" },
	{ "toUnit", arcemu_toUnit, METH_VARARGS, "Casts the Object to a Unit" },
	{ "toCreature", arcemu_toCreature, METH_VARARGS, "Casts the Object to a Creature" },
	{ "toGameObject", arcemu_toGameObject, METH_VARARGS, "Casts the Object to a GameObject" },
	{ "toItem", arcemu_toItem, METH_VARARGS, "Casts the Object to an Item" },
	{ "getGameTime", arcemu_getGameTime, METH_NOARGS, "Returns the seconds elapsed since the Unix epoch (1970-01-01 00:00:00)" },
	{NULL, NULL, 0, NULL }
};

static PyModuleDef ArcemuModule = {
	PyModuleDef_HEAD_INIT, "arcemu", NULL, -1, ArcemuMethods,
	NULL, NULL, NULL, NULL
};

/// Initializes the Arcemu Python module
PyObject* PyInit_Arcemu(void)
{
	PyObject *module = PyModule_Create(&ArcemuModule);

	/// Register Arcemu constants in Python
	registerArcemuConstants( module );

	/// Register Arcemu objects in Python
	registerArcPyAura( module );
	registerArcPyCreature( module );
	registerArcPyGameObject( module );
	registerArcPyGossipMenu( module );
	registerArcPyGuild( module );
	registerArcPyItem( module );
	registerArcPyMapMgr( module );
	registerArcPyObject( module );
	registerArcPyObjectLocator( module );
	registerArcPyUnit( module );
	registerArcPyPlayer( module );
	registerArcPyQuest( module );
	registerArcPySpell( module );
	registerArcPyWorldSession( module );

	registerArcPyCreatureScript( module );
	registerArcPyGameObjectScript( module );
	registerArcPyGossipScript( module );
	registerArcPyQuestScript( module );
	registerArcPyInstanceScript( module );

	return module;
}
