/* GRUPO: COVidoneos */

/* FICHERO CON LAS FUNCIONES DE LAS 13 ESCENAS                      */

/* Las funciones dibujarEscena imprimen el texto correspondiente    */
/* de la historia en la ventana lateral, la imagen de la escena     */
/* que esta guardada en el txt correspondiente y en la ventana      */
/* title se imprime el lugar en el que os encontramos.              */

/* Tanto el texto de la historia como la imagen de la escena se     */
/* encuentran en archivos txt. Para imprimir el texto, usamos la    */
/* función setLateralPath que se encuentra en personaje.c; y la     */
/* escena la imprimimos con la función setMapa que se encuentra en  */
/* personaje.c.                                                     */
/* Mientras que el título lo imprimimos directamente con la función */
/* printCentered definida en el fichero print_helper.c.             */

/* Las funciones updateEscena sirven para llamar a la siguiente     */
/* escena tecleando la tecla 'n'. Pero para poder pasar de escena   */
/* pulsando esta tecla, es necsario que el usuario realice ciertas  */
/* "pruebas". Además, algunas de estas funciones cambiarán el texto */
/* que aparece en la ventana lateral para guiar al usuario y que    */
/* pueda seguir la historia.                                        */

/* AÑADIR COMETARIOS EN CADA FUNCIÓN */

#include <stdio.h>
#include <stdlib.h>
#include "escenas.h"

static int flag;
static int elegida;
static int correcta;

/* Para acabar el juego */
/* *iescena = -1        */

void dibujarEscena0(int *iescena, Window *game, Window *lateral, Window *title) {

    if(game==NULL||title==NULL||lateral==NULL){
        *iescena=-1;
        return;
    }

    flag = FALSE;

    setWindowFile(lateral, "./res/text/inicio.txt");

    clearWin(title);
    printFile(title, "./res/text/title_escena0.txt");

    setMapa(game, "./res/maps/hospital.txt");

    return;
}

void updateEscena0(int *iescena, char *input, Window *game, Window *title, Window *lateral, Personaje *prota) {

    if(iescena==NULL||game==NULL||title==NULL||lateral==NULL||prota==NULL || input==NULL){
        *iescena=-1;
        return;
    }

    if (*input=='o' || *input=='O') {
        setWindowFile(lateral, "./res/text/email.txt");
        flag = TRUE;
        clearInput(input);
    }


    /* Para pasar a la siguiente escena */
    if (flag==TRUE && (*input=='n' || *input=='N')) {
        (*iescena)++; 
        dibujarEscena1(iescena, game, lateral, title);
        clearInput(input);
    }

    return;
}


void dibujarEscena1(int *iescena, Window *game, Window *lateral, Window *title) {

    if(game==NULL||title==NULL||lateral==NULL){
        *iescena=-1;
        return;
    }

    /* flag indica si se ha conseguido el reto */
    flag = FALSE;

    setWindowFile(lateral, "./res/text/madrid.txt");

    clearWin(title);
    printFile(title,"./res/text/title_escena1.txt");

    setMapa(game, "./res/maps/aeropuerto.txt");

    
    return;
}

void updateEscena1(int *iescena, char *input, Window *game, Window *title, Window *lateral, Personaje *prota) {
    
    if(iescena==NULL||game==NULL||title==NULL||lateral==NULL||prota==NULL || input==NULL){
        *iescena=-1;
        return;
    }

    /* Comprobamos que se ha usado la mascarilla y que el valor introducido tiene sentido */
    if (*input>='1' && *input<='4') {
        if (ObjectIsUsed(getObject(prota, MASCARILLA))) {
            /* Indicamos que se ha conseguido el reto */
            flag = TRUE;
            setWindowFile(lateral, "./res/text/next.txt");
        }
        /* Si se usa otro objeto del maletín bajamos la salud */
        else if (*input!='1'){
            bajarSalud(prota);
        }
    }
    
    /* Para pasar a la siguiente escena */
    if (flag && (*input=='n' || *input=='N')) {
        (*iescena)++; 
        dibujarEscena2(iescena, game, lateral, title);
        clearInput(input);
    }

    return;
}


