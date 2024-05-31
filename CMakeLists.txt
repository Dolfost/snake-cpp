cmake_minimum_required(VERSION 3.12)

# project setup
project(snake
	LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)

# make target
add_executable(snake
	./src/main.cpp)

# libraries
file(GLOB coreSrc CONFIGURE_DEPENDS "./src/include/core/*.cpp")
add_library(coreLib
	${coreSrc})

file(GLOB gameSrc CONFIGURE_DEPENDS "./src/include/game/*.cpp")
add_library(gameLib
	${gameSrc})

file(GLOB snakeSrc CONFIGURE_DEPENDS "./src/include/snake/*.cpp")
add_library(snakeLib
	${snakeSrc})

# linking libs to game
target_link_libraries(snake PRIVATE coreLib gameLib snakeLib ncurses)
