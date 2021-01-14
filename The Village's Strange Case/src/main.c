/************************************************************************/
/*                                                                      */
/* Descripción: Inicio del juego                                        */
/*                                                                      */
/* Archivo: main.c                                                      */
/* Autor: Error 404                                                     */
/*                                                                      */
/************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "basics/basic_funcs.h"
#include "interface/interface.h"
#include "maps/maps.h"
#include "npcs/personajes.h"

void game_end();

int main () {
    Global* global = global_init();
    
    introduction(global);
    
    game_end(global);

	return 0;
}

/**
 * @brief Libera la variable global y los hilos
 */
void game_end(Global* global){
    pthread_mutex_destroy(&mutex);
    pthread_cancel(clock_pth);
    global_end(global);
}