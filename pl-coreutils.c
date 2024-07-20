#include <pl-coreutils.h>

void plCUPrintHelp(){
	puts("PortaLinux Core Utilities v0.01");
	printf("(c)2024 CinnamonWolfy & Co., Under MIT License\n\n")

	puts("pl-coreutils is a multi-call binary containing all of the core utilities for the PortaLinux OS.");
	printf("To use this package, you must make symbolic/hard links for the desired command, like so:\n\n");
	puts("	ln -s /bin/pl-coreutils /bin/ls");
	puts("	ls -lh / # symlink to pl-coreutils");
	puts("Alternatively, you can run pl-coreutils with the `--install` flag, and pl-coreutils will install everything for you");

	exit(0);
}

void plCUCreateSymlinks(){
	printf("Creating symlinks...")
	//TODO: Implement symlinks
	puts("Done.")
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

	plptr_t* args = {
		.pointer = argv,
		.size = argc
	};

	return plCULibraryMain(args);
}
