PROJECT(crashreport)

SET( prefix ${ROOT_PATH}/src/${PROJECT_NAME} )
SET( SRCS
 ${prefix}/Main.c )

add_executable( ${PROJECT_NAME} ${SRCS} )
remove_definitions( -std=c++0x )
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)