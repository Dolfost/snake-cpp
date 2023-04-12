#include <stdlib.h>

Windows window;
Flags flag;
Colors color;
Lengths length;

int init(void) {
	atexit(deinit);

	flag.core.logpath = "data/log.txt";

	log.file = fopen(flag.core.logpath, "w+.");

	length.window.stdscr.miny = 27;
	length.window.stdscr.minx = 65;
	length.subwindow.log.miny = length.window.stdscr.miny;
	length.subwindow.log.minx = 95;
	length.subwindow.game.miny = 25;
	length.subwindow.game.minx = length.window.stdscr.minx;
	length.subwindow.bar.miny = 1;
	length.subwindow.bar.minx = length.window.stdscr.minx;


	window.stdscr = initscr();

	if ((window.log = newwin(0,0,0,0)) == NULL)
		fatal_error("Could not initialize log window.");
	else {	
		log.window = window.log;
		log_info("Initialized standart window succsessfully.");
		log_info("Initialized log window succsessfully.");
	}

	scrollok(window.log, TRUE);

	if (!(flag.curses.color = has_colors()))
		log_info("This terminal does not support color.");
	else
		log_info("This terminal does support color.");

	if (start_color() != OK)
		log_error("Could not initialize color.");
	else
		log_info("Initialized colors successfully.");

	// log colors (8^>)
	log.background = COLOR_BLACK;
	init_pair(1, COLOR_BLUE, log.background);    log.levelcolor[0] = 1; log.levelattr[0] = A_DIM; 	 // trace
	init_pair(2, COLOR_YELLOW, log.background);  log.levelcolor[1] = 2; log.levelattr[1] = A_NORMAL;    // debug
	init_pair(3, COLOR_GREEN, log.background);   log.levelcolor[2] = 3; log.levelattr[2] = A_NORMAL; // info
	init_pair(4, COLOR_MAGENTA, log.background); log.levelcolor[3] = 4; log.levelattr[3] = A_BOLD;   // warn
	init_pair(5, COLOR_RED, log.background);     log.levelcolor[4] = 5; log.levelattr[4] = A_BLINK;  // error
	init_pair(6, COLOR_RED, log.background);     log.levelcolor[5] = 6; log.levelattr[5] = A_REVERSE;  // fatal
	init_pair(7, COLOR_WHITE, log.background);   log.filenamecolor = 7; log.filenameattribute = A_UNDERLINE;
	init_pair(8, COLOR_CYAN, log.background);    log.msgcolor = 8;      log.msgattribute = A_NORMAL;


	if (LINES < length.window.stdscr.miny) {
		log_warn("The terminal widndow is too small.");
		log_nl("It has %d lines that is %d less tnan min. req. %d lines.",
				LINES, length.window.stdscr.miny - LINES, length.window.stdscr.miny);
		flag.curses.small_window = true;
	}
	if (COLS < length.window.stdscr.minx) {
		log_warn("The terminal widndow is too small.");
		log_nl("It has %d columns that is %d less tnan min. req. %d columns.",
					COLS, length.window.stdscr.minx - COLS, length.window.stdscr.minx);
		flag.curses.small_window = true;
	}
	if (flag.curses.small_window == true) {
		log_fatal("Could not fit all iterface in such small window. (%d lines by %d columns)",
				LINES, COLS);
		log_nl("Please, increase terminal size to at least %d lines by %d columns.", 
				length.window.stdscr.miny, length.window.stdscr.minx);
 		wrefresh(window.log);
 		wgetch(window.log);
 		fatal_error("Terminal is too small. Resize it to at least %d lines by %d columns.", length.window.stdscr.miny, length.window.stdscr.minx);
	}


	if ((window.help = newwin(0,0,0,0)) == NULL)
		fatal_error("Could not initialize help window.");
	else
		log_info("Initialized help window succsessfully.");
	
	if (curs_set(0) == ERR)
		log_info("This terminal does not support cursor visibilyty settings.");
	else {
		flag.curses.cursor = true;
		log_info("This terminal does support cursor visibilyty settings.");
	}

	
	return EXIT_SUCCESS;
}

void deinit(void) {
	endwin();
}

