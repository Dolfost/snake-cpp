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

target_link_libraries(snakegame
	PRIVATE 
		Curses
		snakesnake 
)

include(GNUInstallDirs)
install(TARGETS snakegame)
install(DIRECTORY DESTINATION "${CMAKE_INSTALL_LOCALSTATEDIR}")
