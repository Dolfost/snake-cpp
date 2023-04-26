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
	bait.distance = sqrt(pow(snake.head.x - bait.position.x, 2) + pow(snake.head.y - bait.position.y, 2));
	log_debug("Bait respawned at (%d;%d).", bait.position.y, bait.position.x);
}

void gamesetup(void) {
	game.score = 0;

	snake.bit = false;
	snake.hit = false;

	snake.body = (Point*)calloc(length.window.game.minl*length.window.game.minc, sizeof(Point));
	memcheck(snake.body, length.window.game.minl*length.window.game.minc*sizeof(Point));

	snake.body[1].y = -1;
	snake.body[1].x = -1;

	snake.direction = D_NONE;
	snake.travelled = 0;

	snake.head.y = get_random(0, length.window.game.minl - 1);
	snake.head.x = get_random(0, length.window.game.minc - 1);

	snake.body[0].y = snake.head.y;
	snake.body[0].x = snake.head.x;

	snake.length = 1;
	snake.pause_time = 0;
	log_debug("Snake spawned at (%d;%d).", snake.head.y, snake.head.x);
}

void gameloop(void) {
	log_trace("Game loop have started.");

	drawgame();



	while (game.playagain == true) {
		setled(3);

		clock_gettime(CLOCK_MONOTONIC, &snake.time_start);
		clock_gettime(CLOCK_MONOTONIC, &game.time_start);
		while (snake.hit != true && snake.bit != true) {
			draw();

			gameinput();

			move();

			wallhit();
			selfbit();
			dinner();

			game.distance = sqrt(pow(snake.head.x - bait.position.x, 2) + pow(snake.head.y - bait.position.y, 2));
		}

		clock_gettime(CLOCK_MONOTONIC, &game.time_end);
		game.time = timediff(&game.time_start, &game.time_end);
		beep();


		log_debug("The final score equals %d points.", game.score);
		log_debug("The final length equals %d.", snake.length);

		gameover();
		finals();
		playagain();

		if (game.playagain == false)
			gamestill();	
		
		wclear(window.game);

		free(snake.body);
		gamesetup();
	}

	desetup();
}

void finals(void) {
	log_trace("Entered finals window.");
	drawfinals();

	char* input = (char*)malloc(sizeof(char)*(length.game.maxnicknamelen + 1));
	memcheck(input, sizeof(char)*(length.game.maxnicknamelen + 1));

	curs_set(1);
	echo();

	wgetnstr(window.finals, input, length.game.maxnicknamelen);
	while (*game.playername == '\0' && *input == '\0')  {
		beep();
		setled(0);
		baradd("Enter a valid nickname!");
		wrefresh(window.bar);
		wclear(window.finals);
		drawfinals();
		wgetnstr(window.finals, input, length.game.maxnicknamelen);
	}
		baradd(NULL);
		setled(2);

	noecho();
	curs_set(0);

	if (*input == '\0') {
		free(input);
		log_debug("The user has chosen an old");
		log_nl   ("'%s' nickname.", game.playername);
	} else {
		free(game.playername);
		game.playername = input;
		log_debug("The user has entered an new");
		log_nl   ("'%s' nickname.", game.playername);
	}
	log_trace("Left finals window.");
}

void playagain(void) {
	log_trace("Entered again window.");
	drawagain();

	int ch;

	while (true) {
		ch = wgetch(window.again);
		if (ch == 'Y' || ch == 'y') {
			log_debug("Starting the game again. [%c]", ch);
			game.playagain = true;
			break;
		}
		if (ch == 'N' || ch == 'n') {
			log_debug("The game session is finished. [%c]", ch);
			game.playagain = false;
			break;
		}
	}

	log_trace("Left again window.");
}

void gameover(void) {
	drawover();

	wtimeout(window.game, 0);
	while (wgetch(window.game) != '\n');
	wmove(window.finals, 8, 9);
	wtimeout(window.game, flag.option.timeout);

	baradd(NULL);
	wrefresh(window.bar);
}

bool gamepause(int ch) {
	if (ch == 'p' || ch == 'P') {
		log_trace("Entered pause window. [%c]", ch);
		setled(0);
		
		touchwin(window.pause);
		wrefresh(window.pause);
		
		struct timespec start;
		struct timespec end;

		clock_gettime(CLOCK_MONOTONIC, &start);
		
		while (true) {
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

		for (short i = 0; i < 4; i++) {
			setled(i);
			napms(flag.option.pausetimeout / 4);
		}

		clock_gettime(CLOCK_MONOTONIC, &end);

		snake.pause_time += timediff(&start, &end);

		flushinp(); // clear garbage

		log_debug("Game has been paused for %0.3fs. [%c]", snake.pause_time, ch);

		return true;
	} else
		return false;
}

void gamestill(void) {
	log_trace("Entered standby mode.");
	setled(0);
	drawgame();

	int ch;

	while (true) {
		ch = wgetch(window.stdscr);
		if (ch == 's' || ch == 'S') {
			log_debug("Started a new game session. [%c]", ch);
			game.playagain = true;
			break;
		} else if (exitgame(ch))
			drawgame();	
		else if (gamelog(ch))
			drawgame();	
		else if (help(ch))
			drawgame();
	}

	log_trace("Left standby mode.");
}

bool gamelog(int ch) {
	length.pad.log.vl = length.pad.log.minl - (LINES - 2) - 1;
	if (ch == 'l' || ch == 'L') {
		log_trace("Opened log pad. [%c]", ch);
		drawlog();

		input_log();

		wborder(window.stdscr, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
		return true;
	} else
		return false;
}

bool help(int ch) {
	length.pad.help.vl = 0;
	if (ch == 'h' || ch == 'H') { 
		if (pad.help == NULL) {
			log_error("Could not open help pad. [%s]", keyname(ch));
			log_nl(   "Help pad isn't initialized.");
			baradd("Help pad isn't initialized");
			setled(0);
			return true;
		}
		log_trace("Entered help pad. [%c]", ch);
		drawhelp();
		
		input_help();

		wborder(window.stdscr, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
		return true;
	} else
		return false;
}

bool exitgame(int ch) {
	if (ch == 'Q' || ch == 'q') {
		log_trace("Entered exit window. [%c]", ch);
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

