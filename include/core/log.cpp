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
