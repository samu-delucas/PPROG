#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>
#include <stdio.h>
#include "../npcs/personajes.h"

typedef void (*character_func)(Global *);

struct _Global{
    NPC** NPCs;
    int clk;
    int clk_lg_leave;
    int paradoja;
    int juego;              /* 0 = Sigues jugando, 1 = Has ganado, 2 = Has perdido */
    char brothel[2];        /* Contiene el número de la casa del burdel */
    char littlegirl_solution[13];

    char set_11 [20];
    char set_12 [20];
    char set_13 [20];
    char set_14 [20];
    char set_2  [20];
    char set_lg1[20];
    char set_lg2[20];
    char object1[20];   /*/7*/
    char object2[20];   /*/8. Also, murder weapon*/
    char object3[20];   /*/9*/

    int assassin;

    int first;          /* 1 = lobo, 2 = adivina */
    int npc_object1;    /* Primer personaje del 'minijuego de 3' */
    int npc_object2;    /* Segundo personaje del 'minijuego de 3' */
    int npc_object3;    /* Tercer personaje del 'minijuego de 3' */

    character_func random_houses[6];
    char random_house_names[6][32];
    int random_house_order[6];
};

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t clock_pth;
static pthread_t animation_pth;
static pthread_t scape_pth;

Global* global_init();
void set_littlegirl_word(Global *global);
void set_npcs_object(Global *global);
void generate_assassin(Global *global);
void generate_sets(Global *g);
void random_houses(Global *global);
void global_end(Global *global);
char* global_get_set(Global* global, char value);


#endif