int wkbhit(WINDOW* window) {
	int ch,r;

	nodelay(window, TRUE);

	ch = getch();
	if( ch == ERR)
		r = FALSE;
	else {
		r = TRUE;
		ungetch(ch);
	}

	nodelay(window, FALSE);

	return r;
}

void gameinput(void) {
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
	} else if (score(ch)) {
		drawgame();
	}

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
		log_debug("Snake turned %s at (%d;%d). [%s]", directionstr[snake.direction], snake.head.y, snake.head.x, keyname(ch));
	
	return interaction;
}

void input_log(void) {
	int ch;
	while (true) {
		ch = wgetch(pad.log);
		if (ch == ERR)
			continue;
		if (ch == 'l' || ch == 'L') {
			log_debug("Left log pad. [%c]", ch);
			break;
		} else if (scrollbpad(pad.log, length.pad.log.minl, length.pad.log.minc, &length.pad.log.vl, ch)) {
			drawlog();
		} else if (exitgame(ch)) {
			drawlog();
		} else if (help(ch)) {
			drawlog();
		} else if (score(ch)) {
			drawlog();
		}
	}
}

void input_help(void) {
	int ch;
	while (true) {
		ch = wgetch(pad.help);
		if (ch == ERR)
			continue;
		if (ch == 'h' || ch == 'H') {
			log_debug("Left help pad. [%c]", ch);
			break;
		} else if (scrollbpad(pad.help, length.pad.help.minl, length.pad.help.minc, &length.pad.help.vl, ch)) {
			drawhelp();
		} else if (exitgame(ch)) {
			drawhelp();
		} else if (gamelog(ch)) {
			drawhelp();
		} else if (score(ch)) {
			drawhelp();
		}
	}
}

void input_score(void) {
	int ch;
	while (true) {
		ch = wgetch(pad.score);
		if (ch == ERR)
			continue;
		if (ch == 'b' || ch == 'B') {
			log_debug("Left scoreboard pad. [%c]", ch);
			break;
		} else if (scrollbpad(pad.score, length.pad.score.minl, length.pad.score.minc, &length.pad.score.vl, ch)) {
			drawscore();
		} else if (exitgame(ch)) {
			drawscore();
		} else if (gamelog(ch)) {
			drawscore();
		} else if (help(ch)) {
			drawscore();
		}
	}
}

bool scrollbpad(WINDOW* pad, int lines, int columns, int* vl, int ch) { // scroll boxed pad
	if (ch == 'w' || ch == 'W'|| ch == 's' || ch == 'S' || ch == KEY_UP || ch == KEY_DOWN) {
		if (ch == KEY_UP || ch == 'w') {
			if (*vl > 0) {
				prefresh(pad, --(*vl), 0, 
						1, 1, LINES - 2, columns - 2);
			}
		} else if (ch == KEY_DOWN || ch == 's') {
			if (*vl + (LINES - 2) < lines ) {
				prefresh(pad, ++(*vl), 0, 
						1, 1, LINES - 2, columns - 2);
			}
		} else if (ch == 'W') {
			if (*vl > 0) {
				if ((*vl -= 4) < 1)
					*vl = 1;
				prefresh(pad, --(*vl), 0, 
						1, 1, LINES - 2, columns - 2);
			}
		} else { //if (ch == 'S') {
			if (*vl + (LINES - 2) < lines ) {
				if ((*vl += 4) + (LINES - 2) >= lines)
					*vl = lines - (LINES-1);
				prefresh(pad, ++(*vl), 0, 
						1, 1, LINES - 2, columns - 2);
			}
		}
		if (*vl == 0 || *vl + (LINES - 2) == lines)
			beep();
		return true;
	} else
		return false;
}


void input_exit(void) {
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
}

