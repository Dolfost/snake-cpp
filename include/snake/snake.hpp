void setup(void);
int get_random(int, int);

int wkbhit(WINDOW*);
#define kbhit() wkbhit(stdscr)

void werase(WINDOW*, short, short);
#define erase(from, to) werase(stdscr, from, to)

double timediff(const struct timespec*, const struct timespec*);

void draw(void);
void drawgame(void);
void drawstdlines(void);

void move(void);

bool input_snake(int);
bool input_global(int);
void input_help(void);

void help(void);


void wallhit(void);

void spawn_bait(void);


