int wkbhit(WINDOW* window) {
	int ch,r;

	nodelay(window, TRUE);
	noecho();

	ch = getch();
	if( ch == ERR)
		r = FALSE;
	else {
		r = TRUE;
		ungetch(ch);
	}

	echo();
	nodelay(window, FALSE);

	return r;
}

void input(void) {
	nodelay(window.game, TRUE);
	noecho();
	
	flushinp();

	switch(getch()) {
		case 'd':
			snake.position.x++;
			log_debug("Snake is moving right. (%d;%d)", snake.position.y, snake.position.x);
			break;
		case 'w':
			snake.position.y--;
			log_debug("Snake is moving up. (%d;%d)", snake.position.y, snake.position.x);
			break;
		case 'a':
			snake.position.x--;
			log_debug("Snake is moving left. (%d;%d)", snake.position.y, snake.position.x);
			break;
		case 's':
			snake.position.y++;
			log_debug("Snake is moving down. (%d;%d)", snake.position.y, snake.position.x);
			break;
		default:
			break;
	}
	echo();
	nodelay(window.game, FALSE);
}
