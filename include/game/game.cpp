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
		gameinput();

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

bool gamepause(int ch) {
	if (ch == 'p' || ch == 'P') {
		
		touchwin(window.pause);
		wrefresh(window.pause);
		
		struct timespec start;
		struct timespec end;

		clock_gettime(CLOCK_MONOTONIC, &start);
		
		while (true) {
			noecho();
			ch = wgetch(window.pause);

			if (ch == ERR)
				continue;
			else if (ch == 'p' || ch == 'P')
				break;
			else if (help(ch)) {
				drawgame();
				touchwin(window.pause);
				wrefresh(window.pause);
			} else if (exitgame(ch)) {
				drawgame();
				touchwin(window.pause);
				wrefresh(window.pause);
			} else if (gamelog(ch)) {
				drawgame();
				touchwin(window.pause);
				wrefresh(window.pause);
			}
		}

		touchwin(window.game);
		wrefresh(window.game);

		napms(flag.option.pausetimeout);

		clock_gettime(CLOCK_MONOTONIC, &end);

		snake.pause_time += timediff(&start, &end);

		flushinp(); // clear garbage

		log_debug("Game has been paused for %0.3fs. [%c]", snake.pause_time, ch);

		echo();
		return true;
	} else
		return false;
}

bool gamelog(int ch) {
	length.pad.log.vl = length.pad.log.minl - (LINES - 2) - 1;
	if (ch == 'l' || ch == 'L') {
		log_debug("Opened log pad. [%c]", ch);
		box(window.stdscr, 0, 0);
		mvwaddstr(window.stdscr, 0, 2, "Snake log");
		wrefresh(window.stdscr);
		touchwin(pad.log);
		prefresh(pad.log, length.pad.log.vl, 0, 1, 1, LINES - 2, COLS - 1);

		input_log();

		wborder(window.stdscr, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
		return true;
	} else
		return false;
}


bool help(int ch) {
	length.pad.help.vl = 0;
	if (ch == 'h' || ch == 'H') { 
		log_debug("Entered help window. [%c]", ch);
		wclear(window.stdscr);
		box(window.stdscr, 0, 0);
		mvwaddstr(window.stdscr, 0, 2, "Snake help");
		wrefresh(window.stdscr);
		prefresh(pad.help, 0, 0, 1, 1, LINES - 2, length.window.game.minc - 2);
		
		input_help();

		wborder(window.stdscr, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
		return true;
	} else
		return false;
}

bool exitgame(int ch) {
	if (ch == 'Q' || ch == 'q') {
		log_debug("Entered exit window. [%c]", ch);
		if (ch == 'Q') {
			log_debug("Force game exit. [%c]", ch);
			exit(EXIT_SUCCESS);
		}
		
		touchwin(window.exit);
		wrefresh(window.exit);

		input_exit();
		
		return true;
	} else
		return false;
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








