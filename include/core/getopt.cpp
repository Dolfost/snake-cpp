#include <unistd.h>
#include <getopt.h>
#include <string.h>

void process_argv(int argc, char** argv) {
	struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"build-help", no_argument, 0, 1000},
		{"help-path", required_argument, 0, 1001},
		{0, 0, 0, 0}
	};

	opterr = 0;

	int opt;
	int option_index = 0;

	while ((opt = getopt_long(argc, argv, ":h", long_options, &option_index)) != -1) {
		switch(opt) {
			case '?':
				if (optopt == 0)
					log_warn("Unknown long option: '%s'.", argv[optind-1]);
				else
					log_warn("Unknown option: '-%c'.", optopt);
				flag.option.unknown = true;
				break;
			case ':':
				if (long_options[option_index].has_arg == required_argument)
					log_warn("No argument(s) provided for '--%s' long option.",  long_options[option_index].name);
				else
					log_warn("No argument(s) provided for '-%c' option.", optopt);
				break;
			case 'h':
				flag.option.help = true;
				log_debug("Recieved [--help | -h] option.");
				break;
			case 1000: // --build-help
				flag.option.buildhelp = true;
				log_debug("Recieved --build-help option.");
				break;
			case 1001: // --help-path
				flag.option.helppath = optarg;
				log_debug("--help-path option recieved an");
				log_nl(   "'%s' argument.", optarg);
				break;
				
		}
	}
	
	if (flag.option.helppath == NULL) { 
		flag.option.helppath = strdup("data/help.txt");
		memcheck(flag.option.helppath);
	}
}
