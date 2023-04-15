void input(void);
void wallhit(void);

struct Snake {
	struct Snake_position {
		int y;
		int x;
	} position;
	bool hit;
};

struct Bait {
	struct Bait_position {
		int y;
		int x;
	} position;
};
