/************************************************************************/
/*                                                                      */
/* Descripción: Funciones relacionadas con la variable global           */
/*                                                                      */
/* Archivo: global.c                                                    */
/* Autor: Error 404                                                     */
/*                                                                      */
/************************************************************************/

#include "global.h"
#include "../npcs/character_funcs.h"


/**
 * @brief Inicializa la estructura Global y sus variables.
 */
Global* global_init(){
    Global* global;
    int i, j;

    term_init();
    
    if(!(global = (Global*) malloc(sizeof(Global)))) return NULL;
    global->NPCs = npcs_init(global);
    global->paradoja = 0;
    global->clk = 0;
    global->first = -1;
    global->juego = 0;

    set_littlegirl_word(global);
    set_npcs_object(global);

    generate_assassin(global);
    npc_generate_liars(global);
    generate_sets(global);

    random_houses(global);

    return global;
}


/**
 * @brief Libera la estructura Global.
 */
void global_end(Global *global){
    int i;

    for(i = 0; i < 11; i++)
        free(global->NPCs[i]);

    free(global->NPCs);
    free(global);
    term_end();

    global = NULL;
}

/**
 * @brief De la lista de posibles palabras en el archivo littlegirl_options elige una al azar y la guarda.
 * 
 * @param global
 */
void set_littlegirl_word(Global *global){
    int rand=generate_rand(0,11), i;
    FILE *f = fopen("aux_files/Text/English/Kid/littlegirl_options.txt", "r");
    
    for(i = 0; fscanf(f, "%s", global->littlegirl_solution) != EOF  &&  i<rand; i++);

    fclose(f);
}

/**
 * @brief Carga al azar el orden de aparición de los tres personajes del 'minijuego de 3'.
 * 
 * @param global
 */
void set_npcs_object(Global *global) {
    int i, j;
    i = generate_rand(0, 2);
    j = generate_rand(1, 2);

    global->npc_object1 = 8 + i % 3;
    global->npc_object2 = 8 + (i + 1 + (j%2 == 0)) % 3;
    global->npc_object3 = 8 + (i + 1 + (j%2 == 1)) % 3;
}

/**
 * @brief Retorna distintas variables de global dependiendo de lo indicado con value
 * 
 * @param value
 */
char* global_get_set(Global* global, char value) {
    if(value == '0') return global->set_11;
    if(value == '1') return global->set_12;
    if(value == '2') return global->set_13;
    if(value == '3') return global->set_14;
    if(value == '4') return global->set_2;
    if(value == '5') return global->set_lg1;
    if(value == '6') return global->set_lg2;
    if(value == '7') return global->object1;
    if(value == '8') return global->object2;
    if(value == '9') return global->object3;
    if(value == 'a') return global->NPCs[global->npc_object1]->name;
    if(value == 'b') return global->NPCs[global,global->npc_object2]->name;
    if(value == 'c') return global->NPCs[global,global->npc_object3]->name;
    if(value == 'd') return global->brothel;
}

/**
 * @brief Asocia de manera aleatoria los personajes posibles con las viviendas.
 * 
 * @param global
 */
void random_houses(Global *global) {
    int *i = NULL;
    int j, aux, k;

    i = generate_perm(6);
    j = generate_rand(0,3);
    aux = i[0];
    i[0] = j;
    for(k = 1; k<6; k++)
        if(i[k]==j) i[k] = aux;
    
    global->random_houses[i[0]] = fortuneteller; 
    sprintf(global->random_house_names[i[0]], " %d. Fortune Teller's House", i[0] + 1 + (i[0] >= 3));                     
    global->random_house_order[i[0]] = 2;  

    global->random_houses[i[1]] = witch;
    sprintf(global->random_house_names[i[1]], " %d. Witch's House", i[1] + 1 + (i[1] >= 3));                 
    global->random_house_order[i[1]] = 3;  
    
    global->random_houses[i[2]] = boyfriend;
    sprintf(global->random_house_names[i[2]], " %d. Boyfriend's House", i[2] + 1 + (i[2] >= 3));         
    global->random_house_order[i[2]] = 4; 

    global->random_houses[i[3]] = bestfriend;
    sprintf(global->random_house_names[i[3]], " %d. Bestfriend's House", i[3] + 1 + (i[3] >= 3));               
    global->random_house_order[i[3]] = 9; 
    
    global->random_houses[i[4]] = peasant;
    sprintf(global->random_house_names[i[4]], " %d. Peasant's House", i[4] + 1 + (i[4] >= 3));             
    global->random_house_order[i[4]] = 10; 

    global->random_houses[i[5]] = madame;
    sprintf(global->random_house_names[i[5]], " %d. Brothel", i[5] + 1 + (i[5] >= 3));       
    global->random_house_order[i[5]] = 6;
    
    /* Guardamos el número de la casa del burdel */
    global->brothel[0] = 48 + i[5] + 1 + (i[5] >= 3);
    global->brothel[1] = '\0';

    free(i);

}

