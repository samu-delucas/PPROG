#include "character_funcs.h"
#include "personajes.h"


void minijuegoNina(Global *global);
void ojos();

/**
 * @brief Función principal de la niña. Diálogo y llamadas a otras funciones. 
 * 
 * @param global
 */
void littlegirl(Global *global) {
    NPC *littlegirl = global->NPCs[7];
    
    /* Bloqueada por cronómetro */
    if(littlegirl->locked == 1) {
    	print_text(global, "aux_files/Text/English/Kid/littlegirl6.txt");
    	wait_for_input(" ");
    }

    /* Bloqueada por paradoja */
    else if (global->paradoja == 1) {
        print_image("aux_files/Art/littlegirl.bmp", 1, 1);
        paradoja();
        wait_for_space(global);
    }
    
    /* Hablar con la niña por primera vez */
    else if (littlegirl->already_talked == 0) {
        littlegirl->already_talked = 1;

        ojos();
        
        print_text(global, "aux_files/Text/English/Kid/littlegirl.txt");
        wait_for_input("12");
        print_text(global, "aux_files/Text/English/Kid/littlegirl1.txt");

	    minijuegoNina(global);
    }
    
    /* Hablar con la niña si no has resuelto el acertijo */
    else if (littlegirl->already_talked == 1 && littlegirl->solved == 0) {
        ojos();
        
        print_text(global, "aux_files/Text/English/Kid/littlegirl5.txt");
        
        minijuegoNina(global);
    }
    
    /* Acertijo solucionado */
    else {
        ojos();
        print_text(global, "aux_files/Text/English/Kid/littlegirl4.txt");
    }
}

/**
 * @brief Minijuego de la niña. Esperará una palabra en concreto dicha por el tonto y si esta no es correcta, la niña se bloquea X tiempo. 
 * 
 * @param Variable global
 */
void minijuegoNina(Global *global) {
    NPC *littlegirl = global->NPCs[7];
    char s[100] = "\0";

    myscanf(303, 80, s);
        
    /* Solución correcta */
    if (strcmp(global->littlegirl_solution,s) == 0) {
        littlegirl->solved = 1;
        if (global->assassin==7)
            print_text(global, "aux_files/Text/English/Kid/littlegirl3.1.txt"); 
        else
            print_text(global, "aux_files/Text/English/Kid/littlegirl3.txt"); 
        wait_for_space(global);
    }
    
    /* Solución incorrecta */
    else {
        littlegirl->locked=1;
        global->clk_lg_leave = global->clk;
        print_text(global, "aux_files/Text/English/Kid/littlegirl2.txt");
        wait_for_space(global);
    }    
}

/**
 * @brief Small animation.
 * 
 */
void ojos() {
    print_image("aux_files/Art/littlegirl.bmp", 1, 1);
    usleep(600000);
    print_image("aux_files/Art/girl_eyes.bmp", 126,145);
}
