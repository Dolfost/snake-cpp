#include <stdlib.h>

void wallhit(void) {
	if (snake.head.y < 0 || snake.head.x < 0 ||
	snake.head.y > length.window.game.minl - 1 ||
	snake.head.x > length.window.game.minc - 1) {
		snake.hit = true;
		log_debug("The snake crashed into the wall at (%d;%d).", snake.head.y, snake.head.x);
	}
}

void selfbit(void) {
	for (int i = snake.length - 1; i > 0; i--) {
		if (snake.head.y == snake.body[i].y && snake.head.x == snake.body[i].x) {
			snake.bit = true;
			log_debug("The snake bit itself at (%d;%d).", snake.head.y, snake.head.x);
			return;
		}
		snake.body[i] = snake.body[i-1];	
	}	

	snake.body[0] = snake.head;
}

void dinner(void) {
	if (snake.head.y == bait.position.y && snake.head.x == bait.position.x) {
		clock_gettime(CLOCK_MONOTONIC, &snake.time_end);
		snake.time = timediff(&snake.time_start, &snake.time_end) - snake.pause_time;
		snake.pause_time = 0;
		clock_gettime(CLOCK_MONOTONIC, &snake.time_start);

		if (snake.travelled - bait.distance >= 15)
			game.score += 10;
		else
			game.score += 25 -(snake.travelled - bait.distance);


		snake.length++;
		snake.travelled = 0;
		bait.eaten = true;
		log_debug("The snake caught the mouse at (%d;%d) in %0.3fs.", snake.head.y, snake.head.x, snake.time);
		log_debug("The score equals %d points.", game.score);
		spawn_bait();
	}
}

void move(void) {
	snake.tail = snake.body[snake.length - 1];

	switch(snake.direction) {
		case D_RIGHT:
			snake.head.x++;
			snake.travelled++;
			break;
		case D_UP:
			snake.head.y--;
			snake.travelled++;
			break;
		case D_LEFT:
			snake.head.x--;
			snake.travelled++;
			break;
		case D_DOWN:
			snake.head.y++;
			snake.travelled++;
			break;
		default:
			break;
	}
}

void doscores(void) {
	int index;

	for (index = 0; index < game.scoreentry && game.score <= game.highscore[index]; index++)
		;

	if (index == 200)
		index = 199;
	

	for (int i = game.scoreentry; i > index; i--) {
		game.highscore[i] = game.highscore[i-1];
		game.highplayer[i] = game.highplayer[i-1];
		game.hightime[i] = game.hightime[i-1];
	}

	if (game.scoreentry < 200)
		++game.scoreentry;

	game.highscore[index] = game.score;
	game.highlength[index] = snake.length;
	game.highplayer[index] = strdup(game.playername);
	game.hightime[index] = game.time;
}

double timediff(const struct timespec* start, const struct timespec* end) {
	return ((int64_t)end->tv_sec - (int64_t)start->tv_sec)
		+ ((int64_t)end->tv_nsec - (int64_t)start->tv_nsec) / (double)1000000000;
}

