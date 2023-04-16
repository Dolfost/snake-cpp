#include <stdlib.h>

Snake snake;
Bait bait;
Game progress;

int get_random(int lower_bound, int upper_bound) {
	return rand() % (upper_bound + 1 - lower_bound) + lower_bound;
}

void setup(void) {
	log_trace("Setup function have started.");

	wtimeout(window.game, GAME_KEY_TIMEOUT);

	srand(time(NULL));

	progress.score = 0;

	snake.body = (Point*)calloc(length.window.game.minl*length.window.game.minc, sizeof(Point));
	memcheck(snake.body, length.window.game.minl*length.window.game.minc*sizeof(Point));

	snake.head.y = get_random(0, length.window.game.minl - 1);
	snake.head.x = get_random(0, length.window.game.minc - 1);

	snake.body[0].y = snake.head.y;
	snake.body[0].x = snake.head.x;

	// snake.tail = snake.body;

	snake.length = 1;

	mvwaddstr(window.game, snake.head.y, snake.head.x, "#");
	snake.length = 1;
	log_debug("Snake spawned at (%d;%d).", snake.head.y, snake.head.x);
	
	spawn_bait();
}

void desetup(void) {
	log_trace("Desetup function have started.");
}
