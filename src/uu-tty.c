#define PLRT_ENABLE_HANDLER
#include <pl-coreutils.h>

// TODO what the fuck is coreutils' TTY_WRITE_ERROR

int uu_tty(plptr_t mainArgs, plmt_t* mt) {
	int argc = mainArgs.size;
	char** argv = mainArgs.pointer;

	int display = 1;

	char* args[3] = {"-s", "--silent", "--quiet"};

	if(argc > 1) {
		for(int i = 1; i < argc; i++) {
			for(int c = 0; c < 3; c++) {
				if(strcmp(argv[i], args[c]) == 0) display = 0;
			}
			if(strcmp(argv[i], "--help") == 0) { fputs("general flags (must be first argument):\n--help - you're reading this, you know\n--version - show some program info\n-s / --silent / --quiet - don't print anything, just provide a return code\n", stdout); return 0; }
			if(strcmp(argv[i], "--version") == 0) { fputs("uutils tty\n", stdout); return 0; }
		}
	}

	char* tty = ttyname(0);
	if(!tty) {
		if(display) write(1, "not a tty\n", 10);
		free(tty);
		return 1;
	}
	if(display) printf("%s\n", tty);
	free(tty);
	return 0;
}
