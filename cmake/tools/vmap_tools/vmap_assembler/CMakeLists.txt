PROJECT(vmap_assembler CXX)
SET( prefix ${ROOT_PATH}/src/tools/vmap_tools/vmap_assembler)
SET( sources
vmap_assembler.cpp
)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${DEPENDENCY_INCLUDE_ROOT}/vmaplib ${DEPENDENCY_INCLUDE_ROOT}/g3dlite )

ADD_EXECUTABLE( ${PROJECT_NAME} ${SRCS} )

IF( APPLE )
	target_link_libraries( ${PROJECT_NAME} vmaplib g3dlite ${ZLIB_LIBRARIES} "-framework Carbon" )
ELSE( APPLE )
	target_link_libraries( ${PROJECT_NAME} vmaplib g3dlite ${ZLIB_LIBRARIES} )
ENDIF( APPLE )

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${ARCEMU_TOOLS_PATH} )
