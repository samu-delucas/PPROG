/**
 * @file    player.c
 * @author  Samuel de Lucas Maroto
 * 
 * @brief   The same as player.h
 *          This file also includes private functions regarding the creation of threads, 
 *          or other internal things.
 *          The functions without comments are explained in the .h file
 * */

#include "player.h"

#define MAXH 13              /*Maximum number of "steps" for the bars in display_bars. Kind of a "scale"*/
#define STARTINGPOINT 5      /*Determines where the kingdom status (food, money, etc) starts*/
#define MAXXP 10             /*Maximum XP*/

#define WAITING_TIME 400000 /*Time that the changes in the kingdom status are displayed*/

extern pthread_mutex_t prints;  /*Initialized in grafiks.c*/
pthread_mutex_t player_thread_flag_mutex;
int nobars=0; /*Shuts down the threads that wanted to display bars but had no time (cuz the player had moved on to the next quest)*/
short NUMQUESTS;

/**
 *  @struct  player  
 *  @brief   struct that stores the player info. This is, the xp/level of the player and the current
 *           status of the kingdom (food, money, armament, crown). It also stores the previous state, to
 *           show the player what changes have occurred.
 *          
 *           This status is modified each time the player makes a choice, and this is handled by "apply_effect"
 *           This status is displayed each time it changes, and this is handled by "display_bars"
 * 
 *           This structure also stores the language (lan) and if it is the first time that this game is played (noob)
 * */
struct player{
    /*Config file flags*/
    short lan;
    short noob;
    
    /*kingdom attributes*/
    short food;
    short crown;
    short armament;
    short money;
    short prev_food;    /*A lo mejor sería mejor hacer un array de 2 shorts tal que tengas food = [prev_food, current_food], etc*/
    short prev_crown;
    short prev_armament;
    short prev_money;
    
    /*Player attributes*/
    short dead;
};



/*_____INI_PLAYER___________________________________*/

status ini_player(player **p){
    if(!p) return ERROR;
    
    if(!(*p = (player *)calloc(1, sizeof(player)))) return ERROR;
    
    if(read_config(*p) == ERROR){
        free_player(p);
        return ERROR;
    }

    if(pthread_mutex_init(&player_thread_flag_mutex,NULL)!=0){
        free_player(p);
        return ERROR;
    }
    (*p)->dead = 0;
    (*p)->food = STARTINGPOINT;
    (*p)->crown = STARTINGPOINT;
    (*p)->armament = STARTINGPOINT;
    (*p)->money = STARTINGPOINT;
    (*p)->prev_food = STARTINGPOINT;
    (*p)->prev_crown = STARTINGPOINT;
    (*p)->prev_armament = STARTINGPOINT;
    (*p)->prev_money = STARTINGPOINT;
    
    return OK;
}

/*_____FREE_PLAYER___________________________________*/

void free_player(player **p){
    if(!p) return;
    if(*p) free(*p);
    *p = NULL;
    nobars=0;
    pthread_mutex_destroy(&player_thread_flag_mutex);
}

/*_____THREAD_RELATED___________________________________*/

/**
 *  @struct bars_thread  
 *  @brief  custom structure to pass arguments to the pthread_create function
 * */
typedef struct bars_thread_struct{
    player *p;
    section *s;
}bars_thread_struct;

/**
 *  @brief  function to start a timer to let the player see the changes in the kingdom's status, 
 *          and then it clears it
 *  
 * */
void *bars_thread(void *aux){
    int i;
    const int factor = 10;
    nobars++;
    pthread_mutex_lock(&player_thread_flag_mutex);
    nobars--;
    for (i=0;i<WAITING_TIME/factor && nobars==0;i++){
        usleep(factor);
    }
    if (nobars==0){          /*If this thread is already active (or other reasons) we dont want to enter here*/
        if (((bars_thread_struct *) aux)->p != NULL && ((bars_thread_struct *)aux)->s != NULL){
            clear_section(((bars_thread_struct *)aux)->s);
            display_bars(((bars_thread_struct *) aux)->p, ((bars_thread_struct *) aux)->s, 0);
        }
    }
    pthread_mutex_unlock(&player_thread_flag_mutex);
    return NULL;
}

void pause_player_thread(){
    nobars=__INT_MAX__ / 2;
}

/*_____DISPLAY_BARS___________________________________*/

