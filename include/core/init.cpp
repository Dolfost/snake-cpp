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

	if (!(flag.curses.color = has_colors()))
		log('s', "This terminal does not support color");
	else
		log('s', "This terminal does support color");

	if (start_color() != OK)
		log('e', "Could not initialize color");
	else
		log('s', "Initialized colors successfully");

	if (wattrset(window.log, COLOR_WHITE | COLOR_BLACK) == ERR)
		log('e', "Could not initialize log window color");
	else
		log('s', "Initialized log window color successfully");
	


	return EXIT_SUCCESS;
}

void deinit(void) {
	endwin();
}

