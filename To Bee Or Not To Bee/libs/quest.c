/**
 * @file    quest.c
 * @author  Samuel de Lucas Maroto
 * 
 * @brief   This module handles the basic functions that are related to the
 *          creation and deletion of the quests. Also provides the user with
 *          getters for the quest information.
 * */

/*_______LIBS USED_________________________________________*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "permutations.h"
#include "types.h"
#include "quest.h"
#include "grafiks.h"

/*______STRUCTS____________________________________________*/

/** 
 * @typedef effect
 * @brief   stores the effects that each quest produces.
 *              > FOOD: pretty simple, tells you how much food you have.
 *              > CROWN: represents what do people think about your actions.
 *              > ARMAMENT: this one represents how well you can defend your kingdom
 *                          in case of an attack, and also your relation with other countries.
 *              > MONEY: another simple one.
 *              
 *              > ONGOING: this one isn't an effect, but it is needed for other functions
 * @see     get_quest_eff()
 * */
struct effect{
    short food;
    short crown;
    short armament;
    short money;
    short ongoing; /*status that clarifies whether the choice entails an ongoing quest or if
                    it's a one-time thing*/
};

/** 
 * @struct  quest
 * @brief   stores the quest information
 *              > NUMBER: specific number of the quest. Identifies the file in the quests/ folder
 *              > CHARACTER: Name of the character that is talking at that moment
 *              > DESCRIPTION: Description of the quest. 
 *              > OPT1 / OPT2: The answers that the queen can give
 *              > EFF1 / EFF2: The effect that each quest produces
 *              > CHOICE: player's choice. 0: not defined, 1: option 1, 2: option 2
 * */
struct quest{
    int number;
    char character[NAMELEN];
    char description[DESCLEN];
    char opt1[OPTLEN];
    char opt2[OPTLEN];
    effect *eff1;
    effect *eff2;
    short choice;
};

/*_______PRIVATE FUNCTIONS__________________________________*/

/**
 * @brief   Private function that creates a filename for the quests (used in read_quest_from_file)
 *          Also handles continuations.
 *          It works like this:
 *                  
 *              quests/lan/n.txt -> new quest with number <n>, of languaje <lan>
 *              quests/lan/n_ma.txt -> continuation for option <m> of quest with number <n>, of languaje <lan>.
 *                                     Letter <a> represents different continuations for the same option. This letter is chosen randomly. 
 *                                     There can be as many continuations as numOfCont indicates (argument of the function)
 *          
 *          Examples can be found in quest.h, read_quest_from_file()
 * 
 * @param   lan identifies the language code to be used. 1 = spanish (es); default = english (en)
 * @param   n identifies the query number.
 * @param   cont number that indicates if this quest is a continuation to another quest (0 = no cont, 1 or 2 = cont option 1 or 2)
 * @param   numOfCont flag that indicates if we need to generate a random option or not (0 = no, otherwise = yes). Only useful when you have a continuation. If not 0 it indicates how many continuations are for that option;
 * @return  string with the quest's path
 * */
char *create_filename(short lan, int n, int cont, int numOfCont){
    char *str = NULL;
    int randnum;
    char rnd = 'a';
    char lang[3];
    /*{quests/en/} + {log10(n+1) + 1} + {.txt} + {\0}
     (the logarithm gets the number of digits that n has)*/
    int len = 10 + (int)log10(n+1)+1 + 4 + 1;
    
    if(n < 0) return NULL;
    
    if(cont == 1 || cont == 2){
        /*{_} + {log10(cont+1) + 1} + {a}*/
        len += 1 + (int)log10(cont+1)+1 + 1;
        if(numOfCont != 0){
            randnum = random_num(1, numOfCont);
            rnd = 'a' + randnum - 1;
        }
    }
    
    if(!(str = (char *)calloc(len, sizeof(char)))){
        return NULL;
    }
    
    switch(lan){    /*More languages could be added here*/
        case 1:
            strcpy(lang, "es");
            break;
        default:
            strcpy(lang, "en");
            break;
    }
    if(cont == 1 || cont == 2){
        sprintf(str, "quests/%s/%d_%d%c.txt", lang, n, cont, rnd);
    } else{
        sprintf(str, "quests/%s/%d.txt", lang, n);
    }

    return str;
}   

