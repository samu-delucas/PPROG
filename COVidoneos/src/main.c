#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "types.h"
#include "config.h"
#include "print_helper.h"
#include "input_helper.h"
#include "escenas.h"
#include "personaje.h"
#include "menu.h"


#define STR_LEN 256

extern int usleep(int);


/***************
 ** FUNCIONES **
 ***************/
void start();
void update();
void end();
void *timer();

/***************
 ** VARIABLES **
 ***************/
Window *game, *lateral, *title, *bottom;
pthread_t thread1 = 1;
char input;
int *flag;
int pause;
int iescena;
int tiempo;
int min;
void (*u_escenas[ESCENAS]) (int*, char*, Window*, Window*, Window*, Personaje*);
void (*d_escenas[ESCENAS]) (int*, Window*, Window*, Window*);
Personaje *prota;

/***********************************************************
 **             ESTRUCTURA DE LA FUNCIÓN MAIN             **
 ***********************************************************
 *  1. Función start():
 *     Inicia las variables y estructuras del juego,
 *     configura la terminal, y crea las ventanas.
 *  2. Función timer():
 *     Inicia mediante un hilo la función que
 *     gestiona el temporizador del juego.
 *  3. Función update():
 *     Se ejecuta dentro de un bucle while, y justo
 *     después de que el usuario pulse una tecla.
 *  4. Función end():
 *     Recupera el estado inicial de la terminal, y libera
 *     todas las variables y estructuras utilizadas.
 * 
 ***********************************************************/
