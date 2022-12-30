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
${prefix}/python/Python.cpp
${prefix}/python/PythonCallable.cpp
${prefix}/python/PythonObject.cpp
${prefix}/python/PythonTuple.cpp
${prefix}/python/PythonValue.cpp
${prefix}/PythonEngine.cpp
${prefix}/ServerHookRegistry.cpp
${prefix}/serverhooks.cpp
)

SET( HEADERS
${prefix}/engine.h
${prefix}/python/Python.hpp
${prefix}/python/PythonCallable.hpp
${prefix}/python/PythonObject.hpp
${prefix}/python/PythonTuple.hpp
${prefix}/python/PythonValue.hpp
${prefix}/PythonEngine.hpp
${prefix}/ServerHookRegistry.hpp
${prefix}/serverhooks.hpp
)

IF( IS_LINUX )
	include_directories( ${include_dirs} ${prefix} ${SCRIPTS_PATH}/python34/src/Include ${SCRIPTS_PATH}/python34/src/platform/Linux )
	add_definitions(-Wno-deprecated)
ELSE()
	include_directories( ${include_dirs} ${prefix} ${SCRIPTS_PATH}/python34/src/Include ${SCRIPTS_PATH}/python34/src/platform/Windows )
ENDIF()


add_library(${PROJECT_NAME} SHARED ${SRCS} ${HEADERS})
add_dependencies( ${PROJECT_NAME} ${dependancies})
target_link_libraries( ${PROJECT_NAME} ${linkages} python34 )
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${ARCEMU_SCRIPTLIB_PATH} LIBRARY DESTINATION ${ARCEMU_SCRIPTLIB_PATH})

#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( HEADERS )
