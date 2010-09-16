INCLUDES = -I$(top_srcdir)/src/arcemu-logonserver \
   -I$(top_srcdir)/src/arcemu-realmserver -I$(top_srcdir)/src/arcemu-shared -I$(top_srcdir)/src/arcemu-voicechat \
   -I$(top_srcdir)/src/arcemu-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src -I../Common
INCLUDES += -I$(srcdir)/../../../arcemu-shared/g3dlite -I$(srcdir)/../../../arcemu-shared/vmap

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libArcEventScripts.la

libArcEventScripts_la_SOURCES = Setup.cpp \
../Common/Base.cpp \
Event_Halloween.cpp \
Event_WinterVeil.cpp 
