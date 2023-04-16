void game(void) {
	log_trace("Game function have started.");

	wrefresh(window.stdscr);

	while (snake.hit != true) {
		input();
		move();
		mvwaddch(window.game, snake.position.y, snake.position.x, '#');
		
		draw();
		wallhit();
		napms(GAME_NAP);
	}

	desetup();
}
