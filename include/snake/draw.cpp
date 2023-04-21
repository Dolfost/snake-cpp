void werase(WINDOW* window, short from, short to) {
	short y = getcury(window);
	while (from <= to) {
		mvwaddch(window, y, from, ' ');
		from++;
	}
}

void draw(void) {
	// snake
	mvwaddch(window.game, snake.tail.y, snake.tail.x, ' ');
	mvwaddch(window.game, snake.head.y, snake.head.x, '0');

	if (snake.length > 1) {
		mvwaddch(window.game, snake.body[1].y, snake.body[1].x, 'O'); 
		if (bait.eaten == true) {
			mvwaddch(window.game, snake.body[1].y, snake.body[1].x, '#');
			bait.eaten = false;
		}
	}

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


	// bar
	werase(window.bar, length.bar.scorename, length.bar.scorename + length.bar.score - 1);
	mvwprintw(window.bar, 0, length.bar.scorename, "%d", game.score);

	werase(window.bar, length.bar.scorename + length.bar.score + length.bar.timename,
			length.bar.scorename + length.bar.score + length.bar.timename + length.bar.time);
	if (snake.time > 99.9)
		mvwprintw(window.bar, 0, length.bar.scorename + length.bar.score + length.bar.timename,
				">99.9s");
	else	
		mvwprintw(window.bar, 0, length.bar.scorename + length.bar.score + length.bar.timename,
				"%0.1fs", snake.time);

	
	// window refreshing
	wrefresh(window.game);
	wrefresh(window.bar);
	wrefresh(window.sidelog);
}
