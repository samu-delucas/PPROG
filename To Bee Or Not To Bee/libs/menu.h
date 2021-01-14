/**
 * @file    menu.h
 * @author  Samuel de Lucas Maroto and Bhavuk Sikka
 * 
 * @brief   This module has all the functions needed for the game menu. Each of them are
 *          called once the user has chosen an option from the menu.
 * */

#ifndef _MENU
#define _MENU
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "control.h"
#include "grafiks.h"
#include "kolors.h"
#include "quest.h"
#include "body.h"
#include "player.h"
#include "musik.h"
#include "randomizer.h"
#include "types.h"
#include "screen.h"
#include "achievement.h"

/** @brief  Function that starts the game. This is only called if the
 *          user chooses to play in the initial menu
 *  @param  pl player struct that has been initialiced in the main program.
 *             Has information such as the language to be used.
 */
int play_game(player *pl);

/** @brief  Function that displays the instructions. This is only called if the
 *          user chooses the option how to play in the initial menu
 *  @param  lan language to be used
 */
int how_to(int lan);

/** @brief  Function that displays the instructions. This is only called if the
 *          user chooses the option settings in the initial menu
 *  @param  lan language to be used
 */
int settings(int lan);

/** @brief  Function that displays the instructions. This is only called if the
 *          user chooses the option achievments in the initial menu
 *  @param  lan language to be used
 */
int achievments(int lan);
#endif /*_MENU*/
 
  
 
