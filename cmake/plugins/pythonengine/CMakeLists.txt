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
${prefix}/creature/PythonCreatureAIScript.cpp
${prefix}/creature/PythonCreatureAIScriptFactory.cpp
${prefix}/engine/FunctionRegistry.cpp
${prefix}/engine/plugin.cpp
${prefix}/engine/PythonEngine.cpp
${prefix}/gameobject/GOScriptRegisterer.cpp
${prefix}/gameobject/GOGossipScriptRegisterer.cpp
${prefix}/gameobject/GOGossipScriptReleaser.cpp
${prefix}/gameobject/PythonGameObjectAIScript.cpp
${prefix}/gameobject/PythonGameObjectAIScriptFactory.cpp
${prefix}/gossip/PythonGossipScript.cpp
${prefix}/instance/InstanceScriptRegisterer.cpp
${prefix}/instance/PythonInstanceScript.cpp
${prefix}/instance/PythonInstanceScriptFactory.cpp
${prefix}/item/ItemGossipScriptRegisterer.cpp
${prefix}/item/ItemGossipScriptReleaser.cpp
${prefix}/python/arcemu_extensions.cpp
${prefix}/python/ArcPyTuple.cpp
${prefix}/python/ArcPython.cpp
${prefix}/python/modules/arcemu_module.cpp
${prefix}/python/modules/arcemu_module_consts.cpp
${prefix}/python/modules/ArcPyAura.cpp
${prefix}/python/modules/ArcPyGameObject.cpp
${prefix}/python/modules/ArcPyGossipMenu.cpp
${prefix}/python/modules/ArcPyGuild.cpp
${prefix}/python/modules/ArcPyItem.cpp
${prefix}/python/modules/ArcPyUnit.cpp
${prefix}/python/modules/ArcPyPlayer.cpp
${prefix}/python/modules/ArcPyQuest.cpp
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
${prefix}/serverhooks/ServerHookRegistry.cpp
${prefix}/serverhooks/ServerHookHandler.cpp
)

SET( HEADERS
${prefix}/creature/CreatureFunctionRegisterer.hpp
${prefix}/creature/CreatureFunctionTuple.hpp
${prefix}/creature/CreatureFunctionTupleVisitor.hpp
${prefix}/creature/CreatureGossipScriptRegisterer.hpp
${prefix}/creature/CreatureGossipScriptReleaser.hpp
${prefix}/creature/PythonCreatureAIScript.hpp
${prefix}/creature/PythonCreatureAIScriptFactory.hpp
${prefix}/creature/PythonCreatureEventTypes.hpp
${prefix}/engine/FunctionRegistry.hpp
${prefix}/engine/PythonEngine.hpp
${prefix}/engine/FunctionTuple.hpp
${prefix}/gameobject/GOFunctionTuple.hpp
${prefix}/gameobject/GOFunctionTupleVisitor.hpp
${prefix}/gameobject/GOGossipScriptRegisterer.hpp
${prefix}/gameobject/GOGossipScriptReleaser.hpp
${prefix}/gameobject/GOScriptRegisterer.hpp
${prefix}/gameobject/PythonGameObjectAIScript.hpp
${prefix}/gameobject/PythonGameObjectAIScriptFactory.hpp
${prefix}/gameobject/PythonGOEventTypes.hpp
${prefix}/gossip/GossipFunctionTuple.hpp
${prefix}/gossip/GossipFunctionTupleVisitor.hpp
${prefix}/gossip/PythonGossipEventTypes.hpp
${prefix}/gossip/PythonGossipScript.hpp
${prefix}/instance/InstanceFunctionTuple.hpp
${prefix}/instance/InstanceFunctionTupleVisitor.hpp
${prefix}/instance/InstanceScriptRegisterer.hpp
${prefix}/instance/PythonInstanceEventTypes.hpp
${prefix}/instance/PythonInstanceScript.hpp
${prefix}/instance/PythonInstanceScriptFactory.hpp
${prefix}/item/ItemGossipScriptRegisterer.hpp
${prefix}/item/ItemGossipScriptReleaser.hpp
${prefix}/python/ArcPyTuple.hpp
${prefix}/python/ArcPython.hpp
${prefix}/python/Python.hpp
${prefix}/python/PythonCallable.hpp
${prefix}/python/PythonObject.hpp
${prefix}/python/PythonTuple.hpp
${prefix}/python/PythonValue.hpp
${prefix}/python/modules/ArcPyAura.hpp
${prefix}/python/modules/ArcPyGameObject.hpp
${prefix}/python/modules/ArcPyGossipMenu.hpp
${prefix}/python/modules/ArcPyGuild.hpp
${prefix}/python/modules/ArcPyItem.hpp
${prefix}/python/modules/ArcPyUnit.hpp
${prefix}/python/modules/ArcPyPlayer.hpp
${prefix}/python/modules/ArcPyQuest.hpp
${prefix}/python/modules/ArcPySpell.hpp
${prefix}/python/modules/ArcPyWorldSession.hpp
${prefix}/quest/PythonQuestEventTypes.hpp
${prefix}/quest/PythonQuestScript.hpp
${prefix}/quest/PythonQuestScriptFactory.hpp
${prefix}/quest/QuestFunctionTuple.hpp
${prefix}/quest/QuestFunctionTupleVisitor.hpp
${prefix}/quest/QuestScriptRegisterer.hpp
${prefix}/serverhooks/ServerHookRegistry.hpp
${prefix}/serverhooks/ServerHookHandler.hpp
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

IF( INSTALL_EXAMPLE_PYTHON_SCRIPTS )
	SET( PYTHON_SCRIPTS_PATH ${ROOT_PATH}/src/scripts/pythonscripts )
	FILE( GLOB PYTHON_SCRIPTS ${PYTHON_SCRIPTS_PATH}/*.py )
	INSTALL(FILES ${PYTHON_SCRIPTS} DESTINATION ./pythonscripts )
ENDIF()

UNSET(PYTHON_SCRIPTS)
unset( prefix)
unset( SRCS )
unset( HEADERS )
