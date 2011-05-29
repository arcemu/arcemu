if(NOT BUILD_MISCSCRIPTS )
  message("\tMiscScripts was skipped because BUILD_MISCSCRIPTS is unchecked.")
  return()
endif(NOT BUILD_MISCSCRIPTS)

PROJECT(MiscScripts)

SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )

#sources -contains source files for our library.
SET( sources
Creatures.cpp
DarkmoonFaireObjects.cpp
GameObjects.cpp
GameObjectTeleportTable.cpp
RandomScripts.cpp
Setup.cpp )

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

IF( APPLE )
	SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -undefined dynamic_lookup" )
	SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -undefined dynamic_lookup" )
ENDIF( APPLE )

include_directories( ${include_dirs} )
add_library( ${PROJECT_NAME} SHARED ${SRCS} )
add_dependencies( ${PROJECT_NAME} ${dependancies} )
target_link_libraries( ${PROJECT_NAME} ${linkages} )
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${ARCEMU_SCRIPTLIB_PATH} LIBRARY DESTINATION ${ARCEMU_SCRIPTLIB_PATH})
#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( sources )