#include <stdlib.h>

Snake snake;
Bait bait;
// Game game; // defined in init.cpp

int get_random(int lower_bound, int upper_bound) {
	return rand() % (upper_bound + 1 - lower_bound) + lower_bound;
}

void setup(void) {
	log_trace("Setup function have started.");


	// two or one window
	if (COLS - length.window.game.minc - 1 >= length.window.sidelog.minc) {
		log_debug("Terminal window is large enough");
		log_nl(   "to fit game and log windows in it.");

		if ((window.sidelog = newwin(LINES, COLS - length.window.game.minc - 1, 0, length.window.game.minc + 2 - 1)) == NULL) {
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


	// pause window
	if ((window.pause = newwin(length.window.pause.minl, length.window.pause.minc,
					(length.window.game.minl - length.window.pause.minl ) / 2,
					(length.window.game.minc - length.window.pause.minc ) / 2)) == NULL)
		fatal_error("Could not initialize pause window.");
	else
		log_debug("Initialized pause window succsessfully.");
	box(window.pause, 0, 0);
	wattrset(window.pause, A_BOLD);
	center(window.pause, length.window.pause.minl / 2, "PAUSE");
	wattroff(window.pause, A_BOLD);


	// again window
	if ((window.again = newwin(length.window.again.minl, length.window.again.minc,
					(length.window.game.minl - length.window.again.minl ) / 2,
					(length.window.game.minc - length.window.again.minc ) / 2)) == NULL) {
		log_fatal("Could not initialize again window.");
		fatal_error("Could not initialize again window.");
	} else
		log_debug("Initialized again window succsessfully.");
	box(window.again, 0, 0);
	wattrset(window.again, A_BOLD);
	center(window.again, 2, "Do you want to play again?");
	wattroff(window.again, A_BOLD);
	center(window.again, 3, "Y/y   N/n");

	
	// exit window
	if ((window.exit = newwin(length.window.exit.minl, length.window.exit.minc,
					(LINES - length.window.exit.minl ) / 2,
					(COLS - length.window.exit.minc ) / 2)) == NULL)
		fatal_error("Could not initialize exit window.");
	else
		log_debug("Initialized exit window succsessfully.");
	box(window.exit, 0, 0);
	wattrset(window.exit, A_BOLD);
	center(window.exit, 2, "Do you really want to exit?");
	wattroff(window.exit, A_BOLD);
	center(window.exit, 3, "Q/Y/q/y   C/N/c/n");
	

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

	
	// scoreboard pad initialization
	if ((pad.score = newpad(length.pad.score.minl, length.pad.score.minc)) == NULL)
		fatal_error("Could not initialize scoreboard pad.");
	else {	
		scrollok(pad.score, TRUE);
		log_debug("Initialized scoreboard pad succsessfully. (%d lines)", length.pad.score.minl);
	}
	if (keypad(pad.score, TRUE) == ERR) {
		log_error("Could not initialize function keys for scoreboard pad.");
		log_nl(   "Arrow keys might not work properly.");
	} else
		log_debug("Initialized function keys for scoreboard pad successfully.");	


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

	
	// line drawing
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


	readdata();

	if (flag.option.help == true) {
		help('H');
		drawgame();
	}
}

void desetup(void) {
	log_trace("Desetup function have started.");
	free(snake.body);

	if (game.game == true)
		writedata();
}
