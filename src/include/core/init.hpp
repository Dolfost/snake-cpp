#ifndef INIT_SNAKE
#define INIT_SNAKE

int init(void);
void desetup(void);
void deinit(void);

#include "../types.hpp"
#include "../game/game.hpp"
#include "log.hpp"
#define CORE_DEFAULT_LOG_SCROLLBACK 100


extern Windows window;
extern Subwindows subwindow;
extern Pads pad;
extern Flags flag;
extern Colors color;
extern Lengths length;
extern Positions positions;
extern Attributes attribute;
extern Game game;
extern Snake snake;
extern Bait bait;

#endif /* ifndef INIT_SNAKE */

