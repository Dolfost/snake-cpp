#include <unistd.h>
#include <getopt.h>
#include <string.h>

void process_argv(int argc, char** argv) {
	struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"timeout", required_argument, 0, 't'},
		{"pause-timeout", required_argument, 0, 'p'},
		{"build-help-pad", no_argument, 0, 1000},
		{"help-pad-path", required_argument, 0, 1001},
		{0, 0, 0, 0}
	};

	opterr = 0;

	int opt;
	int option_index = 0;

	flag.option.timeout = GAME_DEFAUTL_KEY_TIMEOUT;
	flag.option.pausetimeout = GAME_DEFAULT_PAUSE_TIMEOUT;

	while ((opt = getopt_long(argc, argv, ":ht:", long_options, &option_index)) != -1) {
		switch(opt) {
			case '?': // unknown option
				if (optopt == 0)
					log_warn("Unknown long option: '%s'.", argv[optind-1]);
				else
					log_warn("Unknown option: '-%c'.", optopt);
				flag.option.unknown = true;
				break;
			case ':': // no argument
				if (long_options[option_index].has_arg == required_argument)
					log_warn("No argument(s) provided for '--%s' long option.",  long_options[option_index].name);
				else
					log_warn("No argument(s) provided for '-%c' option.", optopt);
				break;
			case 'h': // --help | -h
				flag.option.help = true;
				log_debug("Recieved [--help | -h] option.");
				break;
			case 't': // --timeout | -t
				log_debug("Recieved [--timeout | -t] option.");
				log_nl(   "with argument '%s'.", optarg);
				flag.option.timeout = strtol(optarg, &optarg, 0);
				if (flag.option.timeout <= 0 || *optarg != '\0') {
					log_error("Could not get proper argument for");
					log_nl(   "[--timeout | -t] option.");
					log_nl(   "It will be set to default %dms.",
							GAME_DEFAUTL_KEY_TIMEOUT);
					flag.option.timeout = GAME_DEFAUTL_KEY_TIMEOUT;
				}
				break;
			case 'p': // --pause-timeout | -p
				log_debug("Recieved [--pause-timeout | -p] option");
				log_nl(   "with argument '%s'.", optarg);
				flag.option.pausetimeout = strtol(optarg, &optarg, 0);
				if (flag.option.pausetimeout <= 0 || *optarg != '\0') {
					log_error("Could not get proper argument for");
					log_nl(   "[--pause-timeout | -p] option.");
					log_nl(   "It will be set to default %dms.",
							GAME_DEFAULT_PAUSE_TIMEOUT);
					flag.option.timeout = GAME_DEFAULT_PAUSE_TIMEOUT;
				}
				break;
			case 1000: // --build-help-pad
				flag.option.buildhelppad = true;
				log_debug("Recieved --build-help-pad option.");
				break;
			case 1001: // --help-pad-path
				flag.option.helppadpath = optarg;
				log_debug("Recieved --help-pad-path option with");
				log_nl(   "'%s' argument.", optarg);
				break;
		}
	}
	
	if (flag.option.helppadpath == NULL)
		flag.option.helppadpath = "data/help.pad.window";
}
