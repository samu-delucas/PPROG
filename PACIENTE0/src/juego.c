/*
    Este fichero es el main del juego, en este main se llama al menu en el que elige
    la opción  el usuario. Además aquí está la función play o load, las cuales hacen 
    justicia a su nombre: inicializan todo lo necesario para el desarrollo del juego 
    (contenido, interfaz, lector de comandos...) llaman al hilo del reloj, y en un 
    bucle infinito leen la entrada llamando al procesador de comandos para ir jugando.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

#include <juego.h>

#include <menu.h>
#include <start.h>
#include <intrf.h>
#include <jugador.h>
#include <laberinto.h>
#include <sco.h>
#include <cop.h>
#include <colorear.h>
#include <assert.h>

#define ERROR_ESPACIOS "                                                                                                "
#define COLUMNA_ERROR 2
#define FILA_ERROR 2

int counter_exit = 0;       /* Si el tiempo se ha acabado se pone a 1 */
int pruebaActiva = FALSE;   /* Si hay una prueba en desarrollo se pone a TRUE */
                            /* para que si se acaba el tiempo se espere a acabar */

typedef struct {
  time_t initial; // Initial time at which the function is first called
  intrf  *intrf;    // pointer to the interface where the clock is to be displayed
} clock_data;

/* Esta variable se usa como externa en otros ficheros */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct termios initial;
/*
  Initializes the terminal in such a way that we can read the input
  without echo on the screen
*/
void _term_init()
{
    struct termios new; /*a termios structure contains a set of attributes about 
					  how the terminal scans and outputs data*/

    tcgetattr(fileno(stdin), &initial); /*first we get the current settings of out 
						 terminal (fileno returns the file descriptor 
						 of stdin) and save them in initial. We'd better 
						 restore them later on*/
    new = initial;                      /*then we copy them into another one, as we aren't going 
						to change ALL the values. We'll keep the rest the same */
    new.c_lflag &= ~ICANON;             /*here we are setting up new. This line tells to stop the 
						canonical mode (which means waiting for the user to press 
						enter before sending)*/
    new.c_lflag &= ~ECHO;               /*by deactivating echo, we tell the terminal NOT TO 
						show the characters the user is pressing*/
    new.c_cc[VMIN] = 1;                 /*this states the minimum number of characters we have 
					       to receive before sending is 1 (it means we won't wait 
					       for the user to press 2,3... letters)*/
    new.c_cc[VTIME] = 0;                /*I really have no clue what this does, it must be somewhere in the book tho*/
    new.c_lflag &= ~ISIG;               /*here we discard signals: the program won't end even if we 
						press Ctrl+C or we tell it to finish*/

    tcsetattr(fileno(stdin), TCSANOW, &new); /*now we SET the attributes stored in new to the 
						    terminal. TCSANOW tells the program not to wait 
						    before making this change*/
}


void printGameOver(){
    char c;
    FILE *f;

    f=fopen("./aux/plantillas/gameOver.txt", "r");
    printf("%c[%d;%dH", 27, 0, 0);
    while(fscanf(f,"%c",&c)==1){
        if(c=='X'){
            printf("\x1b[%dm" " ", TEXT_COLOR1);
            fflush(stdout);
        }
        else{
            printf("\x1b[%dm" " ", BACKGROUND_COLOR2);
            fflush(stdout);
        }
    }
    fclose(f);
}

/*
  Manages the clock, reading time and updating the clock on the screen
*/
void *_ck_count(void *q) {
  intrf *iq = (intrf *) q;
  time_t itime = time(NULL);

  while(1) {
    time_t t = time(NULL);

    pthread_mutex_lock(&lock);
    if(i_set_time(iq, t-itime)==0 && pruebaActiva==FALSE){
        iq->elapsed=iq->max_time;
        counter_exit = 1;
        printGameOver();
        sleep(2);
        pthread_mutex_unlock(&lock);
        pthread_exit(NULL);
    }
    pthread_mutex_unlock(&lock);
    
    usleep(10000);
  }
}

