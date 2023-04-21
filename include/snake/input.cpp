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
	} else 	if (ch == 'p') {
		gamepause();
		log_debug("Game has been paused for %0.3fs.", snake.pause_time);
		interaction = true;
	}

	if (previous != snake.direction)
		log_debug("Snake turned %s at (%d;%d).", directionstr[snake.direction], snake.head.y, snake.head.x);
	return interaction;
}

bool input_global(int ch) {
	bool interaction = false;

	if (ch == 'h') {
		log_debug("Entered help window.");
		help();
		interaction = true;
	}// else if (ch == 'q') {
// 		log_debug("Exiting the game.");
// 		interaction = true;
// 		exitgame();
// 	}

	return interaction;
}

void input_help(void) {
	noecho();

	int ch;

	while (true) {
		ch = wgetch(pad.help);
		if (ch == 'h' || ch == 'q') {
			log_debug("Left help window.");
			break;
		}
	}

	echo();
}
