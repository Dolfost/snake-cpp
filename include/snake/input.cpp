int kbhit(void)
{
	int ch,r;

	nodelay(stdscr,TRUE);
	noecho();

	ch = getch();
	if( ch == ERR)
		r = FALSE;
	else {
		r = TRUE;
		ungetch(ch);
	}

	echo();
	nodelay(stdscr,FALSE);
	return(r);
}

