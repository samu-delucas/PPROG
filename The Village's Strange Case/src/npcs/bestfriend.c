#include "character_funcs.h"
#include "personajes.h"

void minijuego3(Global *global);

/**
 * @brief Función principal de la mejor amiga. Diálogo y llamadas a otras funciones. 
 * 
 * @param global
 */
void bestfriend(Global *global){
    NPC *bestfriend = global->NPCs[9];
    char c;

	/* Descomentar para probar personaje 
    bestfriend->locked = 0; */

	/* Personaje bloqueado */
	if (bestfriend->locked != 0){ 
        blocked_message(global);
    }

	/* Personaje bloqueado por paradoja */
    else if (global->paradoja == 1){
        print_image("aux_files/Art/bestfriend.bmp", 1, 1);
        paradoja();
        wait_for_space(global);
    }
    
	/* Primera vez que se habla con la mejor amiga */
    else if (bestfriend->already_talked == 0) {

		bestfriend->already_talked = 1;

        print_image("aux_files/Art/bestfriend.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/BestFriend/bestfriend.txt");

		/* Primera parte del texto que resuelve el 'minijuego de 3' de la mejor amiga */
		if(global->npc_object1==9)
            print_text(global, "aux_files/Text/English/BestFriend/bestfriend1.txt");
        else if(global->npc_object2==9)
            print_text(global, "aux_files/Text/English/BestFriend/bestfriend2.txt");
        else
            print_text(global, "aux_files/Text/English/BestFriend/bestfriend3.txt");
        
		c = wait_for_input("12");
        bestfriend->opcion = c;
        
        if(c !='1') minijuego3(global);
    }

	/* Vuelve a hablar con él después de haber escogido la opción 1 */
    else if(bestfriend->opcion == '1') {
    	print_image("aux_files/Art/bestfriend.bmp", 1, 1);
    	print_text(global, "aux_files/Text/English/BestFriend/bestfriend4.txt");
        minijuego3(global);
        bestfriend->opcion = '2';
    }

    else {
        print_image("aux_files/Art/bestfriend.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/BestFriend/bestfriend5.txt");
    }
}

/**
 * @brief La parte del 'minijuego de 3' relacionada con la mejor amiga.
 * 
 * @param global
 */
void minijuego3(Global *global) {
		
		/* Primero de priest/BFF/peasant en desbloquearse */
		if(global->npc_object1==9){
            unblock2(global, 0.34);
            print_text(global, "aux_files/Text/English/BestFriend/bestfriend6.txt");
        }

		/* Segundo */
        else if(global->npc_object2==9)
            print_text(global, "aux_files/Text/English/BestFriend/bestfriend7.txt");
        
		/* Tercero */
        else
            print_text(global, "aux_files/Text/English/BestFriend/bestfriend8.txt");
}
