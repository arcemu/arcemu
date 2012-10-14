project(Arcemu)

cmake_minimum_required(VERSION 2.8)
#Do system checking.

SET( GLOBAL_DEFINES "-DHAVE_CONFIG_H")

IF( CMAKE_GENERATOR MATCHES Visual*)
	SET( GLOBAL_DEFINES ${GLOBAL_DEFINES} "/MP")
ENDIF()

IF(CMAKE_GENERATOR MATCHES Visual*)
	SET(GLOBAL_DEFINES ${GLOBAL_DEFINES} "/bigobj")
ENDIF()

#check platform version.
IF( CMAKE_SYSTEM_PROCESSOR MATCHES "[xX]64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "[xX]86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "[aA][mM][dD]64" OR CMAKE_GENERATOR MATCHES Win64*)
	#define for 64bit
	SET( GLOBAL_DEFINES ${GLOBAL_DEFINES} -"DX64")
	SET( IS_64BIT TRUE)
	IF(CMAKE_GENERATOR MATCHES NMake*)
		SET( GLOBAL_DEFINES ${GLOBAL_DEFINES} "-bigobj")
	ENDIF()
ENDIF()

#mac osx
IF( APPLE )
	SET( GLOBAL_DEFINES ${GLOBAL_DEFINES} "-DHAVE_DARWIN")
	SET( IS_MAC TRUE)
ELSEIF(UNIX)
	#mac has unix defined too but if 'if(APPLE)' fails, then it's not apple.
	
	#check for freebsd
	IF( ( CMAKE_SYSTEM_NAME  STREQUAL "FreeBSD" ) OR ( CMAKE_SYSTEM_NAME STREQUAL kFreeBSD ) )
		SET( GLOBAL_DEFINES ${GLOBAL_DEFINES} "-DUSE_KQUEUE")
		SET( IS_FREEBSD TRUE)
	ELSE()
		SET( GLOBAL_DEFINES ${GLOBAL_DEFINES} "-DUSE_EPOLL")
		SET( IS_LINUX TRUE)
	ENDIF()
ENDIF()

SET( ROOT_PATH  ${CMAKE_SOURCE_DIR}/..)

#We have our own custom modules that we use. This tells cmakes where to find them.
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ROOT_PATH}/cmake_modules )
SET( DEPENDENCY_ROOT ${ROOT_PATH}/dep )
SET( DEPENDENCY_SOURCES ${DEPENDENCY_ROOT}/src)
SET( DEPENDENCY_LIBS ${DEPENDENCY_ROOT}/lib)
SET( DEPENDENCY_INCLUDE_ROOT ${DEPENDENCY_ROOT}/include)
SET( DEPENDENCY_DLLS64 ${DEPENDENCY_ROOT}/dll64)

IF(IS_64BIT)
	SET(DEPENDENCY_DLLS ${DEPENDENCY_ROOT}/dll64)
ELSE()
	SET(DEPENDENCY_DLLS ${DEPENDENCY_ROOT}/dll)
ENDIF()

#Install libmysql.dll required for our core to run.
IF( WIN32)
	SET( INSTALLED_DEPENDENCIES ${DEPENDENCY_DLLS}/libmysql.dll ${DEPENDENCY_DLLS}/libeay32.dll)
ENDIF(WIN32)

#Our configurable variables.
IF( WIN32 )
	SET(ARCEMU_CONFIGSFILE_PATH configs CACHE PATH "Path where the arcemu config files are.")
ELSE( WIN32 )
	SET(ARCEMU_CONFIGSFILE_PATH etc CACHE PATH "Path where the arcemu config files are.")
ENDIF( WIN32 )

SET(ARCEMU_SCRIPTLIB_PATH "modules" CACHE PATH "The directory which contains the 'lib' directory that has the script libraries." )
SET(BUILD_ARCEMUSCRIPTS TRUE CACHE BOOL "Set to false to NOT build script libraries." )
SET(BUILD_TOOLS TRUE CACHE BOOL "Build Arcemu tools" )
SET(VISUALSTUDIO_COMPILERHEAPLIMIT 400 CACHE STRING "Visual Studio compiler heap limit. Ignore on darwin and unix platforms.")

