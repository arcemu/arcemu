PROJECT(dbcfile)

set(prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )

set(sources
dbcfile.cpp
)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${DEPENDENCY_INCLUDE_ROOT}/${PROJECT_NAME} ${DEPENDENCY_INCLUDE_ROOT}/loadlib ${DEPENDENCY_INCLUDE_ROOT}/libmpq_new )
link_directories( ${DEPENDENCY_LIBS} ${EXTRA_LIBS_PATH} )
add_library(${PROJECT_NAME} STATIC ${SRCS} )
target_link_libraries( ${PROJECT_NAME} libmpq_new )

unset(sources)
unset(SRCS)
unset(prefix)

