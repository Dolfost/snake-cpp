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
	// wrefresh(window.stdscr);
}

void drawgame(void) {
	drawgamelines();
	touchwin(window.stdscr);
	wrefresh(window.stdscr);

	touchwin(window.game);
	wrefresh(window.game);
	touchwin(window.sidelog);
	wrefresh(window.sidelog);
	touchwin(window.bar);
	wrefresh(window.bar);
}

void drawgamelines(void) {
	mvwvline(window.stdscr, 0, length.window.game.minc, 0, LINES);
	mvwhline(window.stdscr, length.window.game.minl, 0, 0, length.window.game.minc);
	mvwhline(window.stdscr, length.window.game.minl + length.window.bar.minl + 1, 0, 0, length.window.game.minc);
}

void drawhelp(void) {
	wclear(window.stdscr);
	box(window.stdscr, 0, 0);
	mvwaddstr(window.stdscr, 0, 2, "Snake help");
	wrefresh(window.stdscr);

	touchwin(pad.help);
	prefresh(pad.help, length.pad.help.vl, 0, 
			1, 1, LINES - 2, length.window.game.minc - 2);
}

void drawlog(void) {
	wclear(window.stdscr);
	box(window.stdscr, 0, 0);
	mvwaddstr(window.stdscr, 0, 2, "Snake log");
	wrefresh(window.stdscr);

	touchwin(window.stdscr);
	wrefresh(window.stdscr);
	touchwin(pad.log);
	prefresh(pad.log, length.pad.log.vl, 0, 
			1, 1, LINES - 2, COLS - 2);
}

void drawover(void) {
	wattrset(window.bar, A_BOLD);
	baradd("Press return");
	setled(1);
	wattroff(window.bar, A_BOLD);
	wrefresh(window.bar);
	wrefresh(window.sidelog);
}

void drawfinals(void) {
	int mins = game.time / 60;
	int secs = game.time -  mins*60;

	wclear(window.finals);
	box(window.finals, 0, 0);
	wattrset(window.finals, A_BOLD);
	center(window.finals, 1, "Game is over");
	center(window.finals, 2, snake.bit ? "You bit yourself" : "You crashed into the wall");
	wattroff(window.finals, A_BOLD);
	center(window.finals, 4, "You ate %d mouses in %d:%dm", snake.length - 1, mins, secs);
	center(window.finals, 5, "The score is %d points", game.score);


	if (*game.playername == '\0') {
		mvwaddstr(window.finals, 8, 3, "Who are you?");
	} else {
		mvwaddstr(window.finals, 7, 3, "Who are you?");
		mvwprintw(window.finals, 8, 3, "If you are %s - press return.", game.playername);
	}

	mvwaddstr(window.finals, 10, 3, "I am ");
	touchwin(window.finals);
	wrefresh(window.finals);
}

void drawagain(void) {
	touchwin(window.again);
	wrefresh(window.again);
}

int baradd(const char* str) {
	static size_t len = 0;
	int offset = 4;
	 
	werase(window.bar, length.window.bar.minc - offset - len - 1, length.window.bar.minc - offset - 1); 

	if (str == NULL) {
		return 0;
	}

	len = strlen(str);

	wattrset(window.bar, A_BOLD);
	mvwaddstr(window.bar, 0, length.window.bar.minc - offset - len - 1, str);
	wattroff(window.bar, A_BOLD);
	wrefresh(window.bar);
	return len;
}

short setled(short indx) {
	wattrset(window.bar, COLOR_PAIR(color.pair.led[indx]));
	mvwaddstr(window.bar, 0, length.window.bar.minc - 2 - 1, "  ");
	wattroff(window.bar, COLOR_PAIR(color.pair.led[indx]));
	wrefresh(window.bar);
	return indx;
}


int center(WINDOW* window, int line, const char* fmt, ...) {
	if (window == NULL)
		return -1;

	va_list ap;
	va_start(ap, fmt);
	
	int len = vsnprintf(NULL, 0, fmt, ap);
	char* str = (char*)malloc(sizeof(char)*len + 1);
	memcheck(str);
	vsnprintf(str, len + 1, fmt, ap);

	mvwaddstr(window, line, (getmaxx(window) - len) / 2, str);

	free(str);
	va_end(ap);

	return len;
}

