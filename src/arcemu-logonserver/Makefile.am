INCLUDES += -I$(srcdir) -I$(srcdir)/../arcemu-shared

bin_PROGRAMS = arcemu-logonserver

arcemu_logonserver_SOURCES = \
	AccountCache.cpp \
	AccountCache.h \
	AuthSocket.cpp \
	AuthSocket.h \
	AuthStructs.h \
	LogonCommServer.cpp \
	LogonCommServer.h \
	LogonConsole.cpp \
	LogonConsole.h \
	LogonOpcodes.cpp \
	LogonOpcodes.h \
	LogonStdAfx.cpp \
	LogonStdAfx.h \
	AutoPatcher.h \
	AutoPatcher.cpp \
	Main.cpp \
	Main.h \
	PeriodicFunctionCall_Thread.h

arcemu_logonserver_LDADD = -lshared -lz
arcemu_logonserver_LDFLAGS = -L$(srcdir)/../arcemu-shared
