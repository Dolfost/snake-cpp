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
	game.time = 0;
	snake.time = 0;

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

	spawn_bait();
}

void gameloop(void) {
	log_trace("Game loop have started.");
	game.game = true;

	drawgame();

	baradd("Press [H | h] for help");


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

		if (game.score > 0)
			doscores();

		playagain();

		if (game.playagain == false)
			gamestill();	
		
		wclear(window.game);

		free(snake.body);
		gamesetup();
	}
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
		baradd("Paused");
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
			} else if (score(ch)) {
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
		baradd(NULL);

		return true;
	} else
		return false;
}

void gamestill(void) {
	log_trace("Entered standby mode.");
	setled(2);
	baradd("Standby. [S | s] to restart the game");
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
		 else if (score(ch))
			drawgame();
	}

	log_trace("Left standby mode.");
	baradd(NULL);
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

bool score(int ch) {
	length.pad.score.vl = 0;
	if (ch == 'b' || ch == 'B') { 
		log_trace("Entered scoreboard pad. [%c]", ch);
		putscore();
		drawscore();
		
		input_score();

		wborder(window.stdscr, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
		return true;
	} else
		return false;
}

void putscore(void) {
	wclear(pad.score);
//	center(pad.score, 0, "<-< HISCORES >->");
	wattrset(pad.score, A_BOLD);
	mvwprintw(pad.score, 0, 1, "No");
	mvwprintw(pad.score, 0, 5, "Nickname");
	mvwprintw(pad.score, 0, 3*COLS/8, "Score");
	mvwprintw(pad.score, 0, 5*COLS/8, "Length");
	mvwprintw(pad.score, 0, 7*COLS/8, "Time");
	wattroff(pad.score, A_BOLD);

	for (int i = 0; i < game.scoreentry; i++) {
		mvwprintw(pad.score, i+1, 1, "%d", i+1);
		mvwprintw(pad.score, i+1, 5, "%s", game.highplayer[i]);
		mvwprintw(pad.score, i+1, 3*COLS/8, "%d", game.highscore[i]);
		mvwprintw(pad.score, i+1, 5*COLS/8, "%d", game.highlength[i]);
		mvwprintw(pad.score, i+1, 7*COLS/8, "%02dm:%02ds", game.hightime[i]/60, game.hightime[i]%60);
	}
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


void readdata(void) {
	FILE* playerfile = fopen("data/player.dat", "r");

	if (playerfile == NULL) {
		log_error("Could not open 'data/player.dat' for reading.");
		log_nl(   "Maybe there is no previous player.");
		*game.playername = '\0';
	} else {
		log_debug("Opened 'data/payer.dat' for reading succesfully.");
		size_t read = fread(game.playername, sizeof(char), length.game.maxnicknamelen, playerfile);
		log_debug("Read %d/%d bytes from 'data/payer.dat'.", read, length.game.maxnicknamelen);
		fclose(playerfile);
	}

	game.highscore = (short*)calloc(200, sizeof(short));
	memcheck(game.highscore, 200*sizeof(short));
	game.highlength = (short*)calloc(200, sizeof(short));
	memcheck(game.highlength, 200*sizeof(short));
	game.highplayer = (char**)calloc(200, sizeof(char*));
	memcheck(game.highplayer, 200*sizeof(char*));
	game.hightime = (short*)calloc(200, sizeof(short));
	memcheck(game.hightime, 200*sizeof(short));

	FILE* scores = fopen("data/scores.dat", "r");

	if (scores == NULL) {
		log_error("Could not open 'data/scores.dat' for reading.");
		log_nl(   "Maybe there is no saved scoreboard.");
		flag.core.score = false;
	} else {
		log_debug("Opened 'data/scores.dat' for reading sucessfully.");
		int read = 0;
		int i, j;
		for (int idx = 0; idx < 200; idx++) {
			game.highplayer[idx] = (char*)malloc(sizeof(char)*(length.game.maxnicknamelen + 1));
			j = -1;
			do {
				j++;
				i = fread(game.highplayer[idx] + j, sizeof(char), 1, scores);
				if (i < 1)
					break;
				read += i;
			} while (*(game.highplayer[idx] + j) != '\0');

			i = fread(&game.highscore[idx], sizeof(short), 1, scores);
			if (i < 1)
				break;
			read += i;
			i = fread(&game.highlength[idx], sizeof(short), 1, scores);
			if (i < 1)
				break;
			read += i;
			i = fread(&game.hightime[idx], sizeof(short), 1, scores);
			if (i < 1)
				break;
			read += i;

			game.scoreentry++;
		}
		log_debug("Read %d bytes from 'data/scores.dat'.", read);
		fclose(scores);
	}

	for (int i = 0; i < game.scoreentry; i++) {
		char* highplayer = (char*)malloc((strlen(game.highplayer[i]) + 1)*sizeof(char));
		memcheck(game.highplayer, (strlen(game.highplayer[i]) + 1)*sizeof(char));
		memcpy(highplayer, game.highplayer[i], strlen(game.highplayer[i]) + 1);
		free(game.highplayer[i]);
		game.highplayer[i] = highplayer;
	}
}

void writedata(void) {
	// save player name
	FILE* playerfile = fopen("data/player.dat", "w+");
	if (playerfile == NULL) {
		log_error("Could not open 'data/player.dat' for writing.");
	} else {
		log_error("Opened 'data/player.dat' for writing successfully.");
		int written;
		written = fwrite(game.playername, sizeof(char), length.game.maxnicknamelen, playerfile);
		log_debug("The %d/%d bytes has been written to 'data/player.dat'.", written, length.game.maxnicknamelen);
		fclose(playerfile);
	}

	// save score
	FILE* scores = fopen("data/scores.dat", "w");
	if (scores == NULL) {
		log_error("Could not open 'data/scores.dat' for writting.");
	} else {
		log_debug("Opened 'data/scores.dat' for writting sucessfully.");
		int written = 0;
		for (int idx = 0; idx < game.scoreentry; idx++) {
			written += fwrite(game.highplayer[idx], sizeof(char), strlen(game.highplayer[idx]) + 1, scores);
			written += fwrite(&game.highscore[idx], sizeof(short), 1, scores);
			written += fwrite(&game.highlength[idx], sizeof(short), 1, scores);
			written += fwrite(&game.hightime[idx], sizeof(short), 1, scores);
		}
		log_debug("Written %d bytes to 'data/scores.dat'.", written);
		fclose(scores);
	}
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

