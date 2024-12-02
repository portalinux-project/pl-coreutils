#define PLRT_ENABLE_HANDLER
#include <pl-coreutils.h>

unsigned int ipow(unsigned int i, unsigned int f) {
	unsigned int r = 1;
	while(f) {
		if(f & 1) r *= i;
		f >>= 1;
		i *= i;
	}
	return r;
}

int oct2dec(int o) {
	int d = 0, i = 0, t;
	while (o != 0) {
		t = o % 10;
		o /= 10;
		d += t * ipow(8, i);
		i++;
	}
	return d;
}

int uu_echo(plptr_t args, plmt_t* mt) {
	int argc = args.size;
	char** argv = args.pointer;

	// options
	int newline = 1, process = 0;

	uint8_t nw = 0, start = 2, posix = !!getenv("POSIXLY_CORRECT");

	if(argc == 1) { write(1, "\n", 1); return 0; }

	if(posix) {
		process = 1;
		if(strcmp(argv[1], "-n") == 0) newline = 0;
		else start = 1;
	} else {
		if(argv[1][0] == '-') {
			if(argv[1][1] == '-') {
				if(strcmp(argv[1], "--help") == 0) { fputs("general flags (must be first argument):\n--help - you're reading this, you know\n--version - show some program info\n-n - disable trailing newline\n-e - enable text processing\n\nif text processing is enabled:\n\\\\ - backslash\n\\a - alert\n\\b - backspace\n\\c - exit when reached\n\\e - escape\n\\f - form feed\n\\n - newline\n\\r - carriage return\n\\t - tab\n\\v - vertical tab\n\\xHH - 1 or 2 hex numbers\n\\0NNN - 1-3 octal numbers\n\nYour shell might implement echo, in which case it may be used instead of uutils echo. Make sure you're using the appropriate version of echo.\n", stdout); return 0; }
				if(strcmp(argv[1], "--version") == 0) { fputs("uutils echo\n", stdout); return 0; }
			} else for(int i = 1; i < strlen(argv[1]); i++) {
				switch(argv[1][i]) {
					case 'n':
						newline = 0;
						break;
					case 'e':
						process = 1;
						break;
				}
			}
		} else start = 1;
	}

	for(int i = start; i < argc; i++) {
		if(process) {
			for(int c = 0; c < strlen(argv[i]); c++) {
				if(argv[i][c] == '\\') {
					switch(argv[i][c+1]) {

						case 'a':
							nw+=2;
							write(1, "\x07", 1);
							break;
						case 'b':
							nw+=2;
							write(1, "\x08", 1);
							break;
						case 'e':
							nw+=2;
							write(1, "\x1b", 1);
							break;
						case 'f':
							nw+=2;
							write(1, "\x0c", 1);
							break;
						case 'n':
							nw+=2;
							write(1, "\n", 1);
							break;
						case 'r':
							nw+=2;
							write(1, "\x0d", 1);
							break;
						case 't':
							nw+=2;
							write(1, "\x09", 1);
							break;
						case 'v':
							nw+=2;
							write(1, "\x0b", 1);
							break;
						case 'x': ;
							char hex;
							int nums = 0;
							for(int r = 0; r < 2; r++) {
								char tmp = argv[i][c+r+2];
								if(strchr("0123456789abcdefABCDEF", tmp) == NULL) break;
								if(tmp == 0) break;
								nw++;
								int off;
								if(tmp <= 'f') off = 71;
								if(tmp <= 'F') off = 39;
								if(tmp <= '9') off = 48;
								tmp -= off;
								if(r == 0) hex = tmp;
								if(r == 1) hex = (hex << 4) | tmp;
							}
							if(nw != 0) {write(1, &hex, 1); nw+=2;}
							break;
						case '0':
							nw+=2;
							int oct = 0;
							for(int r = 0; r < 3; r++) {
								char tmp = argv[i][c+r+2];
								if(tmp > '8' || tmp < '0') break;
								nw++;
								oct = (tmp-48) + (oct*10);
							}
							char dec = oct2dec(oct);
							write(1, &dec, 1);
							break;

						case '\\':
							nw++;
							break;
						case 'c':
							return 0;
							break;
					}
				}
				if(nw == 0) write(1, &argv[i][c], 1);
				if(nw > 0) nw--;
			}
		} else write(1, argv[i], strlen(argv[i]));
		if(i < argc-1) write(1, " ", 1);
	}

	if(newline) write(1, "\n", 1);

	// tf errors could echo even *have*
	return 0;
}
