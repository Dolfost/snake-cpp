#include <stdlib.h>

Windows window;
Subwindows subwindow;
Flags flag;
Colors color;
Lengths length;
Positions positions;

int init(void) {
	atexit(deinit);

	flag.core.logpath = "data/log.txt";

	if ((flag.core.logfile = fopen(flag.core.logpath, "w+")) == NULL)
		fatal_error("Coulnd not open log file '%s' for writing.", flag.core.logpath);

	log_log_add(flag.core.logfile);

	// windows sizes
	length.window.game.minl = 30; // 30
	length.window.game.minc = 60; // 60
	length.window.bar.minl = 1;
	length.window.bar.minc = length.window.game.minc;
	length.window.pause.minl = 5;
	length.window.pause.minc = 13;

	length.window.stdscr.minl = length.window.game.minl + length.window.bar.minl + 1;
	length.window.stdscr.minc = length.window.game.minc;
	length.window.sidelog.minl = length.window.stdscr.minl;
	length.window.sidelog.minc = 100;



	window.stdscr = initscr(); // if initscr() fails, it stops the program and prints error message


	// log window and stdscr initialization
	if ((window.log = newwin(0,0,0,0)) == NULL)
		fatal_error("Could not initialize log window.");
	else {	
		log_log_add(window.log);
		log_debug("Initialized standart window successfully.");
		log_debug("Initialized log window succsessfully.");
	}

	scrollok(window.log, TRUE);

	// colors check
	if (!(flag.curses.color = has_colors()))
		log_info("This terminal does not support color.");
	else
		log_info("This terminal does support color.");

	// colors initialization
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
	
	// small terminal check
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

	// pause window
	if ((window.pause = newwin(length.window.pause.minl, length.window.pause.minc,
					(length.window.game.minl - length.window.pause.minl ) / 2,
					(length.window.game.minc - length.window.pause.minc ) / 2)) == NULL)
		fatal_error("Could not initialize pause window.");
	else
		log_debug("Initialized pause window succsessfully.");
	box(window.pause, 0, 0);
	mvwaddstr(window.pause, length.window.pause.minl / 2,
			(length.window.pause.minc - 5 /* 5 is an word length */) / 2, "PAUSE");

	// help window initialization
	if ((window.help = newwin(0,0,0,0)) == NULL)
		fatal_error("Could not initialize help window.");
	else
		log_debug("Initialized help window succsessfully.");
	
	// two or one window
	if (COLS - length.window.game.minc - 1 >= length.window.sidelog.minc) {
		log_debug("Terminal window is large enough");
		log_nl(   "to fit game and log windows in it.");

		if ((window.sidelog = newwin(LINES, COLS - length.window.game.minc - 1, 0, length.window.game.minc + 2 - 1)) == NULL) {
			log_fatal("Could not initialize sidelog window.");
			fatal_error("Could not initialize sidelog window.");
		} else {
			log_log_add(window.sidelog);
			scrollok(window.sidelog, TRUE);
			log_debug("Initialized sidelog window succesfully.");
		}
	} else {
		log_debug("Terminal window is not large enough");
		log_nl(   "to fit game and log windows in it.");
	}

	// game window initialization
	if ((window.game = newwin(length.window.game.minl, length.window.game.minc, 0, 0)) == NULL) {
		log_fatal("Could not initialize game window.");
		fatal_error("Could not initialize game window.");
	} else 
		log_debug("Initialized game subwindow succesfully.");
	
	if (keypad(window.game, TRUE) == ERR) {
		log_error("Could not initialize function keys.");
		log_nl(   "Arrow keys might not work propersly.");
	} else
		log_debug("Initialized function keys successfully.");

	// bar subwindow initialization
	if ((window.bar = newwin(length.window.bar.minl, length.window.bar.minc, length.window.game.minl + 1, 0)) == NULL) {
		log_fatal("Could not initialize bar window.");
		fatal_error("Could not initialize bar window.");
	} else
		log_debug("Initialized bar window succesfully.");

	// cursor visibility check
	if (curs_set(0) == ERR)
		log_info("This terminal does not support cursor visibilyty settings.");
	else {
		flag.curses.cursor = true;
		log_info("This terminal does support cursor visibilyty settings.");
	}

	
	// lines drawing
	mvwvline(stdscr, 0, length.window.game.minc, 0, LINES);
	mvwhline(stdscr, length.window.game.minl, 0, 0, length.window.game.minc);


	#if SNAKE_WINDOW_POSITIONING_DEMO == 1
	init_pair(9, COLOR_WHITE, COLOR_RED);
	init_pair(10, COLOR_BLUE, COLOR_YELLOW);
	init_pair(11, COLOR_GREEN, COLOR_BLUE);
	
	wbkgd(window.game, COLOR_PAIR(9));
	wbkgd(window.sidelog, COLOR_PAIR(10));
	wbkgd(window.bar, COLOR_PAIR(11));
	refresh();
	wrefresh(window.game);
	wrefresh(window.sidelog);
	wrefresh(window.bar);
	getch();
	fatal_error("This was windows positioning test.");
	#endif /* if SNAKE_WINDOW_POSITIONING_DEMO == 1 */


	return EXIT_SUCCESS;
}

void deinit(void) {
	flushinp();
	endwin();
}

