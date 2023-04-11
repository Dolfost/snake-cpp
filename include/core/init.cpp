#include <stdlib.h>

Windows window;
Flags flag;
Colors color;
Lengths length;

int init(void) {
	atexit(deinit);

	flag.core.logpath = "data/log.txt";

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
		fatal_error("Could not initialize log window");
	else {	
		log('s', "Initialized standart window succsessfully");
		log('s', "Initialized log window succsessfully");
	}

	scrollok(window.log, TRUE);

	if (!(flag.curses.color = has_colors()))
		log('w', "This terminal does not support color");
	else
		log('s', "This terminal does support color");

	if (start_color() != OK)
		log('e', "Could not initialize color");
	else
		log('s', "Initialized colors successfully");


	// log colors
	color.log.background = COLOR_BLACK;
	init_pair(1, COLOR_MAGENTA, color.log.background);
	color.log.msg[0] = COLOR_PAIR(1);
	init_pair(2, COLOR_YELLOW, color.log.background);
	color.log.msg[1] = COLOR_PAIR(2);
	init_pair(3 , COLOR_RED, color.log.background);
	color.log.msg[2] = COLOR_PAIR(3);
	init_pair(4, COLOR_GREEN, color.log.background);
	color.log.msg[3] = COLOR_PAIR(4);
	init_pair(5, COLOR_BLUE, color.log.background);
	color.log.msg[4] = COLOR_PAIR(5);


	if (LINES < length.window.stdscr.miny) {
		log('w', "The terminal widndow is too small. It has %d lines that is %d less tnan min. req. %d lines",
				LINES, length.window.stdscr.miny - LINES, length.window.stdscr.miny);
		flag.curses.small_window = true;
	}
	if (COLS < length.window.stdscr.minx) {
		log('w', "The terminal widndow is too small. It has %d columns that is %d less tnan min. req. %d columns",
				COLS, length.window.stdscr.minx - COLS, length.window.stdscr.minx);
		flag.curses.small_window = true;
	}
	if (flag.curses.small_window == true) {
		log('e', "Could not fit all iterface in such small window (%d lines by %d columns). Please, increase terminal size to at least %d lines by %d columns",
				LINES, COLS, length.window.stdscr.miny, length.window.stdscr.minx);
 		wrefresh(window.log);
 		wgetch(window.log);
 		fatal_error("Terminal is too small. Resize it to at least %d lines by %d columns", length.window.stdscr.miny, length.window.stdscr.minx);
	}


	if ((window.help = newwin(0,0,0,0)) == NULL)
		fatal_error("Could not initialize help window");
	else
		log('s', "Initialized help window succsessfully");
	
	if (curs_set(0) == ERR)
		log('w', "This terminal does not support cursor visibilyty settings");
	else {
		flag.curses.cursor = true;
		log('s', "This terminal does support cursor visibilyty settings");
	}

	
	return EXIT_SUCCESS;
}

void deinit(void) {
	endwin();
}

