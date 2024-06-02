cmake_minimum_required(VERSION 3.12)

# project setup
set(CMAKE_COLOR_DIAGNOSTICS ON)
list(APPEND CMAKE_MESSAGE_CONTEXT "snake")
project(snake
	LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(datafolder "${CMAKE_SOURCE_DIR}/data")
set(helpfile "${CMAKE_SOURCE_DIR}/data/help.pad.window")

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


message(CHECK_START "Searching for data folder ${datafolder}")
if (EXISTS ${datafolder})
	message(CHECK_PASS "found")
else()
	message(CHECK_FAIL "not found. Creating it...")
	file(MAKE_DIRECTORY ${datafolder})
endif()

message(CHECK_START "Searching for help file ${helpfile}")
if (EXISTS ${helpfile})
	message(CHECK_PASS "found")
else()
	message(CHECK_FAIL "not found. You should consider to generate it with ./build/snake --build-help-pad")
endif()
