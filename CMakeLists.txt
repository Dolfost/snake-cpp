cmake_minimum_required(VERSION 3.12)

# project setup
project(snake
	LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(datafolder "${CMAKE_SOURCE_DIR}/data")

# make target
add_executable(snake
	./src/main.cpp)

# libraries
file(GLOB coreSrc CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/include/core/*.cpp")
add_library(snakecore
	${coreSrc})

file(GLOB gameSrc CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/include/game/*.cpp")
add_library(snakegame
	${gameSrc})

file(GLOB snakeSrc CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/include/snake/*.cpp")
add_library(snakesnake
	${snakeSrc})

# linking libs to game
target_link_libraries(snake PRIVATE snakecore snakegame snakesnake ncurses)


if (EXISTS ${datafolder})
	message("Data folder (${datafolder}) exists")
else()
	message("Creating data folder (${datafolder})")
	file(MAKE_DIRECTORY ${datafolder})
endif()
