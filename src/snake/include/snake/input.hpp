#ifndef INPUT_SNAKE
#define INPUT_SNAKE

#include <ncurses.h>

int wkbhit(WINDOW*);
#define kbhit() wkbhit(stdscr)

extern const char* directionstr[4];

void gameinput(void);

bool input_snake(int);
void input_global(void);
void input_log(void);
void input_help(void);
void input_score(void);
void input_exit(void);

bool scrollbpad(WINDOW*, int, int, int*, int);

#endif /* ifndef INPUT_SNAKE */
