if(NOT BUILD_MISCSCRIPTS )
  message("\tMiscScripts was skipped because BUILD_MISCSCRIPTS is unchecked.")
  return()
endif(NOT BUILD_MISCSCRIPTS)

PROJECT(MiscScripts)

SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )
#dependancies that must be built before this project.
SET( dependancies arcemu-world )
#libraries/archives that must be linked to this project.
SET( linkages arcemu-world )

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

include_directories( ${include_dirs} )
add_definitions(-Wno-deprecated)
add_library( ${PROJECT_NAME} SHARED ${SRCS} )
add_dependencies( ${PROJECT_NAME} ${dependancies} )
target_link_libraries( ${PROJECT_NAME} ${linkages} )
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( sources )
unset( dependancies )
unset( linkages)