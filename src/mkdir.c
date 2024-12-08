#define PLRT_ENABLE_HANDLER
#include <pl-coreutils.h>

int plcu_mkdir(plptr_t args, plmt_t* mt){
	char** rawArgs = args.pointer;
	if(args.size < 2){
		plCUPrintArgsError(1, "mkdir");
		return 1;
	}else{
		for(int i = 1; i < args.size; i++){
			if(strcmp("--help", rawArgs[i]) == 0){
				printf("PortaLinux Core Utilities v0.01\n\n");
				printf("Usage: mkdir [options] PATH\n\n");
				printf("Creates a directory at PATH\n\n");
				puts("Options:");
				puts("-p	Currently unimplemented");
				return 0;
			}
		}
	}

	int success = mkdir(rawArgs[1], 0644);
	if(success < 0){
		printf("Something went wrong, directory not created!\n");
		return 2;
	}

	return 0;
}
