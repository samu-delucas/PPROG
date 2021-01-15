/**
 * @file    randomizer.h
 * @author  Samuel de Lucas Maroto
 * 
 * @brief   This module handles the order of the quests. Each quest only appears once.
 *          We are using permutations.c/.h and queue.c/.h in order to make this work 
 *          propperly.
 * */

#include "permutations.h"
#include "quest.h"
#include "types.h"

#ifndef _RANDOMIZER
#define _RANDOMIZER

/** @brief  Gets the next quest to be displayed. 
 *  @param  q pointer to quest. When you exit the function it will have the next quest inside
 *  @param  p pointer to player with relevant info (language for instance)
 *  @return Returns ERROR in case of error and OK if everything goes well
 */
status next_quest(quest *q, player *p);

/** @brief  Pushes a new continuation into the queue. This parameters make more sense
 *          when reading quest.c/.h (read_quest_from_file)
 *  @param  qnum number of the quest
 *  @param  cont number that indicates if this quest is a continuation to another quest (0 = no cont, 1 or 2 = cont option 1 or 2)
 *  @param  numOfCont number of continuations for this quest
 *  @return Returns ERROR in case of error and OK if everything goes well
 */
status push_continuation(int qnum, int cont, int numOfCont);

/** @brief  Frees the global variables used in this module
 */
void free_randomizer();

#endif /*_RANDOMIZER*/ 
 
