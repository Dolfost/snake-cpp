#ifndef SNAKE_CORE
#define SNAKE_CORE

#include <ncurses.h>

#define CORE_DEFAULT_LOG_SCROLLBACK 100
int init(void);
void deinit(void);

void process_argv(int, char**);

WINDOW* buildhelppad(const char*);
void fillhelp(FILE* file);

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
	} option;
	struct Flags_curses {
		bool color = false;
		bool change_color = false;
		bool cursor = false;
		bool small_window = false;
	} curses;
	struct Flags_core {
		const char* logpath;
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

#endif /* ifndef SNAKE_CORE */
