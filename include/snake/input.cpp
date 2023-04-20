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
	

	Direction previous = snake.direction;
	bool move_keyhit = false;
	

	if (( ch == 'd' || ch == 'D' || ch == KEY_RIGHT) && (snake.direction != D_LEFT)) {
		snake.direction = D_RIGHT;
		move_keyhit = true;
	} else if (( ch == 'a' || ch == 'A' || ch == KEY_LEFT) && (snake.direction != D_RIGHT)) {
		snake.direction = D_LEFT;
		move_keyhit = true;
	} else if ((ch == 's' || ch == 'S' || ch == KEY_DOWN) && (snake.direction != D_UP)) {
		snake.direction = D_DOWN;
		move_keyhit = true;
	} else if ((ch == 'w' || ch == 'W' || ch == KEY_UP) && (snake.direction != D_DOWN)) {
		snake.direction = D_UP;
		move_keyhit = true;
	}

	if (previous == snake.direction) {
		if (move_keyhit == true)
			snake.nitro = true;
		else
			snake.nitro = false;
	} else {
		log_debug("Snake turned %s at (%d;%d).", directionstr[snake.direction], snake.head.y, snake.head.x);
	}


	echo();
}

