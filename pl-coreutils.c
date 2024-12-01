#include <pl-coreutils.h>
#include <config.h>

#include <applets.h>
#define PLCU_APPLET_AMNT 1

void plCUPrintCommands(plptr_t commandList){
	plcucmdlist_t* rawCmdList = commandList.pointer;

	fputs("true false pwd yes ", stdout);
	for(int i = 0; i < commandList.size; i++)
		printf("%s ", rawCmdList[i].name);

	fputs("\n\n", stdout);
}

void plCUPrintHelp(plptr_t commandList){
	puts("PortaLinux Core Utilities v0.01");
	puts("(c)2024 CinnamonWolfy & Co, Under MIT License\n");

	puts("pl-coreutils is a multi-call binary containing all of the core utilities for the PortaLinux OS.");
	printf("To use this package, you must make symbolic/hard links for the desired command, like so:\n\n");
	puts("    ln -s /bin/pl-coreutils /bin/ls");
	puts("    ls -lh / # symlink to pl-coreutils\n");
	puts("Alternatively, you can run pl-coreutils with the `--install` flag, and pl-coreutils will install everything for you\n");

	fputs("Available commands:\n", stdout);
	plCUPrintCommands(commandList);

	puts("Credits:");
	puts("CinnamonWolfy - Multicall mechanism, plkeyb, true, false, pwd, yes");
	puts("mothcompute - All uutils commands");

	exit(0);
}

void plCUCreateSymlinks(plptr_t commandList){
	printf("Creating symlinks...");
	//TODO: Implement symlinks
	puts("Error: Not implemented");
	exit(0);
}

int main(int argc, char* argv[]){
	bool isPLCUName = (strcmp(basename(argv[0]), "pl-coreutils") == 0);
	plmt_t* mt = plMTInit(32 * 1024);
	plptr_t args = {
		.pointer = argv,
		.size = argc
	};
	plcucmdlist_t cmdList[PLCU_APPLET_AMNT];

	plCUFillEntryList(cmdList);
	plptr_t commandList = { .pointer = &cmdList, .size = PLCU_APPLET_AMNT };

	if(isPLCUName){
		if(argc > 1){
			if(strcmp(argv[1], "--install") == 0){
				plCUCreateSymlinks(commandList);
			}else if(strcmp(argv[1], "--list") == 0){
				plCUPrintCommands(commandList);
			}else if(strstr(argv[1], "--") != argv[1]){
				args.pointer = ((char**)argv + 1);
				args.size--;
			}else{
				plCUPrintHelp(commandList);
			}
		}else{
			plCUPrintHelp(commandList);
		}
	}

	return plCUMulticall(args, mt, commandList);
}
