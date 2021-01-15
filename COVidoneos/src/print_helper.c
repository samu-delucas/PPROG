/* GRUPO: COVidóneos */

/*FICHERO CON FUNCIONES PARA LA ORGANIZACIÓN DE LAS VENTANAS Y PARA IMPRIMIR*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "print_helper.h"
#include "types.h"
#include "config.h"

#define FG_BEGIN    30
#define BG_BEGIN    40
#define RESET_COLOR "[0m"
#define RESET_POS   "[H"
#define PATH_LEN 512

/* Estructura para ventana que indica las posiciones horizontales y verticales, */
/* el número de filas y el de columnas.                                         */
struct _Window {
    int pos_x;
    int pos_y;
    int cols;
    int lines;
};

static char filePath[PATH_LEN];

/* Esta función se emplea para imprimir en output con los colores indicados a continuación */
/* (primero el del texto y luego el del fondo). */ 
void setColorCode(char *output, Color fg, Color bg) {

    if (fg < 0) {
        if (bg < 0) {
            strcpy(output, "");
            return;
        }
        sprintf(output, "%c[%dm", ESC_SEQ, BG_BEGIN+bg);
        return;
    }
    if (bg < 0) {
        sprintf(output, "%c[%dm", ESC_SEQ, FG_BEGIN+fg);
        return;
    }
    sprintf(output, "%c[%d;%dm", ESC_SEQ, FG_BEGIN+fg, BG_BEGIN+bg);

    return;
}


/* Esta función ajusta un texto (pasado como primer argumento) a la longitud */
/* de la ventana (wLen) y devuelve una cadena de strings (char **) con el    */
/* texto fraccionado para que tenga la longitud adecuada. */
char** adjustString(char *output, int wLen, int *nOut) {
    char **res;
    int i, j, k, l, lSpc, llSpc;

    res = NULL;
    l = strlen(output);
    llSpc = 0;
    for (i=0, j=0; i<l; i++) {
        if (i - llSpc > wLen) {
            res = (char**) realloc(res, (j+1)*sizeof(char*));
            if (res==NULL) return NULL;
            if (llSpc==0) {
                res[j] = (char*) malloc((lSpc+1)*sizeof(char));
                if (res[j]==NULL) {
                    for (k=0; k<j; k++) {
                        free(res[k]);
                    }
                    free(res);
                    return NULL;
                }
                strncpy(res[j], output, lSpc);
                res[j][lSpc] = '\0';
            }
            else {
                res[j] = (char*) malloc((lSpc-llSpc)*sizeof(char));
                if (res[j]==NULL) {
                    for (k=0; k<j; k++) {
                        free(res[k]);
                    }
                    free(res);
                    return NULL;
                }
                strncpy(res[j], &output[llSpc+1], lSpc-llSpc-1);
                res[j][lSpc-llSpc-1] = '\0';
            }
            j++;
            llSpc = lSpc;
        }
        if (output[i]==' ') lSpc=i;
    }
    res = (char**) realloc(res, (j+1)*sizeof(char*));
    if (res==NULL) return NULL;
    res[j] = (char*) malloc((l-llSpc)*sizeof(char));
    if (res[j]==NULL) {
        for (k=0; k<j; k++) {
            free(res[k]);
        }
        free(res);
        return NULL;
    }
    strcpy(res[j], &output[llSpc+1]);
    *nOut = j+1;

    return res;
}

/*
** Función privada para asignar a cada estructura de ventana
** sus dimensiones en función de la longitud de game (dada
** por gameWidth) y de la de las ventanas laterales (dadas por
** lateralWidth), así como de las alturas de cada ventana
** lateral (dadas por titleHeight, lateralHeight, y
** bottomHeight), donde game, title, lateral y bottom siguen
** la siguiente estructura:
**             |----------------+------------|
**             |                |   title    |
**             |                +------------|
**             |                |            |
**             |                |            |
**             |                |            |
**             |      main      |  lateral   |
**             |                |            |
**             |                |            |
**             |                |            |
**             |                +------------|
**             |                |   bottom   |
**             |----------------+------------|
** El objetivo de esta función es poder darle como
** argumentos, las dimensiones para cada ventana relativas
** al tamaño total de la ventana de la terminal.
 */
