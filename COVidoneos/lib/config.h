#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#define STR_LEN 256

int loadConfig();

int configGetTime();

int configGetGameWidth();

int configGetLateralWidth();

char* getIncorrectoStr();

int loadStr(char *dest, char *file_src);

#endif