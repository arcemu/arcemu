if(NOT BUILD_WORLDPVPSCRIPTS )
  message("\tWorldPvPScripts was skipped because BUILD_WORLDPVPSCRIPTS is unchecked.")
  return()
endif(NOT BUILD_WORLDPVPSCRIPTS)

PROJECT(WorldPvPScripts)

SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )

SET( sources 
ZoneHellfirePeninsula.cpp
ZoneSilithus.cpp
ZoneZangarmarsh.cpp
ZoneTerokkarForest.cpp
ZoneNagrand.cpp
ZoneWintergrasp.cpp
Setup.cpp )

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${include_dirs} ${prefix} )
add_library( ${PROJECT_NAME} SHARED ${SRCS} )
add_dependencies( ${PROJECT_NAME} ${dependancies} )
target_link_libraries( ${PROJECT_NAME} ${linkages} )
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${ARCEMU_SCRIPTLIB_PATH} LIBRARY DESTINATION ${ARCEMU_SCRIPTLIB_PATH})
#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( sources )