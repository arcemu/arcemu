PROJECT(WorldPvP)

SET( prefix ${SCRIPTS_PATH}/WorldPvP )

set(sources
plugin.cpp
EasternPlaguelands.cpp
HellfirePeninsula.cpp
Nagrand.cpp
Silithus.cpp
TerokkarForest.cpp
Zangarmarsh.cpp
)

SET( headers
plugin.h
)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

foreach(src IN ITEMS ${headers} )
  SET( HEADERS ${HEADERS} ${prefix}/${src} )
endforeach(src)

link_directories( ${EXTRA_LIBS_PATH} ${DEPENDENCY_LIBS} )

include_directories( ${include_dirs} ${prefix} )
add_library( ${PROJECT_NAME} SHARED ${SRCS} ${HEADERS} )
add_dependencies( ${PROJECT_NAME} ${dependancies} )
target_link_libraries( ${PROJECT_NAME} ${linkages} shared )

include(PCHSupport)
IF( CMAKE_GENERATOR MATCHES Visual*)
	ADD_NATIVE_PRECOMPILED_HEADER(${PROJECT_NAME} plugin.h ${prefix}/plugin.cpp)
ENDIF()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${ARCEMU_SCRIPTLIB_PATH} LIBRARY DESTINATION ${ARCEMU_SCRIPTLIB_PATH})

unset( prefix)
unset( SRCS )
unset( sources )