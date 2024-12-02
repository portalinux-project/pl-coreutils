#define PLRT_ENABLE_HANDLER
#include <pl-coreutils.h>
#include <getopt.h>

int verify_format(char* f) {

	// TODO stub
	for(int i = 0; i < strlen(f); i++) {
	}
	return 0;

}

int setenv(const char* name, const char* value, int overwrite);
int unsetenv(const char* name);

int uu_seq(plptr_t args, plmt_t* mt) {
	int argc = args.size;
	char** argv = args.pointer;

	double start = 1, increment = 1, end;

	char *terminator = "\n", *format = "%g";

	int opt, ind = 0, eq_width = 0, posix = !!getenv("POSIXLY_CORRECT");

	struct option longopts[] = {
		{"separator", 1, 0, 's'},
		{"format", 1, 0, 'f'},
		{"equal-width", 0, 0, 'w'},
		{0, 0, 0, 0}
	};

	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "--help") == 0) { fputs("general flags:\n--help - you're reading this, you know\n--version - show some program info\n-f / --format=FORMAT - format confaining %f\n-s / --separator=%s - separate numbers with this (default \\n)\n-w / --equal-width - pad with zeros\n", stdout); return 0; }
		if(strcmp(argv[i], "--version") == 0) { fputs("uutils seq\n", stdout); return 0; }
	}

	if(!posix) setenv("POSIXLY_CORRECT", "1", 0); // cheap hack for getopt
	while((opt = getopt_long(argc, argv, "ws:f:", longopts, &ind)) != -1) {
		switch(opt) {
			case 's':
				terminator = optarg;
				break;
			case 'f':
				if(verify_format(optarg) == 0) format = optarg;
				break;
			case 'w':
				eq_width = 1;
				break;
		}
	}
	if(!posix) unsetenv("POSIXLY_CORRECT");

	switch(argc - optind) {
		case 0:
			write(1, "no arguments provided\n", 22);
			return 1;
			break;
		case 1:
			end = atof(argv[optind]);
			break;
		case 2:
			start = atof(argv[optind]);
			end = atof(argv[optind+1]);
			break;
		case 3:
			start = atof(argv[optind]);
			increment = atof(argv[optind+1]);
			end = atof(argv[optind+2]);
			break;
		default:
			write(1, "too many arguments\n", 19);
			return 1;
			break;
	}

	if(start <= end) do {
		printf(format, start);
		if(start <= end-increment) printf("%s", terminator);
		else printf("\n");
		start += increment;
	} while(start <= end);

}
