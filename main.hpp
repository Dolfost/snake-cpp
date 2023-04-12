#define INCLUDEPATH "./include"

const char* execname;

struct Windows {
	WINDOW* stdscr = NULL;
	WINDOW* log = NULL;
	WINDOW* help = NULL;
};

struct Flags {
	struct Flags_option {
		bool unknown = false;
		bool help = false;
	} option;
	struct Flags_curses {
		bool color = false;
		bool cursor = false;
		bool small_window = false;
	} curses;
	struct Flags_core {
		const char* logpath;
	} core;
};

struct Colors {
};

struct Lengths {
	struct Lengths_windows {
		struct window {
			int miny;
			int minx;
		} stdscr;
	} window;
	struct Lengths_subwindow {
		struct subwindow {
			int miny;
			int minx;
		} log, game, bar;
	} subwindow;
};


