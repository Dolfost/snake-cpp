#include <math.h>

void spawn_bait(void) {
	bool is_taken = true;

	while (is_taken == true) {
		is_taken = false;
		bait.position.y = get_random(0, length.window.game.minl - 1);
		bait.position.x = get_random(0, length.window.game.minc - 1);
		for (int i = 0; i < snake.length; i++) {
			if (snake.body[i].y == bait.position.y && snake.body[i].x == bait.position.x) {
				is_taken = true;
				break;
			}
		}
	}
	bait.distance = game.distance;
	log_debug("Bait spawned at (%d;%d).", bait.position.y, bait.position.x);
}

void gameloop(void) {
	log_trace("Game loop have started.");

	wrefresh(window.stdscr);
	draw();

	clock_gettime(CLOCK_MONOTONIC, &snake.time_start);

	while (snake.hit != true && snake.bit != true) {
		input();

		move();

		wallhit();

		game.distance = sqrt(pow(snake.head.x - bait.position.x, 2) + pow(snake.head.y - bait.position.y, 2));
		
		draw();
	}
	beep();

	log_debug("The final score equals %d points.", game.score);
	log_debug("The final length equals %d.", snake.length);

	desetup();
}

void gamepause(void) {
	noecho();
	
	touchwin(window.pause);
	wrefresh(window.pause);
	
	struct timespec start;
	struct timespec end;

	clock_gettime(CLOCK_MONOTONIC, &start);
	
	while (getch() != 'p');

	touchwin(window.game);
	wrefresh(window.game);

	napms(flag.option.pausetimeout);

	clock_gettime(CLOCK_MONOTONIC, &end);

	snake.pause_time += timediff(&start, &end);

	flushinp(); // clear garbage

	echo();
}

void help(void) {
	wclear(window.stdscr);
	box(window.stdscr, 0, 0);
	mvwaddstr(window.stdscr, 0, 2, "Snake help");
	wrefresh(window.stdscr);
	prefresh(pad.help, 0, 0, 1, 1, length.window.game.minl - 1, length.window.game.minc - 1);
	
	input_help();


	wborder(window.stdscr, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	drawstdlines();
	drawgame();
}

char* fgetline(FILE* filea) {
	static FILE* file;

	if (filea != NULL)
		file = filea;

	long int position = ftell(file);

	if (position == -1) {
		log_fatal("Could not get position from file.");
		return NULL;
	}

	long int len = 0;
	int ch;

	do {
		ch = fgetc(file);
		len++;
	} while (ch != '\n' || ch != EOF);
	len--;

	char* str = (char*)malloc(sizeof(char)*len + 1);

	fseek(file, SEEK_SET, position);

	for (int i = 0; i < len; i++) {
		str[i] = fgetc(file);
	}

	return str;
}


long countlines(FILE* file) {
	long position = ftell(file);
	long lines = 0;

	if (file == NULL) {
		log_fatal("Got null pointer.");
		return -1;
	}

	int ch;

	do {
		ch = fgetc(file);
		if (ch == '\n')
			lines++;
	} while (ch != EOF); 
	lines++;

	fseek(file, SEEK_SET, position);

	return lines;
}




WINDOW* buildhelppad(const char* path) {
	log_debug("Entered buildhelppad().");

	FILE* padfile = fopen(path, "w+");
	if (padfile == NULL) {
		log_error("Could not open pad file");
		log_nl(   "'%s' for writting.");
		return NULL;
	}

	WINDOW* pad = newpad(length.pad.help.minl, length.pad.help.minc);


	wprintw(pad, "This is help pad ");
	incolor(pad, color.pair.help.keyword, attribute.help.keyword, "v%s", SNAKE_VERSION);
	waddstr(pad, ".\n");
	helppad_title(pad, "Help pad navigation");

	helppad_key(pad, "hq");
	
	waddstr(pad, "exit help pad.\n");

	helppad_key(pad, "asdf");
	waddstr(pad, "sussy baka.\n");

	putwin(pad, padfile);
	fclose(padfile);
	return pad;
}

void helppad_title(WINDOW* pad, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	waddstr(pad, "\n    ");
	wattrset(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
	waddch(pad, '-');

	wattrset(pad, COLOR_PAIR(color.pair.help.title) | attribute.help.title);
	vw_printw(pad, fmt, args);
	wattroff(pad, COLOR_PAIR(color.pair.help.title) | attribute.help.title);
	wattrset(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
	waddstr(pad, "-\n");

	wattroff(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);

	va_end(args);
}

void helppad_key(WINDOW* pad, const char* keys) {
	wprintw(pad, "  [");
	int printed = 0;
	for (int i = strlen(keys) - 1; i >= 0; i--, printed += 2) {
		wattrset(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
		waddch(pad, keys[i]);
		wattroff(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
		waddch(pad, ' ');
	}
	mvwprintw(pad, getcury(pad), getcurx(pad) - 1, "]");
	
	for (int i = (length.window.game.minc/8) - printed; i > 0; i--) {
		waddch(pad, ' ');
	}
}

void incolor(WINDOW* window, short col, int attr, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	wattrset(window, COLOR_PAIR(col) | attr);
	vw_printw(window, fmt, args);
	wattroff(window, COLOR_PAIR(col) | attr);

	va_end(args);
}




