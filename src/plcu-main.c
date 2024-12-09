#define PLRT_ENABLE_HANDLER
#include <pl-coreutils.h>
#include <config.h>
#include <errno.h>

/********************************************

plCULoadKeyboardMap: Loads a keyboard map
Returns: 0 (terminates the program otherwise)

*********************************************/
int plCULoadKeyboardMap(int mapFile, int consoleFile){
	char keyFlags[MAX_NR_KEYMAPS];
	uint16_t keyValues[EMBEDDED_NR_KEYS];
	struct kbentry keyEntry;

	read(mapFile, keyFlags, 7);
	if(strcmp(keyFlags, "bkeymap") != 0){
		fputs("Invalid keymap\n", stderr);
		exit(1);
	}

	read(mapFile, keyFlags, MAX_NR_KEYMAPS);
	for(int i = 0; i < MAX_NR_KEYMAPS; i++){
		if(keyFlags[i] != 1)
			continue;

		read(mapFile, keyValues, EMBEDDED_NR_KEYS * sizeof(uint16_t));
		for(int j = 0; j < EMBEDDED_NR_KEYS; j++){
			keyEntry.kb_index = j;
			keyEntry.kb_table = i;
			keyEntry.kb_value = keyValues[j];
			ioctl(consoleFile, KDSKBENT, &keyEntry);
		}
	}

	return 0;
}

/********************************************

plCUCheckPassword: Checks if the password given matches the actual
		   password of the username given
Returns: 0 when password matches
	-1 when password doesn't match

********************************************/

int plCUCheckPassword(char* username, char* password){
	if(username == NULL || password == NULL)
		plRTPanic("plCUCheckPassword", PLRT_ERROR | PLRT_NULL_PTR, true);

	struct passwd* userEntryPtr = getpwnam(username); // TODO: Replace getpwnam with getpwnam_r
	if(userEntryPtr == NULL){
		fputs("Error: User doesn't exist!\n", stderr);
		exit(2);
	}

	if(userEntryPtr->pw_passwd != NULL && strcmp("", userEntryPtr->pw_passwd) != 0){
		if(strcmp("", password) == 0)
			return -1;

		char* hashedPassword = crypt(password, userEntryPtr->pw_passwd);
		if(hashedPassword == NULL){
			fputs("Error: crypt() failed!\n", stderr);
			exit(2);
		}

		if(strcmp(userEntryPtr->pw_passwd, hashedPassword) != 0)
			return -1;
	}

	return 0;
}

/*********************************************

plCUPrintArgsError: Prints out a command-line error
Returns: Nothing

**********************************************/

void plCUPrintArgsError(uint8_t type, char* progName){
	switch(type){
		case 1:
			puts("Not enough arguments");
			break;
		case 2:
			puts("Unknown option");
			break;
	}

	printf("Run `%s --help` for more information\n", progName);
}

/*********************************************

plCUPrintError: Prints an error message based on the given error code
Returns: Nothing

*********************************************/

void plCUPrintError(char* msg, plrtret_t errCode){
	// implement thing
}

/***********************************************

plCUMulticall: Multi-call entrypoint
Returns: 0 to 255 (Depends on routine called)

***********************************************/

int plCUMulticall(plptr_t args, plmt_t* mt, plptr_t commandList){
	char** rawArgs = args.pointer;
	plcucmdlist_t* rawCmdList = commandList.pointer;
	if(strcmp(basename(rawArgs[0]), "true") == 0){
		return 0;
	}else if(strcmp(basename(rawArgs[0]), "false") == 0){
		return 1;
	}else if(strcmp(basename(rawArgs[0]), "pwd") == 0){
		char currentPath[4096];
		void* errorCheck = getcwd(currentPath, 4096);
		if(errorCheck == NULL)
			plRTPanic("plCUMulticall", PLRT_ERROR | PLRT_ERRNO | errno, false);

		puts(currentPath);
	}else if(strcmp(basename(rawArgs[0]), "yes") == 0){
		if(args.size == 1){
			while(1)
				printf("y\n");
		}else{
			while(1){
				for(int i = 1; i < args.size; i++)
					fputs(rawArgs[i], stdout);

				fputs("\n", stdout);
			}
		}
	}else{
		int i = 0;
		while(i < commandList.size && strcmp(rawCmdList[i].name, basename(rawArgs[0])) != 0)
			i++;

		if(i >= commandList.size){
			puts("Error: Applet not found");
			return 127;
		}

		return rawCmdList[i].function(args, mt);
	}

	return 0;
}
