#include <stdlib.h>

Windows window;
Subwindows subwindow;
Pads pad;
Flags flag;
Colors color;
Lengths length;
Positions positions;
Attributes attribute;

Game game;

int init(void) {
	atexit(deinit);
	
	flag.core.logpath = "data/log.txt";

	if ((flag.core.logfile = fopen(flag.core.logpath, "w+")) == NULL)
		fatal_error("Coulnd not open log file '%s' for writing.", flag.core.logpath);

	log_log_add(flag.core.logfile);


	window.stdscr = initscr(); // if initscr() fails, it stops the program and prints error message
	log_debug("Initialized standart window successfully.");


	// windows size
// 	length.window.game.minl = 22; // lldb
// 	length.window.game.minc = 80; // lldb
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

	length.pad.help.minl = 50;
	length.pad.help.minc = length.window.game.minc;
	length.pad.log.minl = CORE_DEFAULT_LOG_SCROLLBACK;
	length.pad.log.minc = COLS - 2;

	length.window.finals.minl = 12;
	length.window.finals.minc = length.window.game.minc / 2;

	
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


	
	// two or one window
	if (COLS - length.window.game.minc - 1 >= length.window.sidelog.minc) {
		log_debug("Terminal window is large enough");
		log_nl(   "to fit game and log windows in it.");

		if ((window.sidelog = newwin(LINES + 1, COLS - length.window.game.minc - 1, 0, length.window.game.minc + 2 - 1)) == NULL) {
			log_fatal("Could not initialize sidelog window.");
			fatal_error("Could not initialize sidelog window.");
		} else {
			wmove(window.sidelog, LINES - 1, 0);
			log_log_add(window.sidelog);
			scrollok(window.sidelog, TRUE);
			log_debug("Initialized sidelog window succesfully.");
			log_info("Above log source do not contain all logs");
			log_nl(  "because it is initialized too late.");
			log_nl(  "For full log see '%s' file.", flag.core.logpath);

		}
	} else {
		log_debug("Terminal window is not large enough");
		log_nl(   "to fit game and log windows in it.");
	}


	// colors check
	if (!(flag.curses.color = has_colors()))
		log_info("This terminal does not support color.");
	else
		log_info("This terminal does support color.");

	if ((flag.curses.change_color = can_change_color()) == FALSE)
		log_info("This terminal cannot change color.");
	else
		log_info("This terminal can change color.");

	// colors initialization
	if (start_color() != OK)
		log_error("Could not initialize color.");
	else
		log_debug("Initialized colors successfully.");

	// redefining colors
	if (flag.curses.change_color == true) {
		init_color(COLOR_BLACK, 0, 0, 0);
		init_color(COLOR_RED, 949, 47, 47);
		init_color(COLOR_GREEN, 149, 929, 149);
		init_color(COLOR_YELLOW, 1000, 933, 0);
		init_color(COLOR_BLUE, 0, 82, 1000);
		init_color(COLOR_MAGENTA, 1000, 0, 969);
		init_color(COLOR_CYAN, 0, 1000, 1000);
		init_color(COLOR_WHITE, 900, 900, 900);
	}

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
//  		wrefresh(window.log);
//  		wgetch(window.log);
  		gamelog('l');
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
	
	// exit window
	if ((window.exit = newwin(length.window.exit.minl, length.window.exit.minc,
					(LINES - length.window.exit.minl ) / 2,
					(COLS - length.window.exit.minc ) / 2)) == NULL)
		fatal_error("Could not initialize exit window.");
	else
		log_debug("Initialized exit window succsessfully.");
	box(window.exit, 0, 0);
	mvwaddstr(window.exit, 2, (length.window.exit.minc - 27 /* 27 is an sentetce length */) / 2, "Do you really want to exit?");
	mvwaddstr(window.exit, 3, (length.window.exit.minc - 13 /* 13 is an sentetce length */) / 2, "Y/y/q   N/n/c");
	

	// game window initialization
	if ((window.game = newwin(length.window.game.minl, length.window.game.minc, 0, 0)) == NULL) {
		log_fatal("Could not initialize game window.");
		fatal_error("Could not initialize game window.");
	} else 
		log_debug("Initialized game window succesfully.");
	
	if (keypad(window.game, TRUE) == ERR) {
		log_error("Could not initialize function keys for game window.");
		log_nl(   "Arrow keys might not work properly.");
	} else
		log_debug("Initialized function keys for game window successfully.");

	// finals window initialization
	if ((window.finals = newwin(length.window.finals.minl, length.window.finals.minc,
					(length.window.game.minl - length.window.finals.minl) / 2,
					(length.window.game.minc - length.window.finals.minc) / 2)) == NULL) {
		log_fatal("Could not initialize finals window.");
		fatal_error("Could not initialize finals window.");
	} else 
		log_debug("Initialized finals window succesfully.");
	if (keypad(window.finals, TRUE) == ERR) {
		log_error("Could not initialize function keys for finals window.");
		log_nl(   "Arrow keys might not work properly.");
	} else
		log_debug("Initialized function keys for finals window successfully.");
	
	wattrset(window.finals, A_BOLD);
	center(window.finals, 1, "Game is over");
	wattroff(window.finals, A_BOLD);

	// bar window initialization
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
	drawgamelines();

	#if SNAKE_WINDOW_POSITIONING_DEMO == 1
	init_paisr(9, COLOR_WHITE, COLOR_RED);
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

	length.game.maxnicknamelen = length.window.finals.minc - 3 - 26 - 3;
	game.playername = (char*)malloc(sizeof(char)*(length.game.maxnicknamelen + 1));
	memcheck(game.playername, sizeof(char)*(length.game.maxnicknamelen + 1));

	FILE* playerfile = fopen("data/player.dat", "r");
//	fwrite("Imposter\0\n", sizeof(char), 10, playerfile);

	if (playerfile == NULL) {
		log_error("Could not open playerdata.");
		log_nl(   "Maybe there is no previous player...");
		*game.playername = '\0';
	} else {
		log_debug("Opened playerdata succesfully.");
		size_t read = fread(game.playername, sizeof(char), length.game.maxnicknamelen, playerfile);
		log_debug("Read %d/%d bytes from playerdata.", read, length.game.maxnicknamelen);
		fclose(playerfile);
	}

	return EXIT_SUCCESS;
}


void deinit(void) {
	log_trace("Deinit functions has started.");
	flushinp();

	if (isendwin() == false)
		endwin();
}