void setValues(Window *game, Window *lateral, Window *title, Window *bottom, int gameWidth, int lateralWidth, int titleHeight, int lateralHeight, int bottomHeight) {
    int totalWidth, totalHeight;

    totalWidth = gameWidth + lateralWidth;
    totalHeight = titleHeight + lateralHeight + bottomHeight;
    clearAll();
    printf("%c[8;%d;%dt%c[3J", ESC_SEQ, totalHeight, totalWidth, ESC_SEQ);
    game->pos_x=1;
    game->pos_y=1;
    game->cols=gameWidth-2;
    game->lines=totalHeight-2;
    title->pos_x=gameWidth;
    title->pos_y=1;
    title->cols=lateralWidth-2;
    title->lines=titleHeight-2;
    bottom->pos_x=gameWidth;
    bottom->pos_y=titleHeight+lateralHeight+1;
    bottom->cols=lateralWidth-2;
    bottom->lines=bottomHeight-2;
    lateral->pos_x=gameWidth;
    lateral->pos_y=titleHeight;
    lateral->cols=lateralWidth-2;
    lateral->lines=lateralHeight;

    return;
}

/* Esta función nos permite dibujar cada una de las ventanas con sus límites */
/* empleando caracteres. */
int setWindows(Window **game, Window **lateral, Window **title, Window **bottom) {
    int i, gameWidth, lateralWidth;
    Window *g, *l, *t, *b;

    gameWidth = configGetGameWidth();
    if (gameWidth < 0) return ERR;
    lateralWidth = configGetLateralWidth();
    if (lateralWidth < 0) return ERR;

    g = (Window*) malloc(sizeof(Window));
    if (g==NULL) return ERR;
    l = (Window*) malloc(sizeof(Window));
    if (l==NULL) {
        free(g);
        return ERR;
    }
    t = (Window*) malloc(sizeof(Window));
    if (t==NULL) {
        free(g);
        free(l);
        return ERR;
    }
    b = (Window*) malloc(sizeof(Window));
    if (b==NULL) {
        free(g);
        free(l);
        free(b);
        return ERR;
    }

    setValues(g, l, t, b, gameWidth + 4, lateralWidth + 3, 6, (gameWidth*9/32) - 8, 4);
    clearAll();

    printf("%c[%d;%dH┏", ESC_SEQ, g->pos_y, g->pos_x);
    printf("%c[%d;%dH┗", ESC_SEQ, g->lines+2, g->pos_x);
    printf("%c[%d;%dH┓", ESC_SEQ, t->pos_y, t->pos_x+t->cols+2);
    printf("%c[%d;%dH┛", ESC_SEQ, b->pos_y+b->lines+1, b->pos_x+b->cols+2);
    for (i=g->pos_x+1; i<g->cols+2; i++) {
        printf("%c[%d;%dH━", ESC_SEQ, g->pos_y, i);
        printf("%c[%d;%dH━", ESC_SEQ, g->lines+2, i);
    }
    for (i=g->pos_x+1; i<g->lines+2; i++) {
        printf("%c[%d;%dH┃", ESC_SEQ, i, g->pos_x);
    }
    for(i=t->pos_y+1; i<t->pos_y+t->lines+1; i++) {
        printf("%c[%d;%dH┃", ESC_SEQ, i, t->pos_x);
        printf("%c[%d;%dH┃", ESC_SEQ, i, t->pos_x+t->cols+2);
    }
    for(i=b->pos_y+1; i<b->pos_y+b->lines+1; i++) {
        printf("%c[%d;%dH┃", ESC_SEQ, i, b->pos_x);
        printf("%c[%d;%dH┃", ESC_SEQ, i, b->pos_x+b->cols+2);
    }
    for(i=l->pos_y+1; i<l->pos_y+l->lines+1; i++) {
        printf("%c[%d;%dH┃", ESC_SEQ, i, l->pos_x);
        printf("%c[%d;%dH┃", ESC_SEQ, i, l->pos_x+l->cols+2);
    }
    for(i=l->pos_x+1; i<l->pos_x+l->cols+2; i++) {
        printf("%c[%d;%dH━", ESC_SEQ, t->pos_y, i);
        printf("%c[%d;%dH━", ESC_SEQ, l->pos_y, i);
        printf("%c[%d;%dH━", ESC_SEQ, b->pos_y, i);
        printf("%c[%d;%dH━", ESC_SEQ, b->pos_y+b->lines+1, i);
    }
    printf("%c[%d;%dH┳", ESC_SEQ, t->pos_y, t->pos_x);
    printf("%c[%d;%dH┻", ESC_SEQ, b->pos_y+b->lines+1, b->pos_x);
    printf("%c[%d;%dH┫", ESC_SEQ, l->pos_y, l->pos_x+l->cols+2);
    printf("%c[%d;%dH┫", ESC_SEQ, b->pos_y, b->pos_x+b->cols+2);
    printf("%c[%d;%dH┣", ESC_SEQ, l->pos_y, l->pos_x);
    printf("%c[%d;%dH┣", ESC_SEQ, b->pos_y, b->pos_x);

    *game = g;
    *lateral = l;
    *title = t;
    *bottom = b;

    return OK;
}

