#include <ncurses.h>

#include "main.hpp"

#include "include/core/core.hpp"
#include "include/core/init.cpp"
#include "include/core/log.cpp"
#include "include/core/getopt.cpp"


int main(int argc, char** argv) {
	execname = *argv;

	init();
	process_argv(argc, argv);

	addstr("Hello, nCurses!");
	refresh();
	getch();

	wrefresh(window.log);
	getch();
	
	return 0;
}
