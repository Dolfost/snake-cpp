#include <stdlib.h>

Snake snake;
Bait bait;

int get_random(int lower_bound, int upper_bound) {
	return rand() % (upper_bound + 1 - lower_bound) + lower_bound;
}

void setup(void) {
	log_trace("Setup function have started.");

	srand(time(NULL));


	floor = (short*)calloc(length.window.game.minl*length.window.game.minc, sizeof(short));
	memcheck(floor, length.window.game.minl*length.window.game.minc*sizeof(short));

	snake.position.y = get_random(0, length.window.game.minl - 1);
	snake.position.x = get_random(0, length.window.game.minc - 1);
	floor[snake.position.y*sizeof(short) + snake.position.x] = 1;
	mvwaddstr(window.game, snake.position.y, snake.position.x, "#");
	snake.length = 1;
	log_debug("Snake start coordinates is (%d;%d).", snake.position.y, snake.position.x);
	
	bait.position.y = get_random(0, length.window.game.minl - 1);
	bait.position.x = get_random(0, length.window.game.minc - 1);
	floor[bait.position.y*sizeof(short) + bait.position.x] = -1;
	mvwaddstr(window.game, bait.position.y, bait.position.x, "@");
	log_debug("Bait start coordinates is (%d;%d).", bait.position.y, bait.position.x);



	
}

void desetup(void) {
	log_trace("Desetup function have started.");
	free(floor);
}