/* Devuelve el número de columnas de la ventana indicada */
int winGetCols(Window* win) {
    if (win==NULL) return ERR;

    return win->cols;
}

/* Imprime en la ventana win la información almacenada en output. */
int print(Window* win, Bool adjustStr, char *output) {
    char **rStr;
    int n, i;

    if (output==NULL || win==NULL) return ERR;

    if (strlen(output) > win->cols && adjustStr) {
        rStr = adjustString(output, win->cols, &n);
        if (rStr==NULL) return ERR;
        for (i=0; i<n; i++) {
            printf("%c%s%c[%d;%dH%s", ESC_SEQ, RESET_POS, ESC_SEQ, win->pos_y + 1 + i, win->pos_x + 1, rStr[i]);
            free(rStr[i]);
        }
        free(rStr);
        return i;
    }

    printf("%c%s%c[%d;%dH%s", ESC_SEQ, RESET_POS, ESC_SEQ, win->pos_y + 1, win->pos_x + 1, output);
    fflush(stdout);
    return 1;
}

/* Función para imprimir la información almacenada en output en la ventana deseada (win) */
/* y en una posición en concreto indicada por las coordenadas x, y.*/
int printPos(Window* win, int x, int y, Bool adjustStr, char *output) {
    char *sStr;
    char **rStr;
    int sLen, l, n, i, lSpc;

    if (win==NULL || x<=0 || y<=0 || output==NULL) return ERR;
    if (x > win->cols || y > win->lines) return ERR;

    sLen = win->cols - x + 1;
    l = strlen(output);
    if (l > sLen && adjustStr) {
        for (i=0; i<sLen; i++) {
            if (output[i]==' ') lSpc=i;
        }
        i = 0;
        sStr = (char*) malloc((lSpc+1)*sizeof(char));
        if (sStr==NULL) return ERR;
        strncpy(sStr, output, lSpc);
        sStr[lSpc] = '\0';
        printf("%c%s%c[%d;%dH%s", ESC_SEQ, RESET_POS, ESC_SEQ, win->pos_y + y, win->pos_x + x, sStr);
        free(sStr);
        if ((l - (win->cols -x)) > win->cols) {
            rStr = adjustString(&output[lSpc+1], win->cols, &n);
            if (rStr==NULL) return ERR;
            for (i=0; i<n; i++) {
                printf("%c%s%c[%d;%dH%s", ESC_SEQ, RESET_POS, ESC_SEQ, win->pos_y + y + i + 1, win->pos_x + 1, rStr[i]);
                free(rStr[i]);
            }
            free(rStr);
        }
        else {
            printf("%c%s%c[%d;%dH%s", ESC_SEQ, RESET_POS, ESC_SEQ, win->pos_y + y + 1, win->pos_x + 1, &output[lSpc+1]);
            i = 1;
        }
        fflush(stdout);
        return y + i;
    }
    printf("%c%s%c[%d;%dH%s", ESC_SEQ, RESET_POS, ESC_SEQ, win->pos_y + y, win->pos_x + x, output);
    fflush(stdout);
    return y;
}


/* Función para imprimir un texto (almacenado en output) con los colores que se deseen */
/* tanto en primer plano (fg) como en el fondo (bg) en la ventana win. */
int printColor(Window *win, Color fg, Color bg, Bool adjustStr, char *output) {
    char **rStr;
    char colorCode[9];
    int n, i;

    if (output==NULL || win==NULL) return ERR;

    setColorCode(colorCode, fg, bg);

    if (strlen(output) > win->cols && adjustStr) {
        rStr = adjustString(output, win->cols, &n);
        if (rStr==NULL) return ERR;
        for (i=0; i<n; i++) {
            printf("%c%s%c[%d;%dH%s%s%c%s", ESC_SEQ, RESET_POS, ESC_SEQ, win->pos_y + 1 + i, win->pos_x + 1, colorCode, rStr[i], ESC_SEQ, RESET_COLOR);
            free(rStr[i]);
        }
        free(rStr);
        fflush(stdout);
        return i;
    }

    printf("%c%s%c[%d;%dH%s%s%c%s", ESC_SEQ, RESET_POS, ESC_SEQ, win->pos_y + 1, win->pos_x + 1, colorCode, output, ESC_SEQ, RESET_COLOR);
    fflush(stdout);
    return 1;
}

