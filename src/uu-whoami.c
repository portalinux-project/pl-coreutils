#define PLRT_ENABLE_HANDLER
#include <pl-coreutils.h>

int uu_whoami(plptr_t args, plmt_t* mt) {
	int argc = args.size;
	char** argv = args.pointer;

	// TODO clean this up
	if(!getenv("POSIXLY_CORRECT")) for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "--help") == 0) { fputs("general flags:\n--help - you're reading this, you know\n--version - show some program info\n", stdout); return 0; }
		if(strcmp(argv[i], "--version") == 0) { fputs("uutils whoami\n", stdout); return 0; }
	} else if(argc > 1) {
		if(strcmp(argv[1], "--help") == 0) { fputs("general flags:\n--help - you're reading this, you know\n--version - show some program info\n", stdout); return 0; }
		if(strcmp(argv[1], "--version") == 0) { fputs("uutils whoami\n", stdout); return 0; }
		write(1, "invalid argument\n", 17);
		return 1;
	}
	return system("id -un"); // TODO multicall and remove system() call
}