status display_bars(player *p, section *s, short prev){
    int i, r, c, pr, pc, w;
    pts *pt1=NULL, *pt2=NULL;
    kolor *k=NULL;
    char *str = NULL;
    pthread_t thread_id;
    char *tags_es[4] = {"COMIDA", "PUEBLO", "ARMAMENTO", "DINERO"};
    char *tags_en[4] = {"FOOD", "CROWN", "ARMAMENT", "MONEY"};
    char **tags;
    
    
    
    /*Error checking*/
    if(!p || !s) return ERROR;
    
    r = get_section_row(s);
    c = get_section_col(s);
    
    if(!(pt1 = get_section_point(s))) return ERROR;
    pr = get_point_row(pt1);
    pc = get_point_col(pt1);
    free(pt1);
    /*BUBBA STUFF
    pts *p1,*p2;
    create_point(&p1);create_point(&p2);
    set_point(p1,r+pr,c+pc);
    draw_line(pt1,p1,NULL,s,NULL,0);
    getchar();
    END OF BUBBA STUFF*/

    /* Check the number of columns of the section. 
     * 4 must divide the num of cols as we have 4 bars.
     * The -3 is for the spaces between bars */
    if(((c-3)%4)) return ERROR;
    
    
    /*  Allocate a string matching the width of each bar
     *  We'll use it in draw_line */
    w = (c-3)/4;
    if(!(str = (char *)calloc(3*w +1, sizeof(char)))){
        return ERROR;
    }
    for(i = 0; i < w; i++){
        strcat(str, "█");
    }
    

    if(create_point(&pt1) == ERROR){
        free(str);
        return ERROR; 
    }
    if(create_point(&pt2) == ERROR){
        free_point(&pt1);
        free(str);
        return ERROR;
    }
    if(create_kolor(&k) == ERROR) {
        free_point(&pt1);
        free_point(&pt2);
        free(str);
        return ERROR;
    }
    /*Main loop to draw the lines*/
    for(i = 0; i < 4; i++){
        int current, previous, diff;
        int c1 = i*w + pc + i, c2 = i*w + pc + i;
        
        /*First we choose the effect that we want to draw*/
        switch(i){
            case 0:
                current = p->food;
                previous = p->prev_food;
                break;
            case 1:
                current = p->crown;  
                previous = p->prev_crown;
                break;
            case 2:
                current = p->armament;
                previous = p->prev_armament;
                break;
            default:
                current = p->money;
                previous = p->prev_money;
                break;
        }

        /*This gets the current values relative to the actual heigth*/
        current = (int)((current+.0)/(MAXH+.0) * (r-5));
        previous = (int)((previous+.0)/(MAXH+.0) * (r-5)); 
        diff = current - previous; 
        
        /*Printing the name of each bar*/
        if(p->lan == 1){
            tags = tags_es;
        } else {
            tags = tags_en;
        }

        set_kolor(k,DEFAULT_FG,DEFAULT_BG);
        set_point(pt1, r + pr -2, i*w + pc + i + w/2 - strlen(tags[i])/2);     /*This formula just displays the names centered*/
        draw_line(pt1,pt1,tags[i],s,k,0);
        
        /*Set all the points for the lines*/
        if(current != 0){
            if(set_point(pt1, r + pr -4, c1) == ERROR){
                free_point(&pt1);
                free_point(&pt2);
                free_kolor(&k);
                free(str);
                return ERROR;
            }
            
            if(set_point(pt2, r + pr - current -4, c2) == ERROR){
                free_point(&pt1);
                free_point(&pt2);
                free_kolor(&k);
                free(str);
                return ERROR;
            }
            
            /*Set the color for the line*/
            if(set_kolor(k, BARCOLOR, DEFAULT_BG) == ERROR){
                free_point(&pt1);
                free_point(&pt2);
                free_kolor(&k);
                free(str);
                return ERROR;
            }
            
            /*Draw the bar*/
            if(draw_line(pt1, pt2, str, s, k, 0) == ERROR){
                free_point(&pt1);
                free_point(&pt2);
                free_kolor(&k);
                free(str);
                return ERROR;
            }
        }

        /*Loop that handles showing the changes of the kingdom's status*/
        if(prev != 0 && diff != 0){   /*this means: if we want to display the green/red effect && the effect has changed*/
            int r1 = 0, r2 = 0, kol = BARCOLOR;
            
            /*BLINKING OPTION, too flashy*/
            /*printf("%c[5m", 27);
            fflush(stdout);*/
            
            /*We have to see if we have gained something (GREEN) or not (RED)*/
            /*These ecuations ensure that the green/red part is displayed correctly*/
            if(diff > 0){
                kol = GREEN_BRIGHT;
                r1 = r + pr - current -4;
                r2 = r + pr - current + diff -1 -4 + (previous == 0);
            } else if (diff < 0){
                kol = RED_BRIGHT;
                r1 = r + pr - current -1 -4 + (current == 0);
                r2 = r + pr - current + diff -4;
            }
            
            if(set_kolor(k, kol, DEFAULT_BG) == ERROR){
                free_point(&pt1);
                free_point(&pt2);
                free_kolor(&k);
                free(str);
                return ERROR;
            }
            
            /*Setting points again*/
            if(set_point(pt1, r1, c1) == ERROR){
                free_point(&pt1);
                free_point(&pt2);
                free_kolor(&k);
                free(str);
                return ERROR;
            }
            
            if(set_point(pt2, r2, c2) == ERROR){
                free_point(&pt1);
                free_point(&pt2);
                free_kolor(&k);
                free(str);
                return ERROR;
            }

            if(draw_line(pt1, pt2, str, s, k, 0) == ERROR){
                free_point(&pt1);
                free_point(&pt2);
                free_kolor(&k);
                free(str);
                return ERROR;
            }
            
            /*STOP BLINKING*/
            /*printf("%c[0m", 27);
            fflush(stdout);*/
        }
    }
    free_point(&pt1);
    free_point(&pt2);
    free_kolor(&k);
    /*Creating thread so you only see the green/red indicators for 1 sec*/
    if(prev != 0){
        bars_thread_struct *aux;
        if(!(aux = (bars_thread_struct *)malloc(sizeof(bars_thread_struct)))){
            free(str);
            return ERROR;
        }
        aux->p = p;
        aux->s = s;
        if(pthread_create(&thread_id, NULL, bars_thread, aux) != 0){
            free(str);
            return ERROR;
        }
    }
    
    free(str);  
    return OK;
}

