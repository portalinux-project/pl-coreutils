#define PLRT_ENABLE_HANDLER
#include <pl-coreutils.h>

typedef struct {
	char mul;
	double len;
} sleepl;

int nanosleep(const struct timespec *duration, struct timespec* rem);

void getsleep(sleepl* slp, char* src) {
	int nc = 0, nd = 0, div = 1;
	double store = 0;
	for(int i = 0; i < strlen(src); i++) {
		if(src[i] == '.') nd++;
		else if(src[i] >= 'd' && src[i] <= 's') {
			if(nc == 0) slp->mul = src[i];
			nc++;
		}
		else if(src[i] <= '9' && src[i] >= '0') {
			if(!nd) {
				store = (10*store) + (src[i]-48);
			} else {
				div *= 10;
				double f = src[i]-48;
				store += f/div;
			}
		}
		if(nc > 1 || nd > 1) {
			printf("invalid argument %s", src);
			exit(1);
		}
	}
	slp->len = store;

}

int uu_sleep(plptr_t args, plmt_t* mt) {
	int argc = args.size;
	char** argv = args.pointer;

	int posix = !!getenv("POSIXLY_CORRECT"), argchk = argc;
	if(posix) argchk = 2;
	for(int i = 1; i < argchk; i++) {
		if(strcmp(argv[i], "--help") == 0) { fputs("general flags:\n--help - you're reading this, you know\n--version - show some program info\n\nGNU extension - lengths of time can be followed by:\ns - seconds\nm - minutes\nh - hours\nd - days\n", stdout); return 0; }
		if(strcmp(argv[i], "--version") == 0) { fputs("uutils sleep\n", stdout); return 0; }
	}
	long unsigned int sleepsec = 0, sleepnano = 0;
	sleepl slp;
	for(int i = 1; i < argc; i++) {
		for(int c = 0; c < strlen(argv[i]); c++) {
			if(
					(argv[i][c] > '9' || argv[i][c] < '0') &&
					(argv[i][c] != '.') &&
					(argv[i][c] != 's') &&
					(argv[i][c] != 'm') &&
					(argv[i][c] != 'h') &&
					(argv[i][c] != 'd')

					) {
				printf("invalid argument %s\n", argv[i]);
				return 1;
			}
		}
		getsleep(&slp, argv[i]);
		long unsigned int mul = 1000000000;
		if(slp.mul == 'm') mul *= 60;
		if(slp.mul == 'h') mul *= 3600;
		if(slp.mul == 'd') mul *= 86400;
		sleepnano += mul * slp.len;
	}
	sleepsec = sleepnano / 1000000000l;
	sleepnano %= 1000000000l;
	#ifdef UUTILS_DEBUG
	if(!!getenv("UUTILS_DEBUG")) printf("sleep time is %i %i\n", sleepsec, sleepnano);
	#endif
	struct timespec req = {sleepsec, sleepnano};
	return nanosleep(&req, NULL);
}
