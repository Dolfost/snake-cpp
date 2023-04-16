#include <stdlib.h>

void wallhit(void) {
	if (snake.position.y < 0 || snake.position.x < 0 ||
	snake.position.y > length.window.game.minl - 1 ||
	snake.position.x > length.window.game.minc - 1) {
		beep();
		snake.hit = true;
		log_debug("Snake have hit the wall. (%d;%d)", snake.position.y, snake.position.x);
	}
}

void move(void) {
	for (int i = 0; i < length.window.game.minl; i++) {
		for (int j = 0; j < length.window.game.minc; j++) {
		}
	}	
}
