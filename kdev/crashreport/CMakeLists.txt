PROJECT(arcemu-crashreport)

SET( prefix ${ROOT_PATH}/src/${PROJECT_NAME} )
SET( SRCS
 ${prefix}/Main.c )

add_executable( ${PROJECT_NAME} ${SRCS} )
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)