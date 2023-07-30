#include <ncurses.h>

#include "main.hpp"

#include "include/snake/setup.hpp"
#include "include/core/init.hpp"
#include "include/core/options.hpp"
#include "include/core/log.hpp"
#include "include/game/game.hpp"


#include "include/snake/draw.hpp"
#include "include/snake/input.hpp"
#include "include/snake/logic.hpp"

int main(int argc, char** argv) {
	execname = *argv;

	init();

	process_argv(argc, argv);

	setup();

	gameloop();

	if (flag.option.openlog == true) {
		log_info("Press [L l] to close log.");
		gamelog('L');
	}
	
	return 0;
}
