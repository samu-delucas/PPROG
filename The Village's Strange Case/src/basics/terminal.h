#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

struct termios initial;

void term_init();
void term_end();
void term_clear();
void term_set_size(int , int );
void term_move_to(int , int );
void term_hide_cursor();
void term_show_cursor();

#endif