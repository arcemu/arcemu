PROJECT(logon CXX)
SET( prefix ${ROOT_PATH}/src/arcemu-logonserver)
SET( sources
  AccountCache.h
  AccountCache.cpp
  AuthSocket.h
  AuthSocket.cpp
  AuthStructs.h
  AuthStructs.h
  AutoPatcher.cpp
  AutoPatcher.h
  LogonCommServer.cpp
  LogonCommServer.h
  LogonConsole.cpp
  LogonConsole.h
  LogonOpcodes.cpp
  LogonOpcodes.h
  LogonStdAfx.cpp
  LogonStdAfx.h
  Main.cpp
  Main.h
  PeriodicFunctionCall_Thread.h )

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)


include_directories( ${GLOBAL_INCLUDE_DIRS} )
link_directories( ${EXTRA_LIBS_PATH} )
IF( IS_64BIT )
	link_directories( ${DEPENDENCY_DLLS64} )
ELSE()
	link_directories( ${DEPENDENCY_DLLS} )
ENDIF()

ADD_EXECUTABLE( ${PROJECT_NAME} ${SRCS})

#Link our shared lib.
target_link_libraries( ${PROJECT_NAME} shared ${ZLIB_LIBRARIES} ${PCRE_LIBRARIES} )
#Provide make install rules.
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

