/*
    Todo este fichero es un menu para mostrar un portada y los menus por pantallas
    e interactuar con el usuario para que pueda ver eun tutorial, pueda cargar una
    partida previamente guardada o comenzar a jugar una partida nueva.
    Esto no tiene ningún misterio, son un monton de switch dentro de un bucle hasta
    que se escoja una opción, algunas funciones que imprimen por pantalla la portada
    y los menus y dos funciones read_key que junto a imprimir la flecha en una posición 
    con % modulo 4 interactuan con el usuario por teclado.
*/

#include <stdio.h>
#include <stdlib.h>      
#include <termios.h>
#include <menu.h>
#include <intrf.h>

#define RANK_TAM 20

/* Funciones Privadas */
/* Realmente todas son privadas menus menu() */
int ShowLoadMenu();
int LoadMenu();
int ShowMainMenu();
void tutorial();

/* 
    bckg: background color
    text: text color

    This funtion print the cover page. The cover page is in inicio.txt
    and it has a specific format.
*/
void printInicio(int bckg, int text){
    FILE *f;
    char c;
    f=fopen("./aux/plantillas/inicio.txt", "r");
    if(f!=NULL){
        /* Preparamos la pantalla */
        system("clear"); 
        while(fscanf(f, "%c", &c)==1){
            if(c=='`' || c==' ' || c==10 || c=='.'){ /* Background */
                printf("\x1b[%dm" " ", bckg);
                fflush(stdout);
            }
            else if(c=='A'){ /* Paciente 0 */
                printf("\x1b[%dm" " ", text); 
                fflush(stdout);
            }
            else{ /* El virus verde */
                printf("\x1b[42m" " "); 
                fflush(stdout);
            }
        }
        printf("\x1b[0m");
        printf("%c[%d;%dH", 27, 33, 100);
        printf("\x1b[%dm" " ", bckg);
        printf("Intoduce un caracter para continuar");
        printf("\x1b[0m");
        printf("%c[%d;%dH", 27, 0, 0);
        
        fclose(f);
    }
}


/*
    Funcion main en menu. Prepara la pantalla, imprime la portada y los menus
    e interactua con el usuario para devolver la opcion escogida por el usuario. 
*/
int menu() {
    int nChoice = 0, load=0;
    char c;

    /* Preparamos la pantalla */
    printf("%c[8;%d;%dt",  27, 35, 145);
    system("clear");    

    /* Imprimimos la portada */
    printInicio(BACKGROUND_COLOR2, TEXT_COLOR2);

    /* Ajustamos la terminal y esperamos a que 
    introduzcan un caracter para continuar */
    /*_term_init();*/
    while(1){
        if(fgetc(stdin)) break;
    }

    do {
        /* ShowMainMenu imprime el menu y devuelve la 
        opcion escogida por el usuario */
        nChoice = ShowMainMenu();
        switch (nChoice) {
            case 1: {
                return 0;
            }
                break;

            case 2: {
                load=LoadMenu();
                /* 1,2,3 partidas a cargar, 4 back y se repite el mainMenu */
                if(load<4 && load>0){
                    return load;
                }
            }
                break;

            case 3: {
                tutorial(40,37);
            }
                break;
            case 4: {
                system("clear");
                printf("Bye Bye\n\n");
                return 4;
            }
                break;
        }
    } while (nChoice != 4);
    return 4;
}