void dibujarEscena2(int *iescena, Window *game, Window *lateral, Window *title) {
    
    if(game==NULL||title==NULL||lateral==NULL){
        *iescena=-1;
        return;
    }

    clearWin(title);
    printFile(title, "./res/text/title_escena2.txt");

    setMapa(game, "./res/maps/aeropuerto_it.txt");

    return;
}

void updateEscena2(int *iescena, char *input, Window *game, Window *title, Window *lateral, Personaje *prota) {

    if(iescena==NULL||game==NULL||title==NULL||lateral==NULL||prota==NULL || input==NULL){
        *iescena=-1;
        return;
    }

    setWindowFile(lateral, "./res/text/italia1.txt");
    
    /* Comprobamos que se ha usado la PCR y que el valor introducido tiene sentido */
    if (*input>='1' && *input<='4') {
        if (ObjectIsUsed(getObject(prota, PCR))) {
            /* Indicamos que se ha conseguido el reto */
            flag = TRUE;
            setWindowFile(lateral, "./res/text/next.txt");
        }
        /* Si se usa otro objeto del maletín bajamos la salud */
        else if (*input!='2'){
            bajarSalud(prota);
        }
    }
    
    /* Para pasar a la siguiente escena */
    if (flag && (*input=='n' || *input=='N')) {
        (*iescena)++; 
        dibujarEscena3(iescena, game, lateral, title);
        clearInput(input);
    }
    
    return;
}


void dibujarEscena3(int *iescena, Window *game, Window *lateral, Window *title) {

    if(game==NULL||title==NULL||lateral==NULL){
        *iescena=-1;
        return;
    }

    clearWin(title);
    printFile(title, "./res/text/title_escena3.txt");

    setMapa(game, "./res/maps/beirut.txt");
    
    return;
}

void updateEscena3(int *iescena, char *input, Window *game, Window *title, Window *lateral, Personaje *prota) {

    if(iescena==NULL||game==NULL||title==NULL||lateral==NULL||prota==NULL || input==NULL){
        *iescena=-1;
        return;
    }

    setWindowFile(lateral, "./res/text/libano1.txt");

    /* Para pasar a la siguiente escena */
    if (*input=='n' || *input=='N') {
        (*iescena)++; 
        dibujarEscena4(iescena, game, lateral, title);
        clearInput(input);
    }
    
    return;
}


void dibujarEscena4(int *iescena, Window *game, Window *lateral, Window *title) {

    if(game==NULL||title==NULL||lateral==NULL){
        *iescena=-1;
        return;
    }

    /* flag indica si el reto está conseguido */
    flag = FALSE;

    clearWin(title);
    printFile(title, "./res/text/title_escena3.txt");

    setMapa(game, "./res/maps/fabrica.txt");
    
    setWindowFile(lateral, "./res/text/libano2.txt");

    return;
}

