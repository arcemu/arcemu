if(NOT BUILD_LUABRIDGE )
  message("\tluabridge was skipped because BUILD_LUABRIDGE is unchecked.")
  return()
endif(NOT BUILD_LUABRIDGE)

project(luabridge)

#prefix variable - points to the folder which contains the source files.
#	used to point Cmake to the source files.
SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )
#dependancies that must be built before this project.
SET( dependancies world )
#libraries/archives that must be linked to this project.
SET( linkages world )
#SRCS - contains the source files that compose this project.
SET( SRCS
  ${prefix}/lua/lapi.c 
  ${prefix}/lua/lapi.h 
  ${prefix}/lua/lauxlib.c 
  ${prefix}/lua/lauxlib.h 
  ${prefix}/lua/lbaselib.c 
  ${prefix}/lua/lcode.c 
  ${prefix}/lua/lcode.h 
  ${prefix}/lua/ldblib.c 
  ${prefix}/lua/ldebug.c 
  ${prefix}/lua/ldebug.h 
  ${prefix}/lua/ldo.c 
  ${prefix}/lua/ldo.h 
  ${prefix}/lua/ldump.c 
  ${prefix}/lua/lfunc.c 
  ${prefix}/lua/lfunc.h 
  ${prefix}/lua/lgc.c 
  ${prefix}/lua/lgc.h 
  ${prefix}/lua/linit.c 
  ${prefix}/lua/liolib.c 
  ${prefix}/lua/llex.c 
  ${prefix}/lua/llex.h 
  ${prefix}/lua/llimits.h 
  ${prefix}/lua/lmathlib.c 
  ${prefix}/lua/lmem.c 
  ${prefix}/lua/lmem.h 
  ${prefix}/lua/loadlib.c 
  ${prefix}/lua/lobject.c 
  ${prefix}/lua/lobject.h 
  ${prefix}/lua/lopcodes.c 
  ${prefix}/lua/lopcodes.h 
  ${prefix}/lua/loslib.c 
  ${prefix}/lua/lparser.c 
  ${prefix}/lua/lparser.h 
  ${prefix}/lua/lstate.c 
  ${prefix}/lua/lstate.h 
  ${prefix}/lua/lstring.c 
  ${prefix}/lua/lstring.h 
  ${prefix}/lua/lstrlib.c 
  ${prefix}/lua/ltable.c 
  ${prefix}/lua/ltable.h 
  ${prefix}/lua/ltablib.c 
  ${prefix}/lua/ltm.c 
  ${prefix}/lua/ltm.h 
  ${prefix}/lua/lua.h 
  ${prefix}/lua/luaconf.h 
  ${prefix}/lua/lualib.h 
  ${prefix}/lua/lundump.c 
  ${prefix}/lua/lundump.h 
  ${prefix}/lua/lvm.c 
  ${prefix}/lua/lvm.h 
  ${prefix}/lua/lzio.c 
  ${prefix}/lua/lzio.h 
  ${prefix}/lua/print.c 
  ${prefix}/interpreter/lua_defs.h 
  ${prefix}/interpreter/lua_instance.h 
  ${prefix}/interpreter/LUAEngine.cpp 
  ${prefix}/interpreter/LUAEngine.h 
  ${prefix}/luabridge_src/luabridge.hpp 
  ${prefix}/luabridge_src/luabridge.cpp 
  ${prefix}/luabridge_src/impl/class.hpp 
  ${prefix}/luabridge_src/impl/module.hpp 
  ${prefix}/luabridge_src/impl/stack.hpp 
  ${prefix}/luabridge_src/impl/typelist.hpp 
  ${prefix}/object_functions/aura_functions.h 
  ${prefix}/object_functions/gameobject_functions.h 
  ${prefix}/object_functions/global_functions.h 
  ${prefix}/object_functions/instance_functions.h 
  ${prefix}/object_functions/item_functions.h 
  ${prefix}/object_functions/lua_hooks.h 
  ${prefix}/object_functions/object_functions.h 
  ${prefix}/object_functions/packet_functions.h 
  ${prefix}/object_functions/player_functions.h 
  ${prefix}/object_functions/quest_functions.h 
  ${prefix}/object_functions/register_functions.h 
  ${prefix}/object_functions/spell_functions.h 
  ${prefix}/object_functions/sql_functions.h 
  ${prefix}/object_functions/taxi_functions.h 
  ${prefix}/object_functions/unit_functions.h 
  ${prefix}/object_functions/creature_functions.h 
  ${prefix}/object_functions/GlobalFunctions.h )

include_directories( ${include_dirs} )
add_definitions(-Wno-deprecated)
add_library(${PROJECT_NAME} SHARED ${SRCS} )
add_dependencies( ${PROJECT_NAME} ${dependancies} )
target_link_libraries( ${PROJECT_NAME} ${linkages} )
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( dependancies )
unset( linkages)
