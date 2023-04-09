#include <stdlib.h>

Windows window;
Parameters status; 

int init(void) {
	atexit(deinit);

	window.stdscr = initscr();

	if ((window.log = newwin(0,0,0,0)) == NULL)
		fatal_error("Could not initialize log window");
	else {	
		log("Initialized standart window succsessfully");
		log("Initialized log window succsessfully");
	}

	if ((window.help = newwin(0,0,0,0)) == NULL)
		fatal_error("Could not initialize help window");
	else
		log("Initialized help window succsessfully");
	
	if (curs_set(0) == ERR)
		log("This terminal does not support cursor visibilyty settings");
	else
		log("This terminal does support cursor visibilyty settings");
		
	return EXIT_SUCCESS;
}

void deinit(void) {
	endwin();
}

