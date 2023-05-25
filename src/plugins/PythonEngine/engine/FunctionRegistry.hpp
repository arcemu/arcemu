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

#ifndef _FUNCTION_REGISTRY_H_
#define _FUNCTION_REGISTRY_H_

#include "creature/CreatureFunctionTuple.hpp"
#include "creature/CreatureFunctionTupleVisitor.hpp"

#include "gossip/GossipFunctionTuple.hpp"
#include "gossip/GossipFunctionTupleVisitor.hpp"

#include "gameobject/GOFunctionTuple.hpp"
#include "gameobject/GOFunctionTupleVisitor.hpp"

#include "instance/InstanceFunctionTuple.hpp"
#include "instance/InstanceFunctionTupleVisitor.hpp"

#include "quest/QuestFunctionTuple.hpp"
#include "quest/QuestFunctionTupleVisitor.hpp"

#include "spells/DummySpellHandlerVisitor.hpp"
#include "spells/ScriptedEffectHandlerVisitor.hpp"
#include "spells/DummyAuraHandlerVisitor.hpp"

/// Organized storage of Python function references
class FunctionRegistry
{
public:
	///
	/// Registers a creature gossip event handler Python function reference
	///
	/// Parameters
	///   creatureId    -  The id of the creature
	///   gossipEvent   -  The event this function will handle. See PythonGossipEventTypes
	///   function      -  The function reference
	///
	/// Return value
	///   None
	///
	static void registerCreatureGossipFunction( unsigned int creatureId, unsigned int gossipEvent, void* function );

	///
	/// Registers a GO gossip event handler Python function reference
	///
	/// Parameters
	///   goId          -  The id of the GameObject
	///   gossipEvent   -  The event this function will handle. See PythonGossipEventTypes
	///   function      -  The function reference
	///
	/// Return value
	///   None
	///
	static void registerGOGossipFunction( unsigned int goId, unsigned int gossipEvent, void* function );


	///
	/// Registers an item gossip event handler Python function reference
	///
	/// Parameters
	///   itemId    -  The id of the creature
	///   gossipEvent   -  The event this function will handle. See PythonGossipEventTypes
	///   function      -  The function reference
	///
	/// Return value
	///   None
	///
	static void registerItemGossipFunction( unsigned int itemId, unsigned int gossipEvent, void* function );


	///
	/// Registers a GameObject event handler Python function reference
	///
	/// Parameters
	///   goId          -  The id of the GameObject
	///   goEvent   -  The event this function will handle. See PythonGOEventTypes
	///   function      -  The function reference
	///
	/// Return value
	///   None
	///
	static void registerGOEventFunction( unsigned int goId, unsigned int goEvent, void* function );

	///
	/// Registers a Creature event handler Python function reference
	///
	/// Parameters
	///   creatureId    -  The id of the Creature
	///   creatureEvent   -  The event this function will handle. See PythonCreatureEventTypes
	///   function      -  The function reference
	///
	/// Return value
	///   None
	///
	static void registerCreatureEventFunction( unsigned int creatureId, unsigned int creatureEvent, void* function );


	///
	/// Registers a Quest event handler Python function reference
	///
	/// Parameters
	///   questId       -  The id of the Quest
	///   questEvent   -  The event this function will handle. See PythonQuestEventTypes
	///   function      -  The function reference
	///
	/// Return value
	///   None
	///
	static void registerQuestEventFunction( unsigned int questId, unsigned int questEvent, void* function );

	///
	/// Registers an Instance event handler Python function reference
	///
	/// Parameters
	///   instanceId    -  The id of the Creature
	///   instanceEvent   -  The event this function will handle. See PythonInstanceEventTypes
	///   function      -  The function reference
	///
	/// Return value
	///   None
	///
	static void registerInstanceEventFunction( unsigned int instanceId, unsigned int instanceEvent, void* function );


	///
	/// Registers a dummy spell handler
	///
	/// Parameters
	///   spellId    -  The id of the Spell
	///   function   -  The function reference
	///
	/// Return value
	///   None
	///
	static void registerDummySpellHandler( unsigned long spellId, void* function );


	///
	/// Registers a scripted spell effect handler
	///
	/// Parameters
	///   spellId    -  The id of the Spell
	///   function   -  The function reference
	///
	/// Return value
	///   None
	///
	static void registerScriptedEffectHandler( unsigned long spellId, void* function );


	///
	/// Registers a dummy aura handler
	///
	/// Parameters
	///   spellId    -  The id of the Spell that applies this Aura
	///   function   -  The function reference
	///
	/// Return value
	///   None
	///
	static void registerDummyAuraHandler( unsigned long spellId, void* function );


	///
	/// Visits all registered creature gossip functions
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitCreatureGossipFunctions( GossipFunctionTupleVisitor *visitor );


	///
	/// Visits all registered GameObject gossip functions
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitGOGossipFunctions( GossipFunctionTupleVisitor *visitor );


	///
	/// Visits all registered item gossip functions
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitItemGossipFunctions( GossipFunctionTupleVisitor *visitor );

	///
	/// Visits all registered GameObject event functions
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitGOEventFunctions( GOFunctionTupleVisitor *visitor );

	///
	/// Visits all registered Instance event functions
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitInstanceEventFunctions( InstanceFunctionTupleVisitor *visitor );

	///
	/// Visits all registered Creature event functions
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitCreatureEventFunctions( CreatureFunctionTupleVisitor *visitor );

