#include <stdlib.h>

Snake snake;
Bait bait;
// Game game; // defined in init.cpp

int get_random(int lower_bound, int upper_bound) {
	return rand() % (upper_bound + 1 - lower_bound) + lower_bound;
}

void setup(void) {
	log_trace("Setup function have started.");

	// help window colors
	color.helpbg = COLOR_BLACK;
	init_pair(13, COLOR_YELLOW,  color.helpbg); color.pair.help.key = 13; attribute.help.key = A_BOLD;
	init_pair(14, COLOR_MAGENTA, color.helpbg); color.pair.help.title = 14; attribute.help.title = A_BOLD;
	init_pair(15, COLOR_GREEN,   color.helpbg); color.pair.help.keyword = 15; attribute.help.keyword = A_UNDERLINE;
	init_pair(16, COLOR_CYAN,    color.helpbg); color.pair.help.definition = 16; attribute.help.definition = A_UNDERLINE;


	// --log-scrollback
	if (length.pad.log.minl != CORE_DEFAULT_LOG_SCROLLBACK) {
		int helppadcopy;
		WINDOW* newlogpad = NULL;
		
		log_debug("Changing log pad scrollback");
		log_nl(   "from %d to %d lines.", CORE_DEFAULT_LOG_SCROLLBACK, length.pad.log.minl);

		if ((newlogpad = newpad(length.pad.log.minl, length.pad.log.minc)) == NULL) {
			log_fatal("Could not initialize new log pad. (%d lines)", length.pad.log.minl);
			fatal_error("Could not change the log pad scrollback to %d lines.", length.pad.log.minl);
		} else
			log_debug("Initialized new log pad succesfully. (%d lines)", length.pad.log.minl);

		if (length.pad.log.minl > CORE_DEFAULT_LOG_SCROLLBACK) { // TODO fix this (new line after copying window)
			// log_fatal("%d %d %d", CORE_DEFAULT_LOG_SCROLLBACK, length.pad.log.minl, length.pad.log.minc);
			helppadcopy = copywin(pad.log, newlogpad, 0, 0, // something is wrong
					length.pad.log.minl - CORE_DEFAULT_LOG_SCROLLBACK - 1, 0,
					length.pad.log.minl - 2, length.pad.log.minc - 1, // sould be length.pad.log.minl - 1, but is causes segfault. WTF???
					TRUE);
		} else {
			helppadcopy = copywin(pad.log, newlogpad, CORE_DEFAULT_LOG_SCROLLBACK - length.pad.log.minl - 1, 0,
					0, 0,
					length.pad.log.minl - 1, length.pad.log.minc - 1, FALSE);
		}

		if (helppadcopy == ERR) {
			log_fatal("Could not copy old log pad to new one.");
			fatal_error("Could not copy old log pad to new one.");
		}

			delwin(pad.log);
			log_log_remove(pad.log);
			pad.log = newlogpad;
			wmove(pad.log, length.pad.log.minl - 1, 0);
			scrollok(pad.log, TRUE);
			log_log_add(pad.log);
			log_debug("Copied log pad to new pad successfully.");
	}


	// help pad initialization
	FILE* helpfile = fopen(flag.option.helppadpath, "r");
	if (helpfile == NULL) {
		log_error("Could not open help pad file");
		log_nl(  "'%s' for reading.", flag.option.helppadpath);
	} else {
		if ((pad.help = getwin(helpfile)) == NULL) {
			log_fatal("Could not initialize help pad from");
			log_nl(   "'%s'.", flag.option.helppadpath);
			fatal_error("Could not initialize help pad.");
		} else
			log_debug("Initialized help pad succsessfully.");
		fclose(helpfile);
	}
	if (keypad(pad.help, TRUE) == ERR) {
		log_error("Could not initialize function keys for help pad.");
		log_nl(   "Arrow keys might not work properly.");
	} else
		log_debug("Initialized function keys for help pad successfully.");	


	wtimeout(window.game, flag.option.timeout);
	cbreak();

	srand(time(NULL));
	
	gamesetup();
	game.playagain = true;

	// mouse fear stuff
	color.gamebg = COLOR_BLACK;
	init_pair(9,  COLOR_WHITE,  color.gamebg); bait.fearcolor[3] = 9;  bait.fearattr[3] = A_BOLD;
	init_pair(10, COLOR_YELLOW, color.gamebg); bait.fearcolor[2] = 10; bait.fearattr[2] = A_BOLD;
	init_pair(11, COLOR_BLUE,   color.gamebg); bait.fearcolor[1] = 11; bait.fearattr[1] = A_NORMAL;
	init_pair(12, COLOR_RED,    color.gamebg); bait.fearcolor[0] = 12; bait.fearattr[0] = A_BOLD;

	bait.fearradius[0] = 5;
	bait.fearradius[1] = 10;
	bait.fearradius[2] = 15;

	bait.eaten = false;
	
	spawn_bait();
	
	wattrset(window.bar, A_BOLD);
	waddstr(window.bar, " Score: ");	
	wattroff(window.bar, A_BOLD);
	length.bar.scorename = 8; // " Score: " length
	length.bar.score = 8; // score number is not wider than 4 spaces
	wattrset(window.bar, A_BOLD);
	mvwaddstr(window.bar, 0, length.bar.scorename + length.bar.score, "Time: ");
	wattroff(window.bar, A_BOLD);
	length.bar.timename = 6;
	length.bar.time = 6; // >99.9s

	// led colors
	init_pair(17, COLOR_BLACK, COLOR_GREEN);   color.pair.led[3] = 17; 
	init_pair(18, COLOR_BLACK, COLOR_YELLOW);  color.pair.led[2] = 18;
	init_pair(19, COLOR_BLACK, COLOR_MAGENTA); color.pair.led[1] = 19;
	init_pair(20, COLOR_BLACK, COLOR_RED);     color.pair.led[0] = 20;

	if (flag.option.buildhelppad == true) {
		log_debug("Buildind help to '%s'...", flag.option.helppadpath);
		buildhelppad(flag.option.helppadpath);
		exit(EXIT_SUCCESS);
	}

	if (flag.curses.small_window == true) {
		log_fatal("Could not fit all iterface in such small window. (%d lines by %d columns)",
				LINES, COLS);
		log_nl("Please, increase terminal size to at least %d lines by %d columns.", 
				length.window.stdscr.minl, length.window.stdscr.minc);
		log_info("Press [L | l] to close log pad.");
  		gamelog('l');
 		fatal_error("Terminal is too small. Resize it to at least %d lines by %d columns.", length.window.stdscr.minl, length.window.stdscr.minc);
	}

	if (flag.option.help == true) {
		help('H');
		drawgame();
	}

	readdata();

}

void desetup(void) {
	log_trace("Desetup function have started.");
	free(snake.body);

	if (game.game == true)
		writedata();
}
