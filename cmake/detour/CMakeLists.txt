project(detour)

SET( prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )
SET( hprefix ${DEPENDENCY_INCLUDE_ROOT}/${PROJECT_NAME} )

SET(sources
DetourAlloc.cpp
DetourCommon.cpp
DetourNavMesh.cpp
DetourNavMeshBuilder.cpp
DetourNavMeshQuery.cpp
DetourNode.cpp
DetourObstacleAvoidance.cpp)

SET(headers
DetourAlloc.h
DetourAssert.h
DetourCommon.h
DetourNavMesh.h
DetourNavMeshBuilder.h
DetourNavMeshQuery.h
DetourNode.h
DetourObstacleAvoidance.h)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)
foreach(src IN ITEMS ${headers} )
  SET( HEADERS ${HEADERS} ${hprefix}/${src} )
endforeach(src)


include_directories( ${GLOBAL_INCLUDE_DIRS} )
add_library(${PROJECT_NAME} STATIC ${SRCS} ${HEADERS} )
#Set it to the project name.
set(DETOUR_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "Detour Libraries")