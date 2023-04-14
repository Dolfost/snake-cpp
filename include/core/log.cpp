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
	FILE* file[LOG_LOG_MAX_FILES] = {NULL};
	short files = 0;
	int padding = 0;
	const char* preffixformat = "%s %-5s %s:%d:"; 
	const char* timeformat = "%H:%M:%S";

	#ifdef __NCURSES_H
	WINDOW* window[LOG_LOG_MAX_WINDOWS] = {NULL};
	short windows = 0;
	short levelcolor[6];
	int levelattr[6];
	short filenamecolor;
	int filenameattribute;
	short msgcolor;
	int msgattribute;
	short background;
	#endif /* ifdef __NCURSES_H */
} g_log;

const char* log_log_levelstr[] = {
  "Trace", "Debug", "Info", "Warn", "Error", "Fatal"
};

int log_log_add(FILE* file) {
	if (g_log.files >= LOG_LOG_MAX_FILES)
		return -1;
	else
		g_log.file[g_log.files++] = file;
	return 0;
}

int log_log_remove(FILE* file) {
	int i;
	bool found = false;
	for (i = 0; i < g_log.files; i++) {
		if (g_log.file[i] == file) {
			found = true;
			break;
		}
	}

	if (found == false)
		return -1;

	while (i < LOG_LOG_MAX_FILES - 1 && g_log.file[i+1] != NULL) {
		g_log.file[i] = g_log.file[i+1];
		i++;
	}
	g_log.file[i] =NULL;
	g_log.files--;
	return 0;
}

#ifdef __NCURSES_H
int log_log_add(WINDOW* window) {
	if (g_log.windows >= LOG_LOG_MAX_WINDOWS)
		return -1;
	else
		g_log.window[g_log.windows++] = window;
	return 0;
}

int log_log_remove(WINDOW* file) {
	int i;
	bool found = false;
	for (i = 0; i < g_log.windows; i++) {
		if (g_log.window[i] == file) {
			found = true;
			break;
		}
	}

	if (found == false)
		return -1;

	while (i < LOG_LOG_MAX_WINDOWS - 1 && g_log.window[i+1] != NULL) {
		g_log.window[i] = g_log.window[i+1];
		i++;
	}
	g_log.window[i] =NULL;
	g_log.windows--;
	return 0;
}
#endif /* ifdef __NCURSES_H */


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
		for (int i = 0; i < LOG_LOG_MAX_FILES && g_log.file[i] != NULL; i++) {
			for (short j = 0; j < g_log.padding; j++) {
				fprintf(g_log.file[i], " ");
			}
			fprintf(g_log.file[i], "%s\n", msgstr);
		}

		for (int i = 0; i < LOG_LOG_MAX_WINDOWS && g_log.window[i] != NULL; i++) {
			wattrset(g_log.window[i], COLOR_PAIR(g_log.msgcolor) | g_log.msgattribute);
			mvwaddstr(g_log.window[i], getcury(g_log.window[i]), g_log.padding, msgstr);
			wattroff(g_log.window[i], COLOR_PAIR(g_log.msgcolor) | g_log.msgattribute);
			waddstr(g_log.window[i], "\n");
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
			log_log_levelstr[level],
			filename,
			line);
	g_log.padding = preffixlen + 1;
	char* preffixstr = (char*)malloc(sizeof(char)*preffixlen + 1);
	memcheck(preffixstr);
	snprintf(preffixstr, preffixlen + 1, g_log.preffixformat,
			timestr,
			log_log_levelstr[level],
			filename,
			line);

	for (int i = 0; i < LOG_LOG_MAX_FILES && g_log.file[i] != NULL; i++) {
		fprintf(g_log.file[i], "%s %s\n", preffixstr, msgstr);
	}

	#ifdef __NCURSES_H
	for (int i = 0; i < LOG_LOG_MAX_WINDOWS && g_log.window[i] != NULL; i++) {
		waddstr(g_log.window[i], timestr);
		waddch(g_log.window[i], ' ');
	
		wattrset(g_log.window[i], COLOR_PAIR(g_log.levelcolor[level]) | g_log.levelattr[level]);
		wprintw(g_log.window[i], "%-5s", log_log_levelstr[level]);
		wattroff(g_log.window[i], COLOR_PAIR(g_log.levelcolor[level]) | g_log.levelattr[level]);
	
		waddch(g_log.window[i], ' ');
	
		wattrset(g_log.window[i], COLOR_PAIR(g_log.filenamecolor) | g_log.filenameattribute);
		waddstr(g_log.window[i], filename);
		wattroff(g_log.window[i], COLOR_PAIR(g_log.filenamecolor) | g_log.filenameattribute);
	
		waddch(g_log.window[i], ':');
		wprintw(g_log.window[i], "%d", line);
		waddch(g_log.window[i], ':');
		waddch(g_log.window[i], ' ');
	
		wattrset(g_log.window[i], COLOR_PAIR(g_log.msgcolor) | g_log.msgattribute);
		waddstr(g_log.window[i], msgstr);
		wattroff(g_log.window[i], COLOR_PAIR(g_log.msgcolor) | g_log.msgattribute);
	
		waddstr(g_log.window[i], "\n");
	}
	#endif /* ifdef __NCURSES_H */
	

	free(msgstr);
	free(preffixstr);
}
