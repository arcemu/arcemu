PROJECT(arcemu-shared)

SET( prefix ${ROOT_PATH}/src/${PROJECT_NAME} )
SET( sources
    CThreads.cpp 
    CThreads.h 
    CrashHandler.h 
    CrashHandler.cpp 
    Log.cpp 
    MersenneTwister.cpp 
    MersenneTwister.h 
    PreallocatedQueue.h 
    ByteBuffer.h 
    Common.h 
    Errors.h 
    Log.h 
    printStackTrace.cpp 
    printStackTrace.h 
    Singleton.h 
    Timer.h 
    #UnixThreading.h 
    Util.h 
    Util.cpp 
   # WinThreading.h 
    WorldPacket.h 
    crc32.cpp 
    crc32.h 
    FastQueue.h 
    Threading/Mutex.cpp
    Threading/Threading.h  
    Threading/ThreadPool.cpp 
    Threading/AtomicULong.h 
    Threading/AtomicULong.cpp 
    Threading/AtomicFloat.h 
    Threading/AtomicFloat.cpp 
    Threading/AtomicCounter.h 
    Threading/AtomicCounter.cpp 
    Threading/AtomicBoolean.h 
    Threading/AtomicBoolean.cpp 
    Storage.h 
    arcemu_getopt.h 
    arcemu_getopt.cpp 
    Auth/BigNumber.cpp 
    Auth/BigNumber.h 
    Auth/MD5.cpp 
    Auth/MD5.h 
    Auth/Sha1.cpp 
    Auth/Sha1.h 
    Auth/WowCrypt.cpp 
    Auth/WowCrypt.h 
    Config/Config.cpp 
    Config/Config.h 
    Config/ConfigEnv.h 
    Database/DBC.cpp 
    Database/Database.cpp 
    Database/DBC.h 
    Database/Database.h 
    Database/DBCStores.cpp 
    Database/DatabaseEnv.h 
    Database/DBCStores.h 
    Database/Field.h 
    Database/DataStore.h 
    Database/dbcfile.cpp 
    Database/dbcfile.h 
    Database/MySQLDatabase.cpp 
    Database/MySQLDatabase.h 
    Database/CreateInterface.cpp 
    Network/CircularBuffer.h 
    Network/CircularBuffer.cpp 
    Network/ListenSocketLinux.h 
    Network/ListenSocketFreeBSD.h 
    Network/Network.h 
    Network/Socket.cpp 
    Network/Socket.h 
    Network/SocketDefines.h 
    Network/SocketLinux.cpp 
    Network/SocketFreeBSD.cpp 
    Network/SocketMgrLinux.cpp 
    Network/SocketMgrLinux.h 
    Network/SocketMgrFreeBSD.cpp 
    Network/SocketMgrFreeBSD.h 
    Network/SocketOps.h 
    Network/SocketOpsLinux.cpp 
    Network/SocketOpsFreeBSD.cpp )

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)
find_package(MySQL REQUIRED)
find_package(OpenSSL REQUIRED)
include_directories( ${prefix} ${ZLIB_INCLUDE_DIRS} )
ADD_DEFINITIONS(-Wno-deprecated)
ADD_LIBRARY( ${PROJECT_NAME} STATIC ${SRCS} )
target_link_libraries( ${PROJECT_NAME} ${ZLIB_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${MYSQL_LIBRARIES} ${OPENSSL_LIBRARIES} )
#install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)

