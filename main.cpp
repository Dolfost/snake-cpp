#include <ncurses.h>

#include "main.hpp"

#include "include/core/core.hpp"
#include "include/core/log.hpp"
#include "include/game/game.hpp"

#include "include/snake/snake.hpp"
#include "include/core/init.cpp"
#include "include/core/getopt.cpp"
#include "include/snake/setup.cpp"

#include "include/snake/draw.cpp"
#include "include/snake/input.cpp"
#include "include/snake/logic.cpp"

#include "include/game/game.cpp"


int main(int argc, char** argv) {
	execname = *argv;

	init();
	
	process_argv(argc, argv);

	setup();
	game();




	log_trace("Example trace");
	log_debug("Example debug");
	log_info("Example info");
	log_warn("Example warning");
	log_error("Example error");
	log_fatal("Example fatal");


	wrefresh(window.stdscr);
	wrefresh(window.game);
	wrefresh(window.sidelog);
	wrefresh(window.bar);
	getch();

	wrefresh(window.log);
	getch();
	
	return 0;
}
