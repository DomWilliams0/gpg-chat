cmake_minimum_required (VERSION 3.5)
project (GPGChat)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra")

include_directories(
	shared/include
	server/include
	client/include
	)

add_subdirectory(shared)
add_subdirectory(server)
add_subdirectory(client)
