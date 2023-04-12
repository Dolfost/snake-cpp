#include <ncurses.h>

#include "main.hpp"

#include "include/core/core.hpp"
#include "include/core/log.cpp"
#include "include/core/init.cpp"
#include "include/core/getopt.cpp"


int main(int argc, char** argv) {
	execname = *argv;

	init();
	process_argv(argc, argv);

	printw("Hello, nCurses! - we have %d colors", COLORS);
	refresh();
	getch();

	log_trace("Example trace");
	log_debug("Example debug");
	log_info("Example info");
	log_warn("Example warning");
	log_error("Example error");
	log_fatal("Example fatal");

	wrefresh(window.log);
	getch();
	
	return 0;
}
