#include <stdlib.h>

Windows window;
Parameters status; 

int init(void) {
	atexit(deinit);

	curs_set(0);
	
	if ((window.stdscr = initscr()) == NULL)
		fatal_error("Could not initialize standart window");

	if ((window.log = newwin(0,0,0,0)) == NULL)
		fatal_error("Could not initialize log window");

	if ((window.help = newwin(0,0,0,0)) == NULL)
		fatal_error("Could not initialize help window");
	
	return EXIT_SUCCESS;
}

void deinit(void) {
	endwin();
}

