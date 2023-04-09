#include <unistd.h>
#include <getopt.h>

void process_argv(int argc, char** argv) {
	struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{0, 0, 0, 0}
	};

	opterr = 0;

	int opt;
	int option_index = 0;

	while ((opt = getopt_long(argc, argv, ":h", long_options, &option_index)) != -1) {
		switch(opt) {
			case '?': {
				if (optopt == 0)
					log_warning("Unknown long option: '%s'", argv[optind-1]);
				else
					log_warning("Unknown option: '-%c'", optopt);
				status.unknown_opts = true;
				break;
					  }

			case ':': {
				if (long_options[option_index].has_arg == required_argument)
					log_warning("No argument(s) provided for '--%s' long option",  long_options[option_index].name);
				else
					log_warning("No argument(s) provided for '-%c' option.\n", optopt);
				break;
					  }

			case 'h': {
				status.show_help = true;
				break;
					  }

		}
	}
}
