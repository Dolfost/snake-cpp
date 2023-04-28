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
	mvwaddstr(window.stdscr, 0, 2, "Help pad");
	wrefresh(window.stdscr);

	touchwin(pad.help);
	prefresh(pad.help, length.pad.help.vl, 0, 
			1, 1, LINES - 2, length.window.game.minc - 2);
}

void drawscore(void) {
	wclear(window.stdscr);
	box(window.stdscr, 0, 0);
	mvwaddstr(window.stdscr, 0, 2, "Scoresboard pad");
	wrefresh(window.stdscr);

	touchwin(pad.score);
	prefresh(pad.score, length.pad.score.vl, 0, 
			1, 1, LINES - 2, COLS - 2);
}

void drawlog(void) {
	wclear(window.stdscr);
	box(window.stdscr, 0, 0);
	mvwaddstr(window.stdscr, 0, 2, "Log pad");
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


int center(WINDOW* window, int y, const char* fmt, ...) {
	if (window == NULL)
		return -1;

	va_list ap;
	va_start(ap, fmt);
	
	int len = vsnprintf(NULL, 0, fmt, ap);
	char* str = (char*)malloc(sizeof(char)*len + 1);
	memcheck(str);
	vsnprintf(str, len + 1, fmt, ap);

	mvwaddstr(window, y, (getmaxx(window) - len) / 2, str);

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
	waddstr(pad, ".\n");

	hp_title(pad, "Help pad controls");
	hp_keys(pad, "w", 0); hp_keyword(pad, "Scroll"); waddstr(pad, " the help pad up by 1 line.");
	hp_keys_word(pad, "ua", 0); hp_keyword(pad, "Scroll"); waddstr(pad, " the help pad up by 1 line.");
	hp_keys(pad, "s"); hp_keyword(pad, "Scroll"); waddstr(pad, " the help pad down by 1 line.");
	hp_keys_word(pad, "da"); hp_keyword(pad, "Scroll"); waddstr(pad, " the help pad down by 1 line.");
	hp_keys(pad, "W"); hp_keyword(pad, "Scroll"); waddstr(pad, " the help pad up by 5 lines.");
	hp_keys(pad, "S"); hp_keyword(pad, "Scroll"); waddstr(pad, " the help pad down by 5 lines.");
	hp_keys(pad, "hH"); hp_keyword(pad, "Exit/hide"); waddstr(pad, " the help pad.");

	hp_title(pad, "Global controls");
	hp_add(pad, "These keystrokes work in all "); hp_keyword(pad, "non-promt windows"); waddstr(pad, ".\n\n");
	hp_add(pad, "For example, the exit, gameover and play again windows are\n");
	hp_add(pad, "the prompt windows, so they wont check for global keystokes.\n");
	hp_keys(pad, "hH"); waddstr(pad, "Show/hide the "); hp_keyword(pad, "help pad"); waddstr(pad, ".");
	hp_keys(pad, "bB"); waddstr(pad, "Show/hide the "); hp_keyword(pad, "scoreboard pad"); waddstr(pad, ".");
	hp_keys(pad, "lL"); waddstr(pad, "Open/close the "); hp_keyword(pad, "log pad"); waddstr(pad, ".");
	hp_keys(pad, "q"); hp_keyword(pad, "Quit"); waddstr(pad, " the game.");
	hp_keys(pad, "Q"); hp_keyword(pad, "Force quit"); waddstr(pad, " the game. (exits without a prompt)");

	hp_title(pad, "Game terminology");
	hp_definition(pad, "da", 0); waddstr(pad, "Down arrow");
	hp_definition(pad, "la"); waddstr(pad, "Left arrow");
	hp_definition(pad, "ua"); waddstr(pad, "Up arrow");
	hp_definition(pad, "ra"); waddstr(pad, "Right arrow\n");
	hp_definition(pad, "score"); waddstr(pad, "Depends on your skill.");
	hp_definition(pad, "catch time"); waddstr(pad, "The time beetwen mouse catches.");
	hp_definition(pad, "dist. travelled"); waddstr(pad, "Distance travelled beetween mouse");
	hp_definition(pad, NULL); waddstr(pad, "catches. Inversely proportional to "); hp_keyword(pad, "score"); waddstr(pad, ".");
	hp_definition(pad, "timeout"); waddstr(pad, "The time beetwen game cycles.");
	hp_definition(pad, "pause timeout"); waddstr(pad, "The pause time after you leave "); hp_keyword(pad, "pause window"); waddstr(pad, ".");
	
	hp_title(pad, "Interface terminology");
	hp_definition(pad, "bar", 0); waddstr(pad, "The vedy down line under firs vertical");
	hp_definition(pad, NULL); waddstr(pad, "line separator. Contains ");hp_keyword(pad, "score"); waddstr(pad, ",");
	hp_definition(pad, NULL); hp_keyword(pad, "catch time");waddstr(pad, ", ");hp_keyword(pad, "msgbar");waddstr(pad, " and ");hp_keyword(pad, "led");waddstr(pad, ".");
	hp_definition(pad, "scorebar"); waddstr(pad, "Lives in ");hp_keyword(pad, "bar");waddstr(pad, " in left down corner.");
	hp_definition(pad, NULL);waddstr(pad, "Tells the "); hp_keyword(pad, "score"); waddstr(pad, ".");
	hp_definition(pad, "timebar"); waddstr(pad, "Lives in ");hp_keyword(pad, "bar");waddstr(pad, " in left down corner, right");
	hp_definition(pad, NULL);waddstr(pad, "after ");hp_keyword(pad, "scorebar"); waddstr(pad, ". Tells the ");hp_keyword(pad, "catch time"); waddstr(pad, ".");
	hp_definition(pad, "msgbar");waddstr(pad, "Lives in the end of "); hp_keyword(pad, "bar"); waddstr(pad, " right before ");hp_keyword(pad, "led"); waddstr(pad, ".");
	hp_definition(pad, NULL);waddstr(pad, "Shows some messages.");
	hp_definition(pad, "led"); waddstr(pad, "Located at the very end of "); hp_keyword(pad, "bar"), waddstr(pad, " after "); hp_keyword(pad, "msgbar"); waddstr(pad, ".");
	hp_definition(pad, NULL); waddstr(pad, "Has four colors in the next order:");
	hp_definition(pad, NULL);
	wattrset(pad, COLOR_PAIR(color.pair.led[0]));
	waddstr(pad, "RED");
	wattroff(pad, COLOR_PAIR(color.pair.led[0]));
	waddstr(pad, "    ");
	wattrset(pad, COLOR_PAIR(color.pair.led[1]));
	waddstr(pad, "MAGENTA");
	wattroff(pad, COLOR_PAIR(color.pair.led[1]));
	waddstr(pad, "    ");
	wattrset(pad, COLOR_PAIR(color.pair.led[2]));
	waddstr(pad, "YELLOW");
	wattroff(pad, COLOR_PAIR(color.pair.led[2]));
	waddstr(pad, "    ");
	wattrset(pad, COLOR_PAIR(color.pair.led[3]));
	waddstr(pad, "GREEN");
	wattroff(pad, COLOR_PAIR(color.pair.led[3]));
	hp_definition(pad, NULL); waddstr(pad, "Led colors get less agressive with index grownth.");
	hp_definition(pad, NULL); waddstr(pad, "For example, RED is most angry color, is used when game,");
	hp_definition(pad, NULL); waddstr(pad, "is paused or you entered bad username, or when you try");
	hp_definition(pad, NULL); waddstr(pad, "to open "); hp_keyword(pad, "help pad"); waddstr(pad, " while he isn't loaded.");
	hp_definition(pad, "LINES", 2); waddstr(pad, "The lines count in opened terminal window.");
	hp_definition(pad, "COLS"); waddstr(pad, "The columns count in opened terminal window.");
	hp_definition(pad, "window", 2); waddstr(pad, "The any window.");
	hp_definition(pad, "pad"); waddstr(pad, "Like window, but you can scroll it.");
	hp_definition(pad, "prompt window"); waddstr(pad, "Window that do not lisen for global keystrokes");
	hp_definition(pad, NULL); waddstr(pad, "and want you to decide/enter something.\n");
	hp_add(pad, "For example "); hp_keyword(pad, "windows"); waddstr(pad, " and "); hp_keyword(pad, "pads"); waddstr(pad, " see the "); hp_keyword(pad, "Windows"); waddstr(pad, " and "); hp_keyword(pad, "Pads"); waddstr(pad, " sections.");

	hp_title(pad, "Windows");
	hp_add(pad, "The game won't start if the "); hp_keyword(pad, "LINES"); wprintw(pad, " is less than %d or ", length.window.stdscr.minl); hp_keyword(pad, "COLS"); wprintw(pad, " is less than %d.\n", length.window.stdscr.minc);
	hp_add(pad, "If so, then the "); hp_keyword(pad, "log pad"); waddstr(pad, " will open with corresponding warn and error\n");
	hp_add(pad, "messages. Read them and resize your terminal window accordingly.");
	hp_definition(pad, "game"); waddstr(pad, "The game "); hp_keyword(pad, "window"); waddstr(pad, ". Contains snake and mouse.");
	hp_definition(pad, NULL); wprintw(pad, "Has the constant size %d lines by %d columns.", length.window.game.minl, length.window.game.minc);
	hp_definition(pad, "sidelog"); waddstr(pad, "The log  "); hp_keyword(pad, "window"); waddstr(pad, ". (not "); hp_keyword(pad, "pad"); waddstr(pad, ")");
	hp_definition(pad, NULL); hp_keyword(pad, "Sidelog"); wprintw(pad, " has size atleast "); hp_keyword(pad, "LINES"); wprintw(pad, " lines by %d columns.", length.window.sidelog.minc);
	hp_definition(pad, NULL); wprintw(pad, "If the terminal whindow has less than %d", length.window.game.minc + 1 + length.window.sidelog.minc);
	hp_definition(pad, NULL); waddstr(pad, "columns, sidelog won't open.");
	hp_definition(pad, "bar"); wprintw(pad, "Has the size %d lines by %d columns.", length.window.bar.minl, length.window.bar.minc);
	hp_definition(pad, NULL); waddstr(pad, "See "); hp_keyword(pad, "Interface terminology"); waddstr(pad, " section for more");
	hp_definition(pad, NULL); waddstr(pad, "details about "); hp_keyword(pad, "bar"); waddstr(pad, ".\n");

	hp_definition(pad, "exit"); waddstr(pad, "The exit "); hp_keyword(pad, "prompt"); waddstr(pad, " "); hp_keyword(pad, "widnow"); waddstr(pad, ".");
	hp_definition(pad, NULL); waddstr(pad, "Appears in the center of terminal window.");
	hp_definition(pad, NULL); wprintw(pad, "Has the size %d lines by %d columns. ", length.window.exit.minl, length.window.exit.minc);
	hp_definition(pad, "finals"); waddstr(pad, "The finals "); hp_keyword(pad, "prompt"); waddstr(pad, " ");  hp_keyword(pad, "widnow"); waddstr(pad, ".");
	hp_definition(pad, NULL); waddstr(pad, "Appears in the center of "); hp_keyword(pad, "game window"); 
	hp_definition(pad, NULL); waddstr(pad, "when game ends. Requests you to enter your nickname.");
	hp_definition(pad, NULL); wprintw(pad, "Has the size %d lines by %d columns. ", length.window.exit.minl, length.window.exit.minc);
	hp_definition(pad, "again"); waddstr(pad, "The again "); hp_keyword(pad, "prompt"); waddstr(pad, " ");  hp_keyword(pad, "widnow"); waddstr(pad, ".");
	hp_definition(pad, NULL); waddstr(pad, "Appears in the center of "); hp_keyword(pad, "game window"); 
	hp_definition(pad, NULL); waddstr(pad, "after "); hp_keyword(pad, "finals"); waddstr(pad, " "); hp_keyword(pad, "prompt window"); waddstr(pad, ".");
	hp_definition(pad, NULL); waddstr(pad, "Requests you to paly or enter the "); hp_keyword(pad, "standby mode"); waddstr(pad, ".");
	hp_definition(pad, NULL); wprintw(pad, "Has the size %d lines by %d columns.", length.window.again.minl, length.window.again.minc);
	
	hp_title(pad, "Pads");
	hp_definition(pad, "help", 0); waddstr(pad, "Yes. This is this "); hp_keyword(pad, "pad"); waddstr(pad, " that you are reading.");
	hp_definition(pad, NULL); wprintw(pad, "The visible size is "); hp_keyword(pad, "LINES"); wprintw(pad, "-2 lines by %d columns.", length.pad.help.minc);
	hp_definition(pad, NULL); wprintw(pad, "The real size is ");wprintw(pad, "%d lines by %d columns.", length.pad.help.minl, length.pad.help.minc);
	hp_definition(pad, NULL); wprintw(pad, "Can be rebuilt with "); hp_option(pad, "--build-help-pad"); waddstr(pad, " option.");
	hp_definition(pad, "socreboard", 0); waddstr(pad, "This is this scoreboard "); hp_keyword(pad, "pad"); waddstr(pad, ".\n");
	hp_definition(pad, NULL); wprintw(pad, "The visible size is "); hp_keyword(pad, "LINES"); wprintw(pad, "-2 lines by %d columns.", length.pad.score.minc);
	hp_definition(pad, NULL); wprintw(pad, "The real size is ");wprintw(pad, "%d lines by %d columns.", length.pad.score.minl, length.pad.score.minc);
	hp_definition(pad, "log"); waddstr(pad, "The log ");hp_keyword(pad, "pad"); waddstr(pad, ". Simillar to "); hp_keyword(pad, "sidelog"); waddstr(pad, " but an "); hp_keyword(pad, "pad"); waddstr(pad, ".");
	hp_definition(pad, NULL); wprintw(pad, "The visible size is "); hp_keyword(pad, "LINES"); wprintw(pad, "-2 lines by "); hp_keyword(pad, "COLS"); waddstr(pad, "-2 columns.");
	hp_definition(pad, NULL); waddstr(pad, "The default real size is ");wprintw(pad, "%d lines by ", CORE_DEFAULT_LOG_SCROLLBACK); hp_keyword(pad, "COLS"); waddstr(pad, "-2 columns.");
	hp_definition(pad, NULL); waddstr(pad, "The real lines count can be set with"); 
	hp_definition(pad, NULL); waddstr(pad, "the "); hp_option(pad, "--log-scrollback"); waddstr(pad, " option.");
 

	hp_title(pad, "Game controls");
	hp_keys_word(pad, "ra ua la ra", 0);
	hp_keys(pad, "dwas"); waddstr(pad, "Change snake "); hp_keyword(pad, "direction"); waddstr(pad, ".");
	hp_keys(pad, "DWAS"); waddstr(pad, "\n\n");
	hp_add(pad, "Hold any of snake controls buttons down for "); hp_keyword(pad, "speedup"); waddstr(pad, ".\n");
	hp_keys(pad, "pP"); hp_keyword(pad, "Pause/unpause"); waddstr(pad, " the game.");
	hp_keys(pad, "hH"); waddstr(pad, "Show "); hp_keyword(pad, "help"); waddstr(pad, ".");

	hp_title(pad, "Finals window controls");
	hp_add(pad, "Finals window only want you to insert your nickname or press return\n");
	hp_add(pad, "if the old nickname had been loaded.");


	hp_title(pad, "Again window controls");
	hp_add(pad, "Again window can recognize "); hp_keyword(pad, "ONLY"); waddstr(pad, " next keys:\n");
	hp_keys(pad, "yY"); waddstr(pad, "Play again.");
	hp_keys(pad, "nN"); waddstr(pad, "Do not play again and enter the "); hp_keyword(pad, "standby mode"); waddstr(pad, ".");

	hp_title(pad, "Standby mode");
	hp_add(pad, "This mode is entered when you ansver N/n in the "); hp_keyword(pad, "again window"); waddstr(pad, ".\n");
	hp_add(pad, "This is like regular game, only difference is there is no game going.\n");
	hp_add(pad, "When you enter it, the "); hp_keyword(pad, "led"); waddstr(pad, " turns red.\n");
	hp_add(pad, "Also it has one special keystroke:");
	hp_keys(pad, "sS"); waddstr(pad, "Leave the "); hp_keyword(pad, "standby mode"); waddstr(pad, " and start the game again.");

	hp_title(pad, "Exit window controls");
	hp_add(pad, "Exit window can recognize "); hp_keyword(pad, "ONLY"); waddstr(pad, " next keys:\n");
	hp_keys(pad, "cnCN"); waddstr(pad, "Approve the exit. (mean, "); hp_keyword(pad, "exit"); waddstr(pad, " the game)");
	hp_keys(pad, "qyQY"); waddstr(pad, "Decline the exit and hide the exit window.");

	hp_title(pad, "Log pad controls");
	hp_keys(pad, "w", 0); hp_keyword(pad, "Scroll"); waddstr(pad, " the log pad up by 1 line.");
	hp_keys_word(pad, "ua", 0); hp_keyword(pad, "Scroll"); waddstr(pad, " the log pad up by 1 line.");
	hp_keys(pad, "s"); hp_keyword(pad, "Scroll"); waddstr(pad, " the log pad down by 1 line.");
	hp_keys_word(pad, "da"); hp_keyword(pad, "Scroll"); waddstr(pad, " the log pad down by 1 line.");
	hp_keys(pad, "W"); hp_keyword(pad, "Scroll"); waddstr(pad, " the log pad up by 5 lines.");
	hp_keys(pad, "S"); hp_keyword(pad, "Scroll"); waddstr(pad, " the log pad down by 5 lines.");
	hp_keys(pad, "lL"); hp_keyword(pad, "Exit/hide"); waddstr(pad, " the log pad.");

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
	hp_add(pad, "Score depends on the "); hp_keyword(pad, "dist. travelled"); waddstr(pad, " and is inversely proportional\n");
	hp_add(pad, "to it. "); hp_keyword(pad, "Minimal score"); waddstr(pad, " that you can get from mouse catch is 10\n");
	hp_add(pad, "maximum score from one mouse if 25.\n");
	hp_add(pad, "If you hit the wall or bit itself you lose, the "); hp_keyword(pad, "msgbar"); waddstr(pad, " will scream\n");
	hp_add(pad, "\"Press return\", the "); hp_keyword(pad, "led"); waddstr(pad, " will turn red.\n\n");
	hp_add(pad, "Then you will be asked to enter your nickname or press return if\n");
	hp_add(pad, "there is prefious nickname loaded.\n");
	hp_add(pad, "Then the "); hp_keyword(pad, "again window"); waddstr(pad, " will popup and ask you to play again or\n");
	hp_add(pad, "go to "); hp_keyword(pad, "standby mode"); waddstr(pad, ". The cycle countinues.");
	
	hp_title(pad, "Scoreboard pad controls");
	hp_keys(pad, "w", 0); hp_keyword(pad, "Scroll"); waddstr(pad, " the scoreboard pad up by 1 line.");
	hp_keys_word(pad, "ua", 0); hp_keyword(pad, "Scroll"); waddstr(pad, " the scoreboard pad up by 1 line.");
	hp_keys(pad, "s"); hp_keyword(pad, "Scroll"); waddstr(pad, " the scoreboard pad down by 1 line.");
	hp_keys_word(pad, "da"); hp_keyword(pad, "Scroll"); waddstr(pad, " the scoreboard pad down by 1 line.");
	hp_keys(pad, "W"); hp_keyword(pad, "Scroll"); waddstr(pad, " the scoreboard pad up by 5 lines.");
	hp_keys(pad, "S"); hp_keyword(pad, "Scroll"); waddstr(pad, " the scoreboard pad down by 5 lines.");
	hp_keys(pad, "bB"); hp_keyword(pad, "Exit/hide"); waddstr(pad, " the scoreboard pad.");

	hp_title(pad, "Snake logging lore");
	hp_add(pad, "The log has 6 levels of verbosity:\n");
	for (int i = 0; i < 6; i++) {
		wprintw(pad, "   %d: ", i);
		wattrset(pad, COLOR_PAIR(g_log.levelcolor[i]) | g_log.levelattr[i]);
		wprintw(pad, "%s\n", log_log_levelstr[i]);
		wattroff(pad, COLOR_PAIR(g_log.levelcolor[i]) | g_log.levelattr[i]);
	}
	hp_add(pad, "The smaller the log message level is, the more it is verbose.\n");
	hp_add(pad, "Logs are written to "); hp_keyword(pad, "log pad"); waddstr(pad, ", "); hp_keyword(pad, "sidelog"); waddstr(pad, " and "); hp_keyword(pad, "log file"); waddstr(pad, ".\n");
	hp_add(pad, "The log file path is %s. It can't be changed with options.\n", flag.core.logpath); 
	hp_add(pad, "The default "); hp_keyword(pad, "log pad"); waddstr(pad, " scrollback is "); wprintw(pad,  "%d lines.\n", CORE_DEFAULT_LOG_SCROLLBACK);
	hp_add(pad, "The "); hp_keyword(pad, "log pad"); waddstr(pad, " scrollback lines can be set with"); waddstr(pad, " the "); hp_option(pad, "--log-scrollback"); waddstr(pad, " option.\n");
	hp_add(pad, "You can't set the "); hp_option(pad, "--log-scrollback"); waddstr(pad, " to be less than "); hp_keyword(pad, "LINES"); waddstr(pad, "-2.");


	hp_title(pad, "Options"); 
	hp_add(pad, "This is the terminal snake options.\n");
	hp_add(pad, ""); wprintw(pad, "%s [-h] [-l] [-p <uint>] [-l <uint>] [-t <uint>]", execname);
	hp_definition(pad, NULL, -25);
	hp_definition(pad, "--help"); 
	hp_definition(pad, "-h"); waddstr(pad, "Open "); hp_keyword(pad, "help window"); waddstr(pad, " at start.");
	hp_definition(pad, "--open-log"); 
	hp_definition(pad, "-l"); waddstr(pad, "Open "); hp_keyword(pad, "log pad"); waddstr(pad, " at exit.");
	hp_definition(pad, "--pause-timeout <uint>"); 
	hp_definition(pad, "-p <uint>"); waddstr(pad, "Set the "); hp_keyword(pad, "pause timeout"); waddstr(pad, " with <uint>ms.");
	hp_definition(pad, "--log-scrolback <uint>"); 
	hp_definition(pad, "-l <uint>"); waddstr(pad, "Set the "); hp_keyword(pad, "log pad"); waddstr(pad, " scrollback <uint> lines.");
	hp_definition(pad, "--timeout <uint>"); 
	hp_definition(pad, "-t <uint>"); waddstr(pad, "Set the "); hp_keyword(pad, "timeout"); waddstr(pad, " (game speed) with <uint>ms.");
	hp_definition(pad, "--build-help-pad", 2); waddstr(pad, "Build the "); hp_keyword(pad, "help pad"); waddstr(pad, " and exit.");
	hp_definition(pad, "--help-pad-path <str>"); waddstr(pad, "Set the built "); hp_keyword(pad, "help pad"); waddstr(pad, " window path to load.");

	hp_title(pad, "Portability notes");
	hp_add(pad, "The "); hp_keyword(pad, "help pad"); waddstr(pad, " is terminal-specific. It is being built for your terminal\n");
	hp_add(pad, "especially when compiling with build.sh. If you builded ");  hp_keyword(pad, "help pad"); waddstr(pad, " in one\n");
	hp_add(pad, "terminal and launching the game from other one, please,\n");
	hp_add(pad, "consider adding "); hp_option(pad, "--build-help-pad"); waddstr(pad, " to options.\n\n");

	hp_add(pad, "All the files in the \\data folder exept log.txt are system-specific.\n");
	hp_add(pad, "They may not have any sense on other system.");

	hp_title(pad, "Contacts, contributing  and issue reporting");
	hp_add(pad, "If You find any bugs or unexpected behaivors or just want to\n");
	hp_add(pad, "contribute to the snake-cpp, You are welcome at projecth github homepage.\n");
	hp_add(pad, "This project lives at "); hp_keyword(pad, "snake-cpp - github.com/Dolfost/snake-cpp"); waddstr(pad, ".\n");
	hp_add(pad, "Made by "); hp_keyword(pad, "Dolfost - github.com/Dolfost"); waddstr(pad, ".\n");

	



	wattrset(pad, A_BOLD | A_UNDERLINE);
	center(pad, length.pad.help.minl - 1, "Glory to Ukraine!");
	wattroff(pad, A_BOLD | A_UNDERLINE);

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
	mvwaddch(pad, getcury(pad) + 3, (getmaxx(pad) - len - 2)/2, '-');

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
	static int offset = 18;
	if (plusline < 0) {
		offset = -plusline;
		return;
	}
	int shift = 2;
	static int len = 0;
	if (str == NULL) {
		wmove(pad, getcury(pad) + plusline, offset + shift);
		return;
	}
	len = strlen(str);
	
	
	wmove(pad, getcury(pad) + plusline, (offset - len) / 2 + shift);
	wattrset(pad, COLOR_PAIR(color.pair.help.definition) | attribute.help.definition);
	waddstr(pad, str);
	wattroff(pad, COLOR_PAIR(color.pair.help.definition) | attribute.help.definition);

	wmove(pad, getcury(pad), offset + shift);
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

void hp_option(WINDOW* pad, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	wattrset(pad, COLOR_PAIR(color.pair.help.definition) | attribute.help.definition);
	vw_printw(pad, fmt, args);
	wattroff(pad, COLOR_PAIR(color.pair.help.definition) | attribute.help.definition);

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

	
	for (int i = 0; i < (offset - len)/2 + shift - 1; i++) {
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
