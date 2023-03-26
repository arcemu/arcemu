PROJECT( python_build C )
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

IF( NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES )
    SET( CMAKE_BUILD_TYPE "Debug" )
ENDIF()

IF(NOT CMAKE_GENERATOR MATCHES Visual*)
	MESSAGE( "Build type is ${CMAKE_BUILD_TYPE}" )
ENDIF()

SET( CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/../dist" CACHE PATH "" FORCE )
SET( INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX} )

SET( BUILD_INTERPRETER TRUE CACHE BOOL "Build the interpreter" )
SET( INSTALL_MINIMAL_LIB_ONLY TRUE CACHE BOOL "Install only the minimal library files neccessary" )

ADD_SUBDIRECTORY( pythoncore )

IF( BUILD_INTERPRETER )
	ADD_SUBDIRECTORY( python )
ENDIF()

IF( INSTALL_MINIMAL_LIB_ONLY )
	INSTALL( DIRECTORY ../Lib/encodings DESTINATION ${INSTALL_DESTINATION}/Lib/ )
	INSTALL( FILES
			../Lib/_bootlocale.py
			../Lib/_collections_abc.py
			../Lib/_sitebuiltins.py
			../Lib/_weakrefset.py
			../Lib/abc.py
			../Lib/codecs.py
			../Lib/genericpath.py
			../Lib/io.py
			../Lib/ntpath.py
			../Lib/os.py
			../Lib/posixpath.py
			../Lib/site.py
			../Lib/stat.py
			../Lib/sysconfig.py
			DESTINATION ${INSTALL_DESTINATION}/Lib/ )
ELSE()
	INSTALL( DIRECTORY ../Lib DESTINATION ${INSTALL_DESTINATION}/ )
ENDIF()

