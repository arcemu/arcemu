if(NOT BUILD_EXTRASCRIPTS )
  message("\tExtraScripts was skipped because BUILD_EXTRASCRIPTS is unchecked.")
  return()
endif(NOT BUILD_EXTRASCRIPTS)

project(ExtraScripts)

SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )
#dependancies that must be built before this project.
SET( dependancies arcemu-world )
#libraries/archives that must be linked to this project.
SET( linkages arcemu-world )
#SRCS - contains the source files that compose this project.

set( sources
Setup.cpp
NeutralGuards.cpp )

foreach(src IN ITEMS ${sources} )
  set( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${include_dirs} )
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
