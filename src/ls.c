#define PLRT_ENABLE_HANDLER
#include <pl-coreutils.h>

int plcu_ls(plptr_t args, plmt_t* mt){
	char dirPath[4096] = ".";
	bool sortList = true;
	bool displayHidden = false;
	if(args.size > 1){
		char** rawArgs = args.pointer;
		for(int i = 0; i < args.size; i++){
			if(strstr(rawArgs[i], "-") == rawArgs[i]){
				if(strcmp("--help", rawArgs[i]) == 0){
					printf("PortaLinux Core Utilities v0.01\n\n");
					printf("Usage: ls [options] [path]...\n\n");
					printf("Lists info about the paths given. If no path is given, info about the current directory is given\n\n");
					puts("-U		Don't sort alphabetically, return raw readdir() order");
					puts("-a		Display all entries, including hidden entries (start with '.')");
				}else if(strcmp("-a", rawArgs[i]) == 0){
					displayHidden = true;
				}else if(strcmp("-U", rawArgs[i]) == 0){
					sortList = false;
				}else{
					plCUPrintArgsError(2, "ls");
					return 1;
				}
			}else{
				strncpy(dirPath, rawArgs[i], 4096);
			}
		}
	}

	plptr_t dirList = plRTGetDirents(dirPath, mt);
	struct dirent* rawDirList = dirList.pointer;
	if(sortList)
		plRTSortDirents(dirList);

	for(int i = 0; i < dirList.size - 1; i++){
		bool containsDot = strstr(rawDirList[i].d_name, ".") == rawDirList[i].d_name;
		if(!containsDot || (containsDot && displayHidden))
			printf("%s  ", rawDirList[i].d_name);
	}

	printf("%s\n", rawDirList[dirList.size - 1].d_name);
	return 0;
}
