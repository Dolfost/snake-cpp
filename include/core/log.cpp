#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void fatal_error(const char* tmp, ...) {
	endwin();
	fprintf(stderr, "%s: fatal: ", execname);

	va_list ap;
	va_start(ap, tmp);
	fprintf(stderr, tmp, ap);
	va_end(ap);

	fprintf(stderr, ".\n");
	exit(EXIT_FAILURE);
}

void error(const char* msg) {
	endwin();
	fprintf(stderr, "%s: error: %s.\n", execname, msg);
}

void log(char type, const char* tmp, ...) {
	double time = (double)clock() / (double)CLOCKS_PER_SEC;

	static char const* preffix[] = {
					   "Warning: ", // 0
					   "Status:  ", // 1
					   "Error:   ", // 2
					  };

	signed short index;
	static FILE* logfile = fopen(flag.core.logpath, "w+");
	if (logfile == NULL) {
		fatal_error("Could not open %s", flag.core.logpath);
	}
	
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


	wattron(window.log, color.log.msg[4]);
	fprintf(logfile, "%lfs ", time);
	wprintw(window.log, "%lfs ", time);
	wattroff(window.log, color.log.msg[4]);

	wattron(window.log, color.log.msg[3]);
	fprintf(logfile, "%s: ", execname);
	wprintw(window.log,"%s: ", execname);
	wattroff(window.log, color.log.msg[3]);

	if (type == 'w')
		wattron(window.log, A_BOLD);
	else if (type == 'e')
		wattron(window.log, A_BLINK);
	
	wattron(window.log, color.log.msg[index]);
	fprintf(logfile, "%s", preffix[index]);
	waddstr(window.log, preffix[index]);
	wattroff(window.log, color.log.msg[index]);

	if (type == 'w')
		wattroff(window.log, A_BOLD);
	else if (type == 'e')
		wattroff(window.log, A_BLINK);

	va_list ap;
	va_start(ap, tmp);

	vfprintf(logfile, tmp, ap);
	vwprintw(window.log, tmp, ap);

	va_end(ap);
	waddstr(window.log, ".\n");
	fprintf(logfile, ".\n");
}