/**
 * @brief   Private function that initialices to 0 all values for an effect
 * @param   e effect to be initialiced
 * */
status ini_effect (effect **e){
    if(!e) return ERROR;
    if(!(*e=(effect*)malloc(sizeof(effect)))) return ERROR;
    
    
    (*e)->food = 0;
    (*e)->crown = 0;
    (*e)->armament = 0;
    (*e)->money = 0;
    (*e)->ongoing = 0;
    
    return OK;
}

void free_effect (effect** e){
    if(!e) return;
    if(*e){
        free(*e);
    }
    *e = NULL;
}

/*______INI_QUEST_____________________________________________*/

status ini_quest (quest **q){
    if(!q) return ERROR;
	if(!(*q=(quest*)malloc(sizeof(quest)))) return ERROR;

    (*q)->number = -1;
    strcpy((*q)->character, "NO NAME");
    strcpy((*q)->description, "NO DESCRIPTION");
    strcpy((*q)->opt1, "NO OPTION 1");
    strcpy((*q)->opt2, "NO OPTION 2");
    (*q)->choice = 0;

    if(ini_effect(&((*q)->eff1)) == ERROR || ini_effect(&((*q)->eff2)) == ERROR){
        free_quest(q);
        return ERROR;
    }

    return OK;
}

/*______FREE_QUEST_____________________________________________*/

void free_quest (quest** q){
    if(!q) return;
    if(*q){
        if((*q)->eff1) free_effect(&(*q)->eff1);
        if((*q)->eff2) free_effect(&(*q)->eff2);
        free(*q);
        *q=NULL;
    }
}

/*______READ_QUEST_FROM_FILE___________________________________________*/

status read_quest_from_file (int qnum, int cont, int numOfCont, short lan, quest* q){
    FILE *pf;
    char c;
    char *filename = NULL;
    short flag;
    if(!q || qnum < 0){
        return ERROR;
    }

    q->number = qnum;
    filename = create_filename(lan, qnum, cont, numOfCont);
    if(!(pf = fopen(filename, "r"))){
        return ERROR;
    }
    while(fscanf(pf, "%c", &c) != EOF){
        switch(c){
            case 'P':
                fscanf(pf, " %[^\n]", (q->character));
                break;
            case 'Q':
                fscanf(pf, " %[^\n]", (q->description));
                break;
            case '1':
                fscanf(pf, " %[^\n]", (q->opt1));
                break;
            case 'E':
                flag = 1;
                while(flag){
                    fscanf(pf, "%c", &c);
                    switch(c){
                        case 'F':
                            fscanf(pf, "%hi ", &(q->eff1->food));
                            break;
                        case 'C':
                            fscanf(pf, "%hi ", &(q->eff1->crown));
                            break;
                        case 'A':
                            fscanf(pf, "%hi ", &(q->eff1->armament));
                            break;
                        case 'M':
                            fscanf(pf, "%hi ", &(q->eff1->money));
                            break;
                        case 'Q':
                            fscanf(pf, "%hi ", &(q->eff1->ongoing));
                            flag = 0;
                            break;
                        default:
                            break;
                    }
                }
                break;
            case '2':
                fscanf(pf, " %[^\n]", (q->opt2));
                break;
            case 'F':
                flag = 1;
                while(flag){
                    fscanf(pf, "%c", &c);
                    switch(c){
                        case 'F':
                            fscanf(pf, "%hi ", &(q->eff2->food));
                            break;
                        case 'C':
                            fscanf(pf, "%hi ", &(q->eff2->crown));
                            break;
                        case 'A':
                            fscanf(pf, "%hi ", &(q->eff2->armament));
                            break;
                        case 'M':
                            fscanf(pf, "%hi ", &(q->eff2->money));
                            break;
                        case 'Q':
                            fscanf(pf, "%hi ", &(q->eff2->ongoing));
                            flag = 0;
                            break;
                        default:
                            break;
                    }
                }
                break;
            default:
                break;
        }
    }

    fclose(pf);
    free(filename);
    return OK;
}

