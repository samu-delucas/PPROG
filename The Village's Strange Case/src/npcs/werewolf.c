#include "character_funcs.h"
#include "personajes.h"

/**
 * @brief Función principal del lobo. Diálogo y llamadas a otras funciones. 
 * 
 * @param global
 */
void werewolf(Global *global){
    NPC *werewolf = global->NPCs[1];
    char c = '0';

    /* Desbloquear para hacer pruebas con el personaje 
    werewolf->locked = 0; */

    /* Personaje bloqueado */
    if (werewolf->locked != 0){
		print_image("aux_files/Art/blank_text.bmp", 289, 50);
        print_string(" You've found a hut at a clearing,>but nobody's there.", 289, 50);
        wait_for_input(" ");
    }

    /* Bloqueo por paradoja */
    else if (global->paradoja == 1) {
        print_image("aux_files/Art/werewolf.bmp", 1, 1);
        paradoja();
        wait_for_space(global);
    }

    /* Si aún no se ha hablado con él */
    else if (werewolf->already_talked == 0) {
        werewolf->already_talked = 1;
        
        wsw_unblock(global);
        print_image("aux_files/Art/werewolf.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/Werewolf/werewolf.txt");
        c = wait_for_input("12");
        werewolf->opcion= c;
        if (c != '1'){
            werewolf->already_talked= 1;
            if(global->first==2) unblock2(global, 0.33);
            if(werewolf->liar==1) print_text(global, "aux_files/Text/English/Werewolf/werewolf3.txt");
            else print_text(global, "aux_files/Text/English/Werewolf/werewolf2.txt");
            wait_for_space(global);
        }
    }
    /* Si se vuelve a hablar con el personaje después de haber elegido la opción 1 */
    else if(werewolf->opcion== '1'){
    	print_image("aux_files/Art/werewolf.bmp", 1, 1);
        if(werewolf->liar==1) print_text(global, "aux_files/Text/English/Werewolf/werewolf3.txt");
        else print_text(global, "aux_files/Text/English/Werewolf/werewolf2.txt");
        wait_for_space(global);

        if(global->first==2) unblock2(global, 0.33);
        
        /* Ya se le ha dado la información */
        werewolf->opcion='2';
    }
    /* Si se vuelve a hablar con el personaje después de haber elegido la opción 2 */
    else{
        print_image("aux_files/Art/werewolf.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/Werewolf/werewolf4.txt");
        wait_for_space(global);
    }
}
