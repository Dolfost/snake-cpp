int init(void);
void deinit(void);

struct Windows {
	WINDOW* stdscr = NULL;
	WINDOW* log = NULL;
	WINDOW* help = NULL;
	WINDOW* sidelog = NULL;
	WINDOW* game = NULL;
	WINDOW* bar = NULL;
	WINDOW* pause = NULL;
};
struct Subwindows {

};

struct Flags {
	struct Flags_option {
		bool unknown = false;
		bool help = false;
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
	} core;
	struct Flags_window {
		bool two = false;
	} window;
};

struct Colors {
	short gamebg;
};

struct Lengths {
	struct Lengths_windows {
		struct window {
			int minl;
			int minc;
		} stdscr, log, sidelog, game, bar, pause;
	} window;
	struct Lengths_subwindow {
		struct window {
			int minl;
			int minc;
		};
	} subwindow;
	struct Bar {
		short scorename;
		short score;
		short timename;
		short time;
	} bar;
};

struct Positions {
	struct Positions_window {
		int y;
		int x;
	} pause;
};

