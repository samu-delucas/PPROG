#ifndef BASIC_H
#define BASIC_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "libbmp.h"
#include "terminal.h"
#include "types.h"
#include "global.h"

void *clk(void* global);
void *scape(void* global);
char wait_for_input(char *c);
int generate_rand(int inf, int sup);
int* generate_perm(int N);
void wait_for_space(Global*);
void blocked_message(Global*);

#endif