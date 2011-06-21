PROJECT(shared)

SET( prefix ${ROOT_PATH}/src/arcemu-shared )
SET( sources
    CThreads.cpp 
    CrashHandler.cpp 
    Log.cpp 
    MersenneTwister.cpp 
    printStackTrace.cpp 
    Util.cpp 
    crc32.cpp 
    MapFile.cpp
    MapFileEntry.cpp
    TextFile.cpp
    FindFiles.cpp
    DynLib.cpp
    SysInfo.cpp
    PerformanceCounter.cpp
    Threading/Mutex.cpp
    Threading/Threading.h  
    Threading/ThreadPool.cpp 
    Threading/AtomicULong.cpp 
    Threading/AtomicFloat.cpp 
    Threading/AtomicCounter.cpp 
    Threading/AtomicBoolean.cpp 
    Threading/ConditionVariable.cpp
    arcemu_getopt.cpp 
    Auth/BigNumber.cpp 
    Auth/MD5.cpp 
    Auth/Sha1.cpp 
    Auth/WowCrypt.cpp 
    Config/Config.cpp 
    Database/Database.cpp 
    Database/MySQLDatabase.cpp 
    Database/CreateInterface.cpp 
    Network/CircularBuffer.cpp 
    Network/Socket.cpp  )

IF(WIN32)
	SET( sources ${sources} Network/SocketMgrWin32.cpp  Network/SocketOpsWin32.cpp  Network/SocketWin32.cpp StackWalker.cpp StackTrace.cpp )
ELSEIF(IS_FREEBSD OR APPLE)
	SET( sources ${sources}  Network/SocketMgrFreeBSD.cpp  Network/SocketFreeBSD.cpp  Network/SocketOpsFreeBSD.cpp )
ELSE(WIN32)
	SET( sources ${sources}  Network/SocketMgrLinux.cpp  Network/SocketLinux.cpp  Network/SocketOpsLinux.cpp )
ENDIF(WIN32)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${GLOBAL_INCLUDE_DIRS} )
IF( IS_64BIT )
	link_directories( ${DEPENDENCY_DLLS64} )
ELSE()
	link_directories( ${DEPENDENCY_DLLS} )
ENDIF()
link_directories( ${DEPENDENCY_LIBS} )
ADD_LIBRARY( ${PROJECT_NAME} STATIC ${SRCS} )

target_link_libraries( ${PROJECT_NAME} ${ZLIB_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${MYSQL_LIBRARY} ${OPENSSL_LIBRARIES} ${EXTRA_LIBS} )

