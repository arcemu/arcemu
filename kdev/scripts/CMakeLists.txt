#child CMakeLists use this variable to access the source file directories. 
#e.g ${SCRIPTS_PATH}/LuaEngine to access the source files for LuaEngine.

SET( SCRIPTS_PATH ${ROOT_PATH}/src/scripts/src )
#common include directories that script libraries might include.
SET( include_dirs ${ROOT_PATH}/src/arcemu-world ${ROOT_PATH}/src/arcemu-shared)

#All script libraries have SCRIPTLIB defined.
add_definitions(-DSCRIPTLIB)
SET( BUILD_EVENTSCRIPTS TRUE CACHE BOOL "Build arcEventScripts.")
SET( BUILD_INSTANCESCRIPTS FALSE CACHE BOOL "Build arcInstanceScripts.")
SET( BUILD_EXTRASCRIPTS FALSE CACHE BOOL "Build arcExtraScripts.")
SET( BUILD_GOSSIPSCRIPTS FALSE CACHE BOOL "Build arcGossipScripts.")
SET( BUILD_QUESTSCRIPTS FALSE CACHE BOOL "Build arcQuestScripts.")
SET( BUILD_MISCSCRIPTS FALSE CACHE BOOL "Build arcMiscScripts." )
SET( BUILD_SPELLHANDLERS FALSE CACHE BOOL "Build arcSpellHandlers.")
SET( BUILD_WORLDPVPSCRIPTS FALSE CACHE BOOL "Build WorldPVPScripts.")
SET( BUILD_LUAENGINE FALSE CACHE BOOL "Build LuaEngine.")
SET( BUILD_LUABRIDGE FALSE CACHE BOOL "Build luabridge.")


#luabridge
if( BUILD_LUABRIDGE )
  add_subdirectory(luabridge)
endif( BUILD_LUABRIDGE )
#luaengine
if( BUILD_LUAENGINE )
  add_subdirectory(luaengine)
endif( BUILD_LUAENGINE)
#event scripts
if( BUILD_EVENTSCRIPTS )
  add_subdirectory( eventscripts)
endif( BUILD_EVENTSCRIPTS )
#instance scripts
if( BUILD_INSTANCESCRIPTS )
  add_subdirectory( instancescripts )
endif( BUILD_INSTANCESCRIPTS )
#extra scripts
if( BUILD_EXTRASCRIPTS )
  add_subdirectory(extrascripts)
endif( BUILD_EXTRASCRIPTS )
#Gossip scripts
if( BUILD_GOSSIPSCRIPTS )
  add_subdirectory(gossipscripts)
endif( BUILD_GOSSIPSCRIPTS )
#Quest Scripts
if( BUILD_QUESTSCRIPTS )
  add_subdirectory( questscripts )
endif( BUILD_QUESTSCRIPTS )
#Misc Scripts
if( BUILD_MISCSCRIPTS )
  add_subdirectory( miscscripts )
endif( BUILD_MISCSCRIPTS )
#Spell handlers
if( BUILD_SPELLHANDLERS )
  add_subdirectory( spellhandlers )
endif( BUILD_SPELLHANDLERS )
#world pvp scripts
if( BUILD_WOWPVPSCRIPTS )
  add_subdirectory( wowpvpscripts )
endif( BUILD_WOWPVPSCRIPTS )

unset(SCRIPTS_PATH)
unset(include_dirs)