cmake_minimum_required (VERSION 3.5)
project (GPGChat)

set(SOURCE_FILES
	${SOURCE_FILES}
	src/server.c
	)

set (EXECUTABLE_NAME server)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

# add_subdirectory(../shared)

target_link_libraries(${EXECUTABLE_NAME} ${PROJECT_NAME})

# openssl
#find_package(PkgConfig REQUIRED)
#pkg_search_module(OPENSSL REQUIRED openssl)
#
#if( OPENSSL_FOUND )
#    include_directories(${OPENSSL_INCLUDE_DIRS})
#	target_link_libraries(${EXECUTABLE_NAME} ${OPENSSL_LIBRARIES})
#    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
#else()
#    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
#endif()
