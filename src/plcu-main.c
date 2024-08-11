#include <pl-coreutils.h>
#include <errno.h>
#include <config.h>

/********************************************

plCULoadKeyboardMap: Loads a keyboard map
Returns: 0 (terminates the program otherwise)

*********************************************/
int plCULoadKeyboardMap(int mapFile, int consoleFile){
	char keyFlags[MAX_NR_KEYMAPS];
	uint16_t keyValues[EMBEDDED_NR_KEYS];
	struct kbentry keyEntry;

	read(keymapFile, keyFlags, 7);
	if(strcmp(keyFlags, "bkeymap") != 0){
		fputs("Invalid keymap\n", stderr);
		exit(1);
	}

	read(keymapFile, keyFlags, MAX_NR_KEYMAPS);
	for(int i = 0; i < MAX_NR_KEYMAPS; i++){
		if(keyFlags[i] != 1)
			continue;

		read(keymapFile, keyValues, EMBEDDED_NR_KEYS * sizeof(uint16_t));
		for(int j = 0; j < EMBEDDED_NR_KEYS; j++){
			keyEntry.kb_index = j;
			keyEntry.kb_table = i;
			keyEntry.kb_value = keyValues[j];
			ioctl(consoleFile, KDSKBENT, &keyEntry);
		}
	}

	return 0;
}

int plCUCheckPassword(char* username, char* password){
	//TODO: Implement password checks
	if(username == NULL || password == NULL)
		plRTPanic("plCUCheckPassword", PLRT_ERROR | PLRT_NULL_PTR, true);

	struct pwd* userEntryPtr = getpwnam(username); // TODO: Replace getpwnam with getpwnam_r
	if(userEntryPtr == NULL){
		fputs("Error: User doesn't exist!\n", strerr);
		exit(2);
	}

	if(userEntryPtr->pw_passwd == NULL)
		return 1;
}

int plCUMulticall(plptr_t* args){
	char* rawArgs = args.pointer;
	if(strcmp(rawArgs[0], "true") == 0){
		return 0;
	}else if(strcmp(rawArgs[0], "false") == 0){
		return 1;
	}else if(strcmp(rawArgs[0], "pwd") == 0){
		char currentPath[4096];
		void* errorCheck = getcwd(currentPath, 4096);
		if(errorCheck == NULL)
			plRTPanic("plCUMulticall", PLRT_ERROR | PLRT_ERRNO | errno, false);

		puts(currentPath);
	}else if(strcmp(rawArgs[0], "yes") == 0){
		while(1){
			for(int i = 1; i < args.size; i++)
				fputs(stdout, rawArgs[i]);

			fputs(stdout, "\n")
		}
	}else{
		//TODO: Finish implementing multi-call mechanism
		return 1;

		
	}

	return 0;
}
