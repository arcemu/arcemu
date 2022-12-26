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
)

SET( HEADERS
${prefix}/engine.h
)

include_directories( ${include_dirs} ${prefix} )
IF( IS_LINUX )
	add_definitions(-Wno-deprecated)
ENDIF()


add_library(${PROJECT_NAME} SHARED ${SRCS} ${HEADERS})
add_dependencies( ${PROJECT_NAME} ${dependancies})
target_link_libraries( ${PROJECT_NAME} ${linkages})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${ARCEMU_SCRIPTLIB_PATH} LIBRARY DESTINATION ${ARCEMU_SCRIPTLIB_PATH})

#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( HEADERS )
