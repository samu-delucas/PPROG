#include "character_funcs.h"
#include "personajes.h"

void delete(int i, int j);
int potions(Global *global);
void minigame(Global *global);

/**
 * @brief Función principal de la bruja. Diálogo y llamadas a otras funciones. 
 * 
 * @param global
 */
void witch(Global *global){
    NPC *witch = global->NPCs[3];
    NPC *littlegirl = global->NPCs[7];
	NPC *wolf = global->NPCs[1];
	NPC *fortuneteller = global->NPCs[2];
    char c, d;
	
	/* Desbloquear para hacer pruebas con el personaje
    witch->locked = 0;*/
	
    if (witch->locked != 0.0)
        blocked_message(global);
	
	/* Primera vez que se habla con la bruja */
    else if (witch->already_talked == 0) {
		witch->already_talked= 1;
    	
        print_image("aux_files/Art/witch.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/Witch/witch.txt");
        
        c = wait_for_input("12");
        witch->opcion = c;
        if (c != '1') minigame(global);
    }

    /* Vuelve a hablar con ella después de haberle dado plantón */
    else if(witch->opcion == '1'){
    	print_image("aux_files/Art/witch.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/Witch/witch2.txt");
        witch->opcion = 2;
        
        minigame(global);
    }

    else if(witch->solved == 0)
    	minigame(global);
    
    else{
        print_image("aux_files/Art/witch.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/Witch/witch3.txt");
        do {
            print_string(" 1. The little girl>2. The werewolf>3. The fortune teller>4. Thank you, I'm leaving now", 289,50);
            d = wait_for_input("1234");
            print_image("aux_files/Art/blank_text.bmp", 289, 50);

            /* La persona seleccionada no miente */
            if( (d == '1' && littlegirl->liar== 0) || (d == '2' && wolf->liar == 0 ) || (d == '3' && fortuneteller->liar == 0 ) )
                print_text(global, "aux_files/Text/English/Witch/witch4.txt");

            else if(d!='4') 
                print_text(global, "aux_files/Text/English/Witch/witch5.txt");
        } while(d != '4');
    }
}

/* 
 * @brief Responde ante los resultados obtenidos después de ejecutar el minijuego.
 *
 * @param global
 */
void minigame(Global *global){
	NPC *witch = global->NPCs[3];
	NPC *littlegirl = global->NPCs[7];
	NPC *wolf = global->NPCs[1];
	NPC *fortuneteller = global->NPCs[2];
	char d;
	
	/* Minijuego resuelto */
	if (potions(global) == 1) {
		witch->solved = 1;

		/* MENSAJE QUE INFORMA DE HABER GANADO EL MINIJUEGO */
		print_image("aux_files/Art/blank_text.bmp", 289, 50);
		print_string("CONGRATULATIONS! YOU HAVE WON!", 290, 100);
		wait_for_input(" ");


		print_image("aux_files/Art/witch.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/Witch/witch3.txt");
        
        
        do {
            print_string(" 1. The little girl>2. The werewolf>3. The fortune teller>4. Thank you, I'm leaving now", 289,50);
            d = wait_for_input("1234");
            print_image("aux_files/Art/blank_text.bmp", 289, 50);

            /* La persona seleccionada no miente */
            if( (d == '1' && littlegirl->liar== 0) || (d == '2' && wolf->liar == 0 ) || (d == '3' && fortuneteller->liar == 0 ) )
                print_text(global, "aux_files/Text/English/Witch/witch4.txt");
            else if(d != '4') 
                print_text(global, "aux_files/Text/English/Witch/witch5.txt");
        } while(d != '4');
			
		unblock2(global, 0.33);
		global->paradoja = 0;
	}
	/* Minijuego no resuelto */
	else {
		/* MENSAJE QUE INFORMA DE HABER PERDIDO EL MINIJUEGO */
		print_image("aux_files/Art/blank_text.bmp", 289, 50);
		print_string("WHAT A SHAME! YOU HAVE LOST!", 290, 100);
		wait_for_input(" ");

		print_image("aux_files/Art/witch.bmp", 1, 1);
		print_text(global, "aux_files/Text/English/Witch/witch6.txt");
		global->paradoja = 1;
	}
}

/* 
 * @brief Minijuego de la bruja. 
 *
 * @return Devuelve 1 si gana el jugador. 0 si pierde
 */
int potions(Global *global) {
    int i = 0, j;
    char c[2];
    char aux[2];
    char text[64];

    aux[1] = '\0';

    /*print_image("aux_files/Art/witch_blank.bmp", 1, 1);*/
    print_intro("aux_files/Text/English/Witch/minigame.txt");

    print_image("aux_files/Art/witch_blank.bmp", 1, 1);
    print_image("aux_files/Art/potions.bmp", 30, 30);
	print_string(" The one that drinks the last potion will>suffer from strong hallucinations.", 289, 50);

    c[1]='\0';
    while (1){
        if(i == 14) 
            c[0] = wait_for_input("1");
        else if(i==13) 
            c[0] = wait_for_input("12");
        else
            c[0] = wait_for_input("123");

        j = atoi(c);
        
        delete(i,j);
        
        i += j;

        /*1 potion left. player has won*/
        if (i == 14) return 1;
        /*0 potions left. player has lost*/
        if (i == 15) return 0;

        if ((-(i + 2) % 4) == 0.0)
            j=generate_rand(1,3);
        else
            j = -(i + 2) % 4 + 4;

        strcpy(text, "I'll pick ");
        aux[0] = j + '0';
        strcat(text, aux);
        if (j==1) strcat(text, " potion");
        else strcat(text, " potions");
        print_string(text, 240, 75);
        sleep(2);
        print_image("aux_files/Art/delete_text_potions.bmp", 240, 75);

        delete(i,j);
        i += j;
    }
}

/* 
 * @brief Función que controla los gráficos del minijuego 'potions()'
 *
 */
void delete(int i, int j){
    int k, ancho=32*2;

    for(k=0; k<j; k++){
        if(i+k >= 8) print_image("aux_files/Art/blank_potion.bmp", 165, 76+(k+i-8)*ancho);
        else print_image("aux_files/Art/blank_potion.bmp", 120, 44+(k+i)*ancho);
    }
}
