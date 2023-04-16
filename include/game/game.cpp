void game(void) {
	log_trace("Game function have started.");

	while (snake.hit != true) {
		input();
		// move();
		mvwaddch(window.game, snake.position.y, snake.position.x, '#');
		
		draw();
		wallhit();
		napms(60);
	}

	desetup();
}
