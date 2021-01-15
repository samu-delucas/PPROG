#include "menu.h"
#include "config.h"
#include "escenas.h"
#include "print_helper.h"
#include "personaje.h"

#define MENU_LINE_LEN 70

Bool firstTime = TRUE;

void showHelp();
int showQuit(Window*);

int showMenu(Window *game) {
    int x, i, cont, ret;
    char input;
    char opt[5][STR_LEN];

    if (game==NULL) return ERR;

    clearWin(game);

    x = (winGetCols(game) / 2) - 20;

    /* Imprimimos menú con un tipo de letra distinto */
    printColorPos(game, COLOR_BLUE, COLOR_DEFAULT, x, 5, FALSE,  " ▄▄▄  ▄▄▄                              ");
    printColorPos(game, COLOR_BLUE, COLOR_DEFAULT, x, 6, FALSE, " ███  ███                              ");
    printColorPos(game, COLOR_BLUE, COLOR_DEFAULT, x, 7, FALSE, " ████████   ▄████▄   ██▄████▄  ██    ██");
    printColorPos(game, COLOR_BLUE, COLOR_DEFAULT, x, 8, FALSE, " ██ ██ ██  ██▄▄▄▄██  ██▀   ██  ██    ██");
    printColorPos(game, COLOR_BLUE, COLOR_DEFAULT, x, 9, FALSE, " ██ ▀▀ ██  ██▀▀▀▀▀▀  ██    ██  ██    ██");
    printColorPos(game, COLOR_BLUE, COLOR_DEFAULT, x, 10,FALSE, " ▀▀    ▀▀    ▀▀▀▀▀   ▀▀    ▀▀   ▀▀▀▀ ▀▀");

    /* Imprimimos centrado las flechas para indicar la posibilidad de desplazarse con ellas */
    printXCentered(game, COLOR_WHITE, COLOR_DEFAULT, 12, "↑ ↓");

    /* Cargamos los textos para las opciones del menú */
    if (firstTime) {
        if (loadStr(opt[0], "./res/text/empezar.txt")==ERR) return ERR;
    } else {
        if (loadStr(opt[0], "./res/text/guardar.txt")==ERR) return ERR;
    }
    if (loadStr(opt[1], "./res/text/cargar.txt")==ERR) return ERR;
    if (loadStr(opt[2], "./res/text/ayuda.txt")==ERR) return ERR;
    if (loadStr(opt[3], "./res/text/salir_menu.txt")==ERR) return ERR;
    if (loadStr(opt[4], "./res/text/salir_juego.txt")==ERR) return ERR;

    cont = 0;
    input = 0;
    while (input != '\n') {
        if (input == ESC_SEQ) {
            if (getc(stdin) == '[') {
                switch (getc(stdin)) {
                    case 'A':
                        cont = (cont+4)%5;
                        break;

                    case 'B':
                        cont = (cont+1)%5;
                        break;

                    default:
                        break;
                }
            }
        }

        for (i=0; i<5; i++) {
            if (i==cont) printColorPos(game, COLOR_BLACK, COLOR_BLUE, 57, 15+(2*i), FALSE, opt[i]);
            else printPos(game, 57, 15+(2*i), FALSE, opt[i]);
        }

        input = getc(stdin);
    }

    ret = 0;

    switch(cont) {
        case 0:
            if (firstTime) ret =  0;
            else ret =  1;
            break;
        
        case 1:
            ret = 2;
            break;

        case 2:
            showHelp(game);
            ret =  0;
            break;

        case 3:
            ret =  0;
            break;

        case 4:
            ret =  showQuit(game);
            break;

        default:
            break;
    }

    if (firstTime) firstTime = FALSE;

    return ret;
}

void showHelp(Window* game) {
    FILE *f;
    int x, y;
    char line[MENU_LINE_LEN*3 + 2]; /* Los caracteres de bordes ocupan un tamaño de 3*char */

    if (game==NULL) return;

    f = fopen("./res/text/ayuda_pag.txt", "r");
    if (f==NULL) return;
    
    x = (winGetCols(game) / 2) - (MENU_LINE_LEN / 2);
    y = 4;

    clearWin(game);

    while (fgets(line, MENU_LINE_LEN*3 + 2, f)) {
        if (line[MENU_LINE_LEN]=='\n') line[MENU_LINE_LEN]='\0';
        printPos(game, x, y, FALSE, line);
        y++;
    }
    
    fclose(f);

    getc(stdin);

    return;
}

int showQuit(Window* game) {
    FILE *f;
    int x, y;
    char input;
    char line[MENU_LINE_LEN*3 + 2]; /* Los caracteres de bordes ocupan un tamaño de 3*char */

    if (game==NULL) return ERR;

    f = fopen("./res/text/salir_pag.txt", "r");
    if (f==NULL) return ERR;
    
    x = (winGetCols(game) / 2) - (MENU_LINE_LEN / 2);
    y = 8;

    clearWin(game);

    while (fgets(line, MENU_LINE_LEN*3 + 2, f)) {
        if (line[MENU_LINE_LEN]=='\n') line[MENU_LINE_LEN]='\0';
        printPos(game, x, y, FALSE, line);
        y++;
    }
    
    fclose(f);

    input = getc(stdin);
    while (input!='y' && input!='n' && input!='Y' && input!='N') input=getc(stdin);
    switch(input) {
        case 'y':
        case 'Y':
            return -1;

        case 'n':
        case 'N':
            return 0;

        default:
            break;
    }

    return -1;
}

/* ESTRUCTURA DEL FICHERO game.save (EN BINARIO) 
 *  1º. Entero representando iescena.
 *  2º. Entero representando los minutos restantes en la partida.
 *  3º. Entero representando la salud del personaje.
 *  4º. Lista de enteros indicando los objetos que ya han sido utilizados.
 */

/* El CdE se hace con *iescena=-1 */
void cargarPartida(int* iescena, int* tiempo, Personaje** prota) {
    FILE *save;
    int salud, i, k;
    int objetos[TAM_MALETIN];

    save = fopen("./game.save", "rb");
    if (save==NULL) {
        *iescena = -1;
        return;
    }

    fread(iescena, sizeof(int), 1, save);
    if (*iescena<0 || *iescena>=ESCENAS) {
        *iescena = -1;
        fclose(save);
        return;
    }

    fread(tiempo, sizeof(int), 1, save);
    if (*tiempo <= 0) {
        *iescena = -1;
        fclose(save);
        return;
    }

    fread(&salud, sizeof(int), 1, save);
    if (salud <= 0) {
        *iescena = -1;
        fclose(save);
        return;
    }
    setSalud(*prota, salud);

    for (i=0; i<TAM_MALETIN; i++) {
        k = fread(objetos + i, sizeof(int), 1, save);
        if (k!=1) break; /* Se llega a EOF */
        ObjectUse(getObject(*prota, objetos[i]));
    }

    fclose(save);

    return;
}

int guardarPartida(int iescena, int min, Personaje* prota) {
    FILE *save;
    int salud, i;

    save = fopen("./game.save", "wb+");
    if (save==NULL) return ERR;

    salud = getSalud(prota);

    fwrite(&iescena, sizeof(int), 1, save);
    fwrite(&min, sizeof(int), 1, save);
    fwrite(&salud, sizeof(int), 1, save);
    for (i=0; i<TAM_MALETIN; i++) {
        if (ObjectIsUsed(getObject(prota, i))) {
            fwrite(&i, sizeof(int), 1, save);
        }
    }

    fclose(save);
    return OK;
}