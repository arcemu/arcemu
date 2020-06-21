PROJECT(shared)

SET( prefix ${ROOT_PATH}/src/shared )
SET( sources
    System/CrashHandler.cpp 
    Logging/Log.cpp 
    Util/MersenneTwister.cpp 
    System/printStackTrace.cpp 
    Util/Util.cpp 
    System/crc32.cpp 
    System/MapFile.cpp
    System/MapFileEntry.cpp
    System/TextFile.cpp
    System/FindFiles.cpp
    System/DynLib.cpp
    System/SysInfo.cpp
    System/PerformanceCounter.cpp
	Server/ServerController.cpp
    Threading/CThreads.cpp 
    Threading/Mutex.cpp
    Threading/Threading.h  
    Threading/ThreadPool.cpp 
    Threading/AtomicULong.cpp 
    Threading/AtomicFloat.cpp 
    Threading/AtomicCounter.cpp 
    Threading/AtomicBoolean.cpp 
    Threading/ConditionVariable.cpp
    Util/arcemu_getopt.cpp 
    Crypto/BigNumber.cpp 
    Crypto/MD5.cpp 
    Crypto/Sha1.cpp 
    Config/Config.cpp 
    Database/Database.cpp 
    Database/MySQLDatabase.cpp 
    Database/CreateInterface.cpp 
    Container/CircularBuffer.cpp 
    Network/Socket.cpp  )
	
set( headers 
	Util/arcemu_getopt.h
	Container/Array.h
	Network/AuthCodes.h
	Container/ByteBuffer.h
	Util/CallBack.h
	Container/CircularQueue.h
	System/Platform.h
	System/CrashHandler.h
	System/crc32.h
	Database/DatabaseEnv.h
	System/DynLib.hpp
	System/Errors.h
	Container/FastQueue.h
	System/FindFiles.hpp
	System/FindFilesResult.hpp
	Logging/Log.h
	System/MapFile.h
	System/MapFileEntry.h
	Util/MersenneTwister.h
	System/PerformanceCounter.hpp
	Container/PreallocatedQueue.h
	System/printStackTrace.h
	Util/Singleton.h
	System/StackWalker.h
	System/StackTrace.h
	Container/Storage.h
	System/SysInfo.hpp
	System/TextFile.h
	Util/Tokenizer.h
	Util/Util.h
	Network/PacketBuffer.h
	Crypto/BigNumber.h
	Crypto/MD5.h
	Crypto/Sha1.h
	Crypto/RC4Engine.h
	Config/Config.h
	Config/ConfigEnv.h
	Database/Database.h
	Database/DataStore.h
	Database/Field.h
	Database/MySQLDatabase.h
	Container/CircularBuffer.h
	Network/ListenSocketFreeBSD.h
	Network/ListenSocketLinux.h
	Network/ListenSocketWin32.h
	Network/Network.h
	Network/Packet.h
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
	Threading/CThreads.h
	Threading/CRefCounter.h
	Threading/Guard.h
	Threading/LockedQueue.h
	Threading/Mutex.h
	Threading/Queue.h
	Threading/RWLock.h
	Threading/Threading.h
	Threading/ThreadPool.h
	Threading/ThreadBase.h
	Threading/TLSObject.h
	Server/ServerController.h
)

IF(WIN32)
	SET( sources ${sources} Network/SocketMgrWin32.cpp  Network/SocketOpsWin32.cpp  Network/SocketWin32.cpp System/StackWalker.cpp System/StackTrace.cpp )
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

ENABLE_TESTING()

ADD_EXECUTABLE( UtilTest ${prefix}/Tests/UtilTest.cpp )
TARGET_LINK_LIBRARIES( UtilTest ${PROJECT_NAME} )
ADD_TEST( NAME UtilTest COMMAND UtilTest )

ADD_EXECUTABLE( ServerControllerTest ${prefix}/Tests/ServerControllerTest.cpp )
TARGET_LINK_LIBRARIES( ServerControllerTest ${PROJECT_NAME} )
ADD_TEST( NAME ServerControllerTest COMMAND ServerControllerTest )
