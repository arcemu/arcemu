cmake_minimum_required(VERSION 2.6)
#Do system checking.

SET( GLOBAL_DEFINES -DHAVE_CONFIG_H)

#check platform version.
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "amd64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "i386" )
	#define for 64bit
	SET( GLOBAL_DEFINES -DX64)
	SET( IS_64BIT TRUE)
ENDIF()

#mac osx
IF( APPLE )
	SET( GLOBAL_DEFINES ${GLOBAL_DEFINES} -DHAVE_DARWIN)
	SET( IS_MAC TRUE)
ELSEIF(UNIX)
	#mac has unix defined too but if 'if(APPLE)' fails, then it's not apple.
	
	#check for freebsd
	IF( CMAKE_SYSTEM_NAME  STREQUAL "FreeBSD")
		SET( GLOBAL_DEFINES ${GLOBAL_DEFINES} -DUSE_KQUEUE)
		SET( IS_FREEBSD TRUE)
	ELSE()
		SET( GLOBAL_DEFINES ${GLOBAL_DEFINES} -DUSE_EPOLL)
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
SET( DEPENDENCY_DLLS ${DEPENDENCY_ROOT}/dll)
SET( DEPENDENCY_DLLS64 ${DEPENDENCY_ROOT}/dll64)

#Our configurable variables.
SET(ARCEMU_CONFIGSFILE_PATH etc CACHE PATH "Path where the arcemu config files are.")
SET(ARCEMU_SCRIPTLIB_PATH ".." CACHE PATH "The directory which contains the 'lib' directory that has the script libraries." )
SET(BUILD_ARCEMUSCRIPTS FALSE CACHE BOOL "Set to true to build script libraries." )

#Make sure our revision header is up to date.
if( UNIX )
	add_definitions(-Wno-deprecated)
	execute_process(WORKING_DIRECTORY ${ROOT_PATH} COMMAND sh extract_svn_revision.sh )
ELSEIF( WIN32 )
	execute_process(WORKING_DIRECTORY ${ROOT_PATH}/src/arcemu-shared COMMAND RevisionExtractor)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
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
${DEPENDENCY_INCLUDE_ROOT}/pcre
${DEPENDENCY_INCLUDE_ROOT}/zlib )

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
	SET( GLOBAL_INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS}
	${MYSQL_INCLUDE_DIR} )
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
endif()

add_subdirectory(shared)
add_subdirectory(g3dlite)
add_subdirectory(logon)
add_subdirectory(world)
IF(NOT WIN32)
	add_subdirectory(crashreport)
ENDIF()
add_subdirectory(scripts)