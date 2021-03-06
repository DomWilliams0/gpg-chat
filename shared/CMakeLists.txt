cmake_minimum_required (VERSION 3.5)
project (GPGChat)

set(SOURCE_FILES
	${SOURCE_FILES}
	src/shared_utils.c
	src/socket.c
	src/gpg.c
	)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# openssl
find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)

if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
endif()

# gpgme
find_library (GPGME gpgme)
if (GPGME)
	target_link_libraries(${PROJECT_NAME} gpgme)
	message (STATUS "Using GPGme ${GPGME}")
else()
    message (SEND_ERROR "GPGme is required")
endif()

# pthreads
find_library(PTHREADS_LIB pthread)
if (PTHREADS_LIB)
	target_link_libraries(${PROJECT_NAME} pthread)
	message (STATUS "Found pthreads")
else()
	message (SEND_ERROR "pthreads is required")
endif()
