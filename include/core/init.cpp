#include <stdlib.h>

Windows window;
Subwindows subwindow;
Flags flag;
Colors color;
Lengths length;

int init(void) {
	atexit(deinit);

	flag.core.logpath = "data/log.txt";

	log_log_add(fopen(flag.core.logpath, "w+"));
	FILE* sus = fopen("data/log1.txt", "w+");	
	log_log_add(sus);

	length.window.stdscr.minl = 27;
	length.window.stdscr.minc = 65;
	length.subwindow.log.minl = length.window.stdscr.minl;
	length.subwindow.log.minc = 100;
	length.subwindow.game.minl = 25;
	length.subwindow.game.minc = length.window.stdscr.minc;
	length.subwindow.bar.minl = 1;
	length.subwindow.bar.minc = length.window.stdscr.minc;


	window.stdscr = initscr();

	if ((window.log = newwin(0,0,0,0)) == NULL)
		fatal_error("Could not initialize log window.");
	else {	
		// g_log.window = window.log;
		log_log_add(window.log);
		log_debug("Initialized standart window succsessfully.");
		log_debug("Initialized log window succsessfully.");
	}

	scrollok(window.log, TRUE);
	
	if (COLS - length.subwindow.game.minc - 1 >= length.subwindow.log.minc) {
		log_debug("Terminal window is large enough");
		log_nl(   "to fit game and log windows in it.");

		if ((subwindow.log = subwin(stdscr, LINES, COLS - length.subwindow.game.minc - 1, 0, length.subwindow.game.minc + 2 - 1)) == NULL)
			fatal_error("Could not initialize log subwindow.");
		else
			log_debug("Initialized log subwindow succesfully.");
		log_log_add(subwindow.log);
		scrollok(subwindow.log, TRUE);
	}

	if ((window.help = newwin(0,0,0,0)) == NULL)
		fatal_error("Could not initialize help window.");
	else
		log_debug("Initialized help window succsessfully.");

	log_log_remove(sus);

	if (curs_set(0) == ERR)
		log_info("This terminal does not support cursor visibilyty settings.");
	else {
		flag.curses.cursor = true;
		log_info("This terminal does support cursor visibilyty settings.");
	}

	if (!(flag.curses.color = has_colors()))
		log_info("This terminal does not support color.");
	else
		log_info("This terminal does support color.");

	if (start_color() != OK)
		log_error("Could not initialize color.");
	else
		log_debug("Initialized colors successfully.");

	// log colors (8^>)
	g_log.background = COLOR_BLACK;
	init_pair(1, COLOR_BLUE, g_log.background);    g_log.levelcolor[0] = 1; g_log.levelattr[0] = A_DIM; 	// trace
	init_pair(2, COLOR_YELLOW, g_log.background);  g_log.levelcolor[1] = 2; g_log.levelattr[1] = A_NORMAL;  // debug
	init_pair(3, COLOR_GREEN, g_log.background);   g_log.levelcolor[2] = 3; g_log.levelattr[2] = A_NORMAL;  // info
	init_pair(4, COLOR_MAGENTA, g_log.background); g_log.levelcolor[3] = 4; g_log.levelattr[3] = A_BOLD;    // warn
	init_pair(5, COLOR_RED, g_log.background);     g_log.levelcolor[4] = 5; g_log.levelattr[4] = A_BLINK;   // error
	init_pair(6, COLOR_RED, g_log.background);     g_log.levelcolor[5] = 6; g_log.levelattr[5] = A_REVERSE; // fatal
	init_pair(7, COLOR_WHITE, g_log.background);   g_log.filenamecolor = 7; g_log.filenameattribute = A_UNDERLINE;
	init_pair(8, COLOR_CYAN, g_log.background);    g_log.msgcolor = 8;      g_log.msgattribute = A_NORMAL;


	if (LINES < length.window.stdscr.minl) {
		log_warn("The terminal widndow is too low.");
		log_nl("It has %d lines that is %d less tnan min. req. %d lines.",
				LINES, length.window.stdscr.minl - LINES, length.window.stdscr.minl);
		flag.curses.small_window = true;
	}
	if (COLS < length.window.stdscr.minc) {
		log_warn("The terminal widndow is too narrow.");
		log_nl("It has %d columns that is %d less tnan min. req. %d columns.",
					COLS, length.window.stdscr.minc - COLS, length.window.stdscr.minc);
		flag.curses.small_window = true;
	}
	if (flag.curses.small_window == true) {
		log_fatal("Could not fit all iterface in such small window. (%d lines by %d columns)",
				LINES, COLS);
		log_nl("Please, increase terminal size to at least %d lines by %d columns.", 
				length.window.stdscr.minl, length.window.stdscr.minc);
 		wrefresh(window.log);
 		wgetch(window.log);
 		fatal_error("Terminal is too small. Resize it to at least %d lines by %d columns.", length.window.stdscr.minl, length.window.stdscr.minc);
	}


	
	return EXIT_SUCCESS;
}

void deinit(void) {
	endwin();
}