	///
	/// Visits all registered Quest event functions
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitQuestEventFunctions( QuestFunctionTupleVisitor *visitor );

	///
	/// Visits all registered dummy spell handler functions
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitDummySpellHandlerFunctions( DummySpellHandlerVisitor *visitor );

	///
	/// Visits all registered scripted spell effect handler functions
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitScriptedEffectHandlerFunctions( ScriptedEffectHandlerVisitor *visitor );

	///
	/// Visits all registered dummy aura handler functions
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitDummyAuraHandlerFunctions( DummyAuraHandlerVisitor *visitor );


	///
	/// Deallocate / release all registered functions
	///
	/// Parameters
	///   None
	///
	/// Return value
	///   None
	///
	static void releaseFunctions();


	///
	/// Retrieve the event handler functions registered for the specified GameObject 
	///
	/// Parameters
	///   goId - The numeric identifier of the GameObject
	///
	/// Return value
	///   Returns a pointer to the GOFunctionTuple that contains the registered functions for this GameObject.
	///   Returns NULL if there are no functions registered for this GameObject.
	///
	static GOFunctionTuple* getGOEventFunctions( unsigned int goId );


	///
	/// Retrieve the event handler functions registered for the specified Creature 
	///
	/// Parameters
	///   creatureId - The numeric identifier of the GameObject
	///
	/// Return value
	///   Returns a pointer to the CreatureFunctionTuple that contains the registered functions for this Creature.
	///   Returns NULL if there are no functions registered for this Creature.
	///
	static CreatureFunctionTuple* getCreatureEventFunctions( unsigned int creatureId );


	///
	/// Retrieve the event handler functions registered for the specified Quest 
	///
	/// Parameters
	///   questId - The numeric identifier of the Quest
	///
	/// Return value
	///   Returns a pointer to the QuestFunctionTuple that contains the registered functions for this Quest.
	///   Returns NULL if there are no functions registered for this Quest.
	///
	static QuestFunctionTuple* getQuestFunctions( unsigned int questId );


	///
	/// Retrieve the event handler functions registered for the specified Instance 
	///
	/// Parameters
	///   instanceId - The numeric identifier of the Instance
	///
	/// Return value
	///   Returns a pointer to the InstanceFunctionTuple that contains the registered functions for this Instance.
	///   Returns NULL if there are no functions registered for this Instance.
	///
	static InstanceFunctionTuple* getInstanceFunctions( unsigned int mapId );


	///
	/// Retrieve the dummy spell effect handler function for the specified spell
	///
	/// Parameters
	///   spellId - The numeric identifier of the spell
	///
	/// Return value
	///   Returns a pointer to the dummy spell handler function
	///   Returns NULL if there are no functions registered for the spell
	///
	static void* getDummySpellHandler( unsigned long spellId );


	///
	/// Retrieve the scripted spell effect handler function for the specified spell
	///
	/// Parameters
	///   spellId - The numeric identifier of the spell
	///
	/// Return value
	///   Returns a pointer to the scripted spell effect handler function
	///   Returns NULL if there are no functions registered for the spell
	///
	static void* getScriptedEffectHandler( unsigned long spellId );

	///
	/// Retrieve the dummy aura handler function for the specified spell
	///
	/// Parameters
	///   spellId - The numeric identifier of the spell
	///
	/// Return value
	///   Returns a pointer to the dummy aura handler function
	///   Returns NULL if there are no functions registered for the aura
	///
	static void* getDummyAuraHandler( unsigned long spellId );

private:
	///
	/// Contains the gossip event handler functions registered for Creatures
	///
	/// key:   Creature Id
	/// value: Pointer to a tuple that contains the functions registered
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* > creatureGossipFunctions;

	///
	/// Contains the gossip event handler functions registered for GameObjects
	///
	/// key:   GameObject Id
	/// value: Pointer to a tuple that contains the functions registered
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* > goGossipFunctions;

	///
	/// Contains the gossip event handler functions registered for Items
	///
	/// key:   Item Id
	/// value: Pointer to a tuple that contains the functions registered
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, GossipFunctionTuple* > itemGossipFunctions;

	///
	/// Contains the event handler functions registered for GameObjects
	///
	/// key: GameObject Id
	/// value: Pointer to a tuple that contains the functions registered
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, GOFunctionTuple* > goFunctions;

	///
	/// Contains the event handler functions registered for Instances
	///
	/// key: Instance map Id
	/// value: Pointer to a tuple that contains the functions registered
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, InstanceFunctionTuple* > instanceFunctions;

	///
	/// Contains the event handler functions registered for Creatures
	///
	/// key: Creature Id
	/// value: Pointer to a tuple that contains the functions registered
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, CreatureFunctionTuple* > creatureFunctions;

	///
	/// Contains the event handler functions registered for Quests
	///
	/// key: Quest Id
	/// value: Pointer to a tuple that contains the functions registered
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, QuestFunctionTuple* > questFunctions;

	///
	/// Contains dummy spell effect handler functions for spells
	///
	/// key: spell Id
	/// value: Pointer to a function that handles the dummy spell
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* > dummySpellFunctions;

	///
	/// Contains scripted spell effect handler functions for spells
	///
	/// key: spell Id
	/// value: Pointer to a function that handles the scripted spell effect
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* > scriptedEffectHandlerFunctions;

	///
	/// Contains dummy spell aura handler functions for spells
	///
	/// key: spell Id
	/// value: Pointer to a function that handles the dummy aura
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned long, void* > dummyAuraFunctions;
};

#endif
