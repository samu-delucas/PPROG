/* GRUPO: COVidóneos */

/* FICHERO CON FUNCIONES RELACIONADAS CON INPUTS Y CON LA TERMINAL */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include "types.h"
#include "input_helper.h"

struct termios initial;

/* Configurar el funcionamiento de la terminal */
int initTerm() {
    struct termios result;

    if (tcgetattr(0, &initial)==ERR) return ERR;
    result = initial;
    result.c_lflag &= ~ECHO;
    result.c_lflag &= ~ICANON;
    result.c_cc[VMIN]  = 1;
    result.c_cc[VTIME] = 0;

    return tcsetattr(0, TCSANOW, &result);
}

/* Restaurar el estado inicial de la terminal */
int endTerm() {
    return tcsetattr(0, TCSANOW, &initial);
}

/* Limpiar la variable input */
void clearInput(char *input) {
    if (input==NULL) return;

    *input = '\0';

    return;
}
