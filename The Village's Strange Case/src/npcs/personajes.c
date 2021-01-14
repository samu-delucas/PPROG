/************************************************************************/
/*                                                                      */
/* Descripción: Funciones relacionadas con la estructura del personaje  */
/*                                                                      */
/* Archivo: personajes.c                                                */
/* Autor: Error 404                                                     */
/*                                                                      */
/************************************************************************/

#include "personajes.h"
#include "character_funcs.c"
#include "../basics/basic_funcs.h"

/**
 * @brief Inicializa los personajes y sus estructuras.
 */
NPC** npcs_init(Global* global) {
    NPC **characters;
    int i;

    characters=(NPC **) malloc(sizeof(NPC *) * 11);
        
    for(i=0;i<11;i++){
        characters[i]=(NPC*) malloc (sizeof(NPC));
        characters[i]->locked = 1;
        characters[i]->already_talked = 0;
        characters[i]->solved = 0;
        characters[i]->opcion = '0';
        characters[i]->liar = 0;
    }

    characters[0]->locked = 0;    /* El alcalde siempre se desbloqueará para dar la introducción */
    characters[7]->locked = 0;    /* La niña siempre se desbloqueará al principio */

    strcpy(characters[0]->name, "Mayor");
    strcpy(characters[1]->name, "Werewolf");
    strcpy(characters[2]->name, "Fortune Teller");
    strcpy(characters[3]->name, "Witch");
    strcpy(characters[4]->name, "Boyfriend");
    strcpy(characters[5]->name, "Fool");
    strcpy(characters[6]->name, "Madame");
    strcpy(characters[7]->name, "Little Girl");	
    strcpy(characters[8]->name, "Priest");
    strcpy(characters[9]->name, "Best Friend");
    strcpy(characters[10]->name, "Peasant");

    strcpy(characters[0]->art, "aux_files/Art/mayor.bmp");
    strcpy(characters[1]->art, "aux_files/Art/werewolf.bmp");
    strcpy(characters[2]->art, "aux_files/Art/fortuneteller.bmp");
    strcpy(characters[3]->art, "aux_files/Art/witch.bmp");
    strcpy(characters[4]->art, "aux_files/Art/boyfriend.bmp");
    strcpy(characters[5]->art, "aux_files/Art/fool.bmp");
    strcpy(characters[6]->art, "aux_files/Art/madame.bmp");
    strcpy(characters[7]->art, "aux_files/Art/littlegirl.bmp");
    strcpy(characters[8]->art, "aux_files/Art/priest.bmp");
    strcpy(characters[9]->art, "aux_files/Art/bestfriend.bmp");
    strcpy(characters[10]->art, "aux_files/Art/peasant.bmp");

    return characters;
}

/**
 * @brief Genera de forma aleatoria los personajes mentirosos.
 */
void npc_generate_liars(Global* global) {
    
    /* Miente uno entre lobo y adivina */
    if(global->assassin==1)
        global->NPCs[1]->liar=1;
    else
        global->NPCs[generate_rand(1, 2)]->liar = 1;

    /* Mienten dos entre peasant, priest y bestie */
    global->NPCs[8]->liar = global->NPCs[9]->liar = global->NPCs[10]->liar = 1;
    global->NPCs[generate_rand(8, 10)]->liar = 0;
}

