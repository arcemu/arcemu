if(NOT BUILD_SERVERSTATUSPLUGIN )
  message("\tServerStatusPlugin was skipped because BUILD_SERVERSTATUSPLUGIN is unchecked.")
  return()
endif(NOT BUILD_SERVERSTATUSPLUGIN)

PROJECT(ServerStatusPlugin)

SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )

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
target_link_libraries( ${PROJECT_NAME} ${linkages} )
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( sources )
unset( dependancies )
unset( linkages)