/* 
    bckg: background color
    cursor: color del cursor

    Limpia las posibles posiciones del cursor anterior e
    imprime la flecha en el actual. Hay cuatro posibilidades
    pos=0,1,2,3. Se utiliza tanto en el mainMenu como en loadMenu.
*/
void printFlecha(int pos, int bckg, int cursor){
    char c;
    int i, pos8=pos*8;

        /* Limpiamos las posiciones de las flechas */
        for(i=0; i<32; i+=8){
            printf("%c[%d;%dH", 27, 4+i, 3);
            printf("\x1b[%dm" "                ", bckg);
            printf("%c[%d;%dH", 27, 5+i, 3);
            printf("\x1b[%dm" "                ", bckg);
            printf("%c[%d;%dH", 27, 6+i, 3);
            printf("\x1b[%dm" "                ", bckg);
            printf("%c[%d;%dH", 27, 7+i, 3);
            printf("\x1b[%dm" "                ", bckg);
            printf("%c[%d;%dH", 27, 8+i, 3);
            printf("\x1b[%dm" "                ", bckg);
        }
        /* Imprimimos la flecha en su posicion */
            printf("%c[%d;%dH", 27, 4+pos8, 3);
            printf("\x1b[%dm" "          ", bckg);
            printf("\x1b[%dm" "  ", cursor);
            printf("\x1b[%dm" "  ", bckg);
            printf("%c[%d;%dH", 27, 5+pos8, 3);
            printf("\x1b[%dm" "            ", bckg);
            printf("\x1b[%dm" "  ", cursor);
            printf("\x1b[%dm" " ", bckg);
            printf("%c[%d;%dH", 27, 6+pos8, 3);
            printf("\x1b[%dm" "                ", cursor);
            printf("%c[%d;%dH", 27, 7+pos8, 3);
            printf("\x1b[%dm" "            ", bckg);
            printf("\x1b[%dm" "  ", cursor);
            printf("\x1b[%dm" " ", bckg);
            printf("%c[%d;%dH", 27, 8+pos8, 3);
            printf("\x1b[%dm" "          ", bckg);
            printf("\x1b[%dm" "  ", cursor);
            printf("\x1b[%dm" "  ", bckg);
        
        
        printf("\x1b[0m");
        printf("%c[%d;%dH", 27, 0, 0);  
}

/* 
    bckg: background color
    text: text color

    Imprime en la pantalla el mainMenu (formato en menu.txt).
*/
void printMainMenu(int bckg, int text){
    FILE *f;
    char c;
    f=fopen("./aux/plantillas/menu.txt", "r");
    if(f!=NULL){
        system("clear");
        while(fscanf(f, "%c", &c)==1){
            if(c=='-' || c==10){
                printf("\x1b[%dm" " ", bckg);
                fflush(stdout);
            }
            else if (c=='X'){
                printf("\x1b[%dm" " ", text);
                fflush(stdout);
            }
            else{
                printf("%c", c);
            }
        }
        printf("\x1b[0m");
        printf("%c[%d;%dH", 27, 0, 0);

        fclose(f);
    }
}


#define UP 1
#define DOWN -1
#define NOTHING 0
#define ENTER 10
#define RANKING 5
/*
  Reads a key from the keyboard. If the key is enter it returns ENTER, if it
  is a "regular" (except w and s) key it returns its ascii code; if it is an 
  arrow key, it returns UP, DOWN or ENTER. (left arrow dont do anything). 
  w and s also work as UP and DOWN.
*/
int _read_keyMenu() {
  char choice;
  choice = fgetc(stdin);

    if(choice==10) return ENTER;
    /*Acceso al ranking de puntuaciones*/
    if(choice=='R' || choice=='r') return RANKING;
  if (choice == 27 && fgetc(stdin) == '[') { /* The key is an arrow key */
    choice = fgetc(stdin);

    switch(choice) {
    case('A'):
      return UP;
    case('B'):
      return DOWN;
    case('C'):
      return ENTER;
    default:
      return NOTHING;
    }
  }
  else if(choice=='w') return UP;
  else if(choice=='s') return DOWN;
  else
    return choice;
}


/*Funcion auxiliar para imprimir los rankings*/
sc_rectangle *print_ranking(){

    sc_rectangle *rank;
    char puntuaciones[RANK_TAM][500];
    FILE *f;
    int i;

    printf("%c[8;%d;%dt",  27, 35, 145);
    system("clear");

    rank=win_new(5, 30, 25, 40, 1000, FOREGROUND);

    f=fopen("./aux/puntuacion.txt", "r");

    for(i=0;i<RANK_TAM;i++){
        fgets(puntuaciones[i], 40, f);
    }

    win_write_line_at(rank, 1, 2, "RANKING DE PUNTUACIONES: ");

    for(i=0;i<RANK_TAM;i++){
    win_write_line_at(rank, i+3, 2, puntuaciones[i]);
    }

    fclose(f);

    while(1){
    if(fgetc(stdin)) break;
        }

    return rank;
}


/*
    Muestra el menu principal y se encarga de interactuar
    con el usuario retornando la opcion escogida.
*/
int ShowMainMenu() {
    int nSelected = 0, cursor=0;

    /* Imprime el menu (formato en menu.txt) y la flecha en la posicion inicial */
    printMainMenu(BACKGROUND_COLOR1, TEXT_COLOR1);
    printFlecha(cursor, BACKGROUND_COLOR1, TEXT_COLOR1);

        while(1){
            nSelected=_read_keyMenu();
            if(nSelected==UP){
                cursor+=4; /* Para que no sea negativo */
                cursor=(cursor-1)%4;
                printFlecha(cursor, BACKGROUND_COLOR1, POINTER_COLOR);
            }
            if(nSelected==DOWN){
                cursor=(cursor+1)%4;
                printFlecha(cursor, BACKGROUND_COLOR1, POINTER_COLOR);
            }
            if(nSelected==ENTER){
                return cursor+1;
            }
            if(nSelected==RANKING){
                win_delete(print_ranking());
                printMainMenu(BACKGROUND_COLOR1, TEXT_COLOR1);
                printFlecha(cursor, BACKGROUND_COLOR1, TEXT_COLOR1);
            }
        }
}





