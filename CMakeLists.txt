cmake_minimum_required (VERSION 3.5)
project (GPGChat)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra")

set(SOURCE_FILES
	shared_utils.h
	shared_utils.c
	server.c
	)
set (EXECUTABLE_NAME server)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

# Search OpenSSL
find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)

if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
endif()

target_link_libraries(${EXECUTABLE_NAME} ${OPENSSL_LIBRARIES})
