#
# Arcscripts2 Scripts for Arcemu MMORPG Server
# Copyright (C) 2011 Arscripts2 Team
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
 
if( NOT BUILD_ARCEMUSCRIPTS2 )
  message( WARNING, "BUILD_ARCEMUSCRIPTS2 option is set to false. Script libraries will not be built." )
  return()
endif(NOT BUILD_ARCEMUSCRIPTS2)
message("Configuring scripts :")

#defines
SET( SCRIPTS_PATH ${ROOT_PATH}/src/arcscripts2 )
SET( include_dirs ${GLOBAL_INCLUDE_DIRS} )
SET( dependancies shared world )
SET( linkages world)

#All script libraries have SCRIPTLIB defined.
add_definitions(-DSCRIPTLIB -DSKIP_ALLOCATOR_SHARING)

PROJECT(ArcScripts2)

#simpler way to handle this (it will not require to patch source everytime)
include(${SCRIPTS_PATH}/CmakeLists.txt OPTIONAL)

# DO NOT TOUCH THESE BELOW!
add_library(${PROJECT_NAME} SHARED ${source})
add_dependencies(${PROJECT_NAME} ${dependancies})
target_link_libraries(${PROJECT_NAME} ${linkages})

#precompiled header support
include(PCHSupport)
IF( CMAKE_GENERATOR MATCHES Visual*)
	ADD_NATIVE_PRECOMPILED_HEADER(${PROJECT_NAME} Setup.h ${SCRIPTS_PATH}/system/Setup.cpp)
ENDIF()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${ARCEMU_SCRIPTLIB_PATH} LIBRARY DESTINATION ${ARCEMU_SCRIPTLIB_PATH})
#Cleap up our variables(Not required.)
unset(SCRIPTS_PATH)
unset(SRCS)
unset(HEADERS)
unset(source)
unset(header)
unset(include_dirs)
