#define INCLUDEPATH "./include"

const char* execname;

struct Windows {
	WINDOW* stdscr;
	WINDOW* log;
	WINDOW* help;
};

struct Flags {
	struct Flags_option {
		bool unknown = false;
		bool help = false;
	} option;
	struct Curses {
		bool color = false;
		bool cursor = false;
	} curses;
};

struct Colors {
	struct Colors_log {
		int msg[4] = {0};
		short background;
	} log;
};
