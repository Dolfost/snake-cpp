cmake_minimum_required(VERSION 3.12)

project(snake
	LANGUAGES CXX)

add_executable(snake
	main.cpp)

file(GLOB coreSrc CONFIGURE_DEPENDS "./include/core/*.cpp")
add_library(coreLib
	${coreSrc})
target_link_libraries(coreLib
	PUBLIC ncurses)

file(GLOB gameSrc CONFIGURE_DEPENDS "./include/game/*.cpp")
add_library(gameLib
	${gameSrc})
target_link_libraries(gameLib
	PUBLIC ncurses)

file(GLOB snakeSrc CONFIGURE_DEPENDS "./include/snake/*.cpp")
add_library(snakeLib
	${snakeSrc})
target_link_libraries(snake
	PUBLIC ncurses)

target_link_libraries(snake PRIVATE coreLib gameLib snakeLib ncurses)
