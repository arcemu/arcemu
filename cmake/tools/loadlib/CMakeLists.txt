PROJECT(loadlib)

set(prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )

set(sources
adt.cpp
loadlib.cpp
wdt.cpp
)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${DEPENDENCY_INCLUDE_ROOT}/${PROJECT_NAME} )
link_directories( ${DEPENDENCY_LIBS} ${EXTRA_LIBS_PATH} )
add_library(${PROJECT_NAME} STATIC ${SRCS} )
target_link_libraries( ${PROJECT_NAME} )

unset(sources)
unset(SRCS)
unset(prefix)