void updateEscena4(int *iescena, char *input, Window *game, Window *title, Window *lateral, Personaje *prota) {

    int lastLine, contador=0, i;
    char output[STR_LEN];
    char serie[4] = {'_', '_', '_', '_'};
    int blink[4] = { 5, 25, 25, 25 };
    char inp;
    
    if(iescena==NULL||game==NULL||title==NULL||lateral==NULL||prota==NULL || input==NULL){
        *iescena=-1;
        return;
    }

    if (flag==FALSE && (*input=='p' || *input=='P')) {
        clearWin(game);
        setMapa(game, "./res/maps/papel.txt");
        lastLine = setWindowFile(lateral, "./res/text/libano_num.txt");
        clearInput(input);

        /* Mientras la serie introducida no sea correcta */
        while (flag==FALSE) {
            /* Mientras el usuario introduce los valores */
            while (contador<4) {
                sprintf(output, "%c[%dm%c%c[%dm-%c[%dm%c%c[%dm-%c[%dm%c%c[%dm-%c[%dm%c%c[%dm",  ESC_SEQ, blink[0], serie[0], ESC_SEQ, 25,
                                                                                                ESC_SEQ, blink[1], serie[1], ESC_SEQ, 25,
                                                                                                ESC_SEQ, blink[2], serie[2], ESC_SEQ, 25,
                                                                                                ESC_SEQ, blink[3], serie[3], ESC_SEQ, 25);
                printPos(lateral, 1, lastLine + 1, FALSE, output);
                inp = getc(stdin);
                /* Si no se introducen números */
                while (inp<'1' && inp>'9') {
                    printColorPos(lateral, COLOR_RED, COLOR_BLACK, 1, lastLine, TRUE, getIncorrectoStr());
                    inp = getc(stdin);
                }
                clearLine(lateral, lastLine);
                serie[contador] = inp;
                blink[contador] = 25;
                if (contador<3) blink[contador+1] = 5;
                contador++;
            }

            /* Comprobar que la serie introducida por el usuario es la correcta */ 
            /* Cambio de flag a TRUE, se ha conseguido el reto */
            if (serie[0]=='6' && serie[1]=='7' && serie[2]=='3' && serie[3]=='1') {
                flag = TRUE;
                clearWin(game);
                setMapa(game, "./res/maps/fabrica2.txt");
                setWindowFile(lateral, "./res/text/libano3.txt");
            }
            /* Si los números no coinciden, reasignamos valores y quitamos vida */
            else {
                if (bajarSalud(prota)) break;
                printColorPos(lateral, COLOR_RED, COLOR_BLACK, 1, lastLine, TRUE, getIncorrectoStr());
                for(i=0; i<4; i++) {
                    serie[i]='_';
                }
                blink[0]=5;
                for(i=1; i<4; i++) {
                    blink[i]=25;
                }
                contador=0;
            }
        }
    }   

    /* Para pasar a la siguiente escena */
    if (flag==TRUE && (*input=='n' || *input=='N')) {
        (*iescena)++; 
        dibujarEscena5(iescena, game, lateral, title);
        clearInput(input);
    }

    return;
}


void dibujarEscena5(int *iescena, Window *game, Window *lateral, Window *title) {

    if(game==NULL||title==NULL||lateral==NULL){
        *iescena=-1;
        return;
    }

    clearWin(title);
    printFile(title, "./res/text/title_escena5.txt");

    setMapa(game, "./res/maps/cartel_santini.txt");
    
    return;
}

void updateEscena5(int *iescena, char *input, Window *game, Window *title, Window *lateral, Personaje *prota) {
    
    if(iescena==NULL||game==NULL||title==NULL||lateral==NULL||prota==NULL || input==NULL){
        *iescena=-1;
        return;
    }

    setWindowFile(lateral, "./res/text/rusia1.txt");

    /* Para obtener más información y se pasa a la siguiente escena */
    if (*input=='i' || *input=='I') {
        (*iescena)++; 
        dibujarEscena6(iescena, game, lateral, title);
        clearInput(input);
    }
    
    return;
}


void dibujarEscena6(int *iescena, Window *game, Window *lateral, Window *title) {

   if(game==NULL||title==NULL||lateral==NULL){
        *iescena=-1;
        return;
    }

    /* flag indica si el reto está conseguido */
    flag = FALSE; 

    clearWin(title);
    printFile(title, "./res/text/title_escena5.txt");

    setMapa(game, "./res/maps/rusia.txt");
    setWindowFile(lateral, "./res/text/rusia2.txt");

    return;
}


