/**
 * @file    screen.h
 * @author  Bhavuk Sikka and Samuel de Lucas Maroto
 * 
 * @brief   This module has all the functions related to specific screens that are displayed during
 *          the game (i.e. loading screen, menu screen, game over screen, etc)
 * */

#ifndef _SCREEN
#define _SCREEN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "types.h"
#include "kolors.h"
#include "control.h"
#include "grafiks.h"
#include "musik.h"

/** @brief  Draws the loading screen. This function is called to wait while the music loads
 *  @param  l Bool value that tells us if the music has loaded or not
 */
void *loading_screen(void *l);

/** @brief  Draws the main menu in the selected language
 *  @param  lan language
 *  @param  click Sound when choosing different options
 *  @return -1 in case of error or the user choice otherwise 
 */
int display_menu(int lan, musik *click);

/** @brief  Draws the game over screen. It prints a different message depending on the death cause
 *  @param  n integer that represents different deaths (see is_dead() in player.h for more information)
 *  @param  lang Appropriate language in which text will be displayed
 *  @param  score The score to be displayed
 */
void game_over(int n, int lang, int score);

#endif /*_SCREEN*/
 
 
