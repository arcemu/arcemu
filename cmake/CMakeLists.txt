project(Arcemu)

cmake_minimum_required(VERSION 2.8)
#Do system checking.

MACRO( DETECT_ARCEMU_TARGET_ARCH )
	SET( IS_64BIT FALSE )
	SET( ARCEMU_TARGET_ARCH "unknown/unsupported" )
	
	# Visual Studio can cross-compile without a toolchain file, so we have to detect by the generator here
	IF( CMAKE_GENERATOR MATCHES "^Visual Studio.*" )
	
		IF( CMAKE_GENERATOR MATCHES "Win64" )
			SET( ARCEMU_TARGET_ARCH "amd64" )
			SET( IS_64BIT TRUE )
		ELSEIF( CMAKE_GENERATOR MATCHES "ARM64" )
			SET( ARCEMU_TARGET_ARCH "aarch64" )
			SET( IS_64BIT TRUE )
		ELSEIF( DEFINED CMAKE_GENERATOR_PLATFORM )
			IF( CMAKE_GENERATOR_PLATFORM MATCHES "x64" )
				SET( ARCEMU_TARGET_ARCH "amd64" )
				SET( IS_64BIT TRUE )
			ELSEIF( CMAKE_GENERATOR_PLATFORM MATCHES "ARM64" )
				SET( ARCEMU_TARGET_ARCH "aarch64" )
				SET( IS_64BIT TRUE )
			ENDIF()
		ENDIF()		
				
	ELSE()
		IF( CMAKE_SYSTEM_PROCESSOR MATCHES "[xX]64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "[xX]86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "[aA][mM][dD]64" )
			SET( ARCEMU_TARGET_ARCH "amd64" )
			SET( IS_64BIT TRUE )			
		ELSEIF( CMAKE_SYSTEM_PROCESSOR MATCHES "riscv64" )
			SET( ARCEMU_TARGET_ARCH "riscv64" )
			SET( IS_64BIT TRUE )			
		ELSEIF( CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64" )
			SET( ARCEMU_TARGET_ARCH "aarch64" )
			SET( IS_64BIT TRUE )
		ENDIF()
	ENDIF()
	
	MESSAGE("CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
	MESSAGE("CMAKE_GENERATOR: ${CMAKE_GENERATOR}")
	IF( DEFINED CMAKE_GENERATOR_PLATFORM )
		MESSAGE("CMAKE_GENERATOR_PLATFORM: ${CMAKE_GENERATOR_PLATFORM}")
	ENDIF()
	
	MESSAGE("ARCEMU_TARGET_ARCH: ${ARCEMU_TARGET_ARCH}")
	MESSAGE("IS_64BIT: ${IS_64BIT}")
ENDMACRO( DETECT_ARCEMU_TARGET_ARCH )

SET( CMAKE_INSTALL_PREFIX "../bin" CACHE PATH "Install path prefix" FORCE )

SET( GLOBAL_DEFINES "-DHAVE_CONFIG_H")

IF( CMAKE_GENERATOR MATCHES Visual*)
	SET( GLOBAL_DEFINES ${GLOBAL_DEFINES} "/MP")
ENDIF()

IF(CMAKE_GENERATOR MATCHES Visual*)
	SET(GLOBAL_DEFINES ${GLOBAL_DEFINES} "/bigobj")
ENDIF()

DETECT_ARCEMU_TARGET_ARCH()

IF( NOT IS_64BIT )
	MESSAGE( FATAL_ERROR "Only 64 bit architectures are supported!" )
ENDIF()

IF(CMAKE_GENERATOR MATCHES NMake*)
	SET( GLOBAL_DEFINES ${GLOBAL_DEFINES} "-bigobj")
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
SET( DEPENDENCY_INCLUDE_ROOT ${DEPENDENCY_ROOT}/include)
SET( DEPENDENCY_SOURCES ${DEPENDENCY_ROOT}/src)

IF( ARCEMU_TARGET_ARCH STREQUAL "amd64" )
	SET( DEPENDENCY_LIBS ${DEPENDENCY_ROOT}/lib/amd64)
	SET( DEPENDENCY_DLLS ${DEPENDENCY_ROOT}/dll/amd64)
ELSEIF( ARCEMU_TARGET_ARCH STREQUAL "aarch64" )
	SET( DEPENDENCY_LIBS ${DEPENDENCY_ROOT}/lib/aarch64)
	SET( DEPENDENCY_DLLS ${DEPENDENCY_ROOT}/dll/aarch64)
ENDIF()

#Install libmysql.dll required for our core to run.
IF( WIN32)
	IF( ARCEMU_TARGET_ARCH STREQUAL "amd64" )
		SET( INSTALLED_DEPENDENCIES ${DEPENDENCY_DLLS}/libmysql.dll ${DEPENDENCY_DLLS}/libcrypto-1_1-x64.dll ${DEPENDENCY_DLLS}/libssl-1_1-x64.dll ${DEPENDENCY_DLLS}/libcurl.dll)
	ELSEIF( ARCEMU_TARGET_ARCH STREQUAL "aarch64" )
		SET( INSTALLED_DEPENDENCIES ${DEPENDENCY_DLLS}/libmysql.dll ${DEPENDENCY_DLLS}/libcrypto-1_1-arm64.dll ${DEPENDENCY_DLLS}/libssl-1_1-arm64.dll ${DEPENDENCY_DLLS}/libcurl.dll)
	ENDIF()
ENDIF(WIN32)

#Our configurable variables.
SET(ARCEMU_CONFIGSFILE_PATH configs CACHE PATH "Path where the arcemu config files are.")
SET(ARCEMU_SCRIPTLIB_PATH "plugins" CACHE PATH "The directory which contains the 'lib' directory that has the script libraries." )
SET(BUILD_ARCEMUSCRIPTS TRUE CACHE BOOL "Set to false to NOT build script libraries." )
SET(BUILD_TOOLS TRUE CACHE BOOL "Build Arcemu tools" )
SET(VISUALSTUDIO_COMPILERHEAPLIMIT 400 CACHE STRING "Visual Studio compiler heap limit. Ignore on darwin and unix platforms.")

include(CompilerVersion)
if( CMAKE_GENERATOR MATCHES Unix* )
	IF( NOT CMAKE_BUILD_TYPE )
	    MESSAGE("Setting CMAKE_BUILD_TYPE to Debug.")
	    SET(CMAKE_BUILD_TYPE "Debug")
	ENDIF()

	MESSAGE("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

	IF( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	    ADD_DEFINITIONS(-DARCEMU_DEBUG_BUILD)
	ENDIF()

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
	
	#This fixes PCH issues 'Inconsistent values for /Zm'
	IF( ${CMAKE_CXX_FLAGS} MATCHES "(/Zm)([0-9]+)" )
		STRING( REGEX REPLACE "(/Zm)([0-9]+)" "\\1${VISUALSTUDIO_COMPILERHEAPLIMIT}" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
	ELSE()
		SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm${VISUALSTUDIO_COMPILERHEAPLIMIT}"  )
	ENDIF()
		
	IF( ${CMAKE_C_FLAGS} MATCHES "(/Zm)([0-9]+)" )
		STRING( REGEX REPLACE "(/Zm)([0-9]+)" "\\1${VISUALSTUDIO_COMPILERHEAPLIMIT}" CMAKE_C_FLAGS ${CMAKE_C_FLAGS} )
	ELSE()
		SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Zm${VISUALSTUDIO_COMPILERHEAPLIMIT}"  )
	ENDIF()
		
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
${ROOT_PATH}/src/shared
${ROOT_PATH}/src/world
${ROOT_PATH}/src/logon
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
	find_package(CURL REQUIRED)
ELSE()
	add_subdirectory(zlib)
	add_subdirectory(pcre)
	SET(MYSQL_LIBRARY "")
ENDIF()

IF(WIN32)	#On windows, we have to specify the includes for libeay and mysql libs.
	SET( GLOBAL_INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} 
	${DEPENDENCY_INCLUDE_ROOT}/openssl
	${DEPENDENCY_INCLUDE_ROOT}/mysql
	${DEPENDENCY_INCLUDE_ROOT}/curl)
ELSE()	#On apple n linux, mysql.h should have been found.
	SET( GLOBAL_INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} ${MYSQL_INCLUDE_DIR} )
ENDIF()

#For windows, link to libeay and libmysql
SET( EXTRA_LIBS)
SET( EXTRA_LIBS_PATH)

if(WIN32)
	SET( EXTRA_LIBS libcrypto.lib libcurl_imp.lib )
	SET( EXTRA_LIBS ${EXTRA_LIBS} libmysql.lib )

	#Needed for socket stuff and crash handler
	SET(EXTRA_LIBS ${EXTRA_LIBS} ws2_32.lib dbghelp.lib )
elseif(APPLE)
	SET( EXTRA_LIBS ${EXTRA_LIBS} "-framework Carbon" )
elseif(IS_LINUX)
	SET( EXTRA_LIBS ${EXTRA_LIBS} dl )
else()
endif()

ENABLE_TESTING()

add_subdirectory(g3dlite)
add_subdirectory(vmaplib)
add_subdirectory(recast)
add_subdirectory(detour)
add_subdirectory(shared)
add_subdirectory(logon)
add_subdirectory(world)
add_subdirectory(spellhandlers)
add_subdirectory(battlegrounds)
add_subdirectory(plugins)

IF( BUILD_TOOLS )
	add_subdirectory(tools)
ENDIF( BUILD_TOOLS )


INSTALL(FILES ${INSTALLED_DEPENDENCIES} DESTINATION .)
