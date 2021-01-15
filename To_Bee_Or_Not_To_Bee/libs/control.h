/**
 * @file    control.h
 * @author  Samuel de Lucas Maroto and Bhavuk Sikka
 * 
 * @brief   This module handles the terminal's cursor positioning, and also 
 *          clearing the screen.
 * 
 *          Almost everything is done using ANSI escape codes. 
 *          Visit https://en.wikipedia.org/wiki/ANSI_escape_code for more info
 * */


#ifndef _CONTROL
#define _CONTROL

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "types.h"
#include "grafiks.h"

/** @brief  Clears the terminal screen
 *  @return Returns ERROR in case of error and OK if everything goes well */
void clear_screen();

/** @brief  Shows the cursor in the terminal. Useful for restoring the terminal
 *          initial configuration after exiting the game
 *  @return Returns ERROR in case of error and OK if everything goes well */
void show_cursor();

/** @brief  Hides the cursor in the terminal. Makes the game prettier
 *  @return Returns ERROR in case of error and OK if everything goes well */
void hide_cursor();

/** @brief  Moves the cursor to the position (n, m) 
 *  @param  n row
 *  @param  m column 
 *  @return Returns ERROR in case of error and OK if everything goes well*/
status move_cursor(int n, int m);

/** @brief  Moves the cursor n positions up
 *  @param  n number of positions to move
 *  @return Returns ERROR in case of error and OK if everything goes well*/
status cursor_up(int n);

/** @brief  Moves the cursor n positions down
 *  @param  n number of positions to move
 *  @return Returns ERROR in case of error and OK if everything goes well*/
status cursor_down(int n);

/** @brief  Moves the cursor n positions right
 *  @param  n number of positions to move
 *  @return Returns ERROR in case of error and OK if everything goes well*/
status cursor_right(int n);

/** @brief  Moves the cursor n positions left
 *  @param  n number of positions to move
 *  @return Returns ERROR in case of error and OK if everything goes well*/
status cursor_left(int n);

#endif /*_CONTROL*/ 
