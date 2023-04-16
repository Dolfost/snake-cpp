void draw(void) {

	mvwaddch(window.game, snake.tail.y, snake.tail.x, ' ');
	mvwaddch(window.game, snake.head.y, snake.head.x, 'O');
	
	mvwaddch(window.game, bait.position.y, bait.position.x, '@');
	
	wrefresh(window.game);
	wrefresh(window.sidelog);
}
