/************************************************************************/
/*                                                                      */
/* Descripción: Funciones básicas usadas durante todo el juego          */
/*                                                                      */
/* Archivo: basic_funcs.c                                               */
/* Autor: Error 404                                                     */
/*                                                                      */
/************************************************************************/

#include "basic_funcs.h"

/**
 * @brief Cronómetro.
 * 
 * @param global
 */
void *clk(void* global){
    Global* g = (Global*) global;

    while(1){
        g->clk++;
        sleep(1);

        if(g->clk == g->clk_lg_leave+4.5*60) g->NPCs[7]->locked = 0;

        if     (g->clk ==  9*60) g->NPCs[g->npc_object2]->locked = 0;
        else if(g->clk == 12*60) g->NPCs[g->npc_object3]->locked = 0;
        else if(g->clk == 18*60){
            print_string("Beware. Night has fallen.", 267, 195);

            g->NPCs[6]->locked = 0;
            g->NPCs[7]->locked = 0;

            g->NPCs[1]->locked = 1;
            g->NPCs[4]->locked = 1;
            g->NPCs[5]->locked = 1;
            g->NPCs[8]->locked = 1;
            g->NPCs[9]->locked = 1;
            g->NPCs[10]->locked = 1;
        }
    }
}

/**
 * @brief Espera a que el jugador elija un caracter de la cadena dada.
 * 
 * @param c cadena que contiene uno de los caracteres que van a ser elegidos por el jugador 
 * 
 * @return Devuelve el caracter elegido por el jugador o se queda esperando si introduce uno no válido.
 */
char wait_for_input(char *c){
    char c2;
    int len = strlen(c), i;

    fflush(stdout);

    while(1){
        c2 = fgetc(stdin);
        if(c2 == '\033'){
            term_end();
            exit(0);
        }
        for(i=0; i<len; i++)
            if(c[i] == c2) return c2;
    }
}


/**
 * @brief Elige un número aleatorio entre el mínimo y el máximo
 * 
 * @param inf mínimo numero posible
 * @param sup máximo número posible
 * 
 * @return número aleatorio 
 */
int generate_rand(int inf, int sup) {
    int x;
    srand(time(NULL));

    if(sup<inf) return 0;
    if(inf==sup) return inf;

    x = inf + rand()%(sup - inf + 1);
    
    return x;
}


/**
 * @brief Genera una permutación de tamaño N entre los números 0 y N - 1
 * 
 * @param N Tamaño de la permutación
 * 
 */
int* generate_perm(int N){
  int i, *perm, a, aux;

  if(!(perm = (int *) malloc(sizeof(int) * N))) return NULL;

  for(i=0; i < N; i++) perm[i] = i;

  for(i=0; i < N; i++){
    a = generate_rand(i, N - 1);
    aux = perm[i];
    perm[i] = perm[a];
    perm[a] = aux;
  }

  return perm;
}

/**
 * @brief Muestra un mensaje en pantalla y espera a que el jugador pulse el espacio.
 */
void wait_for_space(Global *global) {
    print_text(global, "aux_files/Text/English/press_space.txt");
    wait_for_input(" ");
}


/**
 * @brief Muestra un mensaje en pantalla informando de que el personaje está bloqueado.
 */
void blocked_message(Global *global) {
    print_text(global, "aux_files/Text/English/blocked_message.txt");
    wait_for_input(" ");
}