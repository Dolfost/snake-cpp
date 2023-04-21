void setup(void);
int get_random(int, int);

int wkbhit(WINDOW*);
#define kbhit() wkbhit(stdscr)

void werase(WINDOW*, short, short);
#define erase(from, to) werase(stdscr, from, to)

double timediff(const struct timespec*, const struct timespec*);
