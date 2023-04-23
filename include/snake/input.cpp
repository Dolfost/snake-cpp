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

void gameinput(void) {
 	noecho();

	int ch = wgetch(window.game);

	if (ch == ERR)
		;
	else if (input_snake(ch))
		;
	else if (help(ch)) {
		drawgame();
	} else if (exitgame(ch)) {
		drawgame();
	} else if (gamepause(ch)) {
		drawgame();
	} else if (gamelog(ch)) {
		drawgame();
	}

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
		log_debug("Snake turned %s at (%d;%d). [%c]", directionstr[snake.direction], snake.head.y, snake.head.x, ch);
	
	return interaction;
}

void input_log(void) {
	noecho();

	int ch;
	while (true) {
		ch = wgetch(pad.log);
		if (ch == ERR)
			continue;
		if (ch == 'l' || ch == 'L') {
			log_debug("Left log pad. [%c]", ch);
			break;
		} else if (ch == KEY_UP || ch == 'w') {
			if (length.pad.log.vl > 0) {
				prefresh(pad.log, --length.pad.log.vl, 0, 
						1, 1, LINES - 2, COLS - 2);
			}
		} else if (ch == KEY_DOWN || ch == 's') {
			if (length.pad.log.vl + (LINES - 1) < length.pad.log.minl ) {
				prefresh(pad.log, ++length.pad.log.vl, 0, 
						1, 1, LINES - 2, COLS - 2);
			}
		} else if (ch == 'W') {
			if (length.pad.log.vl > 0) {
				if ((length.pad.log.vl -= 4) < 0)
					length.pad.log.vl = 1;
				prefresh(pad.log, --length.pad.log.vl, 0, 
						1, 1, LINES - 2, COLS - 2);
			}
		} else if (ch == 'S') {
			if (length.pad.log.vl + (LINES - 1) < length.pad.log.minl ) {
				if ((length.pad.log.vl += 4) + (LINES - 1) >= length.pad.log.minl)
					length.pad.log.vl = length.pad.log.minl - (LINES);
				prefresh(pad.log, ++length.pad.log.vl, 0, 
						1, 1, LINES - 2, COLS - 2);
			}
		} else if (exitgame(ch)) {
			drawlog();
		} else if (help(ch)) {
			drawlog();
		}
	}

	echo();
}

void input_help(void) {
	noecho();

	int ch;
	while (true) {
		ch = wgetch(pad.help);
		if (ch == ERR)
			continue;
		if (ch == 'h' || ch == 'H') {
			log_debug("Left help pad. [%c]", ch);
			break;
		} else if (ch == KEY_UP || ch == 'w') {
			if (length.pad.help.vl > 0) {
				prefresh(pad.help, --length.pad.help.vl, 0, 
						1, 1, LINES - 2, length.window.game.minc - 2);
			}
		} else if (ch == KEY_DOWN || ch == 's') {
			if (length.pad.help.vl + (LINES - 1) < length.pad.help.minl ) {
				prefresh(pad.help, ++length.pad.help.vl, 0, 
						1, 1, LINES - 2, length.window.game.minc - 2);
			}
		} else if (ch == 'W') {
			if (length.pad.help.vl > 0) {
				if ((length.pad.help.vl -= 4) < 0)
					length.pad.help.vl = 1;
				prefresh(pad.help, --length.pad.help.vl, 0, 
						1, 1, LINES - 2, COLS - 2);
			}
		} else if (ch == 'S') {
			if (length.pad.help.vl + (LINES - 1) < length.pad.help.minl ) {
				if ((length.pad.help.vl += 4) + (LINES - 1) >= length.pad.help.minl)
					length.pad.help.vl = length.pad.help.minl - (LINES);
				prefresh(pad.help, ++length.pad.help.vl, 0, 
						1, 1, LINES - 2, COLS - 2);
			}
		} else if (exitgame(ch)) {
			drawhelp();
		} else if (gamelog(ch)) {
			drawhelp();
		}
	}

	echo();
}

void input_exit(void) {
	noecho();

	int ch;

	while (true) {
		ch = wgetch(window.exit);
		if (ch == 'Y' || ch == 'Q' || ch == 'y' || ch == 'q') {
			log_debug("Exiting the game... [%c]", ch);
			exit(EXIT_SUCCESS);
			break;
		}
		if (ch == 'N' || ch == 'C' || ch == 'n' || ch == 'c') {
			log_debug("Exit was interrupted. [%c]", ch);
			return;
		}
	}

	echo();
}

