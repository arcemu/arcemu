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
	
set( headers 
	arcemu_getopt.h
	arcemuConfig.h
	Array.h
	AuthCodes.h
	ByteBuffer.h
	CallBack.h
	CircularQueue.h
	Common.h
	CrashHandler.h
	crc32.h
	CRefcounter.h
	CThreads.h
	Database/DatabaseEnv.h
	DynLib.hpp
	Errors.h
	FastQueue.h
	FindFiles.hpp
	FindFilesResult.hpp
	LocationVector.h
	Log.h
	MapFile.h
	MapFileEntry.h
	MersenneTwister.h
	PerformanceCounter.hpp
	PreallocatedQueue.h
	printStackTrace.h
	RC4Engine.h
	Singleton.h
	StackBuffer.h
	StackWalker.h
	StackTrace.h
	Storage.h
	SysInfo.hpp
	TextFile.h
	TLSObject.h
	Tokenizer.h
	Util.h
	WorldPacket.h
	WoWGuid.h
	Auth/BigNumber.h
	Auth/MD5.h
	Auth/Sha1.h
	Auth/WowCrypt.h
	Config/Config.h
	Config/ConfigEnv.h
	Database/Database.h
	Database/DataStore.h
	Database/Field.h
	Database/MySQLDatabase.h
	Network/CircularBuffer.h
	Network/ListenSocketFreeBSD.h
	Network/ListenSocketLinux.h
	Network/ListenSocketWin32.h
	Network/Network.h
	Network/Socket.h
	Network/SocketMgrFreeBSD.h
	Network/SocketMgrLinux.h
	Network/SocketMgrWin32.h
	Network/SocketOps.h
	Network/SocketDefines.h
	Threading/AtomicBoolean.h
	Threading/AtomicCounter.h
	Threading/AtomicFloat.h
	Threading/AtomicULong.h
	Threading/ConditionVariable.h
	Threading/Guard.h
	Threading/LockedQueue.h
	Threading/Mutex.h
	Threading/Queue.h
	Threading/RWLock.h
	Threading/Threading.h
	Threading/ThreadPool.h
	Threading/ThreadStarter.h
)

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
foreach(src IN ITEMS ${headers} )
  SET(HEADERS ${HEADERS} ${prefix}/${src} )
endforeach(src)

include_directories( ${GLOBAL_INCLUDE_DIRS} )
IF( IS_64BIT )
	link_directories( ${DEPENDENCY_DLLS64} )
ELSE()
	link_directories( ${DEPENDENCY_DLLS} )
ENDIF()
link_directories( ${DEPENDENCY_LIBS} )

ADD_LIBRARY( ${PROJECT_NAME} STATIC ${SRCS} ${HEADERS} )

target_link_libraries( ${PROJECT_NAME} ${ZLIB_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${MYSQL_LIBRARY} ${OPENSSL_LIBRARIES} ${EXTRA_LIBS} )

