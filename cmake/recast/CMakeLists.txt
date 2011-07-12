project(recast)

SET( prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )

SET(sources
Recast.cpp
RecastAlloc.cpp
RecastArea.cpp
RecastContour.cpp
RecastFilter.cpp
RecastMesh.cpp
RecastMeshDetail.cpp
RecastRasterization.cpp
RecastRegion.cpp)

SET(headers
Recast.h
RecastAlloc.h
RecastAssert.h)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)
foreach(src IN ITEMS ${headers} )
  SET( HEADERS ${HEADERS} ${prefix}/${src} )
endforeach(src)

include_directories( ${GLOBAL_INCLUDE_DIRS} )
add_library(${PROJECT_NAME} STATIC ${SRCS} )
#Set it to the project name.
set(RECAST_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "recast Libraries")