cmake_minimum_required(VERSION 3.8)

SET(PROJECT_NAME nash)

project(${PROJECT_NAME} LANGUAGES C)

SET(SOURCES main.c
	src/input.c
	include/input.h
	src/expansion.c
	include/expansion.h
	src/subshell.c
	include/subshell.h
	src/variables.c
	include/variables.h
	src/tokenize.c
	include/tokenize.h
	src/command.c
	include/command.h
	src/execute.c
	include/execute.h
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (CMAKE_BUILD_TYPE EQUAL Debug)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()
