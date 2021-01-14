#include "character_funcs.h"
#include "personajes.h"

void generarSospechoso(Global *global);
void decirSospecha(Global *global);


void* mayor_animation(){
    while(1){
        print_image("aux_files/Art/smoke3.bmp", 56, 36);
        usleep(500000);
        print_image("aux_files/Art/smoke2.bmp", 56, 36);
        usleep(500000);
        print_image("aux_files/Art/smoke1.bmp", 56, 36);
        usleep(500000);
        print_image("aux_files/Art/smoke0.bmp", 56, 36);
        usleep(500000);
    }
}

void mayor_draw(){
    print_image("aux_files/Art/mayor.bmp", 1, 1);
    pthread_create(&animation_pth, NULL, mayor_animation, NULL);
}

/**
 * @brief Función principal del alcalde. Diálogo y llamadas a otras funciones. 
 * 
 * @param global
 */
void mayor(Global *global){
    NPC *mayor = global->NPCs[0];
    NPC *werewolf = global->NPCs[1];
    NPC *fortuneteller = global->NPCs[2];
    
    mayor_draw();

    /* Bloqueado por paradoja */
    if (global->paradoja == 1) {
        paradoja();
        wait_for_space(global);
    }

    /* Si se habla por primera vez */
    else if (mayor->already_talked == 0){
        print_text(global, "aux_files/Text/English/Mayor/mayor.txt");
        
        generarSospechoso(global);

        mayor->already_talked = 1;

        if (wait_for_input("12") != '1') decirSospecha(global);
    }
    
    /* Si se vuelve a hablar con él */
    else decirSospecha(global);

    pthread_cancel(animation_pth);
}

/**
 * @brief Genera un sospechoso entre la adivina y el lobo de forma aleatoria. Luego, desbloquea dicho personaje. 
 * 
 * @param Variable global
 */
void generarSospechoso(Global *global) {
    NPC *werewolf = global->NPCs[1];
    NPC *fortuneteller = global->NPCs[2];
    
    /* Se genera aleatoriamente el primer sospechoso entre el lobo o la adivina */
    /* Sospechoso: lobo */
    if (generate_rand(0, 1) == 1) {
        werewolf->locked=0;
        global->first = 1;
    }
    
    /* Sospechosa: adivina */
    else {
        fortuneteller->locked= 0;
        global->first = 2;
    }
}

/**
 * @brief Dice el resultado de la función generarSospechoso() cuando se habla con el personaje. 
 * 
 * @param Variable global
 */
void decirSospecha(Global *global) {
    NPC *werewolf = global->NPCs[1];
    NPC *fortuneteller = global->NPCs[2];
    char buff[30];

    print_text(global, "aux_files/Text/English/Mayor/mayor2.txt");
        
    if (fortuneteller->locked == 0)
        print_string(" Fortune Teller.>She also lives in the>Main Square.", 60, 300);
    else
        print_string(" Werewolf>Always hunting in the>woods...", 60, 300);
    wait_for_space(global);
}
