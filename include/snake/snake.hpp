#define SNAKE_VERSION "1.1.3"

void setup(void);
int get_random(int, int);

int wkbhit(WINDOW*);
#define kbhit() wkbhit(stdscr)

void werase(WINDOW*, short, short);
#define erase(from, to) werase(stdscr, from, to)

double timediff(const struct timespec*, const struct timespec*);

void draw(void);
void drawgame(void);
void drawgamelines(void);

void helppad_title(WINDOW*, const char*, ...);
void helppad_keys(WINDOW*, const char*);
void helppad_keys_word(WINDOW*, const char*);
void helppad_keyword(WINDOW*, const char*, ...);
void incolor(WINDOW*, short, int, const char*, ...);

void move(void);

bool input_snake(int);
void input_global(void);
void input_help(void);

bool help(int);


void wallhit(void);

void spawn_bait(void);


