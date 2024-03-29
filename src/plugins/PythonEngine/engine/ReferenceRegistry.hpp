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

#ifndef APE_REFERENCE_REGISTRY_H_
#define APE_REFERENCE_REGISTRY_H_

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
#include "quest/PythonQuestOOScriptVisitor.hpp"

#include "spells/DummySpellHandlerVisitor.hpp"
#include "spells/ScriptedEffectHandlerVisitor.hpp"
#include "spells/DummyAuraHandlerVisitor.hpp"

#include "creature/CreatureScriptFactoryVisitor.hpp"
#include "gameobject/GameObjectScriptFactoryVisitor.hpp"

#include "gameobject/GOGossipOOScriptVisitor.hpp"

#include "item/ItemGossipOOScriptVisitor.hpp"

#include "creature/CreatureGossipOOScriptVisitor.hpp"

#include "instance/PythonInstanceOOScriptFactoryVisitor.hpp"

/// Organized storage of Python function / object references
class ReferenceRegistry
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
	/// Registers a creature script factory function
	///
	/// Parameters
	///   creatureId    -  The id of the creature that would use the creature script
	///   function      -  The function reference
	///
	/// Return value
	///   None
	///
	static void registerCreatureScriptFactory( unsigned int creatureId, void* function );

	///
	/// Registers a gameobject script factory function
	///
	/// Parameters
	///   goId      -  The id of the gameobject that would use the gameobject script
	///   function  -  The function reference
	///
	/// Return value
	///   None
	///
	static void registerGameObjectScriptFactory( unsigned int goId, void* function );


	///
	/// Registers a instance script factory function
	///
	/// Parameters
	///   mapId      -  The id of the map that would use the instance script
	///   function   -  The function reference
	///
	/// Return value
	///   None
	///
	static void registerInstanceScriptFactory( unsigned int mapId, void* function );


	///
	/// Registers a Python gameobject gossip script
	///
	/// Parameters
	///   goId      -  The id of the gameobject that would use the gossip script
	///   script    -  Reference to the gossip script
	///
	/// Return value
	///   None
	///
	static void registerGOGossipScript( unsigned int goId, void* script );


	///
	/// Registers a Python item gossip script
	///
	/// Parameters
	///   itemId    -  The id of the item that would use the gossip script
	///   script    -  Reference to the gossip script
	///
	/// Return value
	///   None
	///
	static void registerItemGossipScript( unsigned int itemId, void* script );


	///
	/// Registers a Python creature gossip script
	///
	/// Parameters
	///   creatureId    -  The id of the creature that would use the gossip script
	///   script        -  Reference to the gossip script
	///
	/// Return value
	///   None
	///
	static void registerCreatureGossipScript( unsigned int creatureId, void* script );


	///
	/// Registers a Python quest script
	///
	/// Parameters
	///   questId       -  The id of the quest that would use the quest script
	///   script        -  Reference to the quest script
	///
	/// Return value
	///   None
	///
	static void registerQuestScript( unsigned int questId, void* script );


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
	/// Visits all registered creature gossip scripts
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitCreatureGossipScripts( CreatureGossipOOScriptVisitor *visitor );


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
	/// Visits all registered GameObject OO gossip scripts
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitGOGossipOOScripts(  GOGossipOOScriptVisitor *visitor );


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
	/// Visits all registered item gossip script handles
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitItemGossipScripts( ItemGossipOOScriptVisitor *visitor );


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
	/// Visits all registered Quest scripts
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitQuestScripts( PythonQuestOOScriptVisitor *visitor );

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
	/// Visits all registered creature script factory functions
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitCreatureScriptFactories( CreatureScriptFactoryVisitor *visitor );

	///
	/// Visits all registered gameobject script factory functions
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitGameObjectScriptFactories( GameObjectScriptFactoryVisitor *visitor );


	///
	/// Visits all registered instance script factory functions
	///
	/// Parameters
	///   visitor - The visitor object
	///
	/// Return value
	///   None
	///
	static void visitInstanceScriptFactories( PythonInstanceOOScriptFactoryVisitor *visitor );


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


	///
	/// Retrieve the Creature script factory function for the specified creature
	///
	/// Parameters
	///   creatureId - The numeric identifier of the Creature
	///
	/// Return value
	///   Returns a pointer to the Creature script factory function
	///   Returns NULL if there are no functions registered for the specified Id
	///
	static void* getCreatureScriptFactory( unsigned int creatureId );

	///
	/// Retrieve the GameObject script factory function for the specified gameobject
	///
	/// Parameters
	///   goId - The numeric identifier of the GameObject
	///
	/// Return value
	///   Returns a pointer to the GameObject script factory function
	///   Returns NULL if there are no functions registered for the specified Id
	///
	static void* getGameObjectScriptFactory( unsigned int goId );


	///
	/// Retrieve the Instance script factory function for the specified map
	///
	/// Parameters
	///   mapId - The numeric identifier of the map
	///
	/// Return value
	///   Returns a pointer to the Instance script factory function
	///   Returns NULL if there are no functions registered for the specified Id
	///
	static void* getInstanceScriptFactory( unsigned int goId );


	///
	/// Retrieve the quest script for the specified quest
	///
	/// Parameters
	///   questId - The numeric identifier of the quest
	///
	/// Return value
	///   Returns a reference to the quest script
	///   Returns NULL if there is no quest script registered
	///
	static void* getQuestScript( unsigned int questId );

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


	///
	/// Contains factory functions that create Python based Creature Scripts
	///
	/// key: creature Id
	/// value: Pointer to a function that creates a creature script class instance
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, void* > creatureScriptFactories;

	///
	/// Contains factory functions that create Python based GameObject Scripts
	///
	/// key: creature Id
	/// value: Pointer to a function that creates a gameobject script class instance
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, void* > gameobjectScriptFactories;

	///
	/// Contains factory functions that create Python based Instance Scripts
	///
	/// key: map Id
	/// value: Pointer to a function that creates a instance script class instance
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, void* > instanceScriptFactories;

	///
	/// Contains references to Python gameobject gossip scripts
	///
	/// key: gameobject Id
	/// value: Pointer to an object that is a gameobject gossip script
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, void* > goGossipScripts;


	///
	/// Contains references to Python item gossip scripts
	///
	/// key: item Id
	/// value: Pointer to an object that is a item gossip script
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, void* > itemGossipScripts;


	///
	/// Contains references to Python creature gossip scripts
	///
	/// key: item Id
	/// value: Pointer to an object that is a creature gossip script
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, void* > creatureGossipScripts;


	///
	/// Contains references to Python quest scripts
	///
	/// key: item Id
	/// value: Pointer to an object that is a quest script
	///
	static HM_NAMESPACE::HM_HASH_MAP< unsigned int, void* >questScripts;
};

#endif