void updateEscena6(int *iescena, char *input, Window *game, Window *title, Window *lateral, Personaje *prota) {

    int lastLine, contador=0, i;
    char output[STR_LEN];
    char serie[5] = {'_', '_', '_', '_', '_'};
    int blink[5] = { 5, 25, 25, 25, 25};
    char inp;
    
    if(iescena==NULL||game==NULL||title==NULL||lateral==NULL||prota==NULL || input==NULL){
        *iescena=-1;
        return;
    }

    if (flag==FALSE && (*input=='p' || *input=='P')) {
        lastLine = setWindowFile(lateral, "./res/text/rusia3.txt");
        clearInput(input);

        /* Mientras la serie introducida no sea correcta */
        while (flag==FALSE) {
            /* Mientras el usuario introduce los valores */
            while (contador<5) {
                sprintf(output, "%c[%dm%c%c[%dm%c[%dm%c%c[%dm%c[%dm%c%c[%dm%c[%dm%c%c[%dm%c[%dm%c%c[%dm",  ESC_SEQ, blink[0], serie[0], ESC_SEQ, 25,
                                                                                                ESC_SEQ, blink[1], serie[1], ESC_SEQ, 25,
                                                                                                ESC_SEQ, blink[2], serie[2], ESC_SEQ, 25,
                                                                                                ESC_SEQ, blink[3], serie[3], ESC_SEQ, 25,
                                                                                                ESC_SEQ, blink[4], serie[4], ESC_SEQ, 25);
                printPos(lateral, 1, lastLine + 1, FALSE, output);
                inp = getc(stdin);
                /* Comprobamos que se han introducido letras */
                while ((inp<'a' && inp>'z') && (inp<'A' && inp>'Z') ) {
                    printColorPos(lateral, COLOR_RED, COLOR_BLACK, 1, lastLine, TRUE, getIncorrectoStr());
                    inp = getc(stdin);
                }
                clearLine(lateral, lastLine);
                serie[contador] = inp;
                blink[contador] = 25;
                if (contador<3) blink[contador+1] = 5;
                contador++;
            }

            /* Comprobar que la serie introducida por el usuario es la correcta */ 
            if ((serie[0]=='c' || serie[0]=='C') && (serie[1]=='h' || serie[1]=='H') && (serie[2]=='i' || serie[2]=='I') && (serie[3]=='n' || serie[3]=='N') && (serie[4]=='a' || serie[4]=='A')) {
                flag = TRUE;
                setWindowFile(lateral, "./res/text/next.txt");
            }
            /* Si los números no coinciden, reasignamos valores y quitamos vida*/
            else {
                if (bajarSalud(prota)) break;
                printColorPos(lateral, COLOR_RED, COLOR_BLACK, 1, lastLine, TRUE, getIncorrectoStr());
                for(i=0; i<5; i++) {
                    serie[i]='_';
                }
                blink[0]=5;
                for(i=1; i<5; i++) {
                    blink[i]=25;
                }
                contador=0;
            }
        }
    }   

    /* Para pasar a la siguiente escena */
    if (flag==TRUE && (*input=='n' || *input=='N')) {
        (*iescena)++; 
        dibujarEscena7(iescena, game, lateral, title);
        clearInput(input);
    }

    return;
}

void dibujarEscena7(int *iescena, Window *game, Window *lateral, Window *title) {

    if(game==NULL||title==NULL||lateral==NULL){
        *iescena=-1;
        return;
    }


    clearWin(title);
    printFile(title, "./res/text/title_escena7.txt");

    setMapa(game, "./res/maps/laboratorio.txt");
    
    clearWin(lateral);

    
    return;
}


void updateEscena7(int *iescena, char *input, Window *game, Window *title, Window *lateral, Personaje *prota) {
   
   if(iescena==NULL||game==NULL||title==NULL||lateral==NULL||prota==NULL || input==NULL){
        *iescena=-1;
        return;
    }

    setWindowFile(lateral, "./res/text/wuhan1.txt");
    
    /* Comprobamos que se han usado los guantes y que el valor introducido tiene sentido */
    if (*input>='1' && *input<='4') {
        if (ObjectIsUsed(getObject(prota, GUANTES))) {
            /* Indicamos que se ha conseguido el reto */
            flag = TRUE;
            setWindowFile(lateral, "./res/text/next.txt");
        }
        /* Si se usa otro objeto del maletín bajamos la salud */
        else if (*input!='3'){
            bajarSalud(prota);
        }
    }
    
    /* Para pasar a la siguiente escena */
    if (flag && (*input=='n' || *input=='N')) {
        (*iescena)++; 
        dibujarEscena8(iescena, game, lateral, title);
        clearInput(input);
    }

    return;
}

