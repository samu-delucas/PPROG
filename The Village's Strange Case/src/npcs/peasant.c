#include "character_funcs.h"
#include "personajes.h"

void minijuego2(Global *global);

/**
 * @brief Función principal del campesino. Diálogo y llamadas a otras funciones. 
 * 
 * @param global
 */
void peasant(Global *global){
    NPC *peasant = global->NPCs[10];
    char c;

    /* Descomentar para probar personaje 
    peasant->locked = 0; */
    
	/* Personaje bloqueado */
    if (peasant->locked != 0){
        blocked_message(global);
    }
	
	/* Personaje bloqueado por paradoja */
	else if (global->paradoja == 1){
        print_image("aux_files/Art/peasant.bmp", 1, 1);
        paradoja();
        wait_for_space(global);
    }

	/* Primera vez que se habla con el campesino */
    else if (peasant->already_talked == 0) {

		peasant->already_talked = 1;

        print_image("aux_files/Art/peasant.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/Peasant/peasant.txt");

		c = wait_for_input("12");
        peasant->opcion = c;
        
        if(c !='1') minijuego2(global);
    }

	/* Vuelve a hablar con él después de haber escogido la opción 1 */
    else if(peasant->opcion == '1') {
    	print_image("aux_files/Art/peasant.bmp", 1, 1);
    	print_text(global, "aux_files/Text/English/Peasant/peasant4.txt");
        minijuego2(global);
        peasant->opcion = '2';
    }

    else {
		print_image("aux_files/Art/peasant.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/Peasant/peasant5.txt");
    }
}

/**
 * @brief La parte del 'minijuego de 3' relacionada con el campesino.
 * 
 * @param global
 */
void minijuego2(Global *global) {
		
		/* Primero de priest/BFF/peasant en desbloquearse */
		if(global->npc_object1 == 10) {
            print_text(global, "aux_files/Text/English/Peasant/peasant1.txt");
            unblock2(global, 0.34);
        }

		/* Segundo */
        else if(global->npc_object2 == 10)
            print_text(global, "aux_files/Text/English/Peasant/peasant2.txt");
        
		/* Tercero */
        else
            print_text(global, "aux_files/Text/English/Peasant/peasant3.txt");
}
