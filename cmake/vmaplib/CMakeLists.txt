PROJECT(vmaplib)

set(prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )

set(sources
    BIH.cpp
    MapTree.cpp
    ModelInstance.cpp
    TileAssembler.cpp
    VMapFactory.cpp
    VMapManager2.cpp
    WorldModel.cpp)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${GLOBAL_INCLUDE_DIRS} )
link_directories( ${DEPENDENCY_LIBS} ${EXTRA_LIBS_PATH} )
add_library(${PROJECT_NAME} STATIC ${SRCS} )
target_link_libraries( ${PROJECT_NAME} g3dlite )

unset(sources)
unset(SRCS)
unset(prefix)

