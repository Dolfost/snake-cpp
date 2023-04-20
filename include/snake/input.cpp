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
	

	if (( ch == 'd' || ch == 'D' || ch == KEY_RIGHT) && (snake.direction != D_LEFT)) {
		snake.direction = D_RIGHT;
	} else if (( ch == 'a' || ch == 'A' || ch == KEY_LEFT) && (snake.direction != D_RIGHT)) {
		snake.direction = D_LEFT;
	} else if ((ch == 's' || ch == 'S' || ch == KEY_DOWN) && (snake.direction != D_UP)) {
		snake.direction = D_DOWN;
	} else if ((ch == 'w' || ch == 'W' || ch == KEY_UP) && (snake.direction != D_DOWN)) {
		snake.direction = D_UP;
	}

	echo();
}

