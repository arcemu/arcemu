PROJECT(crashreport)

SET( prefix ${ROOT_PATH}/src/tools/${PROJECT_NAME} )
SET( SRCS
 ${prefix}/Main.c )

add_executable( ${PROJECT_NAME} ${SRCS} )
REMOVE_DEFINITIONS( -DHAS_CXX0X -std=gnu++0x )
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)