INCLUDES = -I$(top_srcdir)/src/arcemu-logonserver \
   -I$(top_srcdir)/src/arcemu-realmserver -I$(top_srcdir)/src/arcemu-shared -I$(top_srcdir)/src/arcemu-voicechat \
   -I$(top_srcdir)/src/arcemu-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libSunGossipScripts.la

libSunGossipScripts_la_SOURCES = \
Gossip_Battlemaster.cpp \
Gossip_Innkeepers.cpp \
Gossip_Guard.cpp \
Gossip_Trainer.cpp \
Gossip_Mulgore.cpp \
Gossip_Shattrath.cpp \
Gossip_Tanaris.cpp \
Gossip_Stormwind.cpp\
Gossip_Moonglade.cpp\
Setup.cpp
