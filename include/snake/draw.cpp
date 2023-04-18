void draw(void) {
	// snake
	mvwaddch(window.game, snake.tail.y, snake.tail.x, ' ');
	mvwaddch(window.game, snake.head.y, snake.head.x, 'O');
	

	// mouse	
	if (game.distance <= bait.fearradius[0])
		wattrset(window.game, COLOR_PAIR(bait.fearcolor[0]) | bait.fearattr[0]);
	else if (game.distance <= bait.fearradius[1])
		wattrset(window.game, COLOR_PAIR(bait.fearcolor[1]) | bait.fearattr[1]);
	else if (game.distance <= bait.fearradius[2])
		wattrset(window.game, COLOR_PAIR(bait.fearcolor[2]) | bait.fearattr[2]);
	else
		wattrset(window.game, COLOR_PAIR(bait.fearcolor[3]) | bait.fearattr[3]);
	
	mvwaddch(window.game, bait.position.y, bait.position.x, '@');

	wattrset(window.game, COLOR_PAIR(bait.fearcolor[3]) + bait.fearattr[3]);
	

	// window refreshing
	wrefresh(window.game);
	wrefresh(window.sidelog);
}
