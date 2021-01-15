/**
 * @file    player.h
 * @author  Samuel de Lucas Maroto
 * 
 * @brief   This module handles the current status of the game (or the status of the player), i.e. storing 
 *          this status, displaying it in the form of vertical bars, applying all the different effects that 
 *          each quest produces (gaining/losing money, food, etc.)       
 * */

#ifndef _PLAYER
#define _PLAYER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "quest.h"
#include "grafiks.h"
#include "control.h"
#include "kolors.h"
#include "types.h"

/**
 * @brief   struct that stores the player info. This is, the xp/level of the player and the current
 *          status of the kingdom (food, money, armament, crown). It also stores the previous state, to
 *          show the player what changes have occurred.
 *          
 *          This status is modified each time the player makes a choice, and this is handled by "apply_effect"
 *          This status is displayed each time it changes, and this is handled by "display_bars"
 */
typedef struct player player;

/**
 * @brief   Reserves memory for a player and initialices its values.
 *              -level/xp are initialiced to 0
 *              -all the status are initialiced to the midpoint
 *                      (actually this value could change, it is indicated by the macro STARTINGPOINT in player.c) 
 * 
 * @param   p double pointer to the player to be initialiced
 * @see     free_player()
 * @return  OK if everything went well or ERROR otherwise
 * */
status ini_player(player **p);

/**
 * @brief   Frees the allocated memory for the player
 * @param   p pointer to the player to be freed
 * */
void free_player(player **p);

/**
 * @brief   Displays the current status of the kingdom and, optionally, the changes that have occurred
 *          in green if you have gained something, or red otherwise
 *          
 *          It should look like this (more or less)
 *           _____________________
 *          |                     |
 *          | XP:[############  ] |
 *          |                     |
 *          |                     |
 *          |                     |
 *          |                     |
 *          |                     |
 *          | ####                |
 *          | ####                |
 *          | ####                |
 *          | ####                |
 *          | ####      ####      |
 *          | ####      ####      |
 *          | #### #### ####      |
 *          | #### #### #### #### |
 *          | #### #### #### #### |
 *          | #### #### #### #### |
 *          | #### #### #### #### |
 *          | #### #### #### #### |
 *           _____________________
 * 
 * @param   p pointer to the player that we want to display the bars
 * @param   s section in which we want to display the bars
 * @param   prev flag that indicates if we want to display the change between the previous effect and the current one (0 = no; otherwise = yes)
 * @return  OK if everything went well or ERROR otherwise
 * */
status display_bars(player *p, section *s, short prev);

/**
 * @brief   It applies the effects of eff to the player p
 * @param   p pointer to the player
 * @param   eff pointer to the effect
 * @return  ERROR in case of error or OK otherwise
 */
status apply_effect(player *p, effect *eff);

/**
 * @brief   Function to know if the player is still alive. 
 *          Useful for knowing when to get out of the main loop in the main program.
 *          It also returns a different value depending on the death cause.
 * @param   p pointer to the player
 * @return  -1 in case of error; 0 if it is alive; OTHER if it is dead (1 == low on food, 2 == low on crown, 3 == low on armament, 4 == low on money)
 */
short is_dead(player *p);

/**
 * @brief   Getter for the player language
 * @param   p pointer to the player
 * @return  -1 in case of error; language value otherwise (1 = es, 0 = en)
 */
short get_player_lan(player *p);

/**
 * @brief   Functionto know if the player plays the game for the first time
 * @param   p pointer to the player
 * @return  -1 in case of error; 0 if the fame hasn't been played; 1 otherwise
 */
short is_first_time(player *p);

/**
 * @brief   This function reads the config file and stores the values in the provided player struct
 * @param   p pointer to the player
 * @return  ERROR in case of error or OK otherwise
 */
status read_config(player *p);

/**
 * @brief   This function updates the config file using the values provided. If these values are negative,
 *          their value in the config file remains unchanged. If you want the changes to be applied to the
 *          current game you must call read_config() after this function.
 * @param   lan language
 * @param   noob represents if the player has already played or it is the first time
 * @return  ERROR in case of error or OK otherwise
 */
status update_config(short lan, short noob);

/*This is just a function to avoid some problems with threads*/
void pause_player_thread();

#endif /*_PLAYER*/
 
