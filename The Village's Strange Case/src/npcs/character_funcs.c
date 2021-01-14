/************************************************************************/
/*                                                                      */
/* Descripción: Funciones relacionadas con los personajes               */
/*                                                                      */
/* Archivo: character_funcs.c                                           */
/* Autor: Error 404                                                     */
/*                                                                      */
/************************************************************************/

#include "character_funcs.h"
#include "personajes.h"

#include "mayor.c"
#include "fortuneteller.c"
#include "werewolf.c"
#include "littlegirl.c"
#include "witch.c"
#include "bestfriend.c"
#include "boyfriend.c"
#include "fool.c"
#include "madame.c"
#include "peasant.c"
#include "priest.c"

/**
 * @brief Tras con la adivina/lobo se desbloquean los siguientes personajes:
 *        -La bruja
 *        -El lobo/adivina, el que no se desbloqueara al hablar con el alcalde
 *        -El primer personaje participante en el 'minijuego de 3'
 * 
 * @param global
 */
void wsw_unblock(Global *global) {
    NPC *witch = global->NPCs[3];
    NPC *fortuneteller =global->NPCs[2];
    NPC *werewolf = global->NPCs[1];
    NPC *object1 = global->NPCs[global->npc_object1];

    if (witch->locked != 0) witch->locked = 0;
    if (fortuneteller->locked != 0) fortuneteller->locked = 0;
    if (werewolf->locked != 0) werewolf->locked = 0;
    if (object1->locked != 0) object1->locked = 0;
}


/**
 * @brief Tras hablar con: la bruja, el primer personaje del 'minijuego de 3' y adivina/lobo, se debloquean el tonto y el novio.
 * 
 * @param global
 * @param resta bruja resta 0,34, los otros dos restan 0,33
 */
void unblock2(Global *global, float resta) {
    NPC *fool = global->NPCs[5];
    NPC *boyfriend = global->NPCs[4];
    NPC *littlegirl = global->NPCs[7];

    if (fool->locked != 0) fool->locked -= resta;
    if (boyfriend->locked != 0) boyfriend->locked -= resta;
}


/**
 * @brief Si no se ha resuelto correctamente el minijuego de la bruja, se activa el bloqueo por paradoja en todos los personajes.
 */
void paradoja() {
    FILE *f = fopen("aux_files/Text/English/paradojas.txt", "r");
    char s[100], buf[10][100];
    int i;

    /* Imprime una frase aleatoria del archivo paradojas.txt */ 
    for (i=0; fgets(s, 100, f) != NULL; i++)
        strcpy(buf[i], s);

    /* i marca el índice de la frase a imprimir */
    i = generate_rand(0, i - 1);
    print_string(buf[i], 50, 300);
    
    fclose(f);
}