/*_____APPLY_EFFECT___________________________________*/

status apply_effect(player *p, effect *eff){
    short i;
    if(!p || !eff) return ERROR;
    
    for(i = 0; i < 4; i++){
        short *current, currentEff, result;
        /*This switch statement is similar to the one used in display bars*/
        switch(i){
            case 0:
                current = &(p->food);
                currentEff = get_eff_food(eff);
                p->prev_food = p->food;
                break;
            case 1:
                current = &(p->crown);            
                currentEff = get_eff_crown(eff);
                p->prev_crown = p->crown;
                break;
            case 2:
                current = &(p->armament);
                currentEff = get_eff_armament(eff);
                p->prev_armament = p->armament;
                break;
            default:/*3*/
                current = &(p->money);
                currentEff = get_eff_money(eff);
                p->prev_money = p->money;
                break;
        }
        
        result = *current + currentEff;
        
        /*Checking if we have reached the top/bottom values*/
        if(result >= MAXH){
            *current = MAXH;
        } else if (result <= 0) {
            *current = 0;
            p->dead = i+1;    /*Save the iteration where the player died*/
        } else {
            *current = result;
        }
    }
    return OK;
}

/*_____GETTERS___________________________________*/

short is_dead(player *p){
    if(!p) return -1;
    return p->dead;
}

short get_player_lan(player *p){
    if(!p) return -1;
    return p->lan;
}

short is_first_time(player *p){
    if(!p) return -1;
    return p->noob;
}

/*_____CONFIG____________________________________*/

/*TODO: Se podrían añadir también niveles de dificultad y guardar el actual nivel en el config file*/

status read_config(player *p){
    FILE *pf;
    char c;
    
    if(!p) return ERROR;
    if(!(pf = fopen("config", "r"))){
        return ERROR;
    }
    
    while(fscanf(pf, "%c", &c) != EOF){
        switch(c){
            case 'L':   /*Language*/
                fscanf(pf, " %hd", &(p->lan));
                break;
            
            case 'N':   /*Number of quests*/
                fscanf(pf, " %hd", &NUMQUESTS);
                break;   
            
            case 'F':   /*Has the game been played?*/
                fscanf(pf, " %hd", &(p->noob));
                break;   
            
            default:    /*Skip the line if it finds a comment or something unknown*/
                fscanf(pf, "%*[^\n]\n");
                break;
        }
    }
    
    fclose(pf);
    return OK;
}

status update_config(short lan, short noob){
    FILE *pf;
    char c;
    
    if(!(pf = fopen("config", "r+"))){
        return ERROR;
    }
    
    while(fscanf(pf, "%c", &c) != EOF){
        switch(c){
            case 'L':
                if(lan >= 0){
                    fseek(pf, 0, SEEK_CUR);
                    fprintf(pf, " %hd", lan);
                    fseek(pf, 0, SEEK_CUR);
                }
                break;
            
            case 'F':
                if(noob >= 0){
                    fseek(pf, 0, SEEK_CUR);
                    fprintf(pf, " %hd", noob);
                    fseek(pf, 0, SEEK_CUR);
                }
                break;
            default:
                fscanf(pf, "%*[^\n]\n");      /*Skip the line*/
                break;
        }
    }
    
    fclose(pf);
    return OK;
}
