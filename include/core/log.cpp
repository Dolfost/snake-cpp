// #include "log.hpp" 

void fatal_error(const char* tmp, ...) {
	#ifdef __NCURSES_H
	endwin();
	#endif /* ifdef __NCURSES_H */
	
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


static struct Log_Log {
	FILE* file = NULL;
	bool stdo = false;
	int padding = 0;
	const char* preffixformat = "%s %-5s %s:%d:"; 
	const char* timeformat = "%H:%M:%S";

	#ifdef __NCURSES_H
	WINDOW* window = NULL;
	short levelcolor[6];
	short filenamecolor;
	int filenameattribute;
	short msgcolor;
	int msgattribute;
	short background;
	int levelattr[6];
	#endif /* ifdef __NCURSES_H */
} g_log;

const char* levelstr[] = {
  "Trace", "Debug", "Info", "Warn", "Error", "Fatal"
};

void log_log(int level, const char* filename, int line, const char* fmt, ...) {
	time_t t;
    time(&t);
    struct tm *tmp;
    tmp = localtime(&t);

	va_list ap;
	va_start(ap, fmt);

	// message string
	int msglen = vsnprintf(NULL, 0, fmt, ap);
	char* msgstr = (char*)malloc(sizeof(char)*msglen + 1);
 	memcheck(msgstr);
	vsnprintf(msgstr, msglen + 1, fmt, ap);

	va_end(ap);

	if (level == LOG_NL) {
		if (g_log.file != NULL) {
			for (short i = 0; i < g_log.padding; i++) {
				fprintf(g_log.file, " ");
			}
			fprintf(g_log.file, "%s\n", msgstr);
		}

		if (g_log.window != NULL) {
			wattrset(g_log.window, COLOR_PAIR(g_log.msgcolor) | g_log.msgattribute);
			mvwaddstr(g_log.window, getcury(g_log.window), g_log.padding, msgstr);
			wattroff(g_log.window, COLOR_PAIR(g_log.msgcolor) | g_log.msgattribute);
			waddstr(g_log.window, "\n");
		}

		if (g_log.stdo == true) {
			for (short i = 0; i < g_log.padding; i++) {
				putchar(' ');
			}
			printf("%s\n", msgstr);
		}	

		free(msgstr);
		return;
	}

	// time string
	static char timestr[9];
	strftime(timestr, 9, g_log.timeformat, tmp);

	// preffix string
	int preffixlen = snprintf(NULL, 0, g_log.preffixformat, 
			timestr, 
			levelstr[level],
			filename,
			line);
	g_log.padding = preffixlen + 1;
	char* preffixstr = (char*)malloc(sizeof(char)*preffixlen + 1);
	memcheck(preffixstr);
	snprintf(preffixstr, preffixlen + 1, g_log.preffixformat,
			timestr,
			levelstr[level],
			filename,
			line);

	if (g_log.file != NULL)
		fprintf(g_log.file, "%s %s\n", preffixstr, msgstr);

	if (g_log.stdo == true)
		fprintf(stdout, "%s %s\n", preffixstr, msgstr);


	#ifdef __NCURSES_H
	if (g_log.window != NULL) {
		waddstr(g_log.window, timestr);
		waddch(g_log.window, ' ');
	
		wattrset(g_log.window, COLOR_PAIR(g_log.levelcolor[level]) | g_log.levelattr[level]);
		wprintw(g_log.window, "%-5s", levelstr[level]);
		wattroff(g_log.window, COLOR_PAIR(g_log.levelcolor[level]) | g_log.levelattr[level]);
	
		waddch(g_log.window, ' ');
	
		wattrset(g_log.window, COLOR_PAIR(g_log.filenamecolor) | g_log.filenameattribute);
		waddstr(g_log.window, filename);
		wattroff(g_log.window, COLOR_PAIR(g_log.filenamecolor) | g_log.filenameattribute);
	
		waddch(g_log.window, ':');
		wprintw(g_log.window, "%d", line);
		waddch(g_log.window, ':');
		waddch(g_log.window, ' ');
	
		wattrset(g_log.window, COLOR_PAIR(g_log.msgcolor) | g_log.msgattribute);
		waddstr(g_log.window, msgstr);
		wattroff(g_log.window, COLOR_PAIR(g_log.msgcolor) | g_log.msgattribute);
	
		waddstr(g_log.window, "\n");
	}
	#endif /* ifdef __NCURSES_H */
	

	free(msgstr);
	free(preffixstr);
}
