#define PLRT_ENABLE_HANDLER
#include <pl-coreutils.h>

extern char** environ;

char* cmpenv(char* name, char* env) {
	for(int i = 0; i < strlen(name); i++) {
		if(name[i] != env[i]) return NULL;
		if(name[i+1] == 0 && env[i+1] == '=') return &env[i+2];
	}
}

int uu_printenv(plptr_t args, plmt_t* mt) {
	int argc = args.size;
	char** argv = args.pointer;
	char** envp = environ;

	char newline = '\n';
	int start = 1, matches = 0;

	if(argc > 1) for(; start < argc; start++) {
		if(strcmp(argv[start], "--null") == 0 || strcmp(argv[start], "-0") == 0) newline = 0;
		else if(strcmp(argv[start], "--help") == 0) { fputs("general flags (must be before any variables):\n--help - you're reading this, you know\n--version - show some program info\n-0 / --null - null terminate instead of using newlines\n", stdout); return 0; }
		else if(strcmp(argv[start], "--version") == 0) { fputs("uutils printenv\n", stdout); return 0; }
		else break;
	}

	if(argc == start) {
		for(char** i = envp; *i != NULL; i++) printf("%s%c", *i, newline);
		return 0;
	}

	for(int is = start; is < argc; is++) {
		for(char** i = envp; *i != NULL; i++) {
			char* res = cmpenv(argv[is], *i);
			if(res != NULL) {
				printf("%s%c", res, newline);
				matches++;
			}
		}
	}

	if(matches == argc-start) return 0;
	return 1;

}
