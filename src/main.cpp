#include <ncurses.h>

#include <snake/types.hpp>

const char* execname;
const char* directionstr[] = {"right", "up", "left", "down"};

Windows window;
Subwindows subwindow;
Pads pad;
Flags flag;
Colors color;
Lengths length;
Positions positions;
Attributes attribute;

Game game;

Snake snake;
Bait bait;

#include <snake/setup.hpp>
#include <snake/init.hpp>
#include <snake/options.hpp>
#include <snake/log.hpp>
#include <snake/game.hpp>


#include <snake/draw.hpp>
#include <snake/input.hpp>
#include <snake/logic.hpp>

int main(int argc, char** argv) {
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