/* Guarda tanto los datos de la interfaz como las pistas, acertijos y la agenda. */
void save(int i, intrf *Interfaz, ARGComandos *argumentos_comandos){
    FILE *f=NULL, *g=NULL;
    char slot[500], *pista, cadena[500];
    int aux, k, acertijos[3], n=0, m;
    Objeto *obj=NULL;
    Habitacion *hab=NULL;
    Personaje *pers;

    if(i<1 || i>3) 
        return;
    
    strcpy(slot, "./aux/slots/slotX.txt");
    slot[16]='0'+i;
    f=fopen(slot,"w");
    assert(f!=NULL);

    /* Paciente 0 */
    fprintf(f, "%d\n", Interfaz->paciente0);
    /* Tiempo restante */
    fprintf(f, "%ld\n", Interfaz->max_time-Interfaz->elapsed);
    /* Puntuacion hasta el momento */
    fprintf(f, "%d\n", Interfaz->score);
    /* Pruebas pasadas */
    for(k=0; k<NUM_PRUEBAS; k++){
        fprintf(f, "%d\n", Interfaz->pruebas[k]);
    }

    /* Datos del jugador */
    fprintf(f, "%s\n", jugador_getNombre(Interfaz->player));
    if(jugador_getObjeto(Interfaz->player)!=NULL){
        aux=TRUE;
        fprintf(f, "%d\n", aux);
        fprintf(f, "%s\n", objeto_getNombre(jugador_getObjeto(Interfaz->player)));
    }
    else{
        aux=FALSE;
        fprintf(f, "%d\n", aux);
    }

    /* Datos del mapa */
    fprintf(f, "%d\n", mapa_getPos(Interfaz->map));
    fprintf(f, "%d\n", mapa_areConnected(Interfaz->map,5,9));
    fprintf(f, "%d\n", mapa_areConnected(Interfaz->map,9,9));
    /* Habitaciones */
    for(k=0;k<9;k++){
        hab=mapa_getHab(Interfaz->map,k);
        obj=habitacion_getObjeto(hab);
        if(obj!=NULL){
            aux=TRUE;
            fprintf(f, "%d\n", aux);
            fprintf(f, "%s\n", objeto_getNombre(obj));
        }
        else{
            aux=FALSE;
            fprintf(f, "%d\n", aux);
        }
        pers=habitacion_getPersonaje(hab);
        fprintf(f, "%s\n", personaje_getNombre(pers));
        fprintf(f, "%d %d %d\n", personaje_getPrueba(pers), personaje_getNumPrueba(pers), personaje_getNumSosp(pers));
        pista=personaje_getPista(pers);
        fprintf(f,"%s\n",pista); /* PISTAS */

        /* GUARDAMOS LOS NUMEROS DE ACERTIJOS */
        if(personaje_getPrueba(pers)==ACERTIJO){
            acertijos[n]=personaje_getNumPrueba(pers);
            n++;
        }
    }
    /* Caso especial la habitacion de Santini */
    hab=mapa_getHab(Interfaz->map,k);
    obj=habitacion_getObjeto(hab);
    if(obj!=NULL){
        aux=TRUE;
        fprintf(f, "%d\n", aux);
        fprintf(f, "%s\n", objeto_getNombre(obj));
    }
    else{
        aux=FALSE;
        fprintf(f, "%d\n", aux);
    }

    /* ACERTIJOS */
    for(k=0; k<3; k++){
        n=0;
        strcpy(slot, "./aux/AcertijosPistas/acertijoX.txt");
        slot[30]='0'+acertijos[k];
        g=fopen(slot,"r");
        assert(g!=NULL);
        for(n=0;fgets(cadena,500,g);) n++;
        fclose(g);
        g=fopen(slot,"r");
        fprintf(f, "%d %d\n", acertijos[k], n);
        for(m=0;m<n;m++){
            fgets(cadena,500,g);
            cadena[strlen(cadena)-1]=0;
            fprintf(f, "%s\n", cadena);
        }
        fclose(g);
    }

    /*AGENDA*/
    agenda_save(argumentos_comandos,f);

    fclose(f);
}

