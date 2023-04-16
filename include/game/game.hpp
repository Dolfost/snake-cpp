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
	Point head;
	Point tail;
	bool hit;
	bool bit;
	int length;
	Direction direction = D_NONE;
	Point* body;	
};

struct Bait {
	Point position;
};

struct Game {
	int score;
};

