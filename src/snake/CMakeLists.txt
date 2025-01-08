add_library(snakesnake
	src/whereami.cpp
	src/draw.cpp
	src/game.cpp
	src/input.cpp
	src/init.cpp
	src/log.cpp
	src/logic.cpp
	src/options.cpp
	src/setup.cpp
)

include(GNUInstallDirs)
set(SNAKE_STATE_DIRECTORY ${CMAKE_INSTALL_LOCALSTATEDIR})
target_compile_definitions(snakesnake 
	PUBLIC SNAKE_DATA_DIR="${SNAKE_STATE_DIRECTORY}"
)

configure_file(
	include/snake/version.hpp.in 
	include/snake/version.hpp
	@ONLY
)

target_sources(snakesnake
	PUBLIC FILE_SET HEADERS 
	BASE_DIRS 
		include
		${CURSES_INCLUDE_DIRS}
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
	FILES 
		include/snake/whereami.hpp
		include/snake/core.hpp
		include/snake/draw.hpp
		include/snake/game.hpp
		include/snake/init.hpp
		include/snake/input.hpp
		include/snake/log.hpp
		include/snake/logic.hpp
		include/snake/options.hpp
		include/snake/setup.hpp
		include/snake/types.hpp
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include/snake/version.hpp>"
		"$<INSTALL_INTERFACE:include/snake/version.hpp>"
)

target_link_libraries(snakesnake
	PRIVATE ${CURSES_LIBRARIES}
)

return(PROPAGATE SNAKE_STATE_DIRECTORY)