void dibujarEscena8(int *iescena, Window *game, Window *lateral, Window *title) {

    if(game==NULL||title==NULL||lateral==NULL){
        *iescena=-1;
        return;
    }

    clearWin(title);
    printFile(title, "./res/text/title_escena7.txt");

    setMapa(game, "./res/maps/lab_guantes.txt");

    /* flag indica si el reto está conseguido */
    flag = FALSE; 
    
    clearWin(lateral);

    return;
}


void updateEscena8(int *iescena, char *input, Window *game, Window *title, Window *lateral, Personaje *prota) {

    int lastLine, contador=0, i;
    char output[STR_LEN];
    char serie[3] = {'_', '_', '_'};
    int blink[3] = { 5, 25, 25};
    char inp;

    if(iescena==NULL||game==NULL||title==NULL||lateral==NULL||prota==NULL || input==NULL){
        *iescena=-1;
        return;
    }

    if (flag==FALSE) {
        lastLine = setWindowFile(lateral, "./res/text/wuhan2.txt");
        clearInput(input);

        /* Mientras la serie introducida no sea correcta */
        while (flag==FALSE) {
            /* Mientras el usuario introduce los valores */
            while (contador<3) {
                sprintf(output, "%c[%dm%c%c[%dm%c[%dm%c%c[%dm%c[%dm%c%c[%dm", ESC_SEQ, blink[0], serie[0], ESC_SEQ, 25,
                                                                              ESC_SEQ, blink[1], serie[1], ESC_SEQ, 25,
                                                                              ESC_SEQ, blink[2], serie[2], ESC_SEQ, 25);
                printPos(lateral, 1, lastLine + 1, FALSE, output);
                inp = getc(stdin);
                /* Comprobamos si se ha introducido letras */
                while ((inp<'a' && inp>'z') && (inp<'A' && inp>'Z')) {
                    printColorPos(lateral, COLOR_RED, COLOR_BLACK, 1, lastLine, TRUE, getIncorrectoStr());
                    inp = getc(stdin);
                }
                clearLine(lateral, lastLine);
                serie[contador] = inp;
                blink[contador] = 25;
                if (contador<2) blink[contador+1] = 5;
                contador++;
            }

            /* Comprobar que la serie introducida por el usuario es la correcta */ 
            if ((serie[0]=='e' || serie[0]=='E') && (serie[1]=='l' || serie[1]=='L') && (serie[2]=='s' || serie[2]=='S')){
                flag = TRUE;
                clearWin(game);
                setMapa(game, "./res/maps/cofre.txt");
                setWindowFile(lateral, "./res/text/wuhan3.txt");
            }
            /* Si las letras no coinciden, reasignamos valores y quitamos vida*/
            else {
                if (bajarSalud(prota)) break;
                printColorPos(lateral, COLOR_RED, COLOR_BLACK, 1, lastLine, TRUE, getIncorrectoStr());
                for(i=0; i<3; i++) {
                    serie[i]='_';
                }
                blink[0]=5;
                for(i=1; i<3; i++) {
                    blink[i]=25;
                }
                contador=0;
            }
        }
    }   

    /* Para pasar a la siguiente escena */
    if (flag==TRUE && (*input=='n' || *input=='N')) {
        (*iescena)++; 
        dibujarEscena9(iescena, game, lateral, title);
        clearInput(input);
    }


    
    return;
}

void dibujarEscena9(int *iescena, Window *game, Window *lateral, Window *title) {


    if(game==NULL||title==NULL||lateral==NULL){
        *iescena=-1;
        return;
    }

    clearWin(title);
    printFile(title, "./res/text/title_escena7.txt");

    setMapa(game, "./res/maps/contagio.txt"); 

    return;
}


