#define CORE_DEFAULT_LOG_SCROLLBACK 100
int init(void);
void deinit(void);

WINDOW* buildhelppad(const char*);
void fillhelp(FILE* file);

struct Windows {
	WINDOW* stdscr = NULL;
	WINDOW* sidelog = NULL;
	WINDOW* game = NULL;
	WINDOW* bar = NULL;
	WINDOW* pause = NULL;
	WINDOW* exit = NULL;
};
struct Subwindows {

};

struct Pads {
	WINDOW* help = NULL;
	WINDOW* log = NULL;
};

struct Flags {
	struct Flags_option {
		bool unknown = false;
		bool help = false;
		bool buildhelppad = false;
		const char* helppadpath = NULL;
		long int timeout;
		long int pausetimeout;
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
	short helpbg;
	struct Color_pair {
		struct Colors_help {
			short key;
			short title;
			short keyword;
		} help;
	} pair;
};

struct Attributes {
	struct Attributes_help {
		int key;
		int title;
		int keyword;
	} help;
};

struct Lengths {
	struct Lengths_windows {
		struct window {
			int minl;
			int minc;
		} stdscr, log, sidelog, game, bar, pause, exit;
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
		} help, log;
	} pad;
};

struct Positions {
	struct Positions_window {
		int y;
		int x;
	} pause;
};

