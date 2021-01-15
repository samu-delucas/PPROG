#ifndef PRINT_HELPER
#define PRINT_HELPER

#include <pthread.h>
#include "types.h"

#define ESC_SEQ     0x1B

typedef struct _Window Window;

typedef enum {
    COLOR_DEFAULT=-1,
    COLOR_BLACK=0,
    COLOR_RED=1,
    COLOR_GREEN=2,
    COLOR_YELLOW=3,
    COLOR_BLUE=4,
    COLOR_MAGENTA=5,
    COLOR_CYAN=6,
    COLOR_WHITE=7
} Color;

/* Mutex para bloquear el timer cuando se imprime el mapa */
pthread_mutex_t lock1;

int setWindows(Window **game, Window **lateral, Window **title, Window **bottom);

int winGetCols(Window*);

int print(Window*, Bool, char*);

int printPos(Window*, int, int, Bool, char*);

int printColor(Window*, int, int, Bool, char*);

int printColorPos(Window*, int, int, int, int, Bool, char*);

int printXCentered(Window*, int, int, int, char*);

int printCentered(Window*, int, int, char*);

int printCenteredN(Window*, int, int, char*);

int clearAll();

int clearLine(Window*, int);

int clearWin(Window*);

int setMapa(Window*, char*);

int printFileStr(Window*, char*);

int printFile(Window*, char*);

int updateWindow(Window *win);

int setWindowFile(Window *win, char *path);

#endif
