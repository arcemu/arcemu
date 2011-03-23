PROJECT(arcemu-logonserver)
SET( prefix ${ROOT_PATH}/src/${PROJECT_NAME} )
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

SET( include_dirs 
${ROOT_PATH}/src/arcemu-shared
${ROOT_PATH}/src/arcemu-logonserver )


include(PCHSupport)
include_directories( ${include_dirs} )
#Support for configurable configs folder name.
ADD_DEFINITIONS(-Wno-deprecated )
ADD_EXECUTABLE( ${PROJECT_NAME} ${SRCS})
#ADD_PRECOMPILED_HEADER( ${PROJECT_NAME} LogonStdAfx.h )
#Make sure arcemu-shared is built before.
add_dependencies( ${PROJECT_NAME} arcemu-shared)
#Link our shared lib.
target_link_libraries( ${PROJECT_NAME} arcemu-shared)
#Provide make install rules.
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

