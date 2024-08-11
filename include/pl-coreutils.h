/******************************\
* pl-coreutils v0.01           *
* (c) 2024 CinnamonWolfy & Co. *
* Under MIT License            *
\******************************/
#include <plrt.h>
#include <plterm.h>
#include <fcntl.h>
#include <pwd.h>
#include <spwd.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <linux/keyboard.h>
#define EMBEDDED_NR_KEYS 128

int plCULoadKeyboardMap(int mapFile, int consoleFile);
int plCUCheckPassword(char* username, char* password);
int plCUMulticall(plptr_t* args);
