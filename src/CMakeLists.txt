set (CMAKE_CXX_STANDARD 11)

add_executable(snakegame
	main.cpp
)
set_target_properties(snakegame PROPERTIES
	VERSION ${PROJECT_VERSION}
	OUTPUT_NAME snakegame
	EXPORT_NAME snakegame
)


find_package(Curses REQUIRED)
if (NOT CURSES_HAVE_NCURSES_H)
	message(FATAL_ERROR "Curses package found, but ncurses.h isn't")
endif()

add_subdirectory(snake)

target_include_directories(snakegame 
	PRIVATE ${CURSES_INCLUDE_DIRS}
)

target_link_libraries(snakegame
	PRIVATE 
		snakesnake 
		${CURSES_LIBRARIES}
)

include(GNUInstallDirs)
file(MAKE_DIRECTORY 
	"${CMAKE_CURRENT_BINARY_DIR}/../${SNAKE_STATE_DIRECTORY}"
)
install(TARGETS snakegame)
install(DIRECTORY DESTINATION "${SNAKE_STATE_DIRECTORY}")
