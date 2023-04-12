#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void fatal_error(const char* tmp, ...) {
	endwin();
	fprintf(stderr, "%s: Fatal: ", execname);

	va_list ap;
	va_start(ap, tmp);
	vfprintf(stderr, tmp, ap);
	va_end(ap);

	fprintf(stderr, "\n");
	exit(EXIT_FAILURE);
}

void memcheck(void* pointer, int size = 0) {
	if (pointer == NULL)
		fatal_error("Could not allocate memory.");

	if (size != 0)
		log_trace("Allocated %d bytes of memory.", size);
}


static struct Log {
	WINDOW* window = NULL;
	FILE* file = NULL;
	bool stdo = false;
	const char* preffixformat = "%s %-5s %s:%d:"; 
	short levelcolor[6];
	short filenamecolor;
	int filenameattribute;
	short msgcolor;
	int msgattribute;
	short background;
	int levelattr[6];
	int padding = 0;
} log;

const char* levelstr[] = {
  "Trace", "Debug", "Info", "Warn", "Error", "Fatal"
};

void log_log(int level, const char* filename, int line, const char* fmt, ...) {
	clock_t t = clock();
	double time = (double)t / (double)CLOCKS_PER_SEC;

	va_start(ap, fmt);

	// message string
	int msglen = vsnprintf(NULL, 0, fmt, ap);
	char* msgstr = (char*)malloc(sizeof(char)*msglen + 1);
 	memcheck(msgstr);
	vsnprintf(msgstr, msglen + 1, fmt, ap);

	va_end(ap);

	if (level == LOG_NL) {
		if (log.file != NULL) {
			for (short i = 0; i < log.padding; i++) {
				fprintf(log.file, " ");
			}
			fprintf(log.file, "%s\n", msgstr);
		}

		if (log.window != NULL) {
			wattrset(log.window, COLOR_PAIR(log.msgcolor) | log.msgattribute);
			mvwaddstr(log.window, getcury(log.window), log.padding, msgstr);
			wattroff(log.window, COLOR_PAIR(log.msgcolor) | log.msgattribute);
			waddstr(log.window, "\n");
		}

		if (log.stdo == true) {
			for (short i = 0; i < log.padding; i++) {
				putchar(' ');
			}
			printf("%s\n", msgstr);
		}	

		free(msgstr);
		return;
	}

	// time string
	int timelen = snprintf(NULL, 0,"%lfs", time);
	char* timestr = (char*)malloc(sizeof(char)*timelen + 1);
 	memcheck(timestr);
	snprintf(timestr, timelen + 1, "%lfs", time);

	// preffix string
	int preffixlen = snprintf(NULL, 0, log.preffixformat, 
			timestr, 
			levelstr[level],
			filename,
			line);
	log.padding = preffixlen + 1;
	char* preffixstr = (char*)malloc(sizeof(char)*preffixlen + 1);
	memcheck(preffixstr);
	snprintf(preffixstr, preffixlen + 1, log.preffixformat,
			timestr,
			levelstr[level],
			filename,
			line);

	if (log.file != NULL)
		fprintf(log.file, "%s %s\n", preffixstr, msgstr);

	if (log.stdo == true)
		fprintf(stdout, "%s %s\n", preffixstr, msgstr);


	if (log.window != NULL) {
		waddstr(log.window, timestr);
		waddch(log.window, ' ');

		wattrset(log.window, COLOR_PAIR(log.levelcolor[level]) | log.levelattr[level]);
		wprintw(log.window, "%-5s", levelstr[level]);
		wattroff(log.window, COLOR_PAIR(log.levelcolor[level]) | log.levelattr[level]);

		waddch(log.window, ' ');

		wattrset(log.window, COLOR_PAIR(log.filenamecolor) | log.filenameattribute);
		waddstr(log.window, filename);
		wattroff(log.window, COLOR_PAIR(log.filenamecolor) | log.filenameattribute);
		
		waddch(log.window, ':');
		wprintw(log.window, "%d", line);
		waddch(log.window, ':');
		waddch(log.window, ' ');

		wattrset(log.window, COLOR_PAIR(log.msgcolor) | log.msgattribute);
		waddstr(log.window, msgstr);
		wattroff(log.window, COLOR_PAIR(log.msgcolor) | log.msgattribute);

		waddstr(log.window, "\n");
	}

	free(msgstr);
	free(preffixstr);
	free(timestr);
}
