/* GRUPO: COVidóneos */

/* FICHERO CON LAS FUNCIONES DEL PERSONAJE */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "print_helper.h"
#include "personaje.h"
#include "object.h"
#include "config.h"
#include "types.h"

/* Creamos una estructura para el personaje que contenga su nivel de salud y el maletín.  */
struct _Personaje {
    int salud;
    Object **maletin;
};

static Window *game;
static Window *lateral;
static Window *bottom;
static Bool inventario;
int *flag;
char inventario_str[STR_LEN];
char salud_str[STR_LEN];
char end_str[STR_LEN];
int len;

/* startPersonaje se encarga de crear al personaje reservando memoria para una    */
/* varibale de tipo personaje que será el retorno de la función. Se inicializa    */
/* la salud a 100, se usa la función setObjets para poner en el maletín los       */ 
/* objetos que correspondan. Por último, imprimimos en la ventana correspondiente */
/* el nivel de salud en color verde. Además, cargamos los ficheros que están en   */
/* relación con el inventario o la salud del personaje.                           */
Personaje* startPersonaje(Window *gwin, Window *lwin, Window *bwin, int **f) {
    Personaje *p;

    p = (Personaje*) malloc(sizeof(Personaje));
    if (p==NULL) return NULL;
    
    if (loadStr(end_str, "./res/text/end_salud.txt")==ERR) {
        free(p);
        return NULL;
    }

    if (loadStr(inventario_str, "./res/text/inventario.txt")==ERR) {
        free(p);
        return NULL;
    }

    len = loadStr(salud_str, "./res/text/salud.txt");
    if (len==ERR) {
        free(p);
        return NULL;
    }

    p->salud = 100;
    p->maletin = setObjects();
    if (p->maletin==NULL) {
        free(p->maletin);
        free(p);
        return NULL;
    }

    game = gwin;
    lateral = lwin;
    bottom = bwin;
    flag = *f;
    inventario = FALSE;

    printColorPos(bottom, COLOR_GREEN, COLOR_DEFAULT, 1, 2, FALSE, salud_str);

    return p;
}

/* Actualiza la salud del personaje y según el nivel de anticuerpos, la barra de   */
/* salud se imprimirá de un color o de otro.                                       */
/* Además, esta función imprime la salud del personaje. Si esta está entre el 67%  */
/* y 100% se imprimirá en color verde, si está entre 34% y 66% de color naranja,   */
/* y si está entre 0% y 33% es que nos quedan pocos anticuerpos y se imprimirá en  */
/* rojo. Además, por cada 20% de anticuerpos, imprimimos una barra vertical que    */
/* hace referencia al nivel.                                                       */
/*                                                                                 */
/* Finalmente, si nos quedamos sin anticuerpos (salud<=0), imprimiremos en la      */
/* ventana central GAME OVER con caracteres especiales. Y el fichero end_salud.txt */
/* que hace referencia a end_str.                                                  */
void updateSalud(Personaje *per) {
    char output[6];
    Color outColor;
    int i, l, y;

    /* Imprimir salud */
    l = per->salud / 10;
    /* Color verde si la salud es mayor que el 66% */
    if (per->salud > 66) outColor = COLOR_GREEN;
    /* Color amarillo si la salud es mayor que el 33% */
    else if (per->salud > 33) outColor = COLOR_YELLOW;
    /* Color rojo en otro caso */
    else outColor = COLOR_RED;
    
    /* Para imprimir las barras de salud */
    for (i=1; i<=l; i++) {
        printColorPos(bottom, outColor, COLOR_DEFAULT, len+i, 2, FALSE, "▉");
    }
    /* Imprimimos en blanco hasta completar la barra de salud si no se tiene el 100% de salud */
    for (; i<=10; i++) printColorPos(bottom, COLOR_DEFAULT, COLOR_DEFAULT, len+i, 2, FALSE, " ");
    sprintf(output, " %d%% ", per->salud);
    printPos(bottom, len+i, 2, FALSE, output);

    /* Si nos quedamos sin anticuerpos-->GAME OVER*/
    if (per->salud<=0) {
        *flag = FALSE;
        clearWin(game);
        y = printCentered(game, COLOR_RED, COLOR_DEFAULT, "\t\t\t\t\t  ░█▀▀░█▀█░█▄█░█▀▀░░░█▀█░█░█░█▀▀░█▀▄  ");
        printXCentered(game, COLOR_RED, COLOR_DEFAULT, y+1, "\t\t\t\t\t  ░█░█░█▀█░█░█░█▀▀░░░█░█░▀▄▀░█▀▀░█▀▄  ");
        printXCentered(game, COLOR_RED, COLOR_DEFAULT, y+2, "\t\t\t\t\t  ░▀▀▀░▀░▀░▀░▀░▀▀▀░░░▀▀▀░░▀░░▀▀▀░▀░▀  ");
        printXCentered(game, COLOR_DEFAULT, COLOR_DEFAULT, y+4, end_str);
        return;
    }
}

