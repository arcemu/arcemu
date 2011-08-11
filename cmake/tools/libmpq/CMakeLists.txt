PROJECT(libmpq)

set(prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )

set(sources
common.cpp
explode.cpp
extract.cpp
huffman.cpp
mpq.cpp
mpq_libmpq.cpp
parser.cpp
wave.cpp
)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${DEPENDENCY_INCLUDE_ROOT}/${PROJECT_NAME} ${DEPENDENCY_INCLUDE_ROOT}/zlib )
add_library(${PROJECT_NAME} STATIC ${SRCS} )
target_link_libraries( ${PROJECT_NAME} ${ZLIB_LIBRARIES} )

unset(sources)
unset(SRCS)
unset(prefix)

