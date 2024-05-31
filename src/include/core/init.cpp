#include "init.hpp"

#include <stdlib.h>


#include "log.hpp"


int init(void) {
	atexit(deinit);
	
	flag.core.logpath = "data/log.txt";

	if ((flag.core.logfile = fopen(flag.core.logpath, "w+")) == NULL)
		fatal_error("Coulnd not open log file '%s' for writing.", flag.core.logpath);

	log_log_add(flag.core.logfile);


	window.stdscr = initscr(); // if initscr() fails, it stops the program and prints error message
	log_debug("Initialized standart window successfully.");

	noecho();

	// windows size
 //	length.window.game.minl = 15; // lldb window
 //	length.window.game.minc = 80; // lldb window
	length.window.game.minl = 40; // 30
	length.window.game.minc = 80; // 60
	length.window.bar.minl = 1;
	length.window.bar.minc = length.window.game.minc;

	length.window.pause.minl = 5;
	length.window.pause.minc = 13;
	length.window.exit.minl = 6;
	length.window.exit.minc = 31;

	length.window.stdscr.minl = length.window.game.minl + length.window.bar.minl + 1;
	length.window.stdscr.minc = length.window.game.minc;
	length.window.sidelog.minl = length.window.stdscr.minl;
	length.window.sidelog.minc = 100;

	length.pad.help.minl = 270;
	length.pad.help.minc = length.window.game.minc - 2;
	length.pad.score.minl = 220;
	length.pad.score.minc = COLS - 2;
	length.pad.log.minl = CORE_DEFAULT_LOG_SCROLLBACK;
	length.pad.log.minc = COLS - 2;

	length.window.finals.minl = 12;
	length.window.finals.minc = 2*length.window.game.minc / 3;
	length.window.again.minl = 6;
	length.window.again.minc = 32;

	
	// log pad initialization
	if ((pad.log = newpad(length.pad.log.minl, length.pad.log.minc)) == NULL)
		fatal_error("Could not initialize log pad.");
	else {	
		wmove(pad.log, length.pad.log.minl - 1, 0);
		scrollok(pad.log, TRUE);
		log_log_add(pad.log);
		log_debug("Initialized log pad succsessfully. (%d lines)", length.pad.log.minl);
		log_info("Above log source do not contain all logs");
		log_nl(  "because it is initialized too late.");
		log_nl(  "For full log see '%s' file.", flag.core.logpath);
	}
	if (keypad(pad.log, TRUE) == ERR) {
		log_error("Could not initialize function keys for log pad.");
		log_nl(   "Arrow keys might not work properly.");
	} else
		log_debug("Initialized function keys for log pad successfully.");	
	return EXIT_SUCCESS;
}


void desetup(void) {
	log_trace("Desetup function have started.");
	free(snake.body);

	if (game.game == true)
		writedata();
}


void deinit(void) {
	log_trace("Deinit functions has started.");
	desetup();
	flushinp();

	if (isendwin() == false)
		endwin();
	log_trace("Deinit functions has ended.");
}

