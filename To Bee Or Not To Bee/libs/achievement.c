/**
 * @file    achievement.c
 * @author  Jose Manuel López-Serrano
 * 
 * @brief   This module handles reading, updating and reseting the achievements
 * */

/*___LIBS USED_____________________________________________*/

#include "achievement.h"
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include <string.h>
#include <math.h>


/*______STRUCTS____________________________________________*/

struct achievement{
    char title[30];
    char description[300];
    int counter;
    int total;
    short completed; /*0 if not completed, 1 if completed*/
};

/*_______PRIVATE FUNCTIONS__________________________________*/

/**
 * @brief generates an achievement's text file
 * @param n references a specific achievement text file
 * @param lan references the language of the achievement
 * @see
 * @return a string containing the name of the text file
 * */
char *create_a_filename(short lan, int n){
    char *str = NULL;
    char lang[3];
    /*{achievements/en/} + {log10(n+1) + 1} + {.txt} + {\0}
     (the logarithm gets the number of digits that n has)*/
    int len = 16 + log10(n+1)+1 + 4 + 1;
    
    if(n < 0) return NULL;
    
    if(!(str = (char *)calloc(len, sizeof(char)))){
        return NULL;
    }
    
    switch(lan){
        case 1:
            strcpy(lang, "es");
            break;
        default:
            strcpy(lang, "en");
            break;
    }

    sprintf(str, "achievements/%s/%d.txt", lang, n);

    return str;
}   

/*__________________________________________________________________*/

void free_achievement (achievement **a){
    if(!a) return;
    if(*a){
        free(*a);
    }
    *a = NULL;
}

/*__________________________________________________________________*/

status ini_achievement (achievement **a){
    if(!a) return ERROR;
    *a = (achievement*)malloc(sizeof(achievement));
    if(!(*a)) return ERROR;

    strcpy((*a)->title, "NO TITLE");
    strcpy((*a)->description, "NO DESCRIPTION");
    (*a)->counter = -1;
    (*a)->total = -1;
    (*a)->completed = 0;

    return OK;
}

/*__________________________________________________________________*/

status read_achievement_from_file (short lan, int n, achievement *a){
    FILE *pf = NULL;
    char* filename;
    char c;
    if (!a || n>MAX_N){
        return ERROR;
    }

    filename = create_a_filename(lan, n);
    if(!filename) return ERROR;

    if(!(pf = fopen(filename, "r"))){
        free(filename);
        return ERROR;
    }

    while (fscanf (pf, "%c", &c) != EOF){
        switch (c){
            case 'T':
                fscanf(pf, " %[^\n]", a->title);
                break;
            case 'D':
                fscanf(pf, " %[^\n]", a->description);
                break;
            case 'C':
                fscanf(pf, " %d\n", &(a->counter));
                break;
            case 'Z':
                fscanf(pf, " %d\n", &(a->total));
                break;
        }

    }
    if(set_a_completed(a) == ERROR) return ERROR;

    fclose(pf);
    free(filename);
    return OK;
}

status write_achievement_to_file(short lan, int n, achievement *a){
    FILE *pf = NULL;
    char* filename;
    if (!a || n>MAX_N){
        return ERROR;
    }

    filename = create_a_filename(lan, n);
    if(!filename) return ERROR;

    if(!(pf = fopen(filename, "w+"))){
        free(filename);
        return ERROR;
    }

    fprintf(pf, "T %s\nD %s\nC %d\nZ %d", a->title, a->description, a->counter, a->total);

    free(filename);
    fclose(pf);
    return OK;
}

/*__________________________________________________________________*/

status read_all_achievements (short lan, achievement ***a){
    int i;
    if(!a) return ERROR;

    *a = (achievement**)calloc(MAX_N, sizeof(achievement));
    for(i=0; i<MAX_N; i++){
        if(ini_achievement(&((*a)[i])) == ERROR) return ERROR;
        if(read_achievement_from_file(lan, i+1, (*a)[i]) == ERROR) return ERROR;
    }
    
    return OK;
}

void end_all_achievements (achievement **a){
    int i;
    for(i=0; i<MAX_N; i++){
        free_achievement(&(a[i]));
    }
    free(a);
}

/*__________________________________________________________________*/

status bump_up_counter(int n){
    achievement *a;
    if(n>MAX_N) return ERROR;
    if(ini_achievement(&a) == ERROR) return ERROR;

    /*1: Spanish*/
    if(read_achievement_from_file(1, n, a) == ERROR) return ERROR;
    if(a->counter == a->total){
        free_achievement(&a);
        return OK;
    }
    (a->counter)++;
    if(write_achievement_to_file(1, n, a) == ERROR) return ERROR;

    /*2: English*/
    if(read_achievement_from_file(2, n, a) == ERROR) return ERROR;
    (a->counter)++;
    if(write_achievement_to_file(2, n, a) == ERROR) return ERROR;

    free_achievement(&a);
    return OK;
}

status change_counter(int n, int new_counter){
    achievement *a;
    if(n>MAX_N) return ERROR;
    if(ini_achievement(&a) == ERROR) return ERROR;

    /*1: Spanish*/
    if(read_achievement_from_file(1, n, a) == ERROR) return ERROR;
    if(new_counter<0 || new_counter>(a->total)) return ERROR;
    (a->counter) = new_counter;
    if(write_achievement_to_file(1, n, a) == ERROR) return ERROR;

    /*2: English*/
    if(read_achievement_from_file(2, n, a) == ERROR) return ERROR;
    if(new_counter<0 || new_counter>(a->total)) return ERROR;
    (a->counter) = new_counter;
    if(write_achievement_to_file(2, n, a) == ERROR) return ERROR;

    free_achievement(&a);
    return OK;
}

status reset_all_achievements(){
    int i;
    for(i=1; i<=MAX_N; i++){
        if(change_counter(i, 0) == ERROR) return ERROR;
    }
    return OK;
}

/*__________________________________________________________________*/

char* get_a_title (achievement *a){
    if(!a) return NULL;
    return a->title;
}

char* get_a_description (achievement *a){
    if(!a) return NULL;
    return a->description;
}

int get_a_counter (achievement *a){
    if(!a) return -1;
    return a->counter;
}

int get_a_total (achievement *a){
    if(!a) return -1;
    return a->total;
}

short get_a_completed (achievement *a){
    if(!a) return -1;
    return a->completed;
}

status set_a_completed (achievement *a){
    if(!a) return ERROR;
    if(get_a_counter(a) == get_a_total(a)){
        a->completed = 1;
    }else{
        a->completed = 0;   
    }
    
    return OK;
}
