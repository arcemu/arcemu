PROJECT(vmap_extractor CXX)
SET( prefix ${ROOT_PATH}/src/tools/vmap_tools/vmap_extractor)
SET( sources
adtfile.cpp
dbcfile.cpp
model.cpp
vmapexport.cpp
wdtfile.cpp
wmo.cpp
)

SET( headers
adtfile.h
dbcfile.h
model.h
modelheaders.h
vec3d.h
vmapexport.h
wdtfile.h
wmo.h
)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)
foreach(src IN ITEMS ${headers} )
  SET( HEADERS ${HEADERS} ${prefix}/${src} )
endforeach(src)

include_directories( ${DEPENDENCY_INCLUDE_ROOT}/libmpq_new ${DEPENDENCY_INCLUDE_ROOT}/loadlib ${DEPENDENCY_INCLUDE_ROOT}/zlib ${DEPENDENCY_INCLUDE_ROOT}/bzip2 )

ADD_EXECUTABLE( ${PROJECT_NAME} ${SRCS} ${HEADERS} )

target_link_libraries( ${PROJECT_NAME} libmpq_new loadlib ${ZLIB_LIBRARIES} bzip2 )

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${ARCEMU_TOOLS_PATH} )

