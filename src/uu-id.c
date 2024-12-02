#define PLRT_ENABLE_HANDLER
#include <pl-coreutils.h>
#include <getopt.h>
#define setonly(A) if(only == 0) only = A; else { printf("'only' statement already specified.\n"); return 1; }

int getopttype(char* opt) {
	int type = 0;
	// 0: number
	// 1: name
	// 2: effectively garbage data
	for(int i = 0; i < strlen(opt); i++) {
		if(
				((opt[i] >= 'a' && opt[i] <= 'z') || (opt[i] >= 'A' && opt[i] <= 'Z') || opt[i] == '.' || opt[i] == '_' || opt[i] == '-') && type != 2
		) type = 1;
		else if(opt[i] < '0' || opt[i] > '9') return 2;
	}
	return type;
}

struct passwd* getpdat(int t, char* a) {
	switch(t) {
		case 0:
			return getpwuid(atoi(a));
			break;
		case 1:
			return getpwnam(a);
			break;
		default:
			printf("username is not valid\n");
			exit(1);
			break;
	}
}

int getgrouplist(const char* user, gid_t group, gid_t* groups, int* numgrps);

int process_arg(char* arg, int only, int name, char newline) {
		int argtype = getopttype(arg);
		struct passwd* p = getpdat(argtype, arg);
		struct group* g;
		if(p == NULL) {
			printf("could not get user data (did you enter a correct uid or username?)\n");
			return 1;
		}
		int gts = sysconf(_SC_NGROUPS_MAX), numgrps = gts;
		gid_t gidtable[gts];
		switch(only) {
			case 0:
				g = getgrgid(p->pw_gid);
				getgrouplist(p->pw_name, p->pw_gid, gidtable, &numgrps);
				printf("uid=%i(%s) gid=%i(%s) groups=", p->pw_uid, p->pw_name, p->pw_gid, g->gr_name);
				for(int i = 0; i < numgrps; i++) {
					g = getgrgid(gidtable[i]);
					printf("%i(%s)", g->gr_gid, g->gr_name);
					printf("%c", i+1 < numgrps ? ',' : newline);
				}
				break;
			case 1:
				if(name) printf("%s", p->pw_name);
				else printf("%i", p->pw_uid);
				printf("%c", newline);
				break;
			case 2:
				if(name) {
					g = getgrgid(p->pw_gid);
					printf("%s", g->gr_name);
				}
				else printf("%i", p->pw_gid);
				printf("%c", newline);
				break;
			case 3:
				getgrouplist(p->pw_name, p->pw_gid, gidtable, &numgrps);
				for(int i = 0; i < numgrps; i++) {
					g = getgrgid(gidtable[i]);
					if(name) printf("%s", g->gr_name);
					else printf("%i", g->gr_gid);
					printf("%c", i+1 < numgrps ? (!newline ? newline : ' ') : newline);
				}
				break;
		}
}

int uu_id(plptr_t args, plmt_t* mt) {
	int argc = args.size;
	char** argv = args.pointer;
	int	opt,
		ret = 0,
		ind = 0,
		name = 0,	// 0: uid
				// 1: string
		real = 0,
		only = 0;	// 0: ignored
				// 1: only user
				// 2: only group
				// 3: all groups
		struct option longopts[] = {
			{"context", 0, 0, 'Z'},
			{"groups", 0, 0, 'G'},
			{"group", 0, 0, 'g'},
			{"name", 0, 0, 'n'},
			{"real", 0, 0, 'r'},
			{"user", 0, 0, 'u'},
			{"zero", 0, 0, 'z'},
			{0, 0, 0, 0}
		};
	char newline = '\n';
	while((opt = getopt_long(argc, argv, "agGnruzZ", longopts, &ind)) != -1) {
		switch(opt) {
			case 'g':
				setonly(2);
				break;
			case 'G':
				setonly(3);
				break;
			case 'n':
				name = 1;
				break;
			case 'r':
				real = 1;
				break;
			case 'u':
				setonly(1);
				break;
			case 'z':
				newline = 0;
				break;
			case 'Z':
				printf(" does not support SELinux.\n");
				return 1;
				break;
		}
	}

	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "--help") == 0) { fputs("general flags (must be first argument):\n--help - you're reading this, you know\n--version - show some program info\n-a - you won't get an error, but this doesn't do anything\n-Z / --context - get a fun error message about how i can't be bothered to use SELinux\n-g / --group - show only the group id\n-G / --groups - show all group ids\n-n / --name - substitute ids for their names in text\n-r / --real - substitute effective ids for real ids\n-u / --user - show only the user id\n-z / --zero - null terminate instead of using newlines\n", stdout); return 0; }
		if(strcmp(argv[i], "--version") == 0) { fputs("uutils id\n", stdout); return 0; }
	}

#ifdef UUTILS_DEBUG
	if(!!getenv("UUTILS_DEBUG")) {
		char* reqtypes[4] = {"user and all groups", "only user", "only group", "all groups"};
		printf("req %s %s (%s id)\n", reqtypes[only], name ? "as a string" : "as a number", real ? "real" : "effective");
	}
#endif
	if(argc == optind) {
		char usrname[20];
		sprintf(usrname, "%i", real ? getuid() : geteuid());
		if(process_arg(usrname, only, name, newline) == 1) return 1;
	} else for(int arg = optind; arg < argc; arg++) {
		if(process_arg(argv[arg], only, name, newline) == 1) return 1;
	}

	return 0;

}
