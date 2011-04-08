
if(NOT BUILD_EVENTSCRIPTS )
  message("\tEventScripts was skipped because BUILD_EVENTSCRIPTS is unchecked.")
  return()
endif(NOT BUILD_EVENTSCRIPTS)

PROJECT(EventScripts)

SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )

#sources -contains source files for our library.
SET( sources
Setup.cpp
Event_Halloween.cpp
Event_WinterVeil.cpp
../Common/Base.cpp )

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${include_dirs} ${prefix} )
ADD_DEFINITIONS(-Wno-deprecated)
ADD_LIBRARY( ${PROJECT_NAME} SHARED ${SRCS} )
ADD_DEPENDENCIES( ${PROJECT_NAME} ${dependancies} )
target_link_libraries( ${PROJECT_NAME} ${linkages} )
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( sources )
unset( dependancies )
unset( linkages)
