INCLUDES = -I$(top_srcdir)/src/arcemu-logonserver \
   -I$(top_srcdir)/src/arcemu-realmserver -I$(top_srcdir)/src/arcemu-shared -I$(top_srcdir)/src/arcemu-voicechat \
   -I$(top_srcdir)/src/arcemu-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src
INCLUDES += -I$(srcdir)/../../../collision/g3dlite -I$(srcdir)/../../../collision/vmap
AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libSpellHandlers.la

libSpellHandlers_la_SOURCES = DeathKnightSpells.cpp DruidSpells.cpp HunterSpells.cpp ItemSpells_1.cpp MageSpells.cpp PaladinSpells.cpp PetAISpells.cpp PriestSpells.cpp QIspells.cpp RogueSpells.cpp Setup.cpp ShamanSpells.cpp WarlockSpells.cpp WarriorSpells.cpp