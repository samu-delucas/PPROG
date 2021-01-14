#include "character_funcs.h"
#include "personajes.h"

void minijuego(Global *global);

void* priest_animation(){
    while(1){
        print_image("aux_files/Art/baston1.bmp", 143, 65);
        usleep(500000);
        print_image("aux_files/Art/baston2.bmp", 144, 65);
        usleep(500000);
        print_image("aux_files/Art/baston3.bmp", 143, 65);
        usleep(500000);
        print_image("aux_files/Art/baston2.bmp", 144, 65);
        usleep(500000);
    }
}

void priest_draw(){
    print_image("aux_files/Art/priest.bmp", 1, 1);
    pthread_create(&animation_pth, NULL, priest_animation, NULL);
}


/**
 * @brief Función principal del cura. Diálogo y llamadas a otras funciones. 
 * 
 * @param global
 */
void priest(Global *global) {
    NPC *priest = global->NPCs[8];
    char c;

	/* Desbloquear para hacer pruebas con el personaje*/
    priest->locked = 0;
    
    if (priest->locked != 0) {
		print_image("aux_files/Art/blank_text.bmp", 289, 50);
        print_string(" It seems that the priest is deep in prayers.>Come back later.", 289, 50);
        wait_for_input(" ");
        return;
    }
    
    priest_draw();

    /* Personaje bloqueado por paradoja */
    if (global->paradoja == 1) {
        paradoja();
        wait_for_space(global);
    }

	/* Primera vez que se habla con el cura */
    else if (priest->already_talked == 0) {
		priest->already_talked = 1;

        print_text(global, "aux_files/Text/English/Priest/priest.txt");
        
        c = wait_for_input("12");
        priest->opcion = c;
        
        if(c !='1') minijuego(global);
    }

	/* Vuelve a hablar con él después de haber escogido la opción 1 */
    else if(priest->opcion == '1') {
    	print_text(global, "aux_files/Text/English/Priest/priest4.txt");
        minijuego(global);
        priest->opcion = '2';
    }

    else print_text(global, "aux_files/Text/English/Priest/priest5.txt");

    wait_for_space(global);
    pthread_cancel(animation_pth);
}

/**
 * @brief La parte del 'minijuego de 3' relacionada con el cura.
 * 
 * @param global
 */
void minijuego(Global *global){

	/* Primero de priest/BFF/peasant en desbloquearse */
    if(global->npc_object1 == 8){
        print_text(global, "aux_files/Text/English/Priest/priest1.txt");
        unblock2(global, 0.34);
    }

    /* Segundo */
    else if(global->npc_object2 == 8)
        print_text(global, "aux_files/Text/English/Priest/priest2.txt");

    /* Tercero */
    else
        print_text(global, "aux_files/Text/English/Priest/priest3.txt");
}
