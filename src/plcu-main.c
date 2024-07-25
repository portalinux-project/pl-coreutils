#include <pl-coreutils.h>
#include <errno.h>
#include <config.h>

int plCUMulticall(plptr_t* args){
	char* rawArgs = args.pointer;
	if(strcmp(rawArgs[0], "true") == 0){
		return 0;
	}else if(strcmp(rawArgs[0], "false") == 0){
		return 1;
	}else if(strcmp(rawArgs[0], "pwd") == 0){
		char currentPath[4096];
		void* errorCheck = getcwd(currentPath, 4096);
		if(!errorCheck)
			plRTPanic("plCUMulticall", PLRT_ERROR | PLRT_ERRNO | errno, false);

		puts(currentPath);
	}else if(strcmp(rawArgs[0], "yes") == 0){
		while(1){
			for(int i = 1; i < args.size; i++)
				fputs(stdout, rawArgs[i]);
		}
	}else{
		//Implement command checker
	}

	return 0;
}
