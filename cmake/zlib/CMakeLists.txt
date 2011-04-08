project(zlib C)

SET( prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )

SET(sources
adler32.c
compress.c
crc32.c
deflate.c
example.c
gzio.c
infback.c
inffast.c
inflate.c
inftrees.c
trees.c
uncompr.c
zutil.c)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${DEPENDENCY_INCLUDE_ROOT}/zlib)
add_library(${PROJECT_NAME} STATIC ${SRCS} )
#Set it to the project name.
set(ZLIB_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "Zlib Libraries")