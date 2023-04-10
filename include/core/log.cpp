#include<stdio.h>
#include<stdlib.h>

void fatal_error(const char* msg) {
	endwin();
	fprintf(stderr, "%s: fatal: %s.\n", execname, msg);
	exit(EXIT_FAILURE);
}

// void log_warning(const char* tmp, ...) {
// 	va_list ap;
// 
// 	wprintw(window.log, "%s: Warning: ", execname);
// 		
// 	va_start(ap, tmp);
// 	vwprintw(window.log, tmp, ap);
// 	va_end(ap);
// 
// 	waddstr(window.log, ".\n");
// }

void log(char type, const char* tmp, ...) {
	char const* preffix[] = {
					   "Warning: ", // 0
					   "Status:  ", // 1
					   "Error:   ", // 2
					  };

	signed short index;
	init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);

	switch(type) {
		case 'w': {
					  index = 0;
					  break;
				  }
		case 's': {
					  index = 1;

					  break;
				  }
		case 'e': {
					  index = 2;
					  break;
				  }
		default: {
					 index = 0;
				 }
	}

	va_list ap;
	va_start(ap, tmp);
	
	wattron(window.log, COLOR_PAIR(4));
	waddstr(window.log, execname);
	wattroff(window.log, COLOR_PAIR(4));

	waddstr(window.log, ": ");

	if (type == 'e')
		wattron(window.log, A_BLINK);
	else if (type == 'w')
		wattron(window.log, A_BOLD);
	
	wattron(window.log, COLOR_PAIR(index + 1));
	waddstr(window.log, preffix[index]);
	wattroff(window.log, COLOR_PAIR(index + 1));

	if (type == 'e')
		wattroff(window.log, A_BLINK);
	else if (type == 'w')
		wattroff(window.log, A_BOLD);

	vwprintw(window.log, tmp, ap);
	va_end(ap);
	waddstr(window.log, ".\n");
}
