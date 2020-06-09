PROJECT(logon CXX)
SET( prefix ${ROOT_PATH}/src/logon)
SET( sources
  Auth/AccountMgr.cpp
  Auth/AuthSocket.cpp
  AutoPatcher.cpp
  Auth/IPBanner.cpp
  RealmList/RealmSocket.cpp
  LogonConsole.cpp
  RealmList/RealmLogonLinkOpcodes.cpp
  LogonStdAfx.cpp
  Main.cpp
  RealmList/RealmRegistry.cpp
)

 SET( headers
	Auth/AccountMgr.h
	Auth/AuthSocket.h
	Auth/AuthStructs.h
	AutoPatcher.h
	Auth/IPBanner.h
	RealmList/RealmSocket.h
	LogonConfig.h
	LogonConsole.h
	RealmList/RealmLogonLinkOpcodes.h
	LogonStdAfx.h
	Main.h
	PeriodicFunctionCall_Thread.h
	RealmList/RealmRegistry.h
)
foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)
foreach(src IN ITEMS ${headers} )
  SET( HEADERS ${HEADERS} ${prefix}/${src} )
endforeach(src)


include_directories( ${GLOBAL_INCLUDE_DIRS} )
link_directories( ${EXTRA_LIBS_PATH} ${DEPENDENCY_LIBS} )
IF( IS_64BIT )
	link_directories( ${DEPENDENCY_DLLS64} )
ENDIF()

include(PCHSupport)

ADD_EXECUTABLE( ${PROJECT_NAME} ${SRCS} ${HEADERS} )

IF( CMAKE_GENERATOR MATCHES Visual*)
	ADD_NATIVE_PRECOMPILED_HEADER(${PROJECT_NAME} LogonStdAfx.h ${prefix}/LogonStdAfx.cpp)
ENDIF()
#Link our shared lib.
target_link_libraries( ${PROJECT_NAME} shared ${ZLIB_LIBRARIES} ${PCRE_LIBRARIES} )
#Provide make install rules.
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)

