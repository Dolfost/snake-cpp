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
	wrefresh(window.stdscr);
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
}



WINDOW* buildhelppad(const char* path) {
	log_debug("Entered buildhelppad().");

	FILE* padfile = fopen(path, "w+");
	if (padfile == NULL) {
		log_error("Could not open pad file");
		log_nl(   "'%s' for writting.");
		return NULL;
	}

	WINDOW* pad = newpad(length.pad.help.minl, length.pad.help.minc);
	wprintw(pad, "This is help pad ");
	incolor(pad, color.pair.help.keyword, attribute.help.keyword, "v%s", SNAKE_VERSION);
	waddstr(pad, ".\n");

	helppad_title(pad, "Help pad controls");
	helppad_keys(pad, "w"); helppad_keyword(pad, "Scroll"); waddstr(pad, " the help pad up by 1 line.\n");
	helppad_keys(pad, "s"); helppad_keyword(pad, "Scroll"); waddstr(pad, " the help pad down by 1 line.\n");
	helppad_keys(pad, "W"); helppad_keyword(pad, "Scroll"); waddstr(pad, " the help pad up by 5 lines.\n");
	helppad_keys(pad, "S"); helppad_keyword(pad, "Scroll"); waddstr(pad, " the help pad down by 5 lines.\n");
	helppad_keys(pad, "hH"); helppad_keyword(pad, "Hide"); waddstr(pad, " the help pad.\n");

	helppad_title(pad, "Global controls");
	helppad_keys(pad, "hH"); waddstr(pad, "Show the "); helppad_keyword(pad, "help pad"); waddstr(pad, ".\n");
	helppad_keys(pad, "q"); helppad_keyword(pad, "Quit"); waddstr(pad, " the game.\n");
	helppad_keys(pad, "Q"); helppad_keyword(pad, "Force quit"); waddstr(pad, " the game.\n");

	helppad_title(pad, "In-game controls");
	helppad_keys_word(pad, "arrows"); waddstr(pad, "Change snake "); helppad_keyword(pad, "direction"); waddstr(pad, ".\n");
	helppad_keys(pad, "DWAS"); waddstr(pad, "Change snake "); helppad_keyword(pad, "direction"); waddstr(pad, ".\n");
	helppad_keys(pad, "dwas"); waddstr(pad, "Change snake "); helppad_keyword(pad, "direction"); waddstr(pad, ".\n");
	waddstr(pad, "\n Hold any of snake controls buttons down for "); helppad_keyword(pad, "speedup"); waddstr(pad, ".\n\n");
	helppad_keys(pad, "pP"); helppad_keyword(pad, "Pause/unpause"); waddstr(pad, " the game.\n");
	helppad_keys(pad, "hH"); waddstr(pad, "Show "); helppad_keyword(pad, "help"); waddstr(pad, ".\n");


	putwin(pad, padfile);
	fclose(padfile);
	return pad;
}

void helppad_title(WINDOW* pad, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	waddstr(pad, "\n    ");
	wattrset(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
	waddch(pad, '-');

	wattrset(pad, COLOR_PAIR(color.pair.help.title) | attribute.help.title);
	vw_printw(pad, fmt, args);
	wattroff(pad, COLOR_PAIR(color.pair.help.title) | attribute.help.title);
	wattrset(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
	waddstr(pad, "-\n");

	wattroff(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);

	va_end(args);
}

void helppad_keyword(WINDOW* pad, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	wattrset(pad, COLOR_PAIR(color.pair.help.keyword) | attribute.help.keyword);
	vw_printw(pad, fmt, args);

	wattroff(pad, COLOR_PAIR(color.pair.help.keyword) | attribute.help.keyword);

	va_end(args);
}
	

void helppad_keys(WINDOW* pad, const char* keys) {
	wprintw(pad, "  [");
	int printed = 0;
	for (int i = strlen(keys) - 1; i >= 0; i--, printed += 2) {
		wattrset(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
		waddch(pad, keys[i]);
		wattroff(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
		waddch(pad, ' ');
	}
	mvwprintw(pad, getcury(pad), getcurx(pad) - 1, "]");
	
	for (int i = (length.window.game.minc/8) - printed; i > 0; i--) {
		waddch(pad, ' ');
	}
}


void helppad_keys_word(WINDOW* pad, const char* word) {
	wprintw(pad, "  {");
	int printed = strlen(word) + 1;
	wattrset(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
	waddstr(pad, word);
	wattroff(pad, COLOR_PAIR(color.pair.help.key) | attribute.help.key);
	wprintw(pad, "}");
	
	for (int i = (length.window.game.minc/8) - printed; i > 0; i--) {
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