/**
 * @brief Genera un asesino de forma aleatoria y lo guarda en global.
 * 
 * @param global
 */
void generate_assassin(Global *global) {
    do {
        global->assassin = generate_rand(0,10);
    } while(global->assassin == 2 || global->assassin == 6);
    
}

/**
 * @brief Genera los conjuntos de objetos.
 * 
 * @param global
 * 
 */
void generate_sets(Global *g) {
    FILE *f = fopen("aux_files/Text/English/objects.txt", "r");
    
    FILE *fp = fopen("assassin.txt","w");

    char aux[9][10];
    int i;
    int *perm = NULL;
    
    /*HATS*/
    /*character who is not a liar out of werewolf/fortuneteller*/
    if(g->assassin == 0 || g->assassin == 3 || g->assassin == 10) {
        strcpy(g->set_11, "Mayor");
        strcpy(g->set_12, "Witch");
        strcpy(g->set_13, "Peasant");
        strcpy(g->set_14, "Madame");
    }

    /*PENDANTS*/
    else if(g->assassin == 1 || g->assassin == 9 || g->assassin == 8) {
        g->NPCs[1]->liar = 1;
        g->NPCs[2]->liar = 0;
        strcpy(g->set_11, "Werewolf");
        strcpy(g->set_12, "Best Friend");
        strcpy(g->set_13, "Priest");
        strcpy(g->set_14, "Madame");
    }
    /* FLOWER */
    else if(g->assassin == 4 || g->assassin == 7 || g->assassin == 5) {
        strcpy(g->set_11, "Fool");
        strcpy(g->set_12, "Boyfriend");
        strcpy(g->set_13, "Little Girl");
        strcpy(g->set_14, "Madame");
    }
    
    /* Liar out of werewolf/fortuneteller */
    switch (g->assassin) {
        case 0:
            strcpy(g->set_2, "Witch");
            break;
        case 1:
            strcpy(g->set_2, "Best Friend");
            break;
        case 3:
            strcpy(g->set_2, "Peasant");
            break;
        case 4:
            strcpy(g->set_2, "Little Girl");
            break;
        case 5:
            strcpy(g->set_2, "Boyfriend");
            break;
        case 7:
            strcpy(g->set_2, "Fool");
            break;
        case 8:
            strcpy(g->set_2, "Best Friend");
            break;
        case 9:
            strcpy(g->set_2, "Priest");
            break;
        case 10:
            strcpy(g->set_2, "Mayor");
            break;
    }

    /* Little girl set */
    if(g->assassin == 7) {
        strcpy(g->set_lg1, "Boyfriend");
        g->NPCs[7]->liar = 1;
    }
    else {
        g->NPCs[7]->liar = 0;
        strcpy(g->set_lg1, "Fortune Teller");
        strcpy(g->set_lg2, g->NPCs[g->assassin]->name);
    }

    /* OBJETOS */
    for(i = 0; i < 9; i++){
        fscanf(f, "%s", aux[i]);
        aux[i][9] = '\0';
    }
    perm = generate_perm(9);

    strcpy(g->object1, aux[perm[0]]);
    strcpy(g->object2, aux[perm[1]]);
    strcpy(g->object3, aux[perm[2]]);

    fprintf(fp, "this is the assassin: %d, and the object: %s",  g->assassin, g->object2);
    fclose(fp);
    free(perm);
    fclose(f);
}
