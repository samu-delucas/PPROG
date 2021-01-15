/**
 * @file    quest.h
 * @author  Samuel de Lucas Maroto
 * 
 * @brief   This module handles the basic functions that are related to the
 *          creation and deletion of the quests. Also provides the user with
 *          getters for the quest information.
 * */


#ifndef _QUEST
#define _QUEST
#include "types.h"

/*______MACROS____________________________________________*/

/*Length of different fields in the quest struct*/
#define NAMELEN 50
#define DESCLEN 500
#define OPTLEN 100

/*______STRUCTS____________________________________________*/

typedef struct quest quest;
typedef struct effect effect;

/*______FUNCTIONS____________________________________________*/

/**
 * @brief   Reserves memory for a query and initialices its values with NO NAME, NO DESCRIPTION, etc.
 * @param   q double pointer to the query to be initialiced
 * @see     free_quest()
 * @return  OK if everything went well or ERROR otherwise
 * */
status ini_quest (quest **q);

/**
 * @brief   Frees the memory reserved in ini_quest
 * @param   q Address of pointer to the query to be freed
 * */
void free_quest (quest **q);

/**
 * @brief   Reserves memory for a query and initialices its values with NO NAME, NO DESCRIPTION, etc.
 * @param   q double pointer to the query to be initialiced
 * @see     free_quest()
 * @return  OK if everything went well or ERROR otherwise
 * */
status ini_effect (effect **e);

/**
 * @brief   Frees the memory reserved in ini_quest
 * @param   q Address of pointer to the query to be freed
 * */
void free_effect (effect **e);

/**
 * @brief   Reads a quest from a file with especific format
 *          The format is the following:
 *                  
 *              quests/lan/n.txt -> new quest with number <n>, of languaje <lan>
 *              quests/lan/n_ma.txt -> continuation for option <m> of quest with number <n>, of languaje <lan>.
 *                                     Letter <a> represents different continuations for the same option.
 * 
 *          For example, if the player chooses option 1 in quest 9.txt, a new quest 
 *          will show up (not necessarily right after this one). 
 *          It can be 9_1a.txt, 9_1b.txt, 9_1c.txt, etc
 * 
 *          If the player chooses option 2, the continuations will be 9_2a.txt, and so on
 * 
 * 
 * @param   qnum quest number
 * @param   lan language for the query; 1 = spanish, 0 = english
 * @param   q pointer to the quest in wich the info will be stored
 * @param   cont number that indicates if this quest is a continuation to another quest (0 = no cont, 1 or 2 = cont option 1 or 2)
 * @param   numOfCont number that indicates how many continuations there are (this is indicated by get_quest_ongoing)
 * @return  OK if everything went well or ERROR otherwise
 * */
status read_quest_from_file (int qnum, int cont, int numOfCont, short lan, quest* q);

/**
 * @brief   Getter for quest number
 * @param   q quest to be read
 * @return  returns the quest number
 * */
int get_quest_num(quest *q);

/**
 * @brief   Getter for quest character name
 * @param   q quest to be read
 * @return  returns the quest character. Must be freed
 * */
char* get_quest_char(quest *q);

/**
 * @brief   Getter for quest description
 * @param   q quest to be read
 * @return  returns the quest description. Must be freed
 * */
char*   get_quest_desc(quest *q);

/**
 * @brief   Getter for quest options
 * @param   q quest to be read
 * @param   opt 1 or 2 depending on which quest you want to read
 * @return  returns the selected quest option. Must be freed
 * */
char* get_quest_opt(quest *q, short opt);

/**
 * @brief   Getter for quest effects
 * @param   q quest to be read
 * @param   n 1 for effect 1, 2 for effect 2
 * @return  returns the selected quest effect for the selected option. Must be freed
 * */
effect* get_quest_effect(quest *q, int n);

/**
 * @brief   Getter for quest effects (particular effect)
 * @param   q quest to be read
 * @param   opt 1 or 2 depending on which quest you want to select
 * @param   eff 'F' for food, 'C' for crown, 'A' for armstice, 'M' for money, 'O' for ongoing
 * @return  returns the selected quest effect for the selected option
 * */
short get_quest_eff(quest *q, short opt, char eff);

/**
 * @brief   Getter for the users choice on this quest
 * @param   q quest to be read
 * @return  returns the choice
 * */
short get_quest_choice(quest *q);

/**
 * @brief   Getter for the effect food
 * @param   eff effect to be read
 * @return  returns the effect
 * */
short get_eff_food(effect *eff);

/**
 * @brief   Getter for the effect food
 * @param   eff effect to be read
 * @return  returns the effect
 * */
short get_eff_crown(effect *eff);

/**
 * @brief   Getter for the effect food
 * @param   eff effect to be read
 * @return  returns the effect
 * */
short get_eff_armament(effect *eff);

/**
 * @brief   Getter for the effect food
 * @param   eff effect to be read
 * @return  returns the effect
 * */
short get_eff_money(effect *eff);

/**
 * @brief   Getter for the effect food
 * @param   eff effect to be read
 * @return  returns the effect
 * */
short get_eff_ongoing(effect *eff);

/**
 * @brief   Copies the values of src into dest
 * @param   dest destination quest. Can't be NULL
 * @param   src source quest. Can't be NULL
 * @return  OK if everything went well or ERROR otherwise
 * */
status copy_quest(quest *dest, quest *src);

/**
 * @brief   Copies the values of src into dest
 * @param   dest destination effect. Can't be NULL
 * @param   src source effect. Can't be NULL
 * @return  OK if everything went well or ERROR otherwise
 * */
status copy_effect(effect *dest, effect *src);

#endif /*_QUEST*/