/* Función para imprimir en la ventana win la cadena almacenada en output */
/* con los colores de foreground y background elegidos. */
int printColorPos(Window* win, Color fg, Color bg, int x, int y, Bool adjustStr, char *output) {
    char *sStr;
    char **rStr;
    char colorCode[9];
    int sLen, l, n, i, lSpc;

    if (win==NULL || x<=0 || y<=0 || output==NULL) return ERR;
    if (x > win->cols || y > win->lines) return ERR;

    setColorCode(colorCode, fg, bg);

    sLen = win->cols - x + 1;
    l = strlen(output);
    if (l > sLen && adjustStr) {
        for (i=0; i<sLen; i++) {
            if (output[i]==' ') lSpc=i;
        }
        i = 0;
        sStr = (char*) malloc((lSpc+1)*sizeof(char));
        if (sStr==NULL) return ERR;
        strncpy(sStr, output, lSpc);
        sStr[lSpc] = '\0';
        printf("%c%s%c[%d;%dH%s%s%c%s", ESC_SEQ, RESET_POS, ESC_SEQ, win->pos_y + y, win->pos_x + x, colorCode, sStr, ESC_SEQ, RESET_COLOR);
        free(sStr);
        if ((l - (win->cols -x)) > win->cols) {
            rStr = adjustString(&output[lSpc+1], win->cols, &n);
            if (rStr==NULL) return ERR;
            for (i=0; i<n; i++) {
                printf("%c%s%c[%d;%dH%s%s%c%s", ESC_SEQ, RESET_POS, ESC_SEQ, win->pos_y + y + i + 1, win->pos_x + 1, colorCode, rStr[i], ESC_SEQ, RESET_COLOR);
                free(rStr[i]);
            }
            free(rStr);
        }
        else {
            printf("%c%s%c[%d;%dH%s%s%c%s", ESC_SEQ, RESET_POS, ESC_SEQ, win->pos_y + y + 1, win->pos_x + 1, colorCode, &output[lSpc+1], ESC_SEQ, RESET_COLOR);
            i = 1;
        }
        fflush(stdout);
        return y + i;
    }

    printf("%c%s%c[%d;%dH%s%s%c%s", ESC_SEQ, RESET_POS, ESC_SEQ, win->pos_y + y, win->pos_x + x, colorCode, output, ESC_SEQ, RESET_COLOR);
    fflush(stdout);
    return y;
}

/* Esta función sirve para imprimir de manera centrada respecto al eje horizontal en */
/* la ventana win el texto contenido en output con los colores deseados a la altura  */
/* de la coordenada y. */
int printXCentered(Window* win, Color fg, Color bg, int y, char *output) {
    int len;

    if (win==NULL || y<=0 || output==NULL) return ERR;

    len = strlen(output);

    if (len > win->cols) return printColor(win, fg, bg, FALSE, output);
    return printColorPos(win, fg, bg, (win->cols/2) - (len/2) + 1, y, FALSE, output);
}

/* Está función en similar a la anterior. La diferencia es que escribe en el medio */
/* de la ventana respecto a los dos ejes, quedando el texto totalmente centrado. */
int printCentered(Window* win, Color fg, Color bg, char *output) {
    int len, y;

    if (win==NULL || output==NULL) return ERR;

    len = strlen(output);
    if (win->lines<=5) y = 1;
    else y = (win->lines/2) - 2;

    if (len > win->cols) return printColor(win, fg, bg, FALSE, output);
    printColorPos(win, fg, bg, (win->cols/2) - (len/2) + 1, y, FALSE, output);
    return y;
}

/* Función para borrar una línea imprimiendo espacios*/
int clearLine(Window *win, int line) {
    char *blank;
    int i, sum;

    if (win==NULL || line < 1 || line > win->lines) return ERR;

    blank = (char*) malloc((win->cols+1)*sizeof(char));
    if (blank==NULL) return ERR;
    for (i=0; i<win->cols; i++) {
        blank[i] = ' ';
    }
    blank[i] = '\0';
    sum = printf("%c%s%c[%d;%dH%s", ESC_SEQ, RESET_POS, ESC_SEQ, win->pos_y + line, win->pos_x + 1, blank);
    free(blank);
    fflush(stdout);
    return sum;
}

