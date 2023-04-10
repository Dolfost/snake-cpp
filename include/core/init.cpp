#include <stdlib.h>

Windows window;
Flags flag;
Colors color;

int init(void) {
	atexit(deinit);

	window.stdscr = initscr();

	if ((window.log = newwin(0,0,0,0)) == NULL)
		fatal_error("Could not initialize log window");
	else {	
		log('s', "Initialized standart window succsessfully");
		log('s', "Initialized log window succsessfully");
	}

	if (!(flag.curses.color = has_colors()))
		log('s', "This terminal does not support color");
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


	if ((window.help = newwin(0,0,0,0)) == NULL)
		fatal_error("Could not initialize help window");
	else
		log('s', "Initialized help window succsessfully");
	
	if (curs_set(0) == ERR)
		log('s', "This terminal does not support cursor visibilyty settings");
	else {
		flag.curses.cursor = true;
		log('s', "This terminal does support cursor visibilyty settings");
	}

	
	return EXIT_SUCCESS;
}

void deinit(void) {
	FILE* dump;
	if ((dump = fopen("data/log.txt", "w")) == NULL)
		error("Could not write log to disk");
	else
		putwin(window.log, dump);

	fclose(dump);
	endwin();
}

