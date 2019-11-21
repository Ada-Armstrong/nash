cmake_minimum_required(VERSION 3.8)

SET(PROJECT_NAME nash)

project(${PROJECT_NAME} LANGUAGES C)

SET(SOURCES main.c
			command.c
			command.h
			execute.c
			execute.h
			input.c
			input.h
			tokenize.c
			tokenize.h
)

add_executable(${PROJECT_NAME} ${SOURCES})

if (CMAKE_BUILD_TYPE EQUAL Debug)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()
