#define PLRT_ENABLE_HANDLER
#include <pl-coreutils.h>

int plcu_login(plptr_t args, plmt_t* mt){
	char username[512];
	char password[4096];

	fputs("login: ", stdout);
	fflush(stdout);
	fgets(username, 512, stdin);
	fputs("password: ", stdout);
	fflush(stdout);
	fgets(password, 4096, stdin);

	char* newlines[2];
	newlines[0] = strchr(username, '\n');
	newlines[1] = strchr(password, '\n');

	if(newlines[0] != NULL)
		*(newlines[0]) = '\0';
	if(newlines[1] != NULL)
		*(newlines[1]) = '\0';

	if(plCUCheckPassword(username, password) == 0){
		struct passwd* userStruct = getpwnam(username);
		setuid(userStruct->pw_uid);
		setgid(userStruct->pw_gid);
		chdir(userStruct->pw_dir);

		//TODO: add invocation code
		execv();
	}else{
		puts("Login incorrect");
	}

	return 0;
}
