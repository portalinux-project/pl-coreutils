#define PLRT_ENABLE_HANDLER
#include <pl-coreutils.h>
#include <sys/utsname.h>
#include <getopt.h>

#define addopt(C, O)	case C:\
			show |= O;\
			break;

#define addndopt(C, O)	case C:\
			show |= O;\
			showoptional = 1;\
			break;

uint8_t ns = 1;
char* unknown = "unknown";

void showstr(char* s) {
	ns--;
	if(s != NULL) {
		write(1, s, strlen(s));
		write(1, ns ? " " : "\n", 1);
	} else if(!ns) write(1, "\n", 1);
}

int uu_uname(plptr_t args, plmt_t* mt) {
	int argc = args.size;
	char** argv = args.pointer;

	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "--help") == 0) { showstr("general flags:\n--help - you're reading this, you know\n--version - show some program info\n-a / --all - show all of these fields, except -pi if they cannot be determined\n-s / --kernel-name - show the kernel name\n-n / --nodename - show the computer's network name\n-r / --kernel-release - show the kernel release information\n-v / --kernel-version - show the currently running kernel version\n-m / --machine - show the machine type, or processor architecture\n-p / --processor - STUBBED\n-i / --hardware-platform - STUBBED\n-o / --operating-system - show the useless operating system name that gets burned into the binary"); return 0; }
		if(strcmp(argv[i], "--version") == 0) { showstr("uutils uname\n"); return 0; }
	}

	uint8_t show = 0, showoptional = 0;
	struct utsname u;
	uname(&u);

	int opt, ind = 0;
	struct option longopts[] = {
		{"all", 0, 0, 'a'},
		{"kernel-name", 0, 0, 's'},
		{"nodename", 0, 0, 'n'},
		{"kernel-release", 0, 0, 'r'},
		{"kernel-version", 0, 0, 'v'},
		{"machine", 0, 0, 'm'},
		{"processor", 0, 0, 'p'},
		{"hardware-platform", 0, 0, 'i'},
		{"operating-system", 0, 0, 'o'},
		{0, 0, 0, 0}
	};

	while((opt = getopt_long(argc, argv, "asnrvmpio", longopts, &ind)) != -1) switch(opt) {
		addopt('a', 255);
		addopt('s', 1);
		addopt('n', 2);
		addopt('r', 4);
		addopt('v', 8);
		addopt('m', 16);
		addndopt('p', 32);
		addndopt('i', 64);
		addopt('o', 128);
	}

	if(show == 0) show = 1;

	ns = 0;
	for(uint8_t i = 0; i < 8; i++) ns += (show >> i) & 1;

	char* strs[8] = {u.sysname, u.nodename, u.release, u.version, u.machine, unknown, unknown, "PortaLinux"};

	uint16_t b = 1;
	for(uint8_t i = 0; i < 8; i++) {
		if(show & b) {
			if((strs[i] != unknown || showoptional)) showstr(strs[i]);
			else ns--;
		}
		b<<=1;
	}
}
