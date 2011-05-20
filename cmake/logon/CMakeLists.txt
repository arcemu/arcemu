PROJECT(logon CXX)
SET( prefix ${ROOT_PATH}/src/arcemu-logonserver)
SET( sources
  AccountCache.cpp
  AuthSocket.cpp
  AutoPatcher.cpp
  LogonCommServer.cpp
  LogonConsole.cpp
  LogonOpcodes.cpp
  LogonStdAfx.cpp
  Main.cpp
  Main.h)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)


include_directories( ${GLOBAL_INCLUDE_DIRS} )
link_directories( ${EXTRA_LIBS_PATH} )
link_directories( ${DEPENDENCY_LIBS} )
IF( IS_64BIT )
	link_directories( ${DEPENDENCY_DLLS64} )
ELSE()
	link_directories( ${DEPENDENCY_DLLS} )
ENDIF()

include(PCHSupport)
ADD_EXECUTABLE( ${PROJECT_NAME} ${SRCS})
ADD_PRECOMPILED_HEADER(${PROJECT_NAME} ../../src/arcemu-logonserver/LogonStdAfx.h)
#Link our shared lib.
target_link_libraries( ${PROJECT_NAME} shared ${ZLIB_LIBRARIES} ${PCRE_LIBRARIES} )
#Provide make install rules.
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

