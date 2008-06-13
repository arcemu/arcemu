INCLUDES += -I$(srcdir) -I$(srcdir)/../ascent-shared

bin_PROGRAMS = ascent-logonserver

ascent_logonserver_SOURCES = \
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

ascent_logonserver_LDADD = -lshared -lz
ascent_logonserver_LDFLAGS = -L$(srcdir)/../ascent-shared
