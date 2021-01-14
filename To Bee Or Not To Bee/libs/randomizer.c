/**
 * @file    randomizer.c
 * @author  Samuel de Lucas Maroto
 * 
 * @brief   This module handles the order of the quests. Each quest only appears once.
 *          We are using permutations.c/.h and queue.c/.h in order to make this work 
 *          propperly.
 * */

#include <stdio.h>
#include <stdlib.h>
#include "quest.h"
#include "queue.h"
#include "permutations.h"
#include "player.h"

#define MIN_WT 1        /*WT means Waiting Time (between a quest and its continuation)*/
#define MAX_WT 2

/*____GLOBAL VARS_________________________________________*/ 

int *order = NULL;
Queue *continuations = NULL;
int current = 0;        /*current index of the quest*/
int waiting_time = 0;   /*number of iterations until a continuation can be extracted*/
int prev_wt = 0;
extern short NUMQUESTS;

/*____CUSTOM_FUNCTIONS_NEEDED_FOR_QUEUE.H___________________________*/

void element_free (void *a){
    if((int *)a){
        free((int *)a);
    }
}

void *element_copy (const void *a){
    int *cpy;
    if(!a) return NULL;
    
    if(!(cpy = (int *) calloc(3, sizeof(int)))) return NULL;
    
    cpy[0] = ((int *)a)[0];
    cpy[1] = ((int *)a)[1];
    cpy[2] = ((int *)a)[2];
    return cpy;
}

int element_print(FILE *pf, const void *a){
    if(!pf || !a) return -1;
    
    return fprintf(pf, "quest num: %d; choice: %d", ((int *)a)[0], ((int *)a)[1]);
}

/*____PRIVATE FUNCTIONS___________________________________*/

/** @brief  private function that handles how much time is left 
 *          to show another continuation (each continuation must
 *          be separated from its "parent" at least by MIN_WT and 
 *          at most by MAX_WT quests)
 */
void update_wt(){
    int aux = prev_wt;
    waiting_time = random_num(MIN_WT, MAX_WT);
    prev_wt = waiting_time - 1;
    waiting_time -= aux;
    if(waiting_time < 0) waiting_time = 0;
} 

/** @brief  private function that orders every quest at the start
 *          of the game. Uses NUMQUESTS (config file)
 */
status order_quests(){
    if(!order){
        order = generate_perm(NUMQUESTS);
        if(!order) return ERROR;
        /*current = 0;*/
    }
    return OK;
}

/** @brief  initializes the queue where the continuations will be stored
 */
status ini_continuations(){
    if(!continuations){
        continuations = queue_init(element_free, element_copy, element_print);
        if(!continuations) return ERROR;
    }
    return OK;
}

/** @brief  frees the queue
 */
void free_continuations(Queue **q){
    if(*q){
        queue_free(*q);
        *q = NULL;
    }
}

/*____MAIN FUNCTIONS______________________________________*/

status next_quest(quest *q, player *p){
    int *cont;
    short lan;

    /*Error checking*/
    if(!q || !p) return ERROR;
    
    if(!order){
        if (order_quests() == ERROR) return ERROR;
    }
    if(!continuations){
        if (ini_continuations() == ERROR) return ERROR;
    }
    
    lan = get_player_lan(p);
    
    if(is_first_time(p) == 0)    /*First quest of the game (intro quest)*/
    {  
        if(read_quest_from_file(0, 0, 0, lan, q) == ERROR){
            return ERROR;
        }
        /*We update the config file so the intro quest is no longer showed (even if you play again)*/
        update_config(lan, 1);
        read_config(p);
    }
    else if((queue_isEmpty(continuations) == TRUE || waiting_time != 0) && (current < NUMQUESTS))/*If there are no continuations or it isn't time for one*/
    {    
        if(read_quest_from_file(order[current], 0, 0, lan, q) == ERROR) return ERROR;
        current = (current + 1) % NUMQUESTS;
        if(waiting_time > 0) waiting_time--;
    }
    else /**/
    {
        cont = (int *) queue_extract(continuations);
        if(!cont) return ERROR;
        
        if(read_quest_from_file(cont[0], cont[1], cont[2], lan, q) == ERROR){
            free(cont);
            return ERROR;
        }
        if(queue_isEmpty(continuations) == FALSE){
            update_wt();
        }
        
        free(cont);
    }
    return OK;
}

status push_continuation(int qnum, int cont, int numOfCont){
    int *element = NULL;
    
    if(numOfCont < 1 || cont == 0) return ERROR;
    
    if(!(element = (int *) calloc(3, sizeof(int)))) return ERROR;
    
    element[0] = qnum;
    element[1] = cont;
    element[2] = numOfCont;
    if(queue_isEmpty(continuations) == TRUE) update_wt();    
    return queue_insert(continuations, element);
}

void free_randomizer(){
    current = 0;
    waiting_time = 0;
    prev_wt = 0;
    free_continuations(&continuations);
    free(order);
    order=NULL;
}
