cmake_minimum_required (VERSION 3.5)
project (GPGChat)

set(SOURCE_FILES
	${SOURCE_FILES}
	src/server.c
	src/server_parser.c
	)

set (EXECUTABLE_NAME server)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} ${PROJECT_NAME})
