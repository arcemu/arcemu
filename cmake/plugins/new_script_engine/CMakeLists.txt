if(NOT BUILD_NEW_SCRIPT_ENGINE )
  message("\tNewScriptEngine was skipped because BUILD_NEW_SCRIPT_ENGINE is unchecked.")
  return()
endif(NOT BUILD_NEW_SCRIPT_ENGINE)

project(NewScriptEngine)

#prefix variable - points to the folder which contains the source files.
SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )

#SRCS - contains the source files that compose this project.
SET( SRCS
${prefix}/engine.cpp
${prefix}/python/arcemu_extensions.cpp
${prefix}/python/modules/arcemu_module.cpp
${prefix}/python/modules/ArcPyGameObject.cpp
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
${prefix}/PythonEngine.cpp
${prefix}/ServerHookRegistry.cpp
${prefix}/ServerHookHandler.cpp
)

SET( HEADERS
${prefix}/engine.h
${prefix}/python/Python.hpp
${prefix}/python/PythonCallable.hpp
${prefix}/python/PythonObject.hpp
${prefix}/python/PythonTuple.hpp
${prefix}/python/PythonValue.hpp
${prefix}/python/modules/ArcPyGameObject.hpp
${prefix}/python/modules/ArcPyGuild.hpp
${prefix}/python/modules/ArcPyItem.hpp
${prefix}/python/modules/ArcPyUnit.hpp
${prefix}/python/modules/ArcPyPlayer.hpp
${prefix}/python/modules/ArcPyQuest.hpp
${prefix}/python/modules/ArcPySpell.hpp
${prefix}/python/modules/ArcPyWorldSession.hpp
${prefix}/PythonEngine.hpp
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

add_library(${PROJECT_NAME} SHARED ${SRCS} ${HEADERS})
add_dependencies( ${PROJECT_NAME} ${dependancies} python34 )

IF( IS_LINUX )
    target_link_libraries( ${PROJECT_NAME} ${linkages} python34 pthread m util dl )
ELSE()
    target_link_libraries( ${PROJECT_NAME} ${linkages} python34 )
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
