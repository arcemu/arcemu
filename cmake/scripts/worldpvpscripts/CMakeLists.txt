if(NOT BUILD_WORLDPVPSCRIPTS )
  message("\tWorldPVPScripts was skipped because BUILD_WORLDPVPSCRIPTS is unchecked.")
  return()
endif(NOT BUILD_WORLDPVPSCRIPTS)

PROJECT( WorldPvPScripts )

SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )
#dependancies that must be built before this project.
SET( dependancies arcemu-world )
#libraries/archives that must be linked to this project.
SET( linkages arcemu-world )

#sources -contains source files for our library.
SET( sources
Setup.cpp
ZoneHellfirePeninsula.cpp
ZoneSilithus.cpp
ZoneTerokkarForest.cpp
ZoneZangarmarsh.cpp )

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
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
