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

	gameloop();

	drawgame();
	getch();

	log_info("Press [L l] to close log.");
	gamelog('l');
	
	return 0;
}
