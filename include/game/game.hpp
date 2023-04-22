#define GAME_DEFAUTL_KEY_TIMEOUT 100 // 100
#define GAME_DEFAULT_PAUSE_TIMEOUT 1000 // 1000

void gameloop(void);
void gameinput(void);

bool gamepause(int);
bool exitgame(int);


char* fgetline(FILE*);
long countlines(FILE*);

void showhelp(const char*);

enum Direction {D_RIGHT, D_UP, D_LEFT, D_DOWN, D_NONE};
const char* directionstr[] = {"right", "up", "left", "down"};

struct Point {
	short x;
	short y;
};

struct Snake {
	Point head; // point to draw at frame update
	Point tail; // point to erase at frame update
	bool hit;	// snake have hit the wall
	bool bit;	// skake have bit itelf (hahaha what a fool)
	int length; // snake length
	
	struct timespec time_start;
	struct timespec time_end;
	double time; // how muche seconds elapsed between two mouse catches
	double pause_time;
	
	Direction direction = D_NONE;
	Point* body; // array of all points in snake body	
};

struct Bait {
	Point position;
	double distance; // distance to snake on spawn
	// fear****[0] is more fear than fear****[1] 
	short fearcolor[4];
	int fearattr[4];
	unsigned char fearradius[3];
	bool eaten;
};

struct Game {
	short score;
	float distance; // distance beetwen rat and snake's head
};