/* Función para borrar una ventana imprimiendo espacios*/
int clearWin(Window *win) {
    char *blank;
    int i, sum;

    if (win==NULL) return ERR;

    blank = (char*) malloc((win->cols+1)*sizeof(char));
    if (blank==NULL) return ERR;
    for (i=0; i<win->cols; i++) {
        blank[i] = ' ';
    }
    blank[i] = '\0';
    for (i=1; i<=win->lines; i++) {
        sum += printf("%c%s%c[%d;%dH%s", ESC_SEQ, RESET_POS, ESC_SEQ, win->pos_y + i, win->pos_x + 1, blank);
    }
    free(blank);
    fflush(stdout);
    return sum;
}

/* Imprime la imagen pasada a caracteres y almacenada en mapa en la ventana game. */
int setMapa(Window *game, char *mapa) {
    FILE *map;
    char aux;
    int i;

    if (game==NULL || mapa==NULL) return ERR;

    map = fopen(mapa, "r");
    if (map==NULL) return ERR;

    pthread_mutex_lock(&lock1);

    i = 1;
    printf("%c[%d;%dH ", ESC_SEQ, game->pos_y + i, game->pos_x + 1);
    aux = fgetc(map);
    while (aux!=EOF) {
        if (aux=='\n') {
            i++;
            printf(" %c[%d;%dH ", ESC_SEQ, game->pos_y + i, game->pos_x + 1);
            aux = fgetc(map);
        }
        if (aux==EOF) break;
        printf("%c", aux);
        aux = fgetc(map);
    }
    printf("%c[%d;%dH━", ESC_SEQ, game->pos_y + i, game->pos_x + 1);

    fclose(map);

    pthread_mutex_unlock(&lock1);

    return OK;
}


/* Imprime el texto de narración almacenado en path en la ventana win.*/
int printFileStr(Window *win, char *path) {
    FILE *f;
    char line[512];
    int i, l;

    if (win==NULL) return ERR;

    if (strcmp(path, "")==0) return 0;

    f = fopen(path, "r");
    if (f==NULL) return ERR;

    for (i=1; fgets(line, 512, f)!=NULL; i++) {
        l = strlen(line);
        if (line[l-1]=='\n') line[l-1] = '\0';
        i = printPos(win, 1, i, TRUE, line);
    }

    fclose(f);
    return i;
}

/* Se usa para imprimir los títulos almacenados en path en la ventana win */
int printFile(Window *win, char *path) {
    FILE *f;
    char line[512];
    int i, l;

    if (win==NULL) return -1;

    if (strcmp(path, "")==0) return 0;

    f = fopen(path, "r");
    if (f==NULL) return -1;

    for (i=1; fgets(line, 512, f)!=NULL; i++) {
        l = strlen(line);
        if (line[l-1]=='\n') line[l-1] = '\0';
        printf("%c%s%c[%d;%dH%s",ESC_SEQ, RESET_POS, ESC_SEQ, win->pos_y + i,win->pos_x + 2, line);
    }

    fclose(f);
    return 0;
}

/* Función para limpiar todas las ventanas. */
int clearAll() {
    int res;

    res = printf("%c[2J%c[1;1H", ESC_SEQ, ESC_SEQ);
    fflush(stdout);
    return res;
}


/* Función para actualizar una ventana. Para ello, primero limpiamos la */
/* ventana correspondiente y después imprimimos la nueva información.   */
int updateWindow(Window *win) {
    int l;

    clearWin(win);

    if (filePath==NULL || strcmp(filePath, "")==0) return ERR;

    l = printFileStr(win, filePath);

    return l;
}

/* Cambia el fichero actual de la ventana win (filePath) por el nuevo fichero */
/* path en caso de no ser el mismo. */
int setWindowFile(Window *win, char *path) {
    int l;

    if (win==NULL || path == NULL || strcmp(path, "")==0 || strlen(path) >= PATH_LEN) return ERR;

    l = OK;
    /* Si el fichero de la ventana ha cambiado imprimirlo, sino no hace falta */
    if (strcmp(filePath, path) != 0) {
        strcpy(filePath, path);
        l = updateWindow(win);
    }

    return l;
}