void updateEscena9(int *iescena, char *input, Window *game, Window *title, Window *lateral, Personaje *prota) {
    
     if(iescena==NULL||game==NULL||title==NULL||lateral==NULL||prota==NULL || input==NULL){
        *iescena=-1;
        return;
    }

    setWindowFile(lateral, "./res/text/wuhan4.txt");
    
   /* Comprobamos que se ha usado el gel y que el valor introducido tiene sentido */
    if (*input>='1' && *input<='4') {
        if (ObjectIsUsed(getObject(prota, GEL_HIDROALCOHOLICO))) {
            /* Indicamos que se ha conseguido el reto */
            flag = TRUE;
            setWindowFile(lateral, "./res/text/next.txt");
        }
        /* Si se usa otro objeto del maletín bajamos la salud */
        else if (*input!='4'){
            bajarSalud(prota);
        }
    }
    
    /* Para pasar a la siguiente escena */
    if (flag && (*input=='n' || *input=='N')) {
        (*iescena)++; 
        dibujarEscena10(iescena, game, lateral, title);
        clearInput(input);
    }

    return;
}


void dibujarEscena10(int *iescena, Window *game, Window *lateral, Window *title) {

    if(game==NULL||title==NULL||lateral==NULL){
        *iescena=-1;
        return;
    }

    /* flag indica si el reto está conseguido */
    flag = FALSE; 
    
    clearWin(title);
    printFile(title, "./res/text/title_escena7.txt");

    setMapa(game, "./res/maps/leer.txt"); 

    
    return;
}


void updateEscena10(int *iescena, char *input, Window *game, Window *title, Window *lateral, Personaje *prota) {
    int lastLine=0;
    char inp;
    
    if(iescena==NULL||game==NULL||title==NULL||lateral==NULL||prota==NULL || input==NULL){
        *iescena=-1;
        return;
    }

    lastLine = setWindowFile(lateral, "./res/text/wuhan6.txt");

    /* Mientras no se haya conseguido el reto */
    while (flag==FALSE) {
        inp = getc(stdin);
        switch (inp) {
            
            /* Opción correcta, cambia bandera a TRUE */
            case 'a':
            case 'A':
                clearLine(lateral, lastLine);
                flag = TRUE;
                setWindowFile(lateral, "./res/text/next.txt");
                break;

            /* Opción incorrecta, baja salud e imprime mensaje de incorrecto */
            case 'b':
            case 'B':
                printColorPos(lateral, COLOR_RED, COLOR_BLACK, 1, lastLine, TRUE, getIncorrectoStr());
                if (bajarSalud(prota)) return;
                break;

            /* Opción incorrecta, baja salud e imprime mensaje de incorrecto */
            case 'c':
            case 'C':
                printColorPos(lateral, COLOR_RED, COLOR_BLACK, 1, lastLine, TRUE, getIncorrectoStr());
                if (bajarSalud(prota)) return;
                break;
    
            default:
                printColorPos(lateral, COLOR_RED, COLOR_BLACK, 1, lastLine, TRUE, getIncorrectoStr());
                break;
        }
    }
    
    /* Para pasar a la siguiente escena */
    if (flag==TRUE && (*input=='n' || *input=='N')){
        (*iescena)++;
        dibujarEscena11(iescena, game, lateral, title);
        clearInput(input);
    }

    return;
}


void dibujarEscena11(int *iescena, Window *game, Window *lateral, Window *title) {

    if(game==NULL||title==NULL||lateral==NULL){
        *iescena=-1;
        return;
    }

    setMapa(game, "./res/maps/torre.txt");

    
    clearWin(title);
    printFile(title, "./res/text/title_escena7.txt");

    setWindowFile(lateral, "./res/text/wuhan7.txt");

    return;  
}


void updateEscena11(int *iescena, char *input, Window *game, Window *title, Window *lateral, Personaje *prota) {
          
    if(iescena==NULL||game==NULL||title==NULL||lateral==NULL||prota==NULL || input==NULL){
        *iescena=-1;
        return;
    }

    /* Para pasar a la siguiente escena */
    if (*input=='n' || *input=='N'){
        (*iescena)++;
        dibujarEscena12(iescena, game, lateral, title);
        clearInput(input);
    }

    return;
}