include(CompilerVersion)
if( CMAKE_GENERATOR MATCHES Unix* )
	add_definitions(-Wno-deprecated)
	#Extract Revision
	execute_process(WORKING_DIRECTORY ${ROOT_PATH} COMMAND sh git_version.sh )
	
	#Check support for unordered_map/set
	GetCompilerVersion( gcc_version)
	IF( DEFINED gcc_version AND ( ${gcc_version} VERSION_EQUAL "4.3" OR ${gcc_version} VERSION_GREATER "4.3" ) AND NOT APPLE )
		add_definitions(-DHAS_CXX0X -std=gnu++0x)
	ENDIF()
	
ELSEIF( CMAKE_GENERATOR MATCHES Visual* )
	#Extract revision
	execute_process(WORKING_DIRECTORY ${ROOT_PATH} COMMAND git_version.bat )
	add_definitions(-D_CRT_SECURE_NO_WARNINGS /EHa )
	
	#Check support for unordered_map/set
	GetCompilerVersion( msvc_version)
	IF(DEFINED ${msvc_version} AND ${msvc_version} GREATER "1599" )
		add_definitions(-DHAS_CXX0X)
	ENDIF()
	
	#This fixes PCH issues 'Inconsistent values for /Ym'
	IF( CMAKE_GENERATOR MATCHES Visual* ) 
		STRING( REGEX REPLACE "(/Zm)([0-9]+)" "\\1${VISUALSTUDIO_COMPILERHEAPLIMIT}" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
		STRING( REGEX REPLACE "(/Zm)([0-9]+)" "\\1${VISUALSTUDIO_COMPILERHEAPLIMIT}" CMAKE_C_FLAGS ${CMAKE_C_FLAGS} )
		SET( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} CACHE STRING "CMAKE_CXX_FLAGS" FORCE)
		SET( CMAKE_C_FLAGS ${CMAKE_C_FLAGS} CACHE STRING "CMAKE_C_FLAGS" FORCE)
	ENDIF( CMAKE_GENERATOR MATCHES Visual* )
ENDIF()

#Pass our configurable variables to the C++ preprocessor.
add_definitions( ${GLOBAL_DEFINES} -DCONFDIR=\"${ARCEMU_CONFIGSFILE_PATH}\" -DPREFIX=\"${ARCEMU_SCRIPTLIB_PATH}\")

#These are set by find_package or the zlib and pcre subprojects.
set(ZLIB_LIBRARIES "" CACHE INTERNAL "Zlib libraries.")
set(PCRE_LIBRARIES "" CACHE INTERNAL "PCRE libraries.")
set(PCRE_INCLUDE_DIRS "" CACHE INTERNAL "PCRE include dirs.")
#mark them as advanced so they don't show up on the gui.
mark_as_advanced(ZLIB_LIBRARIES PCRE_LIBRARIES PCRE_INCLUDE_DIRS)

set(GLOBAL_INCLUDE_DIRS 
${ROOT_PATH}/src/arcemu-shared
${ROOT_PATH}/src/arcemu-world
${ROOT_PATH}/src/arcemu-logonserver
${DEPENDENCY_INCLUDE_ROOT}
${DEPENDENCY_INCLUDE_ROOT}/g3dlite
${DEPENDENCY_INCLUDE_ROOT}/vmaplib
${DEPENDENCY_INCLUDE_ROOT}/pcre
${DEPENDENCY_INCLUDE_ROOT}/zlib
${DEPENDENCY_INCLUDE_ROOT}/recast
${DEPENDENCY_INCLUDE_ROOT}/detour )

#Find our needed libs
IF( NOT WIN32 )
	find_package(ZLIB REQUIRED)
	find_package(PCRE REQUIRED)
	find_package(OpenSSL REQUIRED)
	find_package(Threads REQUIRED)
	find_package(MySQL REQUIRED)
ELSE()
	add_subdirectory(zlib)
	add_subdirectory(pcre)
	SET(MYSQL_LIBRARY "")
ENDIF()

IF(WIN32)	#On windows, we have to specify the includes for libeay and mysql libs.
	SET( GLOBAL_INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} 
	${DEPENDENCY_INCLUDE_ROOT}/openssl
	${DEPENDENCY_INCLUDE_ROOT}/mysql )
