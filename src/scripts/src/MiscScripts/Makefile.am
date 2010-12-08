INCLUDES = -I$(top_srcdir)/src/arcemu-logonserver \
   -I$(top_srcdir)/src/arcemu-realmserver -I$(top_srcdir)/src/arcemu-shared -I$(top_srcdir)/src/arcemu-voicechat \
   -I$(top_srcdir)/src/arcemu-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src
INCLUDES += -I$(srcdir)/../../../arcemu-shared/g3dlite -I$(srcdir)/../../../arcemu-shared/vmap

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libArcMiscScripts.la

libArcMiscScripts_la_SOURCES = Creatures.cpp \
DarkmoonFaireObjects.cpp \
GameObjects.cpp \
GameObjectTeleportTable.cpp \
RandomScripts.cpp \
Setup.cpp
