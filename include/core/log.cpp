#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void fatal_error(const char* msg) {
	endwin();
	fprintf(stderr, "%s: fatal: %s.\n", execname, msg);
	exit(EXIT_FAILURE);
}

void log(char type, const char* tmp, ...) {
	char const* preffix[] = {
					   "Warning: ", // 0
					   "Status:  ", // 1
					   "Error:   ", // 2
					  };

	signed short index;

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

	wattron(window.log, color.log.msg[4]);
	wprintw(window.log, "%lfs ", (double)clock() / (double)CLOCKS_PER_SEC);
	wattroff(window.log, color.log.msg[4]);

	wattron(window.log, color.log.msg[3]);
	waddstr(window.log, execname);
	wattroff(window.log, color.log.msg[3]);

	waddstr(window.log, ": ");

	if (type == 'w')
		wattron(window.log, A_BOLD);
	else if (type == 'e')
		wattron(window.log, A_BLINK);
	
	wattron(window.log, color.log.msg[index]);
	waddstr(window.log, preffix[index]);
	wattroff(window.log, color.log.msg[index]);

	if (type == 'w')
		wattroff(window.log, A_BOLD);
	else if (type == 'e')
		wattroff(window.log, A_BLINK);

	vwprintw(window.log, tmp, ap);
	va_end(ap);
	waddstr(window.log, ".\n");
}
