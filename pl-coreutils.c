#include <pl-coreutils.h>
#include <config.h>

void plCUPrintHelp(){
	puts("PortaLinux Core Utilities v0.01");
	puts("(c)2024 CinnamonWolfy & Co, Under MIT License\n")

	puts("pl-coreutils is a multi-call binary containing all of the core utilities for the PortaLinux OS.");
	printf("To use this package, you must make symbolic/hard links for the desired command, like so:\n\n");
	puts("	ln -s /bin/pl-coreutils /bin/ls");
	puts("	ls -lh / # symlink to pl-coreutils");
	puts("Alternatively, you can run pl-coreutils with the `--install` flag, and pl-coreutils will install everything for you\n");

	puts("Credits:");
	puts("CinnamonWolfy - Multicall mechanism, loadkmap/keyb, login");
	puts("mothcompute - The rest of the commands")

	exit(0);
}

void plCUCreateSymlinks(){
	printf("Creating symlinks...");
	//TODO: Implement symlinks
	puts("Error: Not implemented");
	exit(0);
}

int main(int argc, char* argv[]){
	bool isPLCUName = (strcmp(argv[0], "pl-coreutils") == 0)

	if(isPLCUName){
		if(argc > 2 && strcmp(argv[1], "--install") == 0)
			plCUCreateSymlinks();
		else
			plCUPrintHelp();
	}

	plmt_t* mt = plMTInit(32 * 1024);
	plptr_t* args = {
		.pointer = argv,
		.size = argc
	};
	plcucmdlist_t cmdList[PLCU_APPLET_AMNT];
	int i = 0;

	#include <applets.h>

	plarray_t commandList = { .pointer = &cmdList, .size = PLCU_APPLET_AMNT };
	return plCUMulticall(args, mt, commandList);
}
