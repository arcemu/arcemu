PROJECT(bzip2)

set(prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )

set(sources
blocksort.c
bzlib.c
compress.c
crctable.c
decompress.c
huffman.c
randtable.c
)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${DEPENDENCY_INCLUDE_ROOT}/${PROJECT_NAME} )
add_library(${PROJECT_NAME} STATIC ${SRCS} )
target_link_libraries( ${PROJECT_NAME} )

unset(sources)
unset(SRCS)
unset(prefix)

