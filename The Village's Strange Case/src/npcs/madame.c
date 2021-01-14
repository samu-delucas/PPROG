#include "character_funcs.h"
#include "personajes.h"

/**
 * @brief Función principal de la madamme. Diálogo y llamadas a otras funciones. 
 * 
 * @param global
 */
void madame(Global *global){
    NPC *madame = global->NPCs[6];
    char c;
    
    /* Descomentar para probar personaje 
    madame->locked = 0;*/
    
    if (madame->locked != 0)
        blocked_message(global);

    else if (global->paradoja == 1){
        print_image("aux_files/Art/madame.bmp", 1, 1);
        paradoja();
        wait_for_space(global);
    }

	else if (madame->already_talked == 1) {
		print_image("aux_files/Art/madame.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/Madame/madame5.txt");
	}
	
    else{
        print_image("aux_files/Art/madame.bmp", 1, 1);
        
        madame->already_talked=1;
        
        print_text(global, "aux_files/Text/English/Madame/madame1.txt");
        
        /* Elige el texto correcto dependiendo del grupo al que pertenezca el asesino */
        if(global->assassin==0 || global->assassin==3 ||global->assassin==10)
            print_text(global, "aux_files/Text/English/Madame/madame2.txt");
        else if(global->assassin==1 || global->assassin==9 ||global->assassin==8)
            print_text(global, "aux_files/Text/English/Madame/madame3.txt");
        else
            print_text(global, "aux_files/Text/English/Madame/madame4.txt");
    }
}
