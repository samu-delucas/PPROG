/**
 * @file    achievement.h
 * @author  Jose Manuel López-Serrano
 * 
 * @brief   This module handles all achievments
 * */

#ifndef _ACHIEVEMENT
#define _ACHIEVEMENT

#include "types.h"
#include <math.h>

#define MAX_N 16    /*Number of achievements*/

typedef struct achievement achievement; 

/**
 * @brief frees all memory allocation
 * @param a pointer to the achievement
 * */
void free_achievement (achievement **a);

/**
 * @brief initialise an achievement
 * @param a double pointer to the empty structure
 * @see
 * @return OK if everything went well or ERROR otherwise
 * */
status ini_achievement (achievement **a);

/**
 * @brief reads an achievement from a text file and sorts it into its structure
 * @param n references a specific achievement text file
 * @param lan references the language of the achievement
 * @param a pointer to the empty structure
 * @see
 * @return OK if everything went well or ERROR otherwise
 * */
status read_achievement_from_file (short lan, int n, achievement *a);

/**
 * @brief translates a specific structure into a text file (with the pre-established format)
 * @param n references a specific achievement text file (the one that will be overwritten)
 * @param lan references the language of the achievement
 * @param a pointer to the empty structure
 * @see
 * @return OK if everything went well or ERROR otherwise
 * */
status write_achievement_to_file(short lan, int n, achievement *a);

/**
 * @brief stores every achievement in a double pointer, ready to be shown
 * @param lan references the language of the achievement
 * @param max_n references the name of the last achievement
 * @param a triple pointer to the achievement structure
 * @see
 * @return OK if everything goes as planned, ERROR if otherwise
 * */
status read_all_achievements (short lan, achievement ***a);

/**
 * @brief frees every achievement from a double pointer passed as argument
 * @param max_n references the number of the last achievement
 * @param a double pointer to the achievement structure
 * @see
 * */
void end_all_achievements (achievement **a);

/**
 * @brief bumps up by 1 the counter of a specific achievement
 * @param n references the text file to be changed
 * @returns OK if everything went as planned, and ERROR otherwise
 * */
status bump_up_counter(int n);

/**
 * @brief changes the counter of a specific achievement
 * @param n references the text file to be changed
 * @param new_counter the counter must be changed into this parameter
 * @returns OK if everything went as planned, and ERROR otherwise
 * */
status change_counter(int n, int new_counter);

/**
 * @brief resets the counter of all achievements to 0
 * @param max_n references the last text file achievement to be changed
 * @see change_counter
 * @returns OK if everything went as planned, and ERROR otherwise
 * */
status reset_all_achievements();

/**
 * @brief gets the title of an achievement
 * @param a pointer to the achievement
 * @returns the title if everything went as planned, and NULL otherwise
 * */
char* get_a_title (achievement *a);

/**
 * @brief gets the description of an achievement
 * @param a pointer to the achievement
 * @returns the description if everything went as planned, and NULL otherwise
 * */
char* get_a_description (achievement *a);

/**
 * @brief gets the counter of an achievement
 * @param a pointer to the achievement
 * @returns the counter if everything went as planned, and -1 otherwise
 * */
int get_a_counter (achievement *a);

/**
 * @brief gets the total of an achievement
 * @param a pointer to the achievement
 * @returns the total if everything went as planned, and -1 otherwise
 * */
int get_a_total (achievement *a);

/**
 * @brief gets the completed of an achievement
 * @param a pointer to the achievement
 * @returns the completed if everything went as planned, and -1 otherwise
 * */
short get_a_completed (achievement *a);

/**
 * @brief sets the completed parameter from an achievement (would only be 1 if
 * the counter is equal or higher than the total)
 * @param a pointer to the achievement
 * @returns OK if everything went as planned, and ERROR otherwise
 * */
status set_a_completed (achievement *a);

#endif /*_ACHIEVEMENT*/