/* En lineas generales, updatePersonaje sirve para hacer cambios en el personaje. */
/* Así, para el inventario, siempre que pulsemos 'e', abriremos o cerraremos este */
/* Si este está abierto, podremos pulsar el núumero del objeto que queramos       */
/* utilizar, y esté quedará marcado como usado en el maletín (inventario).        */

void updatePersonaje(Personaje *per, char input) {

    if (per==NULL) {
        *flag = FALSE;
        return;
    }

    /* Comandos del inventario */
    switch(input) {
        case 'E':
        case 'e':
            toggleInventario(per);
            break;
        /* Elegir objeto */
        case '1':
        case '2':
        case '3':
        case '4':
            if (inventario) {
                ObjectUse(per->maletin[atoi(&input)-1]);
                hideInventario();
            }
            break;

        default:
            break;
    }


    updateSalud(per);

    return;
}

/* toggleInventario se encarga de mostrar el inventario si no se está mostrando*/
/* o de esconderlo en caso contrario.  */
int toggleInventario(Personaje *per) {
    int f;

    if (per==NULL) return ERR;

    /* Si no se está mostrando el inventario, se muestra */
    if (inventario==FALSE) {
        f = showInventario(per);
    }
    /* En caso contrario, deja de mostrarse */
    else {
        f = hideInventario();
    }

    return f;
}

/* showInventario se usa para mostrar el inventario en la ventana lateral.        */
/*                                                                                */
/* Lo primero que se hace es limpiar está ventana con clearWin. A continuación,   */
/* se imprime en la ventana lateral el texto que se había almacenado al crear el  */
/* personaje en inventario_str. El siguiente paso es imprimir todos los objetos   */
/* que se encuentran en el maletín. Si el objeto ha sido utilizado, se imprimirá  */ 
/* en amarillo, en caso contrario, será en verde. Por último, igualamos el        */
/* inventario a TRUE para que si se comprueba en otra función se sepa que se está */
/* mostrando el inventario. */
int showInventario(Personaje *per) {
    int i, f;
    char aux[40];

    if (per==NULL) return ERR;

    clearWin(lateral);
    printPos(lateral, 2, 2, TRUE, inventario_str);

    /*Imprimir los objetos del inventario*/
    for (i=0, f=OK; i<TAM_MALETIN; i++) {
        if (per->maletin[i]==NULL) f = ERR;
        else {
            sprintf(aux, "\t(%d) %s", i+1, ObjectGetName(per->maletin[i]));
            /* Si se ha usado el objeto se imprime en amarillo */
            if (ObjectIsUsed(per->maletin[i])) printColorPos(lateral, COLOR_YELLOW, COLOR_DEFAULT, 2, 4 + i, TRUE,  aux);
            /* Si no se ha usado, se imprime en verde */
            else printColorPos(lateral, COLOR_GREEN, COLOR_DEFAULT, 2, 4 + i, TRUE, aux);
        }
    }

    inventario = TRUE;

    return f;
}

/* hideInventario se encarga de cerrar el inventario, por tanto, deja de imprimirlo       */
/*                                                                                        */
/* Restaura el fichero de la ventana lateral  y cambia la variable del inventario a FALSE */
/* para indicar que no se está montrando en caso de querer consultarlo en otro momento.   */
int hideInventario() {

    /* Restaurar el fichero de la ventana lateral */
    updateWindow(lateral);

    /* Indica que no se está mostrando el inventario */
    inventario  = FALSE;

    return OK;
}


/* setSalud actualiza la salud del personaje cambiando su salud al valor indicado por s. */
int setSalud(Personaje *per, int s) {

    if (per==NULL || s<0 || s>100) return ERR;

    per->salud = s;

    /* Tras realizar el cambio, hay que actualizar la salud del personaje */
    updateSalud(per);

    return OK;
}

/* bajarSalud disminuye la salud que tuviera el personaje en 5 unidades */
/* mediante la función setSalud.                                         */
int bajarSalud(Personaje *per) {
    int f;

    if (per==NULL) return ERR;

    /* Bajamos el nivel de anticuerpos */
    per->salud -= 5;

    if (per->salud <= 0) f = TRUE;
    else f = FALSE;

    /* Tras realizar el cambio, hay que actualizar la salud del personaje */
    updateSalud(per);

    return f;
}

/* destroyPersonaje se emplea para liberar el personaje cuando se acabe el juego. */
/* Se libera el maletín y el personaje.                                           */
void destroyPersonaje(Personaje *per) {

    if (per==NULL) return;

    /* Liberamos el maletín */
    MaletinFree(per->maletin);

    /* Liberamos el personaje */
    free(per);

    return;
}

/* getObject devuelve el objeto que se solicita mediante su Id accediendo */
/* a la estructura del inventario.                                        */
Object* getObject(Personaje *per, ObjectId ObjId) {

    if (per==NULL || per->maletin==NULL) return NULL;

    return per->maletin[ObjId];
}

/* getSalud devuelve la salud del personaje pasado como parámetro */
int getSalud(Personaje *per) {
    if (per==NULL) return ERR;

    return per->salud;
}