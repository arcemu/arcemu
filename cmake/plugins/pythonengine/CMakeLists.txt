if(NOT BUILD_PYTHONENGINE )
  message("\tArcemu Python Engine (APE) was skipped because BUILD_PYTHONENGINE is unchecked.")
  return()
endif(NOT BUILD_PYTHONENGINE)

SET( INSTALL_EXAMPLE_PYTHON_SCRIPTS FALSE CACHE BOOL "Install example Python scripts")

project(PythonEngine)

SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )

SET( SRCS
${prefix}/creature/CreatureFunctionRegisterer.cpp
${prefix}/creature/CreatureGossipScriptRegisterer.cpp
${prefix}/creature/CreatureGossipScriptReleaser.cpp
${prefix}/creature/CreatureGossipOOScriptRegisterer.cpp
${prefix}/creature/CreatureGossipOOScriptReleaser.cpp
${prefix}/creature/PythonCreatureAIScript.cpp
${prefix}/creature/PythonCreatureAIScriptFactory.cpp
${prefix}/creature/PythonCreatureOOScript.cpp
${prefix}/creature/PythonCreatureOOScriptFactory.cpp
${prefix}/creature/PythonCreatureOOScriptRegisterer.cpp
${prefix}/engine/ReferenceRegistry.cpp
${prefix}/engine/plugin.cpp
${prefix}/engine/PythonEngine.cpp
${prefix}/gameobject/GOScriptRegisterer.cpp
${prefix}/gameobject/GOGossipScriptRegisterer.cpp
${prefix}/gameobject/GOGossipScriptReleaser.cpp
${prefix}/gameobject/GOGossipOOScriptRegisterer.cpp
${prefix}/gameobject/GOGossipOOScriptReleaser.cpp
${prefix}/gameobject/PythonGameObjectAIScript.cpp
${prefix}/gameobject/PythonGameObjectAIScriptFactory.cpp
${prefix}/gameobject/PythonGameObjectOOScript.cpp
${prefix}/gameobject/PythonGameObjectOOScriptFactory.cpp
${prefix}/gameobject/PythonGameObjectOOScriptRegisterer.cpp
${prefix}/gossip/PythonGossipScript.cpp
${prefix}/gossip/PythonGossipOOScript.cpp
${prefix}/instance/InstanceScriptRegisterer.cpp
${prefix}/instance/PythonInstanceScript.cpp
${prefix}/instance/PythonInstanceScriptFactory.cpp
${prefix}/instance/PythonInstanceOOScriptRegisterer.cpp
${prefix}/instance/PythonInstanceOOScript.cpp
${prefix}/instance/PythonInstanceOOScriptFactory.cpp
${prefix}/item/ItemGossipScriptRegisterer.cpp
${prefix}/item/ItemGossipScriptReleaser.cpp
${prefix}/item/ItemGossipOOScriptRegisterer.cpp
${prefix}/item/ItemGossipOOScriptReleaser.cpp
${prefix}/python/arcemu_extensions.cpp
${prefix}/python/ArcPyTuple.cpp
${prefix}/python/ArcPython.cpp
${prefix}/python/modules/arcemu_module.cpp
${prefix}/python/modules/arcemu_module_consts.cpp
${prefix}/python/modules/ArcPyAura.cpp
${prefix}/python/modules/ArcPyCreature.cpp
${prefix}/python/modules/ArcPyCreatureScript.cpp
${prefix}/python/modules/ArcPyGameObject.cpp
${prefix}/python/modules/ArcPyGameObjectScript.cpp
${prefix}/python/modules/ArcPyGossipMenu.cpp
${prefix}/python/modules/ArcPyGossipScript.cpp
${prefix}/python/modules/ArcPyGuild.cpp
${prefix}/python/modules/ArcPyInstanceScript.cpp
${prefix}/python/modules/ArcPyItem.cpp
${prefix}/python/modules/ArcPyMapMgr.cpp
${prefix}/python/modules/ArcPyMath.cpp
${prefix}/python/modules/ArcPyObject.cpp
${prefix}/python/modules/ArcPyObjectLocator.cpp
${prefix}/python/modules/ArcPyUnit.cpp
${prefix}/python/modules/ArcPyPlayer.cpp
${prefix}/python/modules/ArcPyQuest.cpp
${prefix}/python/modules/ArcPyQuestScript.cpp
${prefix}/python/modules/ArcPySpell.cpp
${prefix}/python/modules/ArcPyWorldSession.cpp
${prefix}/python/Python.cpp
${prefix}/python/PythonCallable.cpp
${prefix}/python/PythonObject.cpp
${prefix}/python/PythonTuple.cpp
${prefix}/python/PythonValue.cpp
${prefix}/quest/PythonQuestScriptFactory.cpp
${prefix}/quest/PythonQuestScript.cpp
${prefix}/quest/QuestScriptRegisterer.cpp
${prefix}/quest/PythonQuestOOScriptFactory.cpp
${prefix}/quest/PythonQuestOOScript.cpp
${prefix}/quest/PythonQuestOOScriptRegisterer.cpp
${prefix}/serverhooks/ServerHookRegistry.cpp
${prefix}/serverhooks/ServerHookHandler.cpp
${prefix}/spells/DummyAuraHandlerRegisterer.cpp
${prefix}/spells/DummySpellHandlerRegisterer.cpp
${prefix}/spells/ScriptedEffectHandlerRegisterer.cpp
${prefix}/spells/SpellScriptHandler.cpp
)

SET( HEADERS
${prefix}/creature/CreatureFunctionRegisterer.hpp
${prefix}/creature/CreatureFunctionTuple.hpp
${prefix}/creature/CreatureFunctionTupleVisitor.hpp
${prefix}/creature/CreatureGossipScriptRegisterer.hpp
${prefix}/creature/CreatureGossipScriptReleaser.hpp
${prefix}/creature/CreatureGossipOOScriptRegisterer.hpp
${prefix}/creature/CreatureGossipOOScriptReleaser.hpp
${prefix}/creature/CreatureGossipOOScriptVisitor.hpp
${prefix}/creature/CreatureScriptFactoryVisitor.hpp
${prefix}/creature/PythonCreatureAIScript.hpp
${prefix}/creature/PythonCreatureAIScriptFactory.hpp
${prefix}/creature/PythonCreatureEventTypes.hpp
${prefix}/creature/PythonCreatureOOScript.hpp
${prefix}/creature/PythonCreatureOOScriptFactory.hpp
${prefix}/creature/PythonCreatureOOScriptRegisterer.hpp
${prefix}/engine/ReferenceRegistry.hpp
${prefix}/engine/PythonEngine.hpp
${prefix}/engine/FunctionTuple.hpp
${prefix}/gameobject/GameObjectScriptFactoryVisitor.hpp
${prefix}/gameobject/GOFunctionTuple.hpp
${prefix}/gameobject/GOFunctionTupleVisitor.hpp
${prefix}/gameobject/GOGossipScriptRegisterer.hpp
${prefix}/gameobject/GOGossipScriptReleaser.hpp
${prefix}/gameobject/GOGossipOOScriptRegisterer.hpp
${prefix}/gameobject/GOGossipOOScriptReleaser.hpp
${prefix}/gameobject/GOGossipOOScriptVisitor.hpp
${prefix}/gameobject/GOScriptRegisterer.hpp
${prefix}/gameobject/PythonGameObjectAIScript.hpp
${prefix}/gameobject/PythonGameObjectAIScriptFactory.hpp
${prefix}/gameobject/PythonGameObjectOOScript.hpp
${prefix}/gameobject/PythonGameObjectOOScriptFactory.hpp
${prefix}/gameobject/PythonGameObjectOOScriptRegisterer.hpp
${prefix}/gameobject/PythonGOEventTypes.hpp
${prefix}/gossip/GossipFunctionTuple.hpp
${prefix}/gossip/GossipFunctionTupleVisitor.hpp
${prefix}/gossip/PythonGossipEventTypes.hpp
${prefix}/gossip/PythonGossipScript.hpp
${prefix}/gossip/PythonGossipOOScript.hpp
${prefix}/instance/InstanceFunctionTuple.hpp
${prefix}/instance/InstanceFunctionTupleVisitor.hpp
${prefix}/instance/InstanceScriptRegisterer.hpp
${prefix}/instance/PythonInstanceEventTypes.hpp
${prefix}/instance/PythonInstanceScript.hpp
${prefix}/instance/PythonInstanceScriptFactory.hpp
${prefix}/instance/PythonInstanceOOScript.hpp
${prefix}/instance/PythonInstanceOOScriptFactory.hpp
${prefix}/instance/PythonInstanceOOScriptRegisterer.hpp
${prefix}/instance/PythonInstanceOOScriptFactoryVisitor.hpp
${prefix}/item/ItemGossipScriptRegisterer.hpp
${prefix}/item/ItemGossipScriptReleaser.hpp
${prefix}/item/ItemGossipOOScriptRegisterer.hpp
${prefix}/item/ItemGossipOOScriptReleaser.hpp
${prefix}/item/ItemGossipOOScriptVisitor.hpp
${prefix}/python/ArcPyTuple.hpp
${prefix}/python/ArcPython.hpp
${prefix}/python/Python.hpp
${prefix}/python/PythonCallable.hpp
${prefix}/python/PythonObject.hpp
${prefix}/python/PythonTuple.hpp
${prefix}/python/PythonValue.hpp
${prefix}/python/modules/ArcPyAura.hpp
${prefix}/python/modules/ArcPyCreature.hpp
${prefix}/python/modules/ArcPyCreatureScript.hpp
${prefix}/python/modules/ArcPyGameObject.hpp
${prefix}/python/modules/ArcPyGameObjectScript.hpp
${prefix}/python/modules/ArcPyGossipMenu.hpp
${prefix}/python/modules/ArcPyGossipScript.hpp
${prefix}/python/modules/ArcPyGuild.hpp
${prefix}/python/modules/ArcPyInstanceScript.hpp
${prefix}/python/modules/ArcPyItem.hpp
${prefix}/python/modules/ArcPyMapMgr.hpp
${prefix}/python/modules/ArcPyMath.hpp
${prefix}/python/modules/ArcPyObject.hpp
${prefix}/python/modules/ArcPyObjectLocator.hpp
${prefix}/python/modules/ArcPyUnit.hpp
${prefix}/python/modules/ArcPyPlayer.hpp
${prefix}/python/modules/ArcPyQuest.hpp
${prefix}/python/modules/ArcPyQuestScript.hpp
${prefix}/python/modules/ArcPySpell.hpp
${prefix}/python/modules/ArcPyWorldSession.hpp
${prefix}/quest/PythonQuestEventTypes.hpp
${prefix}/quest/PythonQuestScript.hpp
${prefix}/quest/PythonQuestScriptFactory.hpp
${prefix}/quest/QuestFunctionTuple.hpp
${prefix}/quest/QuestFunctionTupleVisitor.hpp
${prefix}/quest/QuestScriptRegisterer.hpp
${prefix}/quest/PythonQuestOOScript.hpp
${prefix}/quest/PythonQuestOOScriptFactory.hpp
${prefix}/quest/PythonQuestOOScriptRegisterer.hpp
${prefix}/quest/PythonQuestOOScriptVisitor.hpp
${prefix}/serverhooks/ServerHookRegistry.hpp
${prefix}/serverhooks/ServerHookHandler.hpp
${prefix}/spells/DummyAuraHandlerRegisterer.hpp
${prefix}/spells/DummyAuraHandlerVisitor.hpp
${prefix}/spells/DummySpellHandlerRegisterer.hpp
${prefix}/spells/DummySpellHandlerVisitor.hpp
${prefix}/spells/ScriptedEffectHandlerRegisterer.hpp
${prefix}/spells/ScriptedEffectHandlerVisitor.hpp
${prefix}/spells/SpellScriptHandler.hpp
)

