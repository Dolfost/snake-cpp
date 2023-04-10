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
	char* string = NULL;
	char* msg;
	int len = 0;
	int stop = 0;
	static FILE* logfile = fopen(flag.core.logpath, "w+");
	if (logfile == NULL)
		fatal_error("Could not open %s", flag.core.logpath);
	
	switch(type) {
		case 'w':
					  index = 0;
					  break;
		case 's':
					  index = 1;
					  break;
		case 'e':
					  index = 2;
					  break;
		default:
					 index = 0;
	}

	// make string with the entire message
	va_list ap;
	va_start(ap, tmp);
	
	len += snprintf(NULL, 0, "%lfs %s: %s", time, execname, preffix[index]);
	len += vsnprintf(NULL, 0, tmp, ap);
	len += snprintf(NULL, 0, ".\n");

	if ((string = (char*)malloc(sizeof(char)*len)) == NULL)
		fatal_error("Could not allocate memory for log string (%d chars).", len);
	
	stop += snprintf(string, len, "%lfs %s: %s", time, execname, preffix[index]);
	msg = string + stop; // string with the context
	stop += vsnprintf(string + stop, len - stop, tmp, ap);
	stop += snprintf(string + stop, len - stop + 1, ".\n");
	
	fprintf(logfile, "%s", string);

	va_end(ap);

	// print to window.log
	wattron(window.log, color.log.msg[4]);
	wprintw(window.log, "%lfs ", time);
	wattroff(window.log, color.log.msg[4]);

	wattron(window.log, color.log.msg[3]);
	wprintw(window.log,"%s: ", execname);
	wattroff(window.log, color.log.msg[3]);

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
	
	waddstr(window.log, msg);
	free(string);
}
