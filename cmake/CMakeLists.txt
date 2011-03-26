cmake_minimum_required(VERSION 2.8)

SET( ROOT_PATH  ${CMAKE_SOURCE_DIR}/..)

#We have our own custom modules that we use. This tells cnaje where to find them.
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ROOT_PATH}/cmake_modules )

#Our configurable variables.
SET(ARCEMU_CONFIGSFILE_PATH etc CACHE STRING "Path where the arcemu config files are.")
SET(ARCEMU_SCRIPTLIB_PATH ".." CACHE STRING "The directory which contains the 'lib' directory that has the script libraries." )
SET(BUILD_ARCEMUSCRIPTS FALSE CACHE BOOL "Set to true to build script libraries." )

#Make sure our revision header is up to date.
execute_process(WORKING_DIRECTORY ${ROOT_PATH} COMMAND sh extract_svn_revision.sh )

#Pass our configurable variables to the C++ preprocessor.
add_definitions(-DCONFDIR=\"${ARCEMU_CONFIGSFILE_PATH}\" -DPREFIX=\"${ARCEMU_SCRIPTLIB_PATH}\" )

add_subdirectory(g3dlite)
add_subdirectory(shared)
add_subdirectory(logon)
add_subdirectory(world)
add_subdirectory(crashreport)
add_subdirectory(scripts)