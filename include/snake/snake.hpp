void setup(void);

int wkbhit(WINDOW*);
#define kbhit() wkbhit(stdscr)

