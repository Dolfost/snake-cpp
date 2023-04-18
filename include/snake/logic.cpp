#include <stdlib.h>

void wallhit(void) {
	if (snake.head.y < 0 || snake.head.x < 0 ||
	snake.head.y > length.window.game.minl - 1 ||
	snake.head.x > length.window.game.minc - 1) {
		beep();
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
		progress.score += 5;
		snake.length++;
		log_debug("The snake caught the mouse at (%d;%d)", snake.head.y, snake.head.x);
		log_debug("The score equals %d points.", progress.score);
		spawn_bait();
	}
}
