#include "character_funcs.h"
#include "personajes.h"
#include <math.h>

/**
 * @brief Función principal del novio. Diálogo y llamadas a otras funciones. 
 * 
 * @param global
 */
void boyfriend(Global *global){
    NPC *boyfriend = global->NPCs[4];
    char c;
    
    /* Descomentar para probar personaje 
    boyfriend->locked = (float) 0.000000;*/

	/* Personaje bloqueado */
    if (ceil(boyfriend->locked) != 0){
        blocked_message(global);
        return;
    }

    /* Personaje bloqueado por paradoja */
    else if (global->paradoja== 1) {
        print_image("aux_files/Art/boyfriend.bmp", 1, 1);
        paradoja();
        wait_for_space(global);
        return;
    }

	/* La primera vez que se habla con él */
    else if (boyfriend->already_talked == 0) {
        print_image("aux_files/Art/boyfriend.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/Boyfriend/boyfriend.txt");
        boyfriend->already_talked = 1;
        c = wait_for_input("12");
        if (c != '1'){   
            print_text(global, "aux_files/Text/English/Boyfriend/boyfriend2.txt");
            wait_for_space(global);
        }
        boyfriend->opcion = c;
    }

	/* Si se vuelve a hablar con él después de la opción 1 */
    else if(boyfriend->opcion == '1'){
        print_image("aux_files/Art/boyfriend.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/Boyfriend/boyfriend3.txt");
        print_text(global, "aux_files/Text/English/Boyfriend/boyfriend2.txt");
        wait_for_space(global);
        boyfriend->opcion = 2;
    }
    
    else{
        print_image("aux_files/Art/boyfriend.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/Boyfriend/boyfriend2.txt");
        wait_for_space(global);
    }

    if(global->clk<12.5*60){
        global->clk=12.5*60;
        global->NPCs[global->npc_object2]->locked = 0;
        global->NPCs[global->npc_object2]->locked = 0;
    }
}