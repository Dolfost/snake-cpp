#define SNAKE_VERSION "2.0.1"

int wkbhit(WINDOW*);
#define kbhit() wkbhit(stdscr)

void werase(WINDOW*, short, short);
#define erase(from, to) werase(stdscr, from, to)

void setup(void);
void desetup(void);

void draw(void);
void drawgame(void);
void drawgamelines(void);
void drawover(void);
void drawagain(void);

void drawhelp(void);
void drawlog(void);

int baradd(const char*);
short setled(short);

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

void hp_title(WINDOW*, const char*, ...);
void hp_keys(WINDOW*, const char*, short plusline = 1);
void hp_keys_word(WINDOW*, const char*, short plusline = 1);
void hp_keyword(WINDOW*, const char*, ...);
void hp_definition(WINDOW*, const char*, short plusline = 1);
void hp_option(WINDOW* pad, const char*, ...);
void hp_add(WINDOW*, const char*, ...);
void incolor(WINDOW*, short, int, const char*, ...);
