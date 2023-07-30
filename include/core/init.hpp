#ifndef INIT_SNAKE
#define INIT_SNAKE

#include "../snake/setup.hpp"

int init(void);

Windows window;
Subwindows subwindow;
Pads pad;
Flags flag;
Colors color;
Lengths length;
Positions positions;
Attributes attribute;

Game game;

Snake snake;
Bait bait;

#endif /* ifndef INIT_SNAKE */

