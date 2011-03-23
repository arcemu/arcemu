if(NOT BUILD_LUAENGINE )
  message("\tLuaEngine was skipped because BUILD_LUAENGINE is unchecked.")
  return()
endif(NOT BUILD_LUAENGINE)

project(LuaEngine)

#prefix variable - points to the folder which contains the source files.
SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )
#dependancies that must be built before this project.
SET( dependancies arcemu-world )
#libraries/archives that must be linked to this project.
SET( linkages arcemu-world )
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
${prefix}/LUAEngine.cpp 
${prefix}/LUAEngine.h 
${prefix}/LUAFunctions.h
${prefix}/AuraFunctions.h
${prefix}/FunctionTables.h
${prefix}/GameobjectFunctions.h
${prefix}/GlobalFunctions.h
${prefix}/ItemFunctions.h
${prefix}/LUAFunctions.h
${prefix}/LuaSqlApi.h
${prefix}/PacketFunctions.h
${prefix}/SpellFunctions.h
${prefix}/TaxiFunctions.h
${prefix}/UnitFunctions.h )

include_directories( ${include_dirs} )
add_definitions(-Wno-deprecated)
add_library(${PROJECT_NAME} SHARED ${SRCS} )
add_dependencies( ${PROJECT_NAME} ${dependancies} )
#target_link_libraries( ${PROJECT_NAME} ${linkages} )
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)

#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( dependancies )
unset( linkages)
