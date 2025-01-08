#ifndef TYPES_SNAKE
#define TYPES_SNAKE

#include <ncurses.h>
#include <time.h>

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
	int travelled;
	
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
	double distance; // distance beetwen rat and snake's head
	bool playagain;
	struct timespec time_start;
	struct timespec time_end;
	double time;
	char* playername;
	short* highlength;
	short* highscore;
	short* hightime;
	char** highplayer;
	bool game = false;
	short scoreentry;
};

struct Windows {
	WINDOW* stdscr = NULL;
	WINDOW* sidelog = NULL;
	WINDOW* game = NULL;
	WINDOW* bar = NULL;
	WINDOW* pause = NULL;
	WINDOW* exit = NULL;
	WINDOW* finals = NULL;
	WINDOW* again = NULL;
};
struct Subwindows {

};

struct Pads {
	WINDOW* help = NULL;
	WINDOW* log = NULL;
	WINDOW* score = NULL;
};

struct Flags {
	struct Flags_option {
		bool unknown = false;
		bool help = false;
		bool buildhelppad = false;
		const char* helppadpath = NULL;
		long int timeout;
		long int pausetimeout;
		bool openlog = false;
		bool nologcolor = false;
	} option;
	struct Flags_curses {
		bool color = false;
		bool change_color = false;
		bool cursor = false;
		bool small_window = false;
	} curses;
	struct Flags_core {
		char* playerpath = NULL;
		char* scorespath = NULL;
		char* logpath = NULL;
		char* helppadpath = NULL;
		FILE* logfile = NULL;
		bool score = true;
	} core;
	struct Flags_window {
		bool two = false;
	} window;
};

struct Colors {
	short gamebg;
	short helpbg;
	struct Color_pair {
		struct Colors_help {
			short key;
			short title;
			short keyword;
			short definition;
		} help;
		short led[4];
	} pair;
};

struct Attributes {
	struct Attributes_help {
		int key;
		int title;
		int keyword;
		int definition;
	} help;
};

struct Lengths {
	struct Lengths_windows {
		struct window {
			int minl;
			int minc;
		} stdscr, sidelog, game, bar, pause, exit, finals, again;
	} window;
	struct Lengths_subwindow {
		struct window {
			int minl;
			int minc;
		};
	} subwindow;
	struct Lengths_bar {
		short scorename;
		short score;
		short timename;
		short time;
	} bar;

	struct Lengths_pad {
		struct pad {
			int minl;
			int minc;
			int vl; // visible line
		} help, log, score;
	} pad;
	struct Lengths_game {
		short maxnicknamelen;
	} game;
};

struct Positions {
	struct Positions_window {
		int y;
		int x;
	} pause;
};
#endif /* ifndef TYPES_SNAKE */
