cmake_minimum_required (VERSION 3.5)
project (GPGChat)

set(SOURCE_FILES
	${SOURCE_FILES}
	src/client_action_message.c
	src/client_action_connect.c
	src/client_actions.c
	src/client.c
	src/client_parser.c
	)

set(EXECUTABLE_NAME client)
add_definitions(-DEXECUTABLE_NAME=\"${EXECUTABLE_NAME}\")

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} ${PROJECT_NAME})
