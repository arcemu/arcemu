if(NOT BUILD_PYTHONENGINE )
  message("\tArcemu Python Engine (APE) was skipped because BUILD_PYTHONENGINE is unchecked.")
  return()
endif(NOT BUILD_PYTHONENGINE)

project(PythonEngine)

#prefix variable - points to the folder which contains the source files.
SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )

#SRCS - contains the source files that compose this project.
SET( SRCS
${prefix}/plugin.cpp
${prefix}/python/arcemu_extensions.cpp
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
${prefix}/python/ArcPyTuple.cpp
${prefix}/python/ArcPython.cpp
${prefix}/python/Python.cpp
${prefix}/python/PythonCallable.cpp
${prefix}/python/PythonObject.cpp
${prefix}/python/PythonTuple.cpp
${prefix}/python/PythonValue.cpp
${prefix}/creature/PythonCreatureAIScript.cpp
${prefix}/creature/PythonCreatureAIScriptFactory.cpp
${prefix}/PythonEngine.cpp
${prefix}/gameobject/PythonGameObjectAIScript.cpp
${prefix}/gameobject/PythonGameObjectAIScriptFactory.cpp
${prefix}/PythonInstanceScript.cpp
${prefix}/PythonInstanceScriptFactory.cpp
${prefix}/PythonGossipScript.cpp
${prefix}/PythonQuestScriptFactory.cpp
${prefix}/PythonQuestScript.cpp
${prefix}/creature/CreatureFunctionRegisterer.cpp
${prefix}/creature/CreatureGossipScriptRegisterer.cpp
${prefix}/creature/CreatureGossipScriptReleaser.cpp
${prefix}/FunctionRegistry.cpp
${prefix}/gameobject/GOScriptRegisterer.cpp
${prefix}/gameobject/GOGossipScriptRegisterer.cpp
${prefix}/gameobject/GOGossipScriptReleaser.cpp
${prefix}/InstanceScriptRegisterer.cpp
${prefix}/ItemGossipScriptRegisterer.cpp
${prefix}/ItemGossipScriptReleaser.cpp
${prefix}/QuestScriptRegisterer.cpp
${prefix}/ServerHookRegistry.cpp
${prefix}/ServerHookHandler.cpp
)

SET( HEADERS
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
${prefix}/creature/PythonCreatureEventTypes.hpp
${prefix}/PythonEngine.hpp
${prefix}/creature/PythonCreatureAIScript.hpp
${prefix}/creature/PythonCreatureAIScriptFactory.hpp
${prefix}/gameobject/PythonGameObjectAIScript.hpp
${prefix}/gameobject/PythonGameObjectAIScriptFactory.hpp
${prefix}/gameobject/PythonGOEventTypes.hpp
${prefix}/PythonGossipEventTypes.hpp
${prefix}/PythonGossipScript.hpp
${prefix}/PythonInstanceEventTypes.hpp
${prefix}/PythonInstanceScript.hpp
${prefix}/PythonInstanceScriptFactory.hpp
${prefix}/PythonQuestEventTypes.hpp
${prefix}/PythonQuestScript.hpp
${prefix}/PythonQuestScriptFactory.hpp
${prefix}/creature/CreatureFunctionRegisterer.hpp
${prefix}/creature/CreatureFunctionTuple.hpp
${prefix}/creature/CreatureFunctionTupleVisitor.hpp
${prefix}/creature/CreatureGossipScriptRegisterer.hpp
${prefix}/creature/CreatureGossipScriptReleaser.hpp
${prefix}/FunctionRegistry.hpp
${prefix}/gameobject/GOFunctionTuple.hpp
${prefix}/gameobject/GOFunctionTupleVisitor.hpp
${prefix}/GossipFunctionTuple.hpp
${prefix}/GossipFunctionTupleVisitor.hpp
${prefix}/gameobject/GOScriptRegisterer.hpp
${prefix}/gameobject/GOGossipScriptRegisterer.hpp
${prefix}/gameobject/GOGossipScriptReleaser.hpp
${prefix}/InstanceFunctionTuple.hpp
${prefix}/InstanceFunctionTupleVisitor.hpp
${prefix}/InstanceScriptRegisterer.hpp
${prefix}/ItemGossipScriptRegisterer.hpp
${prefix}/ItemGossipScriptReleaser.hpp
${prefix}/QuestFunctionTuple.hpp
${prefix}/QuestFunctionTupleVisitor.hpp
${prefix}/QuestScriptRegisterer.hpp
${prefix}/ServerHookRegistry.hpp
${prefix}/ServerHookHandler.hpp
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

SET( PYTHON_SCRIPTS_PATH ${ROOT_PATH}/src/scripts/pythonscripts )
FILE( GLOB PYTHON_SCRIPTS ${PYTHON_SCRIPTS_PATH}/*.py )
INSTALL(FILES ${PYTHON_SCRIPTS} DESTINATION ./pythonscripts )

#Cleap up our variables(Not required.)
UNSET(PYTHON_SCRIPTS)
unset( prefix)
unset( SRCS )
unset( HEADERS )
