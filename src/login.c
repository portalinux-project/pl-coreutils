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
		char* ttyPath = ttyname(0);
		struct passwd* userStruct = getpwnam(username);

		if(ttyPath != NULL){
			int tty = open(ttyPath, O_RDWR);
			fchown(tty, userStruct->pw_uid, userStruct->pw_gid);
			fchmod(tty, 0600);
		}

		setuid(userStruct->pw_uid);
		setgid(userStruct->pw_gid);
		chdir(userStruct->pw_dir);

		printf("login: %s logged in on %s\n", username, ttyPath);

		char buffers[2][256];
		char* args[2] = { buffers[1], NULL };
		snprintf(buffers[1], 256, "-%s", userStruct->pw_shell);

		execv(realpath(userStruct->pw_shell, buffers[0]), args);
	}else{
		puts("Login incorrect");
	}

	return 0;
}