IF( IS_LINUX )
	include_directories( ${include_dirs} ${prefix} ${SCRIPTS_PATH}/python34/src/Include ${SCRIPTS_PATH}/python34/src/platform/Linux )
	add_definitions(-Wno-deprecated)
ELSE()
	include_directories( ${include_dirs} ${prefix} ${SCRIPTS_PATH}/python34/src/Include ${SCRIPTS_PATH}/python34/src/platform/Windows )
ENDIF()

IF(NOT CMAKE_GENERATOR MATCHES Visual*)
    SET( CMAKE_INSTALL_RPATH "./" )
ENDIF()

link_directories( ${EXTRA_LIBS_PATH} ${DEPENDENCY_LIBS} )

add_library(${PROJECT_NAME} SHARED ${SRCS} ${HEADERS})
add_dependencies( ${PROJECT_NAME} ${dependancies} python34 )

IF( IS_LINUX )
    target_link_libraries( ${PROJECT_NAME} ${linkages} python34 pthread m util dl )
ELSE()
    target_link_libraries( ${PROJECT_NAME} ${linkages} python34 shared)
ENDIF()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${ARCEMU_SCRIPTLIB_PATH} LIBRARY DESTINATION ${ARCEMU_SCRIPTLIB_PATH})

INSTALL(DIRECTORY DESTINATION ./pythonscripts)

SET( PYTHON_SCRIPTS_PATH ${ROOT_PATH}/src/scripts/pythonscripts )
FILE( GLOB PYTHON_SCRIPTS ${PYTHON_SCRIPTS_PATH}/prod/*.py )
INSTALL(FILES ${PYTHON_SCRIPTS} DESTINATION ./pythonscripts )

IF( INSTALL_EXAMPLE_PYTHON_SCRIPTS )
	INSTALL(DIRECTORY DESTINATION ./pythonscripts/dev)
	FILE( GLOB PYTHON_SCRIPTS ${PYTHON_SCRIPTS_PATH}/dev/*.py )
	INSTALL(FILES ${PYTHON_SCRIPTS} DESTINATION ./pythonscripts/dev )
ENDIF()

UNSET(PYTHON_SCRIPTS)
unset( prefix)
unset( SRCS )
unset( HEADERS )
