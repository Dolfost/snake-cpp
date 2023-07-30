#ifndef SETUP_SNAKE
#define SETUP_SNAKE

#include <time.h>
#include <ncurses.h>

void setup(void);

int init(void);
void deinit(void);

void process_argv(int, char**);

WINDOW* buildhelppad(const char*);
void fillhelp(FILE* file);
#endif /* ifndef SETUP_SNAKE */