/*____GETTERS______________________________________________*/

int get_quest_num(quest *q){
    return q->number;
}

char* get_quest_char(quest *q){
    char *str = NULL;
    
    if(!(str = (char *)calloc(NAMELEN, sizeof(char)))) return NULL;
    strcpy(str, q->character);
    return str;
}

char* get_quest_desc(quest *q){
    char *str = NULL;
    
    if(!(str = (char *)calloc(DESCLEN, sizeof(char)))) return NULL;
    strcpy(str, q->description);
    return str;
}

char* get_quest_opt(quest *q, short opt){
    char *str = NULL;
    if(!(str = (char *)calloc(OPTLEN, sizeof(char)))) return NULL;
    
    switch(opt){
        case 1:
            strcpy(str, q->opt1);
            break;
        case 2:
            strcpy(str, q->opt2);
            break;
        default:
            return NULL;
            break;
    }
    return str;
}

effect* get_quest_effect(quest *q, int n){
    effect *eff;
    if(!q || (n!=1 && n!=2)) return NULL;
    if(ini_effect(&eff) == ERROR) return NULL;

    eff->food = get_quest_eff(q, n,'F');
    eff->crown = get_quest_eff(q, n,'C');
    eff->armament = get_quest_eff(q, n,'A');
    eff->money = get_quest_eff(q, n,'M');
    eff->ongoing = get_quest_eff(q, n,'O');
    
    return eff;
    
}

short get_quest_eff(quest *q, short opt, char eff){
    switch(opt){
        case 1:
            switch(eff){
                case 'F':
                    return q->eff1->food;
                    break;
                case 'C':
                    return q->eff1->crown;
                    break;
                case 'A':
                    return q->eff1->armament;
                    break;
                case 'M':
                    return q->eff1->money;
                    break;
                case 'O':
                    return q->eff1->ongoing;
                    break;
                default:
                    return 0;
                    break;
            }
            break;
        case 2:
            switch(eff){
                case 'F':
                    return q->eff2->food;
                    break;
                case 'C':
                    return q->eff2->crown;
                    break;
                case 'A':
                    return q->eff2->armament;
                    break;
                case 'M':
                    return q->eff2->money;
                    break;
                case 'O':
                    return q->eff2->ongoing;
                    break;
                default:
                    return 0;
                    break;
            }
            break;
        default:
            return 0;
            break;
    }
    
}

short get_quest_choice(quest *q){
    return q->choice;
}

short get_eff_food(effect *eff){
    return eff->food;
}

short get_eff_crown(effect *eff){
    return eff->crown;
}

short get_eff_armament(effect *eff){
    return eff->armament;
}

short get_eff_money(effect *eff){
    return eff->money;
}

short get_eff_ongoing(effect *eff){
    return eff->ongoing;
}

/*________COPY FUNCTIONS____________________________________________*/

status copy_quest(quest *dest, quest *src){    
    if(!src || !dest) return ERROR;
    
    dest->number = src->number;
    strcpy(dest->character, src->character);                /*We should use getters and setters but there are no setters soooo...*/
    strcpy(dest->description, src->description);
    strcpy(dest->opt1, src->opt1);
    strcpy(dest->opt2, src->opt2);
    if(copy_effect(dest->eff1, src->eff1) == ERROR) return ERROR;
    if(copy_effect(dest->eff2, src->eff2) == ERROR) return ERROR;
    dest->choice = src->choice;
    
    return OK;
}

status copy_effect(effect *dest, effect *src){      /*Probably redundant since we have get_quest_effect*/
    if(!src || !dest) return ERROR;
    
    dest->food = src->food;
    dest->crown = src->crown;
    dest->armament = src->armament;
    dest->money = src->money;
    dest->ongoing = src->ongoing;
    
    return OK;
}
