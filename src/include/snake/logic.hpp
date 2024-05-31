#ifndef LOGIC_SNAKE
#define LOGIC_SNAKE

void wallhit(void);
void selfbit(void);
void dinner(void);
void move(void);
void doscores(void);
double timediff(const struct timespec*, const struct timespec*);

void spawn_bait(void);


#endif /* ifndef LOGIC_SNAKE */