void dibujarEscena12(int *iescena, Window *game, Window *lateral, Window *title) {

    if(game==NULL||title==NULL||lateral==NULL){
        *iescena=-1;
        return;
    }

    setMapa(game, "./res/maps/puertas_nada.txt");

    setWindowFile(lateral, "./res/text/wuhan8.txt");

    
    clearWin(title);
    printFile(title, "./res/text/title_escena7.txt");
    
    /* Para diferenciar entre la primera elección o la segunda en la siguiente función */
    flag = FALSE;
    srand (time(NULL));
    /* Entre 0 y 2, se escoge aleatoriamente cuál será la puerta correcta */
    correcta = rand()*3/RAND_MAX; 

    return;
}


void updateEscena12(int *iescena, char *input, Window *game, Window *title, Window *lateral, Personaje *prota) {

    if(iescena==NULL||game==NULL||title==NULL||lateral==NULL||prota==NULL || input==NULL){
        *iescena=-1;
        return;
    }

    /* Según la puerta que haya salido correcta aleatoriamente y la elección del usuario los casos posibles son: */
    /* Primera elección */
    if (flag == FALSE) {
        switch (*input)  {
            case 'a':
            case 'A':
                elegida = 0;
                if (correcta==1) setMapa(game, "./res/maps/puertas_der.txt");
                else setMapa(game, "./res/maps/puertas_centro.txt");
                break;

            case 'v':
            case 'V':
                elegida = 1;
                if (correcta==0) setMapa(game, "./res/maps/puertas_der.txt");
                else setMapa(game, "./res/maps/puertas_izq.txt");
                break;

            case 'r':
            case 'R':
                elegida = 2;
                if (correcta==0) setMapa(game, "./res/maps/puertas_centro.txt");
                else setMapa(game, "./res/maps/puertas_izq.txt");
                break;

            default:
                clearInput(input);
                return;
        }
        setWindowFile(lateral, "./res/text/wuhan9.txt");
        flag = TRUE;
        clearInput(input);
        return;
    }
    /* Segunda elección */
    else {
        /* Si se queire cambiar */
        if (*input=='c' || *input=='C') {
            if (elegida==0) {
                if (correcta==1) elegida = 1;
                else elegida = 2;
            }
            else if (elegida==1) {
                if (correcta==0) elegida = 0;
                else elegida = 2;
            }
            else {
                if (correcta==0) elegida = 0;
                else elegida = 1;
            }
            clearInput(input);
        }
        /* Si se mantiene la elección */
        else if (*input=='s' || *input=='S') {
            clearInput(input);
        }
        /* Si pulsa una tecla que no es correcta */
        else {
            clearInput(input);
            return;
        }

        /* Si se ha acertado, se pasa a la siguiente escena */
        if (correcta==elegida) {
            (*iescena)++;
            dibujarEscena13(iescena, game, lateral, title);
            return;
        }
        bajarSalud(prota);
        bajarSalud(prota);
        bajarSalud(prota);
        dibujarEscena12(iescena, game, lateral, title);
        return;
    }
    
    return;
}


void dibujarEscena13(int *iescena, Window *game, Window *lateral, Window *title){

    if(game==NULL||title==NULL||lateral==NULL){
        *iescena=-1;
        return;
    }

    setMapa(game, "./res/maps/hermano.txt");

    setWindowFile(lateral, "./res/text/final.txt");

    clearWin(title);
    printFile(title, "./res/text/title_escena0.txt");

    return;
}

void updateEscena13(int *iescena, char *input, Window *game, Window *title, Window *lateral, Personaje *prota) {

    if(iescena==NULL||game==NULL||title==NULL||lateral==NULL||prota==NULL || input==NULL){
        *iescena=-1;
        return;
    }

    /* Para finalizar el juego */
    if (*input=='f' || *input=='F'){
        (*iescena)=-1;
    }

    return;
}