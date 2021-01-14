#include "character_funcs.h"
#include "personajes.h"

void clock_ft(Global *global);
int solve(Global *global);

/**
 * @brief Función principal de la adivina. Diálogo y llamadas a otras funciones. 
 * 
 * @param global
 */
void fortuneteller(Global *global){
    NPC *fortuneteller = global->NPCs[2];
    NPC *witch = global->NPCs[3];
    char c = '0';
    char s[100] = "\0";
    
    /* Desbloquear para probar el personaje 
    fortuneteller->locked = 0; 
    fortuneteller->already_talked = 1;
    fortuneteller->solved = 1; */

    /* Personaje bloqueado */
    if (fortuneteller->locked != 0)
        blocked_message(global);

	/* Personaje bloqueado por paradoja */
    else if (global->paradoja == 1){
        print_image("aux_files/Art/fortuneteller.bmp", 1, 1);
        paradoja();
        wait_for_space(global);
    }

    /* Primera vez que se habla con el personaje */
    else if (fortuneteller->already_talked == 0){
        wsw_unblock(global);
		fortuneteller->already_talked = 1;
	
        print_image("aux_files/Art/fortuneteller.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/FortuneTeller/FortuneTeller.txt");
        
        c = wait_for_input("12");
        fortuneteller->opcion= c;
        if (c != '1') clock_ft(global);
    }

    /* Se habla con ella después de haber elegido la opción 1 */
    else if(fortuneteller->opcion == '1' || fortuneteller->solved == 0){
        print_image("aux_files/Art/fortuneteller.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/FortuneTeller/FortuneTeller3.txt");

        clock_ft(global);
    }

    /* Se habla con ella una vez resuelto el minijuego */
    else{
        print_image("aux_files/Art/fortuneteller.bmp", 1, 1);
        print_text(global, "aux_files/Text/English/FortuneTeller/FortuneTeller4.txt");
        if(wait_for_input("12")=='1') global->juego = solve(global);
    }
}

/**
 * @brief Minijuego asociado a la adivina. 
 * 
 * @param global
 */
void clock_ft(Global *global){
	NPC *fortuneteller = global->NPCs[2];
	char s[100] = "\0";

	print_image("aux_files/Art/fortuneteller.bmp", 1, 1);
	print_text(global, "aux_files/Text/English/FortuneTeller/FortuneTeller2.txt");
    print_image("aux_files/Art/clock.bmp", 30,30);
    print_string("What time is it?", 40, 210);
    print_string("(The answer must be in the form xx:xx)", 289, 50);
    myscanf(303, 80, s);
    print_image("aux_files/Art/fortuneteller.bmp", 1, 1);
    if (strcmp(s,"12:00") == 0|| strcmp(s,"00:00") == 0 || strcmp(s,"24:00") == 0){
        if(global->first==1) unblock2(global, 0.33);

        fortuneteller->solved = 1;
        fortuneteller->opcion= '2';
        if(fortuneteller->liar==0) print_text(global, "aux_files/Text/English/FortuneTeller/FortuneTeller5.txt");
        else print_text(global, "aux_files/Text/English/FortuneTeller/FortuneTeller7.txt");  
    }
    
    else print_text(global, "aux_files/Text/English/FortuneTeller/FortuneTeller6.txt");
}

/**
 * @brief Final del juego. El jugador se debe presentar ante la adivina y elegir el objeto y el asesino entre una lista mostrada en pantalla.
 * 
 * @param global
 *
 * @return 0 = El jugador ha perdido, 1 = El jugador ha ganado, 2 = No ha elegido opción
 */
int solve(Global *global){
    FILE *f;
    int assassin, object, i;
    char assassin_file[64] = "aux_files/Art/", assassin_text[512]="";
    char object_file[64] = "aux_files/Art/";
    char object_name[16], line[128]="So, finally, are you sure>that the ", *linea;
    int len = 0;
    
    print_image("aux_files/Art/full_blank.bmp", 1, 1);

    print_intro("aux_files/Text/English/FortuneTeller/solve_assassin.txt");
    assassin = wait_for_input("abcdefghijk")- 97;

    print_intro("aux_files/Text/English/FortuneTeller/solve_object.txt");
    object = wait_for_input("abcdefghi")- 97;
    switch(object){
        case 0:
            strcpy(object_name, "rope");
            break;
        case 1:
            strcpy(object_name, "poison");
            break;
        case 2:
            strcpy(object_name, "pencil");
            break;
        case 3:
            strcpy(object_name, "knife");
            break;
        case 4:
            strcpy(object_name, "brick");
            break;
        case 5:
            strcpy(object_name, "scythe");
            break;
        case 6:
            strcpy(object_name, "axe");
            break;
        case 7:
            strcpy(object_name, "hammer");
            break;
        case 8:
            strcpy(object_name, "shovel");
            break;
    }

    print_image(global->NPCs[assassin]->art, 1, 1);

    strcat(object_file, object_name);
    strcat(object_file, ".bmp");
    print_image(object_file, 150, 300);

    strcat(line, global->NPCs[assassin]->name);
    strcat(line, ",>with the ");
    strcat(line, object_name);
    strcat(line, ",>killed the mayor's>daughter?");
    print_string(line, 60, 330);
    
    print_string(" 1. Yes, I am.>2. No, I still have doubts.", 290, 50);
    if (wait_for_input("12") == '1'){
        if(global->assassin == assassin && strcmp(object_name, global->object2)==0){
            f=fopen("aux_files/Text/English/case_solved.txt","r");
            print_image("aux_files/Art/full_blank.bmp", 1, 1);

			print_intro("aux_files/Text/English/won.txt");

            for(i=0; i<assassin*9; i++)
                getline(&linea, &len, f);
            
            for(i=0; i<8;i++){
                getline(&linea,&len, f);
                print_string(linea, 60+i*12, 50);
            }
	
			wait_for_input(" ");
			print_image("aux_files/Art/full_blank.bmp", 1, 1);
			
			print_string("END\0",150,275);
			print_string("Press 'space' to end game", 300, 75);
            wait_for_input(" ");
            fclose(f);
            free(linea);
            return 1;
        }
        else{
            /* El jugador ha perdido */
            print_image("aux_files/Art/full_blank.bmp", 1, 1);
			print_intro("aux_files/Text/English/lost.txt");
            print_string("END\0",150,275);
			print_string("Press 'space' to end game", 300, 75);
            wait_for_input(" ");
            return 2;
        }
    }

    /* El jugador quiere pensárselo más */
    else return 0;
}
