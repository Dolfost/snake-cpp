#include <stdlib.h>

Snake snake;
Bait bait;
// Game game; // defined in init.cpp

int get_random(int lower_bound, int upper_bound) {
	return rand() % (upper_bound + 1 - lower_bound) + lower_bound;
}

void setup(void) {
	log_trace("Setup function have started.");

	// help window colors
	color.helpbg = COLOR_BLACK;
	init_pair(13, COLOR_YELLOW,  color.helpbg); color.pair.help.key = 13; attribute.help.key = A_BOLD;
	init_pair(14, COLOR_MAGENTA, color.helpbg); color.pair.help.title = 14; attribute.help.title = A_BOLD;
	init_pair(15, COLOR_GREEN,   color.helpbg); color.pair.help.keyword = 15; attribute.help.keyword = A_UNDERLINE;
	init_pair(16, COLOR_CYAN,    color.helpbg); color.pair.help.definition = 16; attribute.help.definition = A_UNDERLINE;


	// --log-scrollback
	if (length.pad.log.minl != CORE_DEFAULT_LOG_SCROLLBACK) {
		int helppadcopy;
		WINDOW* newlogpad = NULL;
		
		log_debug("Changing log pad scrollback");
		log_nl(   "from %d to %d lines.", CORE_DEFAULT_LOG_SCROLLBACK, length.pad.log.minl);

		if ((newlogpad = newpad(length.pad.log.minl, length.pad.log.minc)) == NULL) {
			log_fatal("Could not initialize new log pad. (%d lines)", length.pad.log.minl);
			fatal_error("Could not change the log pad scrollback to %d lines.", length.pad.log.minl);
		} else
			log_debug("Initialized new log pad succesfully. (%d lines)", length.pad.log.minl);

		if (length.pad.log.minl > CORE_DEFAULT_LOG_SCROLLBACK) { // TODO fix this (new line after copying window)
			// log_fatal("%d %d %d", CORE_DEFAULT_LOG_SCROLLBACK, length.pad.log.minl, length.pad.log.minc);
			helppadcopy = copywin(pad.log, newlogpad, 0, 0, // something is wrong
					length.pad.log.minl - CORE_DEFAULT_LOG_SCROLLBACK - 1, 0,
					length.pad.log.minl - 2, length.pad.log.minc - 1, // sould be length.pad.log.minl - 1, but is causes segfault. WTF???
					TRUE);
		} else {
			helppadcopy = copywin(pad.log, newlogpad, CORE_DEFAULT_LOG_SCROLLBACK - length.pad.log.minl - 1, 0,
					0, 0,
					length.pad.log.minl - 1, length.pad.log.minc - 1, FALSE);
		}

		if (helppadcopy == ERR) {
			log_fatal("Could not copy old log pad to new one.");
			fatal_error("Could not copy old log pad to new one.");
		}

			delwin(pad.log);
			log_log_remove(pad.log);
			pad.log = newlogpad;
			wmove(pad.log, length.pad.log.minl - 1, 0);
			scrollok(pad.log, TRUE);
			log_log_add(pad.log);
			log_debug("Copied log pad to new pad successfully.");
	}


	// help pad initialization
	FILE* helpfile = fopen(flag.option.helppadpath, "r");
	if (helpfile == NULL) {
		log_error("Could not open help pad file");
		log_nl(  "'%s' for reading.", flag.option.helppadpath);
	} else {
		if ((pad.help = getwin(helpfile)) == NULL) {
			log_fatal("Could not initialize help pad from");
			log_nl(   "'%s'.", flag.option.helppadpath);
			fatal_error("Could not initialize help pad.");
		} else
			log_debug("Initialized help pad succsessfully.");
		fclose(helpfile);
	}
	if (keypad(pad.help, TRUE) == ERR) {
		log_error("Could not initialize function keys for help pad.");
		log_nl(   "Arrow keys might not work properly.");
	} else
		log_debug("Initialized function keys for help pad successfully.");	


	wtimeout(window.game, flag.option.timeout);
	cbreak();

	srand(time(NULL));
	
	gamesetup();
	game.playagain = true;

	// mouse fear stuff
	color.gamebg = COLOR_BLACK;
	init_pair(9,  COLOR_WHITE,  color.gamebg); bait.fearcolor[3] = 9;  bait.fearattr[3] = A_BOLD;
	init_pair(10, COLOR_YELLOW, color.gamebg); bait.fearcolor[2] = 10; bait.fearattr[2] = A_BOLD;
	init_pair(11, COLOR_BLUE,   color.gamebg); bait.fearcolor[1] = 11; bait.fearattr[1] = A_NORMAL;
	init_pair(12, COLOR_RED,    color.gamebg); bait.fearcolor[0] = 12; bait.fearattr[0] = A_BOLD;

	bait.fearradius[0] = 5;
	bait.fearradius[1] = 10;
	bait.fearradius[2] = 15;

	bait.eaten = false;
	
	spawn_bait();
	
	wattrset(window.bar, A_BOLD);
	waddstr(window.bar, " Score: ");	
	wattroff(window.bar, A_BOLD);
	length.bar.scorename = 8; // " Score: " length
	length.bar.score = 8; // score number is not wider than 4 spaces
	wattrset(window.bar, A_BOLD);
	mvwaddstr(window.bar, 0, length.bar.scorename + length.bar.score, "Time: ");
	wattroff(window.bar, A_BOLD);
	length.bar.timename = 6;
	length.bar.time = 6; // >99.9s

	// led colors
	init_pair(17, COLOR_BLACK, COLOR_GREEN);   color.pair.led[3] = 17; 
	init_pair(18, COLOR_BLACK, COLOR_YELLOW);  color.pair.led[2] = 18;
	init_pair(19, COLOR_BLACK, COLOR_MAGENTA); color.pair.led[1] = 19;
	init_pair(20, COLOR_BLACK, COLOR_RED);     color.pair.led[0] = 20;

	if (flag.option.buildhelppad == true) {
		log_debug("Buildind help to '%s'...", flag.option.helppadpath);
		buildhelppad(flag.option.helppadpath);
		exit(EXIT_SUCCESS);
	}

	if (flag.option.help == true) {
		help('H');
		drawgame();
	}

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

void desetup(void) {
	log_trace("Desetup function have started.");
	free(snake.body);

	if (game.game == true) {
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
				written += fwrite(&game.hightime[idx], sizeof(short), 1, scores);
			}
			log_debug("Written %d bytes to 'data/scores.dat'.", written);
			fclose(scores);
		}
	}

}
