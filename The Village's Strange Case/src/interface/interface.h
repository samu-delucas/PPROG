#ifndef SC_WINDOW_H
#define SC_WINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include "../basics/basic_funcs.h"

#define ERR 0
#define OK (!(ERR))

int print_image(char *image, int r, int c);
void myscanf(int r, int c, char *result);
int print_text(Global*, char *file);
void print_string(char *string, int row, int column);



#endif  /* SC_WINDOW_H */



