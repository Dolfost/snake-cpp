#define GAME_NAP 80 // 80
#define GAME_KEY_TIMEOUT 10 // 10

void input(void);
void wallhit(void);

void spawn_bait(void);

enum Direction {D_RIGHT, D_UP, D_LEFT, D_DOWN, D_NONE};

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
};

struct Game {
	short score;
	float distance; // distance beetwen rat and snake's head
};

