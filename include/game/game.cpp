
void game(void) {
	log_trace("Game function have started.");

	while (snake.hit != true) {
		input();
		mvwaddch(window.game, snake.position.y, snake.position.x, '#');
		
		wrefresh(window.game);
		wrefresh(window.sidelog);
		wallhit();
		napms(60);
	}
}