/* Carga tanto los datos de la interfaz como las pistas, acertijos y la agenda. */
void load(int i){
    intrf *Interfaz;
    Jugador *jug;
    char c, buf[72], entrada[72];
    pthread_t pth;
    int j=0, ret=0, ret_com=0;
    entrada[0]=0;
    CoP *procesador_comandos=NULL;
    ARGComandos *argumentos_comandos=NULL;
    counter_exit=0;
    pruebaActiva=FALSE;

    startGame(); /* Solo para asegurar que no este vacio el archivo */
    
    /* Iniciamos las estructuras para jugar */
    jug=jugador_create();
    Interfaz=i_create(jug, 0, 0, 0); /* Al crearla imprime la plantilla */

/********************************************************************************/
    /* Cargamos los datos */
    FILE *f=NULL, *g=NULL;
    char slot[500], cadena[500], cadena2[500];
    int aux, aux2, aux3, k, m;
    long laux;
    Objeto *obj=NULL;
    Habitacion *hab=NULL;
    Personaje *pes=NULL;

    assert(i>=1 && i<=3);
    
    strcpy(slot, "./aux/slots/slotX.txt");
    slot[16]='0'+i;
    f=fopen(slot,"r");
    assert(f!=NULL);

    /* Paciente 0 */
    fscanf(f, "%d\n", &aux);
    Interfaz->paciente0=aux;
    /* Tiempo restante */
    fscanf(f, "%ld\n", &laux);
    Interfaz->max_time=laux;
    /* Puntuacion hasta el momento */
    fscanf(f, "%d\n", &aux);
    Interfaz->score=aux;
    /* Pruebas pasadas */
    for(k=0; k<NUM_PRUEBAS; k++){
        fscanf(f, "%d\n", &Interfaz->pruebas[k]);
    }

    /* Datos del jugador */
    fscanf(f, "%s\n", cadena);
    jugador_setNombre(Interfaz->player,cadena);
    fscanf(f, "%d\n", &aux);
    if(aux==TRUE){
        fscanf(f, "%s %s\n", cadena, cadena2);
        strcat(cadena," ");
        strcat(cadena,cadena2);
        for(k=0;k<10;k++){
            obj=habitacion_getObjeto(mapa_getHab(Interfaz->map,k));
            if(obj!=NULL){
                if(strcmp(objeto_getNombre(obj),cadena)==0){
                    jugador_addObjeto(Interfaz->player, obj);
                    habitacion_deleteObjeto(mapa_getHab(Interfaz->map,k));
                }
            }
        }
    }

    /* Datos del mapa */
    fscanf(f, "%d\n", &aux);
    mapa_setPos(Interfaz->map,aux);
    _i_redraw(Interfaz);
    fscanf(f, "%d\n", &aux);
    if(aux==TRUE) mapa_setConnections(Interfaz->map,5,9);  
    fscanf(f, "%d\n", &aux);
    if(aux==TRUE) mapa_setConnections(Interfaz->map,9,9); 
    /* Habitaciones */
    for(k=0;k<9;k++){
        hab=mapa_getHab(Interfaz->map,k);
        pes=habitacion_getPersonaje(hab);
        fscanf(f, "%d\n", &aux);
        if(habitacion_getObjeto(hab)!=NULL){
            obj=habitacion_getObjeto(hab);
            habitacion_deleteObjeto(hab);
            objeto_free(obj);
        }
        if(aux==TRUE){
            fscanf(f, "%s %s\n", cadena, cadena2);
            strcat(cadena," ");
            strcat(cadena,cadena2);
            obj=objeto_create();
            objeto_setNombre(obj,cadena);
            habitacion_addObjeto(hab,obj);
        }
        fscanf(f, "%s\n", cadena);
        fscanf(f, "%d %d %d\n", &aux,  &aux2,  &aux3);
        fgets(cadena2,499,f);
        cadena2[strlen(cadena2)-1]=0;
        personaje_setNombre(pes,cadena);
        personaje_setPrueba(pes,aux);
        personaje_setNumPrueba(pes,aux2);
        personaje_setNumSosp(pes,aux3);
        personaje_setPista(pes,cadena2);
    }
    hab=mapa_getHab(Interfaz->map,k);
    pes=habitacion_getPersonaje(hab);
    fscanf(f, "%d\n", &aux);
    obj=habitacion_getObjeto(hab);
    habitacion_deleteObjeto(hab);
    objeto_free(obj);
    if(aux==TRUE){
        fscanf(f, "%s %s\n", cadena, cadena2);
        strcat(cadena," ");
        strcat(cadena,cadena2);
        obj=objeto_create();
        objeto_setNombre(obj,cadena);
        habitacion_addObjeto(hab,obj);
    }

    /* ACERTIJOS */
    for(k=0; k<3; k++){
        fscanf(f, "%d %d\n", &aux, &aux2);
        strcpy(slot, "./aux/AcertijosPistas/acertijoX.txt");
        slot[30]='0'+aux;
        g=fopen(slot,"w");
        assert(g!=NULL);
        for(m=0;m<aux2;m++){
            fgets(cadena,500,f);
            cadena[strlen(cadena)-1]=0;
            fprintf(g, "%s\n", cadena);
        }
        fclose(g);
    }

    /*AGENDA*/
    argumentos_comandos=ARGComandos_load(Interfaz,SOSPECHOSOS,10,f);

    fclose(f);
    _i_redraw(Interfaz);
/*******************************************************************************/

    procesador_comandos=comandos_set("./src/Comandos/comandos.txt");

    /* Llamamos al reloj y empezamos el juego */
    pthread_create(&pth, NULL, _ck_count, (void *) Interfaz);
    while(1){
        if (counter_exit == 1)
        {
            pthread_join(pth, NULL);
            interfaz_free(Interfaz);
            CoP_free(procesador_comandos);
            ARGComandos_free(argumentos_comandos);
            break;
        }

        c=fgetc(stdin);
        
        if(((c>='0' && c<='z') || c==' ') && j<70){
            /* Pasamos a mayusculas */
            if(c>='a' && c<='z') c=c-'a'+'A';
            buf[j]=c;
            pthread_mutex_lock(&lock);
            _move_to(Interfaz->left_down, 1, j+2);
            printf("%c",c);
            fflush(stdout);
            j++;
            pthread_mutex_unlock(&lock);
        }
        if(c==10){
            buf[j]=10;
            buf[j+1]=0;
            strcpy(entrada, buf);
            j=0;
            win_write_line_at(Interfaz->left_down, 1, 1, "                         "
            "                                                  ");
            win_write_line_at(Interfaz->left_down, 2, 1, "                                                        ");
            ret_com=CoP_execute(procesador_comandos, entrada, argumentos_comandos);
            if(ret_com!=0){
                pthread_cancel(pth);
                pthread_join(pth, NULL);
                save(ret_com, Interfaz, argumentos_comandos);
                interfaz_free(Interfaz);
                CoP_free(procesador_comandos);
                ARGComandos_free(argumentos_comandos);
                break;
            }
        }
        else if(c==127 && j>=1){
            j--;
            pthread_mutex_lock(&lock);
            _move_to(Interfaz->left_down, 1, j+2);
            printf(" ");
            fflush(stdout);
            pthread_mutex_unlock(&lock);
        }
        
    }    
}

/*
    Inicializa todo lo necesario para el desarrollo del juego (contenido, interfaz,
    lector de comandos...) llama al hilo del reloj, y en un bucle infinito lee la 
    entrada llamando al procesador de comandos para ir jugando.
*/
void play(){
    intrf *interfaz;
    Jugador *jug;
    char c, buf[72], entrada[72], name[72];
    pthread_t pth;
    int i=0, ret=0, ret_com=0, paciente0=0;
    entrada[0]=0;
    CoP *procesador_comandos=NULL;
    ARGComandos *argumentos_comandos=NULL;
    counter_exit=0;
    pruebaActiva=FALSE;
    FILE *f=NULL;

    /* Sortea el Paciente 0, pistas, acertijos... */
    startGame();
    
    /* Iniciamos las estructuras para jugar */
    jug=jugador_create();

    f=fopen("./aux/start.txt", "r");
    if(f==NULL) return;
    fscanf(f, "%d", &paciente0);
    fclose(f);
    interfaz=i_create(jug, 0, 0, paciente0); /* Al crearla imprime la plantilla */

    /*Solicitamos el nombre de jugador*/
    do{
    win_write_line_at(interfaz->left_down,FILA_ERROR,COLUMNA_ERROR,"Introduce tu nombre (20 caracteres maximo)");
     while(1){
        c=fgetc(stdin);
        
        if(((c>='0' && c<='z') || c==' ') && i<70){
            /* Pasamos a mayusculas */
            if(c>='a' && c<='z') c=c-'a'+'A';
            name[i]=c;
            pthread_mutex_lock(&lock);
            _move_to(interfaz->left_down, 1, i+2);
            printf("%c",c);
            fflush(stdout);
            i++;
            pthread_mutex_unlock(&lock);
        }
        if(c==10){
            name[i]=0;
            i=0;
            win_write_line_at(interfaz->left_down, 1, 1, "                         "
            "                                                  ");
            win_write_line_at(interfaz->left_down, 2, 1, "                                                        ");
            break;
        }
        else if(c==127 && i>=1){
            i--;
            pthread_mutex_lock(&lock);
            _move_to(interfaz->left_down, 1, i+2);
            printf(" ");
            fflush(stdout);
            pthread_mutex_unlock(&lock);
        }
    }
    }while(strlen(name)>20 || strlen(name)<1);

    /*Establecemos el nombre del jugador*/
    jugador_setNombre(jug, name);

    procesador_comandos=comandos_set("./src/Comandos/comandos.txt");
    argumentos_comandos=ARGComandos_create(interfaz,SOSPECHOSOS,10);

    i=0;
    /* Llamamos al reloj y empezamos el juego */
    pthread_create(&pth, NULL, _ck_count, (void *) interfaz);
    while(1){
        c=fgetc(stdin);
        if (counter_exit == 1)
        {
            pthread_join(pth, NULL);
            interfaz_free(interfaz);
            CoP_free(procesador_comandos);
            ARGComandos_free(argumentos_comandos);
            break;
        }
        
        if(((c>='0' && c<='z') || c==' ') && i<70){
            /* Pasamos a mayusculas */
            if(c>='a' && c<='z') c=c-'a'+'A';
            buf[i]=c;
            pthread_mutex_lock(&lock);
            _move_to(interfaz->left_down, 1, i+2);
            printf("%c",c);
            fflush(stdout);
            i++;
            pthread_mutex_unlock(&lock);
        }
        if(c==10){
            buf[i]=10;
            buf[i+1]=0;
            strcpy(entrada, buf);
            i=0;
            win_write_line_at(interfaz->left_down, 1, 1, "                         "
            "                                                  ");
            win_write_line_at(interfaz->left_down, 2, 1, "                                                        ");
            pruebaActiva=TRUE;
            ret_com=CoP_execute(procesador_comandos, entrada, argumentos_comandos);
            pruebaActiva=FALSE;
            if(ret_com!=0){
                pthread_cancel(pth);
                pthread_join(pth, NULL);
                save(ret_com, interfaz, argumentos_comandos);
                interfaz_free(interfaz);
                CoP_free(procesador_comandos);
                ARGComandos_free(argumentos_comandos);
                break;
            }
        }
        else if(c==127 && i>=1){
            i--;
            pthread_mutex_lock(&lock);
            _move_to(interfaz->left_down, 1, i+2);
            printf(" ");
            fflush(stdout);
            pthread_mutex_unlock(&lock);
        }
        
    }
    
}


