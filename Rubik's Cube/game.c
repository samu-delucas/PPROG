#include <stdio.h>
#include <termios.h>

#include"types.h"
#include "menu.h"
#include "cube_interface.h"
#include "interface.h"
#include "SDL_interface.h"


#define SAVE_FILE "./txt_files/save_game.dat"


extern int fileno(FILE*);

int main(void){
    int option=0, use_saved_game=TRUE, SDL_window=TRUE;
    struct termios ini__;

    tcgetattr(fileno(stdin), &ini__);  /*tomamos los valores iniciales de la terminal, como medida de seguridad-*/

    while ((option=MenusDisplay(&use_saved_game, &SDL_window))!=0){

        tcsetattr(fileno(stdin), TCSANOW, &ini__); /*reestablece los valores iniciales de la terminal en la terminal*/

        if(SDL_window==FALSE){
            if(c_interface(option, use_saved_game, SAVE_FILE)==ERROR){
                printf("ERROR in c_interface()");

                tcsetattr(fileno(stdin), TCSANOW, &ini__); /*reestablece los valores iniciales de la terminal en la terminal*/
                return -1;
            }
        }
        else
            SDL_interface(option, use_saved_game, SAVE_FILE);

        
        tcsetattr(fileno(stdin), TCSANOW, &ini__); /*reestablece los valores iniciales de la terminal en la terminal*/
    }


    tcsetattr(fileno(stdin), TCSANOW, &ini__); /*reestablece los valores iniciales de la terminal en la terminal*/

    terminal_clear();
    positionate_cursor(1,1);
    return 0;
}