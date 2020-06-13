if(NOT BUILD_GOSSIPSCRIPTS )
  message("\tGossipScripts was skipped because BUILD_GOSSIPSCRIPTS is unchecked.")
  return()
endif(NOT BUILD_GOSSIPSCRIPTS)

PROJECT(GossipScripts)

SET( prefix ${SCRIPTS_PATH}/GossipScripts )

#SRCS - contains the source files that compose this project.

set( sources
Gossip_Dalaran.cpp
Gossip_Innkeepers.cpp
Gossip_Guard.cpp
Gossip_Trainer.cpp
Gossip_Mulgore.cpp
Gossip_Shattrath.cpp
Gossip_Tanaris.cpp
Gossip_Stormwind.cpp
Gossip_Moonglade.cpp
Gossip_DarkmoonFaire.cpp
Gossip_Theramore.cpp
Gossip_Teldrassil.cpp
Gossip_BoreanTundra.cpp
Gossip_WyrmrestTemple.cpp
Gossip_XpEliminator.cpp
Setup.cpp )

SET( headers
Setup.h )

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
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
