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

	if (ch == ERR)
		;
	else if (input_snake(ch))
		;
	else if (input_global(ch))
		;

	echo();
}

bool input_snake(int ch) {
	Direction previous = snake.direction;	
	bool interaction = false;

	if (( ch == 'd' || ch == 'D' || ch == KEY_RIGHT) && (snake.direction != D_LEFT)) {
		snake.direction = D_RIGHT;
		interaction = true;
	} else if (( ch == 'a' || ch == 'A' || ch == KEY_LEFT) && (snake.direction != D_RIGHT)) {
		snake.direction = D_LEFT;
		interaction = true;
	} else if ((ch == 's' || ch == 'S' || ch == KEY_DOWN) && (snake.direction != D_UP)) {
		snake.direction = D_DOWN;
		interaction = true;
	} else if ((ch == 'w' || ch == 'W' || ch == KEY_UP) && (snake.direction != D_DOWN)) {
		snake.direction = D_UP;
		interaction = true;
	}

	if (previous != snake.direction)
		log_debug("Snake turned %s at (%d;%d).", directionstr[snake.direction], snake.head.y, snake.head.x);
	return interaction;
}

bool input_global(int ch) {
	bool interaction = false;

	if (ch == 'p') {
		log_debug("Game has been paused.");
		gamepause();
		log_debug("Game has been unpaused.");
		interaction = true;
	}

	return interaction;
}