int main (int argc, char** argv) {
    int f;

    /* Utilizamos flag como estado (OK/ERR) */
    start();
    if (flag==NULL) return EXIT_FAILURE;
    else if (*flag==ERR) {
        end();
        free(flag);
        return EXIT_FAILURE;
    }

    /* Utilizamos flag como booleana (TRUE/FALSE) */
    *flag = TRUE;
    if (pthread_create(&thread1, NULL, timer, NULL)!=0) *flag = FALSE;
    while(*flag) {
        /* Por perfeccionar */
        input = getc(stdin);
        update();
    }

    /* Volvemos a utilizar flag como estado (OK/ERR) */
    *flag = OK; 
    end();

    f = *flag;
    free(flag);

    if (f==ERR) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

void start() {
    int f;

    flag = (int*) malloc(sizeof(int));
    if (flag==NULL) return;
    *flag = OK;

    if (loadConfig()==ERR) {
        *flag = ERR;
        return;
    }

    if (initTerm()==ERR) {
        *flag = ERR;
        return;
    }

    if (setWindows(&game, &lateral, &title, &bottom)) {
        *flag = ERR;
        return;
    }

    prota = startPersonaje(game, lateral, bottom, &flag);
    if (prota==NULL) {
        *flag = ERR;
        return;
    }

    tiempo = configGetTime();
    if (tiempo < 0) {
        *flag = ERR;
        return;
    }

    u_escenas[0]=updateEscena0;
    d_escenas[0]=dibujarEscena0;
    u_escenas[1]=updateEscena1;
    d_escenas[1]=dibujarEscena1;
    u_escenas[2]=updateEscena2;
    d_escenas[2]=dibujarEscena2;
    u_escenas[3]=updateEscena3;
    d_escenas[3]=dibujarEscena3;
    u_escenas[4]=updateEscena4;
    d_escenas[4]=dibujarEscena4;
    u_escenas[5]=updateEscena5;
    d_escenas[5]=dibujarEscena5;
    u_escenas[6]=updateEscena6;
    d_escenas[6]=dibujarEscena6;
    u_escenas[7]=updateEscena7;
    d_escenas[7]=dibujarEscena7;
    u_escenas[8]=updateEscena8;
    d_escenas[8]=dibujarEscena8;
    u_escenas[9]=updateEscena9;
    d_escenas[9]=dibujarEscena9;
    u_escenas[10]=updateEscena10;
    d_escenas[10]=dibujarEscena10;
    u_escenas[11]=updateEscena11;
    d_escenas[11]=dibujarEscena11;
    u_escenas[12]=updateEscena12;    
    d_escenas[12]=dibujarEscena12;
    u_escenas[13]=updateEscena13;    
    d_escenas[13]=dibujarEscena13;

    printFile(title, "./res/text/title_inicio.txt");
    f = showMenu(game);
    switch (f) {
        case 0:
            iescena = 0;
            break;

        case 2:
            cargarPartida(&iescena, &tiempo, &prota);
            if (iescena==-1) *flag=ERR;
            break;

        default:
            *flag = ERR;
            return;
    }

    (d_escenas[iescena])(&iescena, game, lateral, title);

    update();

    return;
}

void end() {

    if (thread1!=1) pthread_join(thread1, NULL);

    pthread_mutex_destroy(&lock1);

    if (prota!=NULL) destroyPersonaje(prota);

    if (game!=NULL) free(game);
    if (lateral!=NULL) free(lateral);
    if (title!=NULL) free(title);
    if (bottom!=NULL) free(bottom);

    clearAll();

    *flag = endTerm();

    return;
}

void update() {
    int f;

    /**********************************************
     * Para finalizar el bucle poner flag a FALSE *
     **********************************************/
    if (iescena==-1) {
        *flag=FALSE;
        return;
    }

    if (input=='q' || input=='Q') {
        pause = TRUE;
        f = showMenu(game);
        switch (f) {
            case 0:
                break;

            case 1:
                guardarPartida(iescena, min, prota);
                break;

            case 2:
                cargarPartida(&iescena, &tiempo, &prota);
                break;

            default: /* case -1 */
                *flag = FALSE;
                pause = FALSE;
                return;
        }
        (d_escenas[iescena])(&iescena, game, lateral, title);
        pause = FALSE;
    }

    /* Aquí va el juego... */
    (u_escenas[iescena])(&iescena, &input, game, title, lateral, prota);
    
    if (iescena==-1) {
        *flag=FALSE;
        return;
    }

    updatePersonaje(prota, input);

    (u_escenas[iescena])(&iescena, &input, game, title, lateral, prota);

    if (iescena==-1) {
        *flag=FALSE;
        return;
    }

    return;
}


void* timer() {
    int seg, y, l;
    char output[STR_LEN], end_str[STR_LEN];


    if (loadStr(end_str, "./res/text/end_tiempo.txt")==ERR) {
        *flag = FALSE;
        return NULL;
    }

    l = loadStr(output, "./res/text/tiempo.txt");
    if (l==ERR) {
        *flag = FALSE;
        return NULL;
    }

    if (pthread_mutex_init(&lock1, NULL) != 0) {
        *flag = FALSE;
        return NULL;
    }

    printColorPos(bottom, COLOR_RED, COLOR_DEFAULT, 1, 1, FALSE, output);

    pause = FALSE;

    /*2 bucles, uno para min y otro para s*/
    for(min=tiempo-1; min>=0 && *flag==TRUE; min--){
            for(seg=59; seg>=0 && *flag==TRUE; seg--){
            while (pause);
            if (*flag==FALSE) break;
            /*intervalo de 1000000 microseg (equivale a 1 segundo)*/
            if (usleep(1000000) != 0) {
                *flag=FALSE;
                return NULL;
            }

            sprintf(output, "%.2d:%.2d", min, seg);
            printColorPos(bottom, COLOR_BLACK, COLOR_RED, l + 2, 1, FALSE, output);
            
            /*condicional para cronometrar hasta llegar al tiempo deseado*/
            if(min==0 && seg==0){
                *flag = FALSE;
                clearWin(game);
                y = printCentered(game, COLOR_RED, COLOR_DEFAULT, "\t\t\t\t\t  ░█▀▀░█▀█░█▄█░█▀▀░░░█▀█░█░█░█▀▀░█▀▄  ");
                printXCentered(game, COLOR_RED, COLOR_DEFAULT, y+1, "\t\t\t\t\t  ░█░█░█▀█░█░█░█▀▀░░░█░█░▀▄▀░█▀▀░█▀▄  ");
                printXCentered(game, COLOR_RED, COLOR_DEFAULT, y+2, "\t\t\t\t\t  ░▀▀▀░▀░▀░▀░▀░▀▀▀░░░▀▀▀░░▀░░▀▀▀░▀░▀  ");
                printXCentered(game, COLOR_DEFAULT, COLOR_DEFAULT, y+4, end_str);
                return NULL;
            }
        }
    }

    return NULL;
}