/*
    Da la posibilidad de cargar una partida guardada
    o de volver atras al menu principal.
*/
int LoadMenu() {
    FILE *f=NULL;
    int nChoice = 0, aux=0;
    char c;

    system("clear");    

    do {
        nChoice = ShowLoadMenu();
        /* Comprueba si el archivo esta vacio, si lo está te devuelve al
        load menu y si no, devuelve la opcion de cargar ese slot. */
        switch (nChoice) {
            case 1: {
                f=fopen("./aux/slots/slot1.txt", "r");
                if(fread(&c, sizeof(char), 1, f)!=0){
                    fclose(f);
                    return 1;
                }
                else{
                    fclose(f);
                }
            }
                break;

            case 2: {
                f=fopen("./aux/slots/slot2.txt", "r");
                if(fread(&c, sizeof(char), 1, f)!=0){
                    fclose(f);
                    return 2;
                }
                else{
                    fclose(f);
                }
            }
                break;

            case 3: {
                f=fopen("./aux/slots/slot3.txt", "r");
                if(fread(&c, sizeof(char), 1, f)!=0){
                    fclose(f);
                    return 3;
                }
                else{
                    fclose(f);
                }
            }
                break;
            case 4: {
                return 4;
            }
                break;
        }
    } while (nChoice != 4);
}

/* 
    bckg: background color
    text: text color

    Imprime en la pantalla el loadMenu (formato en load.txt).
*/
void printLoadMenu(int bckg, int text){
    FILE *f;
    char c, slot[50];
    f=fopen("./aux/plantillas/load.txt", "r");
    if(f!=NULL){
        system("clear");
        while(fscanf(f, "%c", &c)==1){
            if(c=='-' || c==10){
                printf("\x1b[%dm" " ", bckg);
                fflush(stdout);
            }
            else{
                printf("\x1b[%dm" " ", text);
                fflush(stdout);
            }
        }

        printf("\x1b[0m");
        printf("%c[%d;%dH", 27, 0, 0);

        fclose(f);
    }
}

/*
    Muestra el menu load y se encarga de interactuar
    con el usuario retornando la opcion escogida.
*/
 int ShowLoadMenu() {
    int nSelected = 0, cursor=0;

    printLoadMenu(BACKGROUND_COLOR1, TEXT_COLOR1);
    printFlecha(0, BACKGROUND_COLOR1, TEXT_COLOR1);

        while(1){
            nSelected=_read_keyMenu();
            if(nSelected==UP){
                cursor+=4; /* Para que no sea negativo */
                cursor=(cursor-1)%4;
                printFlecha(cursor, BACKGROUND_COLOR1, POINTER_COLOR);
            }
            if(nSelected==DOWN){
                cursor=(cursor+1)%4;
                printFlecha(cursor, BACKGROUND_COLOR1, POINTER_COLOR);
            }
            if(nSelected==ENTER){
                return cursor+1;
            }
        }
}

#define NEXT 1
#define PREV -1
#define NOTHING 0
#define QUIT 10
/*
  Reads a key from the keyboard. If the key is enter it returns ENTER, if it
  is a "regular" (except w and s) key it returns its ascii code; if it is an 
  arrow key, it returns UP, DOWN or ENTER. (left arrow dont do anything). 
  w and s also work as UP and DOWN.
*/
int _read_keyTutorial() {
  char choice;
  choice = fgetc(stdin);

    if(choice==10 || choice=='q') return QUIT;
    else if(choice=='a') return PREV;
    else if(choice=='d') return NEXT;
  if (choice == 27 && fgetc(stdin) == '[') { /* The key is an arrow key */
    choice = fgetc(stdin);

    switch(choice) {
    case('C'):
      return NEXT;
    case('D'):
        return PREV;
    default:
      return NOTHING;
    }
  }
  else
    return choice;
}

#include <imprimirHall.h>
#include <imprimirEscaleras.h>
#include <string.h>
#include <colorear.h>
#include <imprimirZonaCuarentena.h>
#define COLOR_COMANDOS 34

void printTutorialPart(int pag, int bckg, int text, sc_rectangle *x,sc_rectangle *y, sc_rectangle *z, sc_rectangle *w){
    FILE *f, *f2;
    int i, len;
    char c[200], d;
    sc_rectangle *pantalla;

    f=fopen("./aux/Tutorial/tutorial.txt","r");

    win_cls(x);
    win_cls(y);
    win_cls(z);
    win_cls(w);
    win_write_borders4(x, y, z, w);

    if(pag==0){
        imprimirHall();
        printf("\x1b[48;5;236m" " ");
        win_write_borders4(x, y, z, w);
    }
    else if(pag==1){
        f2=fopen("./aux/plantillas/mapa.txt", "r");
        if(f2!=NULL){
        _move_to(x, 0,0);
        while(fscanf(f2, "%c", &d)==1){
            if(d=='-' || d==10){
                printf("\x1b[%dm" " ", bckg);
                fflush(stdout);
            }
            else if(d=='*'){
                printf("\x1b[%dm" " ", text+10);
                fflush(stdout);
            }
            else{
                printf("\x1b[%dm" "\x1b[%dm" "%c", bckg, text, d);
                fflush(stdout);
            }
        }
        fclose(f2);
        }
    }
    else if(pag==3){
        int matriz[5][9];
        colorear_generar(5, 9, matriz);
        colorear_print_esqueleto(x);
        colorear_print_casillas(x, 5, 9, matriz);
    }
    else if(pag==5){
        imprimirEscaleras(1);
        printf("\x1b[48;5;236m" " ");
        win_write_borders4(x, y, z, w);
    }
    else if(pag==6){
        imprimirZonaCuarentena();
        printf("\x1b[48;5;236m" " ");
        win_write_borders4(x, y, z, w);
    }


    
    
    /*Llegamos hasta la zona dónde esté la info de página*/
    i=0;
    while(i<=pag){
        fscanf(f, "%s", c);
        if(strcmp("FIN_DE_PAGINA",c)==0)
            i++;
    }

    fscanf(f, "%s", c);
    while(strcmp("FIN_DE_PAGINA",c)!=0){
        fscanf(f, "%s", c);
        if(strcmp("IZQ_ARRIBA",c)==0){
            pantalla=x;
        }
        else if(strcmp("DCHA_ARRIBA",c) ==0){
            pantalla=y;
        }
        else if(strcmp("IZQ_ABAJO",c)==0){
            pantalla=z;
        }
        else if(strcmp("DCHA_ABAJO",c)==0){
            pantalla=w;
        }
        i=0;
        len=0;
        while(strcmp("FIN_DE_PAGINA",c)!=0 && strcmp(c,"FIN_DE_PANTALLA")!=0){
            fscanf(f, "%s", c);
            if(strcmp(c,";")==0){
                i++;
                len=0;
            }
            else if(strcmp(c,"*")==0){
                fscanf(f, "%s", c);
                win_write_line_at_colour(pantalla, 3+i, 4+len, c, COLOR_COMANDOS, bckg);
                len += strlen(c)+1;
            }
            else if(strcmp(c,"FIN_DE_PANTALLA")!=0){
                win_write_line_at(pantalla, 3+i, 4+len, c);
                len += strlen(c)+1;
            }
        }
        
    }

    

    fclose(f);

    return;
}

void tutorial(int bckg, int texto){
    sc_rectangle *x, *y, *z, *w;
    int nSelected, pag=0;

    system("clear");
    printf("%c[8;%d;%dt",  27, 35, 145);
    
    x=win_new(0, 0, 25, 100, bckg, texto);
    win_cls(x);

    y=win_new(0, 100, 25, 45, bckg, texto);
    win_cls(y);

    z=win_new(25, 0, 10, 100, bckg, texto);
    win_cls(z);

    w=win_new(25, 100, 10, 45, bckg, texto);
    win_cls(w);

    win_write_borders4(x, y, z, w);
  
    
    while (1)
    {
        printTutorialPart(pag, bckg, texto, x, y, z, w);
        nSelected=_read_keyTutorial();
            if(nSelected==UP){
                if(pag==6) break;
                pag++;
            }
            if(nSelected==DOWN){
                if(pag>0){
                    pag--;
                }
            }
            if(nSelected==QUIT){
                break;
            }
    }
    

    win_delete(x);
    win_delete(y);
    win_delete(z);
    win_delete(w);
}







