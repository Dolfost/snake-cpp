#ifndef GAME_SNAKE
#define GAME_SNAKE

#include <ncurses.h>
#include <time.h>
#include <snake/setup.hpp>

#define GAME_DEFAUTL_KEY_TIMEOUT 200 // 200
#define GAME_DEFAULT_PAUSE_TIMEOUT 1000 // 1000

double timediff(const struct timespec*, const struct timespec*);
int get_random(int, int);

void gamesetup(void);

void spawn_bait(void);

void gameloop(void);
void gameinput(void);
void gameover(void);

void doscores(void);

bool help(int);
bool gamelog(int);
bool gamepause(int);
bool help(int);
bool score(int);
bool exitgame(int);

void readdata(void);
void writedata(void);

void playagain(void);
void finals(void);
void gamestill(void);

bool scrollbpad(WINDOW*, int, int, int*, int);
																
void putscore(void);

char* fgetline(FILE*);
long countlines(FILE*);

void showhelp(const char*);

#endif /* ifndef GAME_SNAKE */
