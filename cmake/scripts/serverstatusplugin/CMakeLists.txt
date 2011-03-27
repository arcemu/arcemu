if(NOT BUILD_SERVERSTATUSPLUGIN )
  message("\tQuestScripts was skipped because BUILD_QUESTSCRIPTS is unchecked.")
  return()
endif(NOT BUILD_SERVERSTATUSPLUGIN)

PROJECT(ServerStatusPlugin)

SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )
#dependancies that must be built before this project.
SET( dependancies arcemu-world )
#libraries/archives that must be linked to this project.
SET( linkages arcemu-world )

SET( sources 
PerfCounters.h
ServerStatusPlugin.cpp )

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${include_dirs} ${prefix} )
add_definitions(-Wno-deprecated)
add_library( ${PROJECT_NAME} SHARED ${SRCS} )
add_dependencies( ${PROJECT_NAME} ${dependancies} )
#target_link_libraries( ${PROJECT_NAME} ${linkages} )
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( sources )
unset( dependancies )
unset( linkages)
