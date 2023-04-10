#define kbhit() wkbhit(stdscr)

int wkbhit(WINDOW* win)
{
	int ch,r;

	nodelay(win, TRUE);
	noecho();

	ch = getch();
	if( ch == ERR)
		r = FALSE;
	else {
		r = TRUE;
		ungetch(ch);
	}

	echo();
	nodelay(win, FALSE);

	return r;
}

