/**
 * Author of this function: Simone Santini
 * 
 **/

#ifndef _TERM_FUNCT_H_
#define _TERM_FUNCT_H_

#include <termios.h>
#include <stdio.h>

struct termios initial;

/** @brief
  Initializes the terminal in such a way that we can read the input
  without echo on the screen
*/
void _term_init();

#endif