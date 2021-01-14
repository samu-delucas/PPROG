#include "character_funcs.h"
#include "personajes.h"
#include <math.h>

void solucion(Global*global);

/**
 * @brief Función principal del tonto. Diálogo y llamadas a otras funciones. 
*/
void fool(Global *global){
    NPC *fool = global->NPCs[5];
    char c;

	/* Desbloquear para hacer pruebas con el personaje
    fool->locked = 0; */
 
	/* Personaje bloqueado */
    if (ceil(fool->locked) != 0){
        print_image("aux_files/Art/fool.bmp", 1, 1);
        print_string(" Hey water ya doing>down there?!>Come later, I'm busy!", 50, 300);
        wait_for_space(global);
    }

    /* Personaje bloqueado por paradoja */
    else if (global->paradoja == 1) {
        print_image("aux_files/Art/Fool/fool.bmp", 1, 1);
        paradoja();
        wait_for_space(global);
    }

    /* Si se habla con él por primera vez */
    else if (fool->already_talked == 0) {
    	fool->already_talked = 1;
    	
    	print_image("aux_files/Art/fool.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/Fool/fool.txt");
        
        c = wait_for_input("12");

        fool->opcion= c;
        if(c != '1') solucion(global);

    }
    
    /* Vuelve a hablar con él después de haber escogido la opción 1 */
    else if (fool->opcion == '1') 
    	solucion(global);
	
    else {
    	print_image("aux_files/Art/fool.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/Fool/fool3.txt");
    }

}

/**
 * @brief Solución al minijuego de la niña. 
 * 
 * @param global
 */
void solucion(Global *global) {
	NPC *littlegirl = global->NPCs[7];
    NPC *madame = global->NPCs[6];
    
    print_image("aux_files/Art/fool.bmp", 1, 1);
    print_text(global, "aux_files/Text/English/Fool/fool4.txt");
    print_string(global->littlegirl_solution, 60, 310);
    wait_for_space(global);
    
    /* Se bloquea la niña tras hablar con el tonto */
    littlegirl->locked = 1;
}
