#child CMakeLists use this variable to access the source file directories. 
#e.g ${SCRIPTS_PATH}/LuaEngine to access the source files for LuaEngine.

if( NOT BUILD_ARCEMUSCRIPTS )
  message( WARNING, "BUILD_ARCEMUSCRIPTS option is set to false. Script libraries will not be built." )
  return()
endif(NOT BUILD_ARCEMUSCRIPTS)
message("Configuring scripts :")
SET( SCRIPTS_PATH ${ROOT_PATH}/src/plugins )
#common include directories that script libraries might include.
SET( include_dirs ${GLOBAL_INCLUDE_DIRS} )
SET( dependancies shared world )
SET( linkages world)

#All script libraries have SCRIPTLIB defined.
add_definitions(-DSCRIPTLIB -DSKIP_ALLOCATOR_SHARING)
SET( BUILD_EVENTSCRIPTS TRUE CACHE BOOL "Build arcEventScripts.")
SET( BUILD_INSTANCESCRIPTS TRUE CACHE BOOL "Build arcInstanceScripts.")
SET( BUILD_EXTRASCRIPTS TRUE CACHE BOOL "Build arcExtraScripts.")
SET( BUILD_GOSSIPSCRIPTS TRUE CACHE BOOL "Build arcGossipScripts.")
SET( BUILD_QUESTSCRIPTS TRUE CACHE BOOL "Build arcQuestScripts.")
SET( BUILD_MISCSCRIPTS TRUE CACHE BOOL "Build arcMiscScripts." )
SET( BUILD_SERVERSTATUSPLUGIN TRUE CACHE BOOL "Build ServerStatusPlugin.")
SET( BUILD_LUAENGINE TRUE CACHE BOOL "Build Arcemu Lua Engine ( ALE ).")
SET( BUILD_PYTHONENGINE TRUE CACHE BOOL "Build Arcemu Python Engine (APE).")
SET( BUILD_WORLDPVP TRUE CACHE BOOL "Build World PvP plugin" )

add_subdirectory( eventscripts)
add_subdirectory( instancescripts )
add_subdirectory( extrascripts )
add_subdirectory( gossipscripts )
add_subdirectory( questscripts )
add_subdirectory( miscscripts )
add_subdirectory( serverstatusplugin )
add_subdirectory( lualib ) #Used by luaengine module.
add_subdirectory( luaengine )

IF( BUILD_WORLDPVP )
	add_subdirectory( worldpvp )
ENDIF()

IF( BUILD_PYTHONENGINE )
	add_subdirectory( python34 )
	add_subdirectory( pythonengine )
ENDIF()

unset(SCRIPTS_PATH)
unset(include_dirs)