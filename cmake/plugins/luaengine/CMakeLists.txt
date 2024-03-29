if(NOT BUILD_LUAENGINE )
  message("\tLuaEngine was skipped because BUILD_LUAENGINE is unchecked.")
  return()
endif(NOT BUILD_LUAENGINE)

project(LuaEngine)

#prefix variable - points to the folder which contains the source files.
SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )

#SRCS - contains the source files that compose this project.
SET( SRCS
${prefix}/LUAEngine.cpp 
)

SET( HEADERS
${prefix}/AuraFunctions.h
${prefix}/FunctionTables.h
${prefix}/GameobjectFunctions.h
${prefix}/GlobalFunctions.h
${prefix}/ItemFunctions.h
${prefix}/LUAEngine.h
${prefix}/LUAFunctions.h
${prefix}/LuaSqlApi.h
${prefix}/PacketFunctions.h
${prefix}/SpellFunctions.h
${prefix}/TaxiFunctions.h
${prefix}/UnitFunctions.h
)

include_directories( ${include_dirs} ${prefix} )
IF( IS_LINUX )
	add_definitions(-Wno-deprecated)
ENDIF()


link_directories( ${EXTRA_LIBS_PATH} ${DEPENDENCY_LIBS} )

add_library(${PROJECT_NAME} SHARED ${SRCS} ${HEADERS})
add_dependencies( ${PROJECT_NAME} ${dependancies} lualib)
IF( WIN32 )
	target_link_libraries( ${PROJECT_NAME} ${linkages} lualib shared)
ELSE()
	target_link_libraries( ${PROJECT_NAME} ${linkages} lualib)
ENDIF()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${ARCEMU_SCRIPTLIB_PATH} LIBRARY DESTINATION ${ARCEMU_SCRIPTLIB_PATH})

#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( HEADERS )
