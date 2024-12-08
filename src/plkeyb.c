#define PLRT_ENABLE_HANDLER
#include <pl-coreutils.h>

char* keyPaths[3] = { "/etc/keymaps/", "/opt/share/keymaps/", "/usr/share/keymaps/" };

int plcu_plkeyb(plptr_t args, plmt_t* mt){
	bool keymapIsPath = false;
	char** rawArgs = args.pointer;
	char* activeKeyPath = NULL;
	char keymapName[256] = "";
	char keymapPath[1024] = "";
	int consolefd = -1;
	int keymapfd = -1;
	char junk = 0;

	if(args.size < 2){
		plCUPrintArgsError(1, "plkeyb");
		return 1;
	}else{
		for(int i = 1; i < args.size; i++){
			if(strcmp("--help", rawArgs[i]) == 0){
				printf("PortaLinux Core Utilities v0.01\n\n");
				printf("Usage: plkeyb [options] keymap\n\n");
				printf("Load a different keyboard layout\n\n");
				puts("Options:");
				puts("--keymapdir [/path/to/dir/]	Use path as the keymap directory");
				puts("--kmapispath			Treat `keymap` as a relative path");
				return 0;
			}else if(strcmp("--keymapdir", rawArgs[i]) == 0){
				activeKeyPath = rawArgs[i + 1];
				i++;
			}else if(strcmp("--kmapispath", rawArgs[i]) == 0){
				keymapIsPath = true;
			}else{
				plCUPrintArgsError(2, "plkeyb");
				return 0;
			}
		}

		strncpy(keymapName, rawArgs[args.size - 1], 256);
	}

	strcpy(keymapPath, "/dev/tty");
	consolefd = open(keymapPath, O_RDWR);
	if(ioctl(consolefd, KDGKBTYPE, &junk) != 0){
		close(consolefd);
		consolefd = open("/dev/console", O_RDWR);
	}

	if(keymapIsPath){
		keymapfd = open(keymapName, O_RDONLY);
	}else if(activeKeyPath != NULL){
		strncpy(keymapPath, activeKeyPath, 768);
		strcpy(keymapPath, keymapName);
		keymapfd = open(keymapPath, O_RDONLY);
	}else{
		for(int i = 0; i < 3; i++){
			strcpy(keymapPath, keyPaths[i]);
			strcpy(keymapPath + strlen(keyPaths[i]), keymapName);
			if((keymapfd = open(keymapPath, O_RDONLY)) != -1)
				i = 3;
		}
	}

	if(keymapfd == -1){
		fputs("Could not open keymap: ", stderr);
		if(activeKeyPath != NULL)
			fputs(keymapPath, stderr);
		else
			fputs(keymapName, stderr);
		fputs("\n", stderr);
		return 2;
	}

	if(consolefd == -1 || ioctl(consolefd, KDGKBTYPE, &junk) != 0){
		fputs("Could not open console\n", stderr);
		return 2;
	}

	plCULoadKeyboardMap(keymapfd, consolefd);

	close(consolefd);
	close(keymapfd);

	return 0;
}
