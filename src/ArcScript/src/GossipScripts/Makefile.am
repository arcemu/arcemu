INCLUDES = -I$(top_srcdir)/src/arcemu-logonserver \
   -I$(top_srcdir)/src/arcemu-realmserver -I$(top_srcdir)/src/arcemu-shared -I$(top_srcdir)/src/arcemu-voicechat \
   -I$(top_srcdir)/src/arcemu-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libMoonGossipScripts.la

libMoonGossipScripts_la_SOURCES = \
Gossip_Innkeepers.cpp \
GuardGossip.cpp \
TrainerGossip.cpp \
Gossip_Mulgore.cpp \
Gossip_Moonglade.cpp \
Gossip_Shattrath.cpp \
Gossip_Stormwind.cpp \
Gossip_Tanaris.cpp \
Setup.cpp
