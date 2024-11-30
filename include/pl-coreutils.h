/******************************\
* pl-coreutils v0.01           *
* (c) 2024 CinnamonWolfy & Co. *
* Under MIT License            *
\******************************/
#include <plrt.h>
#include <fcntl.h>
#include <pwd.h>
#include <libgen.h>
#include <shadow.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <linux/keyboard.h>
#define EMBEDDED_NR_KEYS 128

/**************************************************************

ADD_ENTRY: Macro to add an applet entry. Used within applets.h

**************************************************************/

#define PLCU_CREATE_DEFINITION(functionName) \
int functionName(plptr_t args, plmt_t* mt);

#define PLCU_START_ENTLIST \
void plCUFillEntryList(plcucmdlist_t* cmdList){ \
	int i = 0;

#define PLCU_END_ENTLIST }

#define PLCU_ADD_ENTRY(appletName, functionPtr) \
	cmdList[i].name = appletName; \
	cmdList[i].function = functionPtr; \
	i++;

typedef struct plcucmdlist {
	int (*function)(plptr_t, plmt_t*);
	char* name;
} plcucmdlist_t;

int plCULoadKeyboardMap(int mapFile, int consoleFile);
int plCUCheckPassword(char* username, char* password);
int plCUMulticall(plptr_t args, plmt_t* mt, plptr_t commandList);
