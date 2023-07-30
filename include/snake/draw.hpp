#ifndef DRAW_SNAKE
#define DRAW_SNAKE

#include <ncurses.h>

void werase(WINDOW*, short, short);
#define erase(from, to) werase(stdscr, from, to)

void draw(void);
void drawgame(void);
void drawgamelines(void);
void drawover(void);
void drawagain(void);
void drawhelp(void);
void drawlog(void);
void drawscore(void);
void drawfinals(void);
int baradd(const char*);
short setled(short);

int center(WINDOW*, int, const char*, ...);

void hp_title(WINDOW*, const char*, ...);
void hp_keys(WINDOW*, const char*, short plusline = 1);
void hp_keys_word(WINDOW*, const char*, short plusline = 1);
void hp_keyword(WINDOW*, const char*, ...);
void hp_definition(WINDOW*, const char*, short plusline = 1);
void hp_option(WINDOW* pad, const char*, ...);
void hp_add(WINDOW*, const char*, ...);
void incolor(WINDOW*, short, int, const char*, ...);

#endif /* ifndef DRAW_SNAKE */

