PROJECT(libmpq_new)

set(prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )

set(sources
common.c
explode.c
extract.c
huffman.c
mpq.c
mpq_libmpq.cpp
wave.c
)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${DEPENDENCY_INCLUDE_ROOT}/${PROJECT_NAME} ${DEPENDENCY_INCLUDE_ROOT}/zlib ${DEPENDENCY_INCLUDE_ROOT}/bzip2 ${DEPENDENCY_INCLUDE_ROOT}/loadlib )
add_library(${PROJECT_NAME} STATIC ${SRCS} )
target_link_libraries( ${PROJECT_NAME} ${ZLIB_LIBRARIES} bzip2 )

unset(sources)
unset(SRCS)
unset(prefix)

