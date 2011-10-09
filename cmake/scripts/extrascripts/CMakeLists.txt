if(NOT BUILD_EXTRASCRIPTS )
  message("\tExtraScripts was skipped because BUILD_EXTRASCRIPTS is unchecked.")
  return()
endif(NOT BUILD_EXTRASCRIPTS)

project(ExtraScripts)

SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )
#SRCS - contains the source files that compose this project.

set( sources
Setup.cpp
NeutralGuards.cpp )

SET( headers
Setup.h )

foreach(src IN ITEMS ${sources} )
  set( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)
foreach(src IN ITEMS ${headers} )
  SET( HEADERS ${HEADERS} ${prefix}/${src} )
endforeach(src)

include_directories( ${include_dirs} )
add_library( ${PROJECT_NAME} SHARED ${SRCS} ${HEADERS} )
add_dependencies( ${PROJECT_NAME} ${dependancies} )
target_link_libraries( ${PROJECT_NAME} ${linkages} )

include(PCHSupport)
IF( CMAKE_GENERATOR MATCHES Visual*)
	ADD_NATIVE_PRECOMPILED_HEADER(${PROJECT_NAME} Setup.h ${prefix}/Setup.cpp)
ENDIF()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${ARCEMU_SCRIPTLIB_PATH} LIBRARY DESTINATION ${ARCEMU_SCRIPTLIB_PATH})
#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( sources )
