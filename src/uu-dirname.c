#define PLRT_ENABLE_HANDLER
#include <pl-coreutils.h>
#include <getopt.h>

static struct option longopts[] = {
	{"zero", 0, 0, 'z'},
	{"basename", 0, 0, 'b'},
	{0, 0, 0, 0}
};

int uu_dirname(plptr_t args, plmt_t* mt) {
	int argc = args.size;
	char** argv = args.pointer;

	char terminator = '\n';
	char* (*fun)(char* a) = dirname;
	int opt, ind = 0;

	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "--help") == 0) {return !fputs("general flags (must be first argument):\n--help - you're reading this, you know\n--version - show some program info\n-z / --zero - null terminate output lines\n-b / --basename - use basename() instead of dirname()\n", stdout);}
		else if(strcmp(argv[i], "--version") == 0) {return !fputs("uutils dirname\n", stdout);}
	}

	while((opt = getopt_long(argc, argv, "zb", longopts, &ind)) != -1) switch(opt) {
		case 'z':
			terminator = 0;
			break;
		case 'b':
			fun = basename;
			break;
	}

	for(int i = optind; i < argc; i++) printf("%s%c", fun(argv[i]), terminator);

}
