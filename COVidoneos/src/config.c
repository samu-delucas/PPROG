/* GRUPO: COVidóneos */

/* FICHERO AUXILIAR PARA LA CONFIGURACIÓN DEL VIDEOJUEGO */

#include "config.h"

#define LINE_LEN 256

typedef struct {
    int time;
    int gameWidth;
    int lateralWidth;
    int ups;
} Config;

Config config;
Bool loaded = FALSE;

char incorrecto[STR_LEN];


/* Función para inicializar la estructura Config de configuración del juego. */
int loadConfig() {
    FILE *confFile;
    char line[LINE_LEN];
    char key[LINE_LEN];
    char value[LINE_LEN];
    int i;

    /* game.config tiene inicializado por defecto:    */
    /* - el ancho de la ventana del juego             */
    /* - el ancho de la ventana lateral               */
    /* - el temporizador (tiempo que dura la partida) */
    /* - el idioma de los textos                      */
    confFile = fopen("./game.config", "r");
    if (confFile==NULL) return ERR;

    /* guarda los valores del fichero game.config en la estructura Config*/
    while (fgets(line, LINE_LEN, confFile)!=NULL) {
        for (i=0; line[i]!='='; i++) {
            key[i] = line[i];
        }
        key[i] = '\0';
        strcpy(value, &line[i+1]);
        if (strcmp(key, "game_width")==0) {
            config.gameWidth = atoi(value);
        }
        else if (strcmp(key, "lateral_width")==0) {
            config.lateralWidth = atoi(value);
        }
        else if (strcmp(key, "game_timer")==0) {
            config.time = atoi(value);
        }
        else if (strcmp(key, "updates_per_second")==0) {
            config.ups = atoi(value);
        }
    }

    /* guarda en la cadena incorrecto el texto del fichero txt correspondiente*/
    loadStr(incorrecto, "./res/text/incorrecto.txt");

    fclose(confFile);
    /* bandera para definir que se ha cargado la estructura*/
    loaded = TRUE;

    return OK;
}

/* Función para obtener el cronómetro configurado. */
int configGetTime() {
    if (loaded==FALSE) return -1;

    return config.time;
}

/* Función para obtener el ancho de la ventana de juego configurado. */
int configGetGameWidth() {
    if (loaded==FALSE) return -1;

    return config.gameWidth;
}

/* Función para obtener el ancho de la ventana lateral configurado. */
int configGetLateralWidth() {
    if (loaded==FALSE) return -1;

    return config.lateralWidth;
}

/* Función para obtener la cadena incorrecto en el idioma correspondiente */
/* Usado, por ejemplo, en la escena 4 si se introduce mal la secuencia de números. */
char *getIncorrectoStr() {
    if (loaded==FALSE) return NULL;

    return incorrecto;
}

/* Esta función almacena en la cadena de caracteres dest el texto contenido  */
/* en el fichero file_src. De esta forma según el idioma en el que se quiera */
/* jugar se escogerá un fichero u otro. */
int loadStr(char *dest, char *file_src) {
    FILE *res;
    int l;

    if (file_src==NULL || dest==NULL) return ERR;

    res = fopen(file_src, "r");
    if (res==NULL) return ERR;
    if (!fgets(dest, STR_LEN, res)) {
        fclose(res);
        return ERR;
    }
    fclose(res);
    l = strlen(dest);
    if (dest[l-1]=='\n') {
        dest[l-1] = '\0';
        l--;
    }

    return l;
}