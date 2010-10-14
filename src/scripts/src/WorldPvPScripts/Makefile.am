INCLUDES = -I$(top_srcdir)/src/arcemu-logonserver \
   -I$(top_srcdir)/src/arcemu-realmserver -I$(top_srcdir)/src/arcemu-shared -I$(top_srcdir)/src/arcemu-voicechat \
   -I$(top_srcdir)/src/arcemu-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src
INCLUDES += -I$(srcdir)/../../../arcemu-shared/g3dlite -I$(srcdir)/../../../arcemu-shared/vmap
AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libWorldPvPScripts.la

libWorldPvPScripts_la_SOURCES = Setup.cpp \
ZoneHellfirePeninsula.cpp \
ZoneSilithus.cpp \
ZoneTerokkarForest.cpp \
ZoneZangarmarsh.cpp
