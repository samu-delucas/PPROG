#ifndef NPCS_H
#define NPCS_H

#include <stdio.h>
#include <stdlib.h>
#include "../basics/types.h"
#include "../basics/basic_funcs.h"

struct _NPC {
    char name[512];
    int liar;           /* 1 = sí miente*/
    float locked;       /* 1 = bloqueado */
    int already_talked; /* 1 = ya se ha hablado */
    int solved;         /* 1 = ya se ha resuelto */
    char opcion;        /* A elegir entre 1 y 2 */
    char art[512];
};

typedef void (*minigame_function)(void*);

NPC** npcs_init(Global* global);
void npc_generate_liars(Global* global);

#endif