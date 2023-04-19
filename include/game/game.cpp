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
	log_trace("Game function have started.");

	wrefresh(window.stdscr);
	draw();

	clock_gettime(CLOCK_MONOTONIC, &snake.time_start);

	while (snake.hit != true && snake.bit != true) {
		input();

		move();

		wallhit();

		game.distance = sqrt(pow(snake.head.x - bait.position.x, 2) + pow(snake.head.y - bait.position.y, 2));
		
		draw();
		
		napms(GAME_NAP);
	}

	log_debug("The final score equals %d points.", game.score);
	log_debug("The final length equals %d.", snake.length);

	desetup();
}