WINDOW* buildhelppad(const char* path) {
	log_trace("Entered buildhelp().");

	FILE* padfile = fopen(path, "w+");
	if (padfile == NULL) {
		log_error("Could not open pad file");
		log_nl(   "'%s' for writting.");
		return NULL;
	}

	WINDOW* pad = newpad(length.pad.help.minl, length.pad.help.minc);
	hp_add(pad, "This is help pad ");
	incolor(pad, color.pair.help.keyword, attribute.help.keyword, "v%s", SNAKE_VERSION);
	waddstr(pad, ".");

	hp_title(pad, "Help pad controls");
	hp_keys(pad, "w", 0); hp_keyword(pad, "Scroll"); waddstr(pad, " the help pad up by 1 line.");
	hp_keys(pad, "s"); hp_keyword(pad, "Scroll"); waddstr(pad, " the help pad down by 1 line.");
	hp_keys(pad, "W"); hp_keyword(pad, "Scroll"); waddstr(pad, " the help pad up by 5 lines.");
	hp_keys(pad, "S"); hp_keyword(pad, "Scroll"); waddstr(pad, " the help pad down by 5 lines.");
	hp_keys(pad, "hH"); hp_keyword(pad, "Exit/hide"); waddstr(pad, " the help pad.\n");

	hp_title(pad, "Global controls");
	hp_add(pad, "These keystrokes work in all non-promt windows.\n\n");
	hp_add(pad, "For example, the exit, gameover and play again windows are\n");
	hp_add(pad, "the prompt windows, so they wont check for global keystokes.\n");
	hp_keys(pad, "hH"); waddstr(pad, "Show the "); hp_keyword(pad, "help pad"); waddstr(pad, ".");
	hp_keys(pad, "q"); hp_keyword(pad, "Quit"); waddstr(pad, " the game.");
	hp_keys(pad, "Q"); hp_keyword(pad, "Force quit"); waddstr(pad, " the game. (exits without a prompt)");
	hp_keys(pad, "lL"); waddstr(pad, "Open the "); hp_keyword(pad, "log pad"); waddstr(pad, ".\n");

	hp_title(pad, "Game terminology");
	hp_definition(pad, "da", 0); waddstr(pad, "Down arrow");
	hp_definition(pad, "la"); waddstr(pad, "Left arrow");
	hp_definition(pad, "ua"); waddstr(pad, "Up arrow");
	hp_definition(pad, "ra"); waddstr(pad, "Right arrow\n");
	hp_definition(pad, "score "); waddstr(pad, "Depends on your skill, time and distance to mouse.");
	hp_definition(pad, "catch time"); waddstr(pad, "The time beetwen mouse catches.");
	hp_definition(pad, NULL); waddstr(pad, "Inversely proportional to "); hp_keyword(pad, "score"); waddstr(pad, ".");

	hp_title(pad, "Game controls");
	hp_keys_word(pad, "ra ua la ra", 0);
	hp_keys(pad, "dwas"); waddstr(pad, "Change snake "); hp_keyword(pad, "direction"); waddstr(pad, ".");
	hp_keys(pad, "DWAS"); waddstr(pad, "\n\n");
	hp_add(pad, "Hold any of snake controls buttons down for "); hp_keyword(pad, "speedup"); waddstr(pad, ".\n");
	hp_keys(pad, "pP"); hp_keyword(pad, "Pause/unpause"); waddstr(pad, " the game.");
	hp_keys(pad, "hH"); waddstr(pad, "Show "); hp_keyword(pad, "help"); waddstr(pad, ".\n");

	hp_title(pad, "Game logic");
	hp_add(pad, "When you launch the %s from the terminal, the \n", execname);
	hp_add(pad, "game session will automatically start. At the start of the game session\n");
	hp_add(pad, "the snake has direction 'D_NONE' until any of snake controls keys \n");
	hp_add(pad, "(see "); hp_keyword(pad, "Global controls"); waddstr(pad, " section) is pressed. The "); hp_keyword(pad, "catch time"); waddstr(pad, " is started with\n");
	hp_add(pad, "the game session, so the time waited before first move is counted in.\n\n");
	hp_add(pad, "When you make the first move, the game starts and the snake start moving.\n\n");
	hp_add(pad, "Your "); hp_keyword(pad, "objective"); waddstr(pad, "is to catch as many mouses as possible without hitting the\n");
	hp_add(pad, "wall or biting youself.\n\n");
	hp_add(pad, "The "); hp_keyword(pad, "score"); waddstr(pad, " and "); hp_keyword(pad, "catch time"); waddstr(pad, " is displayed in left down corner of the window.\n");
	hp_add(pad, "Score depends on the "); hp_keyword(pad, "catch time"); waddstr(pad, ", the less points you will get when you\n");
	hp_add(pad, "catch the mouse. "); hp_keyword(pad, "Minimal score"); waddstr(pad, " that you can get from mouse catch is 4.\n");
	hp_add(pad, "If you hit the wall or bit itself you lose and the "); hp_keyword(pad, "msgbar"); waddstr(pad, " will scream\n");
	hp_add(pad, "\"Press return\", and the "); hp_keyword(pad, "led"); waddstr(pad, " will turn red.");


	hp_title(pad, "Log pad controls");
	hp_keys(pad, "w", 0); hp_keyword(pad, "Scroll"); waddstr(pad, " the log pad up by 1 line.");
	hp_keys(pad, "s"); hp_keyword(pad, "Scroll"); waddstr(pad, " the log pad down by 1 line.");
	hp_keys(pad, "W"); hp_keyword(pad, "Scroll"); waddstr(pad, " the log pad up by 5 lines.");
	hp_keys(pad, "S"); hp_keyword(pad, "Scroll"); waddstr(pad, " the log pad down by 5 lines.");
	hp_keys(pad, "lL"); hp_keyword(pad, "Exit/hide"); waddstr(pad, " the log pad.\n");

	hp_title(pad, "Exit window controls");
	hp_add(pad, "Exit window can recognize "); hp_keyword(pad, "ONLY"); waddstr(pad, " next keys:\n");
	hp_keys(pad, "cnCN"); waddstr(pad, "Approve the exit. (mean, "); hp_keyword(pad, "exit"); waddstr(pad, " the game)");
	hp_keys(pad, "qyQY"); waddstr(pad, "Decline the exit and hide the exit window.");





	putwin(pad, padfile);
	fclose(padfile);
	return pad;
}

