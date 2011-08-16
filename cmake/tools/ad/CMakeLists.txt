PROJECT(ad CXX)
SET( prefix ${ROOT_PATH}/src/tools/ad)
SET( sources
	System.cpp
)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${DEPENDENCY_INCLUDE_ROOT}/libmpq_new ${DEPENDENCY_INCLUDE_ROOT}/dbcfile ${DEPENDENCY_INCLUDE_ROOT}/loadlib )

ADD_EXECUTABLE( ${PROJECT_NAME} ${SRCS} )

target_link_libraries( ${PROJECT_NAME} dbcfile loadlib ${ZLIB_LIBRARIES} )

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${ARCEMU_TOOLS_PATH} )

