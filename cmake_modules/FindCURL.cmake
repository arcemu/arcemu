# Find CURL
# Find CURL includes and library
#
# CURL_INCLUDE_DIRS - where to find curl.h
# CURL_LIBRARIES - List of libraries when using curl.
# CURL_FOUND - True if curl found.

IF(CURL_INCLUDE_DIRS)
  SET(CURL_FIND_QUIETLY TRUE)
ENDIF (CURL_INCLUDE_DIRS)

FIND_PATH(CURL_INCLUDE_DIR curl.h
    /usr/include/curl
    /usr/include/x86_64-linux-gnu/curl
    /usr/include/riscv64-linux-gnu/curl
    /usr/include/aarch64-linux-gnu/curl)

FIND_LIBRARY(CURL_LIBRARY
    NAMES curl
    PATHS /usr/lib/x86_64-linux-gnu/)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CURL DEFAULT_MSG CURL_LIBRARY CURL_INCLUDE_DIR)

IF(CURL_FOUND)
  SET( CURL_LIBRARIES ${CURL_LIBRARY} )
  SET( CURL_INCLUDE_DIRS ${CURL_INCLUDE_DIR} )
ELSE(CURL_FOUND)
  SET( CURL_LIBRARIES )
  SET( CURL_INCLUDE_DIRS )
ENDIF(CURL_FOUND)

MARK_AS_ADVANCED( CURL_LIBRARIES CURL_INCLUDE_DIRS )
