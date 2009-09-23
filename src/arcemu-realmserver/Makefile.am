INCLUDES += -I$(srcdir) -I$(srcdir)/../arcemu-shared

bin_PROGRAMS = arcemu-realmserver

arcemu_realmserver_SOURCES = \
ClientManager.h \
ClusterManager.h \
LogonCommClient.h \
LogonCommHandler.h \
Master.h \
ObjectStorage.h \
RStdAfx.h \
Session.h \
Structures.h \
WorkerOpcodes.h \
WorkerServer.h \
WorkerServerSocket.h \
WorldSocket.h \
CharacterHandler.cpp \
ClientManager.cpp \
ClusterManager.cpp \
LogonCommClient.cpp \
LogonCommHandler.cpp \
Master.cpp \
ObjectStorage.cpp \
QueryHandler.cpp \
RStdAfx.cpp \
Session.cpp \
WorkerServer.cpp \
WorkerServerSocket.cpp \
WorldSocket.cpp 
	
arcemu_realmserver_LDADD = -lshared -lz

arcemu_realmserver_LDFLAGS = -L$(srcdir)/../arcemu-shared \
               -L/usr/lib
               
