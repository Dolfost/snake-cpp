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
