#define GAME_NAP 80
#define GAME_KEY_TIMEOUT 10
void input(void);
void wallhit(void);

enum Direction {D_RIGHT, D_UP, D_LEFT, D_DOWN, D_NONE};

struct Snake {
	struct Snake_position {
		int y;
		int x;
	} position;
	bool hit;
	int length;
	Direction direction = D_NONE;
	
};

struct Bait {
	struct Bait_position {
		int y;
		int x;
	} position;
};

short* floor;