void hp_title(WINDOW* pad, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	int len = vsnprintf(NULL, 0, fmt, args);
	char* str = (char*)malloc(sizeof(char)*(len+1));
	memcheck(str);
	vsnprintf(str, len + 1, fmt, args);

	wattrset(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
	mvwaddch(pad, getcury(pad) + 2, (getmaxx(pad) - len - 2)/2, '-');

	wattrset(pad, COLOR_PAIR(color.pair.help.title) | attribute.help.title);
	waddstr(pad, str);
	wattroff(pad, COLOR_PAIR(color.pair.help.title) | attribute.help.title);
	wattrset(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
	waddstr(pad, "-\n");

	wattroff(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);

	va_end(args);
}

void hp_add(WINDOW* pad, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	waddstr(pad, " "); vw_printw(pad, fmt, args);

	va_end(args);
}

void hp_definition(WINDOW* pad, const char* str, short plusline) {
	int offset = 18;
	int shift = 2;
	static int len = 0;
	if (str == NULL) {
		waddch(pad, '\n');
		for (int i = 0; i <= offset + shift + 1; i++) {
			waddch(pad, ' ');
		}
		return;
	}
	len = strlen(str);
	
	
	wmove(pad, getcury(pad) + plusline, (offset - len) / 2 + shift);
	wattrset(pad, COLOR_PAIR(color.pair.help.definition) | attribute.help.definition);
	waddstr(pad, str);
	wattroff(pad, COLOR_PAIR(color.pair.help.definition) | attribute.help.definition);

	for (int i = 0; i < (offset - len)/2 + shift; i++) {
		waddch(pad, ' ');
	}
//	wattrset(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
//	mvwaddch(pad, getcury(pad), getcurx(pad) - 4, '-');
//	wattroff(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
//	wmove(pad, getcury(pad), getcurx(pad) + 2);
}


	

void hp_keyword(WINDOW* pad, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	wattrset(pad, COLOR_PAIR(color.pair.help.keyword) | attribute.help.keyword);
	vw_printw(pad, fmt, args);
	wattroff(pad, COLOR_PAIR(color.pair.help.keyword) | attribute.help.keyword);

	va_end(args);
}
	

void hp_keys(WINDOW* pad, const char* keys, short plusline) {
	int offset = 14;
	int shift = 2; 
	static int len = 0;

	if (keys == NULL) {
		for (int i = 0; i < offset + shift; i++)
			waddch(pad, ' ');
		return;
	}

	len = 2*strlen(keys) - 1 + 2;
	wmove(pad, getcury(pad) + plusline, (offset - len) / 2 + shift);
	wprintw(pad, "[");
	for (int i = strlen(keys) - 1; i >= 0; i--) {
		wattrset(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
		waddch(pad, keys[i]);
		wattroff(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
		waddch(pad, ' ');
	}
	mvwprintw(pad, getcury(pad), getcurx(pad) - 1, "]");
	
	for (int i = 0; i < (offset - len)/2 + shift; i++) {
		waddch(pad, ' ');
	}
}


void hp_keys_word(WINDOW* pad, const char* word, short plusline) {
	int offset = 14;
	int shift = 2; 
	static int len = 0;

	if (word == NULL) {
		for (int i = 0; i < offset + shift; i++)
			waddch(pad, ' ');
		return;
	}

	len = strlen(word) + 2;
	wmove(pad, getcury(pad) + plusline, (offset - len) / 2 + shift);
	wprintw(pad, "{");
	wattrset(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
	wprintw(pad, "%s", word);
	wattroff(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
	wprintw(pad, "}");

	
	for (int i = 0; i < (offset - len)/2 + shift; i++) {
		waddch(pad, ' ');
	}
}

void incolor(WINDOW* window, short col, int attr, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	wattrset(window, COLOR_PAIR(col) | attr);
	vw_printw(window, fmt, args);
	wattroff(window, COLOR_PAIR(col) | attr);

	va_end(args);
}
