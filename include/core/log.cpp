#include<stdio.h>
#include<stdlib.h>

void fatal_error(const char* msg) {
	fprintf(stderr, "%s: fatal: %s.\n", execname, msg);
	exit(EXIT_FAILURE);
}

void log_warning(const char* tmp, ...) {
	va_list ap;

	wprintw(window.log, "%s: Warning: ", execname);
		
	va_start(ap, tmp);
	vwprintw(window.log, tmp, ap);
	va_end(ap);

	waddstr(window.log, ".\n");
}

void log(const char* tmp, ...) {
	va_list ap1;

	wprintw(window.log, "%s: Status:  ", execname);

	va_start(ap1, tmp);
	vwprintw(window.log, tmp, ap1);
	va_end(ap1);

	waddstr(window.log, ".\n");
}
