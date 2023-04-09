#define INCLUDEPATH "./include"

const char* execname;

struct Windows {
	WINDOW* stdscr;
	WINDOW* log;
	WINDOW* help;
};

struct Parameters {
	bool unknown_opts = false;
	bool show_help = false;
};
