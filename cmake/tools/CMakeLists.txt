SET(ARCEMU_TOOLS_PATH "tools" CACHE PATH "The directory where you want the tools installed." )
SET(BUILD_TOOLS_AD TRUE CACHE BOOL "Build DBC and Map extractors." )
SET(BUILD_TOOLS_VMAPS TRUE CACHE BOOL "Build VMAP extractor tools." )
SET(BUILD_TOOLS_CREATUREDATA TRUE CACHE BOOL "Build creature data extractor tools" )

add_subdirectory( bzip2 )
add_subdirectory( libmpq_new )
add_subdirectory( dbcfile )
add_subdirectory( loadlib )

IF( BUILD_TOOLS_AD )
	add_subdirectory( ad )
ENDIF( BUILD_TOOLS_AD )

IF( BUILD_TOOLS_CREATUREDATA )
	add_subdirectory( creature_data )
ENDIF( BUILD_TOOLS_CREATUREDATA )

IF( BUILD_TOOLS_VMAPS )
	add_subdirectory( vmap_tools )
ENDIF( BUILD_TOOLS_VMAPS )

