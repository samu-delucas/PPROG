/************************************************************************/
/*                                                                      */
/* Descripción: Funciones relacionadas con moverse por los escenarios   */
/*                                                                      */
/* Archivo: maps.c                                                      */
/* Autor: Error 404                                                     */
/*                                                                      */
/************************************************************************/

#include "maps.h"
#include "../npcs/personajes.h"
#include "../npcs/character_funcs.h"

void sun(Global *global){
    int row = 0, column = 0;
    if     (global->clk < 60*1*4.5) print_image("aux_files/Art/sun.bmp", 50, 430);
    else if(global->clk < 60*2*4.5) print_image("aux_files/Art/sun.bmp", 40, 300);
    else if(global->clk < 60*3*4.5) print_image("aux_files/Art/sun.bmp", 45, 180);
    else if(global->clk < 60*4*4.5) print_image("aux_files/Art/sun.bmp", 60,  70);
}

/**
 * @brief Función que imprime en pantalla la introducción del juego.
 * 
 * @param Variable global
 */
void introduction(Global *global) {
    print_image("aux_files/Art/full_blank.bmp", 1, 1);
    print_intro("aux_files/Text/English/introduction.txt");
    pthread_create(&clock_pth, NULL, clk, (void *) global);
    town_hall(global);
}

void print_text_townhall(Global *global){
    print_string("Where do you want to go?", 289, 50);

    if(global->NPCs[global->random_house_order[0]]->already_talked==0 && (global->random_house_order[0]!=6 || global->NPCs[4]->opcion!='2')) print_string(" 1. House 1", 297, 50);
    else print_string(global->random_house_names[0], 297, 50);

    if(global->NPCs[global->random_house_order[1]]->already_talked==0 && (global->random_house_order[1]!=6 || global->NPCs[4]->opcion!='2')) print_string(" 2. House 2", 305, 50);
    else print_string(global->random_house_names[1], 305, 50);

    if(global->NPCs[global->random_house_order[2]]->already_talked==0 && (global->random_house_order[2]!=6 || global->NPCs[4]->opcion!='2')) print_string(" 3. House 3", 313, 50);
    else print_string(global->random_house_names[2], 313, 50);

    print_string(" 4. Town Hall", 297, 310);

    if(global->NPCs[global->random_house_order[3]]->already_talked==0 && (global->random_house_order[3]!=6 || global->NPCs[4]->opcion!='2')) print_string(" 5. House 5", 305, 310);
    else print_string(global->random_house_names[3], 305, 310);

    print_string(" 6. Next street", 313, 310);
}


/**
 * @brief Función para elegir entre las opciones del Town Hall.
 * 
 * @param Variable global
 */
void town_hall(Global *global) {
    char c = '\0';
    int i=0, j=0, k=0;

    while (1){
        print_image("aux_files/Art/town_hall.bmp", 1, 1);
        sun(global);

        print_text_townhall(global);
        
        c = wait_for_input("123456");
        switch (c){
        case '1':
            (global->random_houses[0])(global);
            break;
        case '2':
            (global->random_houses[1])(global);
            break;
        case '3':
            (global->random_houses[2])(global);
            break;
        case '4':
            mayor(global);
            break;
        case '5':
            (global->random_houses[3])(global);
            break;
        case '6':
            alley(global);
            break;
        }

        if(global->juego != 0) return;
    }
}

void print_text_alley(Global *global){
    int j = 0;
    if (global->NPCs[7]->locked==0) j=8;

    print_string("Where do you want to go?", 289, 50);

    if(global->NPCs[7]->locked==0) print_string(" 5. Talk to the little girl", 297, 50);

    if(global->NPCs[global->random_house_order[4]]->already_talked==0 && (global->random_house_order[4]!=6 || global->NPCs[4]->opcion!='2')) print_string(" 6. House 6", 297+j, 50);
    else print_string(global->random_house_names[4], 297+j, 50);

    if(global->NPCs[global->random_house_order[5]]->already_talked==0 && (global->random_house_order[5]!=6 || global->NPCs[4]->opcion!='2')) print_string(" 7. House 7", 305+j, 50);
    else print_string(global->random_house_names[5], 305+j, 50);

    print_string(" 8. Next street", 297, 310);

    print_string(" 9. Back to main square", 305, 310);
}

/**
 * @brief Función para elegir entre las opciones del alley.
 * 
 * @param Variable global
 */
void alley(Global *global) {
    char c = '\0';

    while (c != '9') {
        if(global->NPCs[7]->locked == 0) print_image("aux_files/Art/alley.bmp", 1, 1);
        else print_image("aux_files/Art/alley2.bmp", 1, 1);
        sun(global);

        print_text_alley(global);

        c = wait_for_input("56789");
        switch (c) {
        case '5':
            littlegirl(global);
            break;
        case '6':
            (global->random_houses[4])(global);
            break;
        case '7':
            (global->random_houses[5])(global);
            break;
        case '8':
            church(global);
            break;
        case '9':      /* Volver a town hall */
            break;
        }
    }
}

void print_text_church(Global *global){
    print_string("Where do you want to go?", 289, 50);

    print_string(" 1. Church", 297, 50);

    print_string(" 2. Talk to the man", 305, 50);

	print_string(" 3. Go into the forest", 313, 50);

    print_string(" 4. Sit down on the bench", 297, 310);

    print_string(" 5. Back to the alley", 305, 310);
}

/**
 * @brief Función para elegir entre las opciones de la iglesia.
 * 
 * @param Variable global
 */
void church(Global *global) {
    char c = '\0';
    
    while (c != '5'){
        print_image("aux_files/Art/church.bmp", 1, 1);
        sun(global);
        
        print_text_church(global);

        c = wait_for_input("12345");
        switch (c) {
        case '1':
            priest(global);
            break;
        case '2':
            fool(global);
            break;
        case '3':
            werewolf(global);
            break;
        case '4':
            bench(global);
            break;
        case '5':     /* Volver a alley */
            break;
        }
    }
}

void bench(Global *global){
    char opcion;

    print_image ("aux_files/Art/blank_text.bmp", 289, 50);
    
    print_string(" Warning, in this bench, time flies!>What do you want to do?", 289, 50);
    print_string(" 1. Wait just a while.", 305, 50);
    print_string(" 2. Wait until night. Beware. Some go to bed early", 313, 50);
	print_string(" 3. Don't sit down.", 305, 310);

    opcion = wait_for_input("123");

    /*Si elige pasar de ciclo solar*/
    if(opcion == '1'){
        global->NPCs[7]->locked = 0;

        if      (global->clk < 60*1*4.5) global->clk = 60*1*4.5;
        else if (global->clk < 60*2*4.5) global->clk = 60*2*4.5;
        else if (global->clk < 60*3*4.5){
            global->clk = 60*3*4.5;
            global->NPCs[global->npc_object3]->locked = 0;
        }
        else if (global->clk < 60*4*4.5) global->clk = 60*4*4.5;
    }
    
    /*Si elige esperar a la noche*/
    else if(opcion == '2'){
        global->NPCs[7]->locked = 0;
        if(global->clk < 60*4*4.5) global->clk = 60*4*4.5;
    }
}