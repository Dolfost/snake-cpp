void spawn_bait(void) {
	bool is_taken = true;

	while (is_taken == true) {
		is_taken = false;
		bait.position.y = get_random(0, length.window.game.minl - 1);
		bait.position.x = get_random(0, length.window.game.minc - 1);
		for (int i = 0; i < snake.length; i++) {
			if (snake.body[i].y == bait.position.y && snake.body[i].x == bait.position.x) {
				is_taken = true;;
				break;
			}
		}
	}
	log_debug("Bait spawned at (%d;%d).", bait.position.y, bait.position.x);
}

void game(void) {
	log_trace("Game function have started.");

	wrefresh(window.stdscr);
	draw();

	while (snake.hit != true && snake.bit != true) {
		input();

		move();

		wallhit();
		
		draw();
		
		napms(GAME_NAP);
	}

	log_debug("The score equals %d points.", progress.score);

	desetup();
}