ELSE()	#On apple n linux, mysql.h should have been found.
	SET( GLOBAL_INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} ${MYSQL_INCLUDE_DIR} )
ENDIF()

#For windows, link to libeay and libmysql
SET( EXTRA_LIBS)
SET( EXTRA_LIBS_PATH)
if(WIN32)
	if( IS_64BIT )
		SET( EXTRA_LIBS libeay32_win64.lib )
		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
			SET(EXTRA_LIBS ${EXTRA_LIBS} libmysql_debug_x64.lib )
		else()
			SET( EXTRA_LIBS ${EXTRA_LIBS} libmysql_release_x64.lib )
		endif()
	else(IS_64BIT )
		SET( EXTRA_LIBS libeay32_win32.lib )
		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
			SET(EXTRA_LIBS ${EXTRA_LIBS} libmysql_debug_win32.lib )
		else()
			SET( EXTRA_LIBS ${EXTRA_LIBS} libmysql_release_win32.lib )
		endif()
	endif()
	#Needed for socket stuff and crash handler
	SET(EXTRA_LIBS ${EXTRA_LIBS} ws2_32.lib dbghelp.lib )
elseif(APPLE)
	SET( EXTRA_LIBS ${EXTRA_LIBS} "-framework Carbon" )
elseif(IS_LINUX)
	SET( EXTRA_LIBS ${EXTRA_LIBS} dl )
else()
endif()

IF( APPLE )
	IF( NOT IS_64BIT )
		SET( EMPTY "" )
		STRING( REGEX REPLACE ".*-m64" "" CMAKE_C_FLAGS  "${CMAKE_C_FLAGS}" )
		STRING( REGEX REPLACE ".*-m 64" "" CMAKE_C_FLAGS  "${CMAKE_C_FLAGS}" )
		STRING( REGEX REPLACE ".*-m64" "" CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}" )
		STRING( REGEX REPLACE ".*-m 64" "" CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}" )
		STRING( REGEX REPLACE ".*-m64" "" CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}" )
		STRING( REGEX REPLACE ".*-m 64" "" CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}" )
		STRING( REGEX REPLACE ".*-m64" "" CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS}" )
		STRING( REGEX REPLACE ".*-m 64" "" CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS}" )		
		STRING( REGEX REPLACE ".*-m64" "" CMAKE_MODULE_LINKER_FLAGS  "${CMAKE_MODULE_LINKER_FLAGS}" )
		STRING( REGEX REPLACE ".*-m 64" "" CMAKE_MODULE_LINKER_FLAGS  "${CMAKE_MODULE_LINKER_FLAGS}" )
		ADD_DEFINITIONS( -m32 )
		SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32" )
		SET( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32 -undefined dynamic_lookup" )
		SET( CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m32" )
	ENDIF( NOT IS_64BIT )
ENDIF( APPLE )

add_subdirectory(g3dlite)
add_subdirectory(vmaplib)
add_subdirectory(recast)
add_subdirectory(detour)
add_subdirectory(shared)
add_subdirectory(logon)
add_subdirectory(world)
add_subdirectory(spellhandlers)
add_subdirectory(battlegrounds)

IF(NOT WIN32)
	add_subdirectory(crashreport)
ENDIF()
add_subdirectory(scripts)

IF( BUILD_TOOLS )
	add_subdirectory(tools)
ENDIF( BUILD_TOOLS )


INSTALL(FILES ${INSTALLED_DEPENDENCIES} DESTINATION .)
