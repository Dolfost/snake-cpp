#include <stdlib.h>

void wallhit(void) {
	if (snake.head.y < 0 || snake.head.x < 0 ||
	snake.head.y > length.window.game.minl - 1 ||
	snake.head.x > length.window.game.minc - 1) {
		snake.hit = true;
		log_debug("The snake crashed into the wall at (%d;%d).", snake.head.y, snake.head.x);
	}
}

void move(void) {
	snake.tail = snake.body[snake.length - 1];

	switch(snake.direction) {
		case D_RIGHT:
			snake.head.x++;
			break;
		case D_UP:
			snake.head.y--;
			break;
		case D_LEFT:
			snake.head.x--;
			break;
		case D_DOWN:
			snake.head.y++;
			break;
		default:
			break;
	}

	for (int i = snake.length - 1; i > 0; i--) {
		if (snake.head.y == snake.body[i].y && snake.head.x == snake.body[i].x) {
			snake.bit = true;
			log_debug("The snake bit itself at (%d;%d).", snake.head.y, snake.head.x);
			return;
		}
		snake.body[i] = snake.body[i-1];	
	}	

	snake.body[0] = snake.head;

	if (snake.head.y == bait.position.y && snake.head.x == bait.position.x) {
		clock_gettime(CLOCK_MONOTONIC, &snake.time_end);
		snake.time = timediff(&snake.time_start, &snake.time_end) - snake.pause_time;
		snake.pause_time = 0;
		clock_gettime(CLOCK_MONOTONIC, &snake.time_start);


		game.score += (bait.distance)*3 / snake.time + 4;
		snake.length++;
		bait.eaten = true;
		log_debug("The snake caught the mouse at (%d;%d) in %0.3fs.", snake.head.y, snake.head.x, snake.time);
		log_debug("The score equals %d points.", game.score);
		spawn_bait();
	}
}

double timediff(const struct timespec* start, const struct timespec* end) {
	return ((int64_t)end->tv_sec - (int64_t)start->tv_sec)
		+ ((int64_t)end->tv_nsec - (int64_t)start->tv_nsec) / (double)1000000000;
}