/*
    Main, en un primer momento llama a menu y segun la opcion elegida llama a play o otra opcion.
*/
int main()
{
    int choice;
    srand(time(NULL));

    /* Preparamos la terminal */
    _term_init();

    /* Llamamos al menu y ejecutamos la opcion escogida por el usuario */
    do{
        fflush(stdin);
        choice = menu();
        switch (choice)
        {
        case 0:
        { /* CASO INICIAR NUEVO JUEGO */
            play();
            printf("%c[%d;%dH", 27, 27, 2);
            fflush(stdout);
        }
        break;
        case 1:
        { /* CASO CARGAR SLOT 1 (solo si tiene datos slot1.txt) */
            load(1);
            printf("%c[%d;%dH", 27, 27, 2);
            fflush(stdout);
        }
        break;

        case 2:
        { /* CASO CARGAR SLOT 2 (solo si tiene datos slot2.txt) */
            load(2);
            printf("%c[%d;%dH", 27, 27, 2);
            fflush(stdout);
        }
        break;

        case 3:
        { /* CASO CARGAR SLOT 3 (solo si tiene datos slot3.txt) */
            load(3);
            printf("%c[%d;%dH", 27, 27, 2);
            fflush(stdout);
        }
        break;
        }
    }while(choice!=4);

    tcsetattr(fileno(stdin), TCSANOW, &initial);
    return 0;
}
