#include <stdlib.h>

Snake snake;
Bait bait;
Game game;

int get_random(int lower_bound, int upper_bound) {
	return rand() % (upper_bound + 1 - lower_bound) + lower_bound;
}

void setup(void) {
	log_trace("Setup function have started.");

	// help pad initialization
	FILE* helpfile = fopen(flag.option.helppadpath, "r");
	if (helpfile == NULL) {
		log_error("Could not open help pad file.");
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


	wtimeout(window.game, flag.option.timeout);
	cbreak();

	srand(time(NULL));

	game.score = 0;

	snake.body = (Point*)calloc(length.window.game.minl*length.window.game.minc, sizeof(Point));
	memcheck(snake.body, length.window.game.minl*length.window.game.minc*sizeof(Point));

	snake.body[1].y = -1;
	snake.body[1].x = -1;

	snake.head.y = get_random(0, length.window.game.minl - 1);
	snake.head.x = get_random(0, length.window.game.minc - 1);

	snake.body[0].y = snake.head.y;
	snake.body[0].x = snake.head.x;

	snake.length = 1;
	snake.pause_time = 0;
	log_debug("Snake spawned at (%d;%d).", snake.head.y, snake.head.x);

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

	waddstr(window.bar, "Score: ");	
	length.bar.scorename = 7; // "Score: " length
	length.bar.score = 4; // score number is not wider than 4 spaces
	mvwaddstr(window.bar, 0, length.bar.scorename + length.bar.score, "Time: ");
	length.bar.timename = 6;
	length.bar.time = 6; // >99.9s
	

	// help window colors
	color.helpbg = COLOR_BLACK;
	init_pair(13, COLOR_YELLOW,  color.helpbg); color.pair.help.key = 13; attribute.help.key = A_BOLD;
	init_pair(14, COLOR_MAGENTA, color.helpbg); color.pair.help.title = 14; attribute.help.title = A_BOLD;
	init_pair(15, COLOR_GREEN,   color.helpbg); color.pair.help.keyword = 15; attribute.help.keyword = A_UNDERLINE;

	if (flag.option.buildhelppad == true) {
		log_debug("Buildind help to '%s'...", flag.option.helppadpath);
		buildhelppad(flag.option.helppadpath);
	}

	if (flag.option.help == true) {
		help();
	}
}

void desetup(void) {
	log_trace("Desetup function have started.");
	free(snake.body);
}
