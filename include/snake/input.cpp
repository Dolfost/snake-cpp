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
 	noecho();

	int ch = wgetch(window.game);
	
	if(( ch == 'd' || ch == 'D' || ch == KEY_RIGHT) && (snake.direction != D_RIGHT && snake.direction != D_LEFT)) {
		snake.direction = D_RIGHT;
		log_debug("Snake turned right at (%d;%d).", snake.head.y, snake.head.x);
	} else if (( ch == 'a' || ch == 'A' || ch == KEY_LEFT) && (snake.direction != D_RIGHT && snake.direction != D_LEFT)) {
		snake.direction = D_LEFT;
		log_debug("Snake turned left at (%d;%d).", snake.head.y, snake.head.x);
	} else if((ch == 's' || ch == 'S' || ch == KEY_DOWN) && (snake.direction != D_UP && snake.direction != D_DOWN)) {
		snake.direction = D_DOWN;
		log_debug("Snake turned down at (%d;%d).", snake.head.y, snake.head.x);
	} else if((ch == 'w' || ch == 'W' || ch == KEY_UP) && (snake.direction != D_UP && snake.direction != D_DOWN)) {
		snake.direction = D_UP;
		log_debug("Snake turned up at (%d;%d).", snake.head.y, snake.head.x);
	}

	echo();
}
