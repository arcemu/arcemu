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

include_directories( ${include_dirs} ${prefix} )
add_definitions(-Wno-deprecated)
add_library(${PROJECT_NAME} SHARED ${SRCS} )
add_dependencies( ${PROJECT_NAME} ${dependancies} lualib)
target_link_libraries( ${PROJECT_NAME} ${linkages} lualib)
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)

#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( dependancies )
unset( linkages)
