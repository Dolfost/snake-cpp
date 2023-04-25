#define SNAKE_VERSION "1.1.3"

int wkbhit(WINDOW*);
#define kbhit() wkbhit(stdscr)

void werase(WINDOW*, short, short);
#define erase(from, to) werase(stdscr, from, to)

void setup(void);

void draw(void);
void drawgame(void);
void drawgamelines(void);
void drawover(void);
void drawagain(void);

void drawhelp(void);
void drawlog(void);

void move(void);

bool input_snake(int);
void input_global(void);
void input_log(void);
void input_help(void);

void wallhit(void);
void selfbit(void);
void dinner(void);

void spawn_bait(void);

int center(WINDOW*, int, const char*, ...);

void helppad_title(WINDOW*, const char*, ...);
void helppad_keys(WINDOW*, const char*);
void helppad_keys_word(WINDOW*, const char*);
void helppad_keyword(WINDOW*, const char*, ...);
void incolor(WINDOW*, short, int, const char*, ...);
