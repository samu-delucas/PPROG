/*
Este es un fichero que contiene las funciones asociadas a cada comando 
específico, definidos dentro del documento comandos.txt.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cop.h>
#include <sco.h>
#include <unistd.h>
#include <rectangles.h>
#include <jugador.h>
#include <mapa.h>
#include <laberinto.h>
#include <birds.h>
#include <colorear.h>
#include <pthread.h>
#include <snake.h>

extern pthread_mutex_t lock;
extern char *SOSPECHOSOS[10];


#define WORLD ARGComandos
#define PANTALLA_FRON(x) x->interfaz->left_up
#define PANTALLA_LADO(x) x->interfaz->left_down
#define NUM_MSG_AGENDA 20
#define ERROR_ESPACIOS "                                                                                                "
#define ABIERTO 1
#define CERRADO 0
#define COLOR_TACHAR 34
#define COLOR_AGENDA_TITULO 34
#define COLUMNA_ERROR 2
#define FILA_ERROR 2
#define COLOR_COMANDOS 34

/*Cosas que pasar*/
struct _ARGComandos{
    intrf * interfaz;
    Agenda* agenda;
    int mapa_open;
    int comandos_info_open;
};

/*Agenda*/
struct _Agenda{
int open;
int num_msg;
char *msg[NUM_MSG_AGENDA];
int num_sospechosos;
char **sospechosos;
int *marca;
};

/*Devuelve el Procesador de comandos con asociaciones a partir del fichero
en el que los comandos hayan sido definidos*/
CoP * comandos_set(char* fichero){
    CoP *command=NULL;
    FILE *f=NULL;
	
    f=fopen(fichero,"r");
    if(f==NULL){
        return NULL;
    }
    
    command=CoP_create(f);

    CoP_assoc(command, "agenda_cmd", agenda_cmd);
    CoP_assoc(command, "cerrar_cmd", cerrar_cmd);
    CoP_assoc(command, "escribir_cmd", escribir_cmd);
    CoP_assoc(command, "tachar_cmd", tachar_cmd);
    CoP_assoc(command, "error_cmd", error_cmd);
    CoP_assoc(command, "coger_cmd", coger_cmd);
    CoP_assoc(command, "subir_cmd", subir_cmd);
    CoP_assoc(command, "bajar_cmd", bajar_cmd);
    CoP_assoc(command, "este_cmd", este_cmd);
    CoP_assoc(command, "oeste_cmd", oeste_cmd);
    CoP_assoc(command, "hablar_cmd", hablar_cmd);
    CoP_assoc(command, "salir_cmd", salir_cmd);
    CoP_assoc(command, "resolver_cmd", resolver_cmd);
    CoP_assoc(command, "mapa_cmd", mapa_cmd);
    CoP_assoc(command, "comandos_info_cmd", comandos_info_cmd);

    fclose(f);

    return command;
}

/* Devuelve el argumento 3 de CoP_execute */
ARGComandos *ARGComandos_create(intrf *interfaz, char**nombres, int num_sospechosos){
    ARGComandos *com=NULL;

    com=(ARGComandos*)malloc(sizeof(ARGComandos));
    if(com==NULL)
        return NULL;

    com->interfaz=interfaz;
    com->mapa_open=CERRADO;
    com->comandos_info_open=CERRADO;
    
    com->agenda=agenda_create(nombres,num_sospechosos);
    if(com->agenda==NULL){
        free(com);
        return NULL;
    }

    return com;    
}

void ARGComandos_free(ARGComandos* com){
    int i;

    free(com->agenda->marca);
    for(i=0;i<com->agenda->num_msg;i++)
        free(com->agenda->msg[i]);
    for(i=0;i<com->agenda->num_sospechosos;i++)
        free(com->agenda->sospechosos[i]);
    free(com->agenda->sospechosos);
    free(com->agenda);
    free(com);
    return;
}

void free_strings_array(char **strings, int num_strings){
    int i;
    if(num_strings>0){
        for(i=0;i<num_strings;i++)
            free(strings[i]);
        free(strings);
    }
}

void agenda_save(ARGComandos * com, FILE * f){
    int i;
    
    fprintf(f,"%d\n",com->agenda->num_msg);
    for(i=0;i<com->agenda->num_msg;i++){
        fprintf(f,"%s\n", com->agenda->msg[i]);
    }
    for(i=0;i<com->agenda->num_sospechosos;i++){
        fprintf(f, "%d\n", com->agenda->marca[i]);
    }

    return;
}

Agenda * agenda_load(char**nombres, int num_sospechosos, FILE *f){
    Agenda *agenda;
    char mensaje[200];
    int i;

    agenda = (Agenda*) malloc(sizeof(Agenda));
    if(agenda==NULL)
        return NULL;
    
    agenda->open=CERRADO;
    fscanf(f, "%d\n", &agenda->num_msg);
    agenda->num_sospechosos=num_sospechosos;

    agenda->sospechosos=(char**)malloc(num_sospechosos*sizeof(char*));
    if(agenda->sospechosos==NULL){
        free(agenda);
        return NULL;
    }

    agenda->marca=(int*)malloc(num_sospechosos*sizeof(int));
    if(agenda->sospechosos==NULL){
        free(agenda);
        return NULL;
    }

    for(i=0; i<agenda->num_sospechosos; i++){
        agenda->marca[i]=0; 
        agenda->sospechosos[i]=strdup(nombres[i]);
    }

    for(i=0;i<agenda->num_msg;i++){
        fgets(mensaje,200,f);
        mensaje[strlen(mensaje)-1]=0;
        agenda->msg[i] = strdup(mensaje);
    }
    for(i=0;i<agenda->num_sospechosos;i++){
        fscanf(f, "%d", &agenda->marca[i]);
    }

    return agenda;
}

ARGComandos *ARGComandos_load(intrf *interfaz, char**nombres, int num_sospechosos, FILE *f){
    ARGComandos *com=NULL;

    com=(ARGComandos*)malloc(sizeof(ARGComandos));
    if(com==NULL)
        return NULL;

    com->interfaz=interfaz;
    com->mapa_open=CERRADO;
    com->comandos_info_open=CERRADO;
    
    com->agenda=agenda_load(nombres,num_sospechosos,f);
    if(com->agenda==NULL){
        free(com);
        return NULL;
    }

    return com;    
}

Agenda * agenda_create(char**nombres, int num_sospechosos){
    Agenda *agenda;
    int i;

    agenda = (Agenda*) malloc(sizeof(Agenda));
    if(agenda==NULL)
        return NULL;
    
    agenda->open=CERRADO;
    agenda->num_msg=0;
    agenda->num_sospechosos=num_sospechosos;

    agenda->sospechosos=(char**)malloc(num_sospechosos*sizeof(char*));
    if(agenda->sospechosos==NULL){
        free(agenda);
        return NULL;
    }

    agenda->marca=(int*)malloc(num_sospechosos*sizeof(int));
    if(agenda->sospechosos==NULL){
        free(agenda);
        return NULL;
    }

    for(i=0; i<agenda->num_sospechosos; i++){
        agenda->marca[i]=0; 
        agenda->sospechosos[i]=strdup(nombres[i]);
    }

    return agenda;
}

void agenda_refrescar(int num_msg, char **msg, char**sosp, int *marca, sc_rectangle * frontal, int num_sospechosos){
    int i;
    char ag[20];
    FILE *f;

    
    f=fopen("./aux/Comandos/comandos_info.txt","r");
    fscanf(f, "%s", ag);

    pthread_mutex_lock(&lock);
    win_cls(frontal);
    win_write_border(frontal);

    win_write_line_at_colour(frontal, 4, 25, ag, COLOR_AGENDA_TITULO, frontal->bg_color);
    for(i=0;i<num_msg;i++){
        win_write_line_at(frontal, 6+i, 25, msg[i]);
    }

    fscanf(f, "%s", ag);
    win_write_line_at_colour(frontal, 4, 5, ag, COLOR_AGENDA_TITULO, frontal->bg_color);
    for(i=0;i<num_sospechosos;i++){
        if(marca[i]==1){
        win_write_line_at_colour(frontal, 6+i, 5, sosp[i], COLOR_TACHAR, frontal->bg_color);
        }
        else{
        win_write_line_at(frontal, 6+i, 5, sosp[i]);
        }
    }
    pthread_mutex_unlock(&lock);

    fclose(f);
}

/* Limpia la ventana de comandos */
void limpiarComandos(sc_rectangle *x){
    int i;

    pthread_mutex_lock(&lock);
    for(i=0; i<7; i++){
        win_write_line_at(x,FILA_ERROR+i,COLUMNA_ERROR, ERROR_ESPACIOS);
    }
    pthread_mutex_unlock(&lock);
}

/* Prepara la pantalla antes de llamar a un acertijo o minijuego */
void preparar_prueba(intrf *interfaz){
  char buf[50];

    system("clear");
    pthread_mutex_lock(&lock);
  win_write_borders4(interfaz->left_up, interfaz->right_up, interfaz->left_down, interfaz->right_down);
  i_set_score(interfaz, interfaz->score);
  i_set_time(interfaz, interfaz->elapsed);
  

  strcpy(buf,"COMANDOS para abrir la guia de comandos");
  win_write_line_at(interfaz->right_down,1,1,buf);
  strcpy(buf,"MAPA para abrir el mapa");
  win_write_line_at(interfaz->right_down,2,1,buf);
  strcpy(buf,"AGENDA para abrir la agenda");
  win_write_line_at(interfaz->right_down,3,1,buf);
  strcpy(buf,"CERRAR para salir del mapa y agenda");
  win_write_line_at(interfaz->right_down,4,1,buf);
  strcpy(buf,"SALIR para abrir el menu para salir");
  win_write_line_at(interfaz->right_down,5,1,buf);
  pthread_mutex_unlock(&lock);
}


int agenda_cmd (void *pt, char *inutil, char **passed_msg, int n){
    WORLD * juego = (WORLD*) pt;

    /*preparar_prueba(juego->interfaz);*/
    juego->agenda->open=ABIERTO;

    if(juego->mapa_open==ABIERTO || juego->comandos_info_open==ABIERTO){
        juego->mapa_open=CERRADO;
        juego->comandos_info_open=CERRADO;
        _i_redraw(juego->interfaz);
    }

    agenda_refrescar(juego->agenda->num_msg, juego->agenda->msg, juego->agenda->sospechosos, juego->agenda->marca, juego->interfaz->left_up, juego->agenda->num_sospechosos);

    free_strings_array(passed_msg, n);
    free(inutil);
    
    return 0;
}

int cerrar_cmd (void *pt, char *inutil, char **passed_msg, int n){
    WORLD * juego = (WORLD*) pt;
    int i;

    juego->agenda->open=CERRADO;
    juego->mapa_open=CERRADO;
    juego->comandos_info_open=CERRADO;

    _i_redraw(juego->interfaz);

    free_strings_array(passed_msg, n);
    free(inutil);

    return 0;
}

int escribir_cmd (void *pt, char *mensaje, char **passed_msg, int n){
    WORLD * juego = (WORLD*) pt;

    if(juego->agenda->open==CERRADO){
        limpiarComandos(PANTALLA_LADO(juego));
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[0]);
        free_strings_array(passed_msg, n);
        free(mensaje);
        return 0;
    }
    if(juego->agenda->num_msg==NUM_MSG_AGENDA){
        limpiarComandos(PANTALLA_LADO(juego));
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[1]);
        free_strings_array(passed_msg, n);
        free(mensaje);
        return 0;
    }
    

    juego->agenda->msg[juego->agenda->num_msg] = strdup(mensaje);
    juego->agenda->num_msg ++;

    agenda_refrescar(juego->agenda->num_msg, juego->agenda->msg, juego->agenda->sospechosos, juego->agenda->marca, PANTALLA_FRON(juego), juego->agenda->num_sospechosos);

    free_strings_array(passed_msg, n);
    free(mensaje);

    return 0;
}

int tachar_cmd (void *pt, char *mensaje, char **passed_msg, int n){
    WORLD * juego = (WORLD*) pt;
    int i, flag;

    if(juego->agenda->open==CERRADO){
        limpiarComandos(PANTALLA_LADO(juego));
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[1]);
        free_strings_array(passed_msg, n);
        free(mensaje);
        return 0;
    }

    for(i=0, flag=0;i<juego->agenda->num_sospechosos && flag==0;i++){
        if(strcmp(juego->agenda->sospechosos[i],mensaje)==0){
            juego->agenda->marca[i]=1;
            flag=1;
            agenda_refrescar(juego->agenda->num_msg, juego->agenda->msg, juego->agenda->sospechosos, juego->agenda->marca, PANTALLA_FRON(juego),juego->agenda->num_sospechosos);
        }
    }

    if(flag==0){
        limpiarComandos(PANTALLA_LADO(juego));
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[0]);

    }

    free_strings_array(passed_msg, n);
    free(mensaje);
    return 0;
}

int error_cmd (void *pt, char *inutil, char **passed_msg, int n){
    WORLD * juego = (WORLD*) pt;

    limpiarComandos(PANTALLA_LADO(juego));
    win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[0]);

    free_strings_array(passed_msg, n);
    free(inutil);

    return 0;
}

/*
Si el jugador tiene la agenda o el mapa abierto, dice que debería cerrarlo
Si no:
    Si el jugador no tiene un objeto, toma un objeto de la habitación si lo hay
    Si el jugador ya tiene un objeto, lo intercambia con el de la habitación si lo hay
*/
int coger_cmd (void *pt, char *inutil, char **passed_msg, int n){
    WORLD * juego = (WORLD*) pt;
    intrf *Interfaz=juego->interfaz;
    Habitacion *hab;
    int  posicion;
    Objeto *obj_hab=NULL, *obj_jug=NULL;

    if(juego->mapa_open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[0]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    if(juego->agenda->open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[1]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    if(juego->comandos_info_open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[5]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }

    posicion=mapa_getPos(Interfaz->map);
    /* En el deapacho de Santini necesitan estar encendidas las luces */
    if(posicion==9 && (mapa_areConnected(Interfaz->map,9,9)==FALSE)){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[8]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }

    hab=mapa_getHab(Interfaz->map, posicion);
    obj_hab=habitacion_getObjeto(hab);
    /*Caso especial: Despacho de Santini*/
    if(posicion==9 && obj_hab!=NULL){
        obj_jug=jugador_getObjeto(juego->interfaz->player);
        /*El jugador no tiene el portátil*/
        if(obj_jug==NULL || strcmp(objeto_getNombre(obj_jug), "un portatil")!=0){
            limpiarComandos(Interfaz->left_down);
            win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[6]);
            
        }
        /*El jugador sí tiene un portátil */
        else{
            jugador_deleteObjeto(Interfaz->player);
            obj_hab=habitacion_getObjeto(hab);
            jugador_addObjeto(juego->interfaz->player, obj_hab);
            habitacion_deleteObjeto(hab);
            _i_redraw(Interfaz);
            win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[7]);
        }

        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }

    /*Si NO hay objetos en la habitación*/
    if(obj_hab==NULL){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[2]);
    }
    /*Si SÍ hay objetos en la habitación*/
    else{
        /*--Si el jugador no tiene objetos*/
        if((jugador_getObjeto(juego->interfaz->player))==NULL){
            jugador_addObjeto(juego->interfaz->player, obj_hab);
            habitacion_deleteObjeto(hab);
            _i_redraw(Interfaz);
            win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[4]);
        }
        else{/*Si el jugador ya tiene un objeto*/
	        obj_jug=jugador_getObjeto(juego->interfaz->player);
            jugador_addObjeto(juego->interfaz->player,obj_hab);
            habitacion_addObjeto(hab,obj_jug);
            _i_redraw(Interfaz);
            win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[3]);
        }
    }

    free_strings_array(passed_msg, n);
    free(inutil);

    return 0;
}

/* Llama al acertijo o minijuego correspondiente y gestiona si se ha acertado o no
con sus implicaciones (marca la prueba de la habitacion como superada o no). */
void prueba(intrf *Interfaz ,Prueba p, int numP, int numSosp, int pos){
    int ret, i=3;
    char buf[500], c;
    FILE *f=NULL;

    preparar_prueba(Interfaz);
    if(p==MINIJUEGO){
        printf("%c[%d;%dH", 27, 2, 2);
        switch (numP)
        {
            /* LABERINTO */
        case 0:
            strcpy(buf, "Estoy super viciado a este juego de laberintos, pero no consigo pasarme este nivel."
            "Si lo resuelves a tiempo te dire el nombre de un sospechoso que no es el Paciente 0.");
            win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+1);
            ret=laberinto();
            i_set_score(Interfaz, Interfaz->score+ret);
            _i_redraw(Interfaz);
            if(ret>99){
                /* Prueba de esta habitacion superada*/
                Interfaz->pruebas[pos]=TRUE;
                strcpy(buf, "¡¡Lo has conseguido!!");
                win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+1);
                strcpy(buf, "Cumplire mi palabra, ya puedes descartarlo porque no es el Paciente 0:");
                win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+2);
                win_write_pista_sc4(Interfaz->left_down, SOSPECHOSOS[numSosp], FILA_ERROR+3);
            }
            else{
                strcpy(buf, "Sabia que era demasiado dificil, vuleve a hablar conmigo cuando quieras"
                " volver a intentarlo.");
                win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+1);
            }
            break;
            /* BIRDS */
        case 1:
            strcpy(buf, "Quiero conseguir las 5 estrellas en este juego, pero para eso necesito"
            "conseguir al menos 100 puntos. Si lo consigues te dire el nombre de un sospechoso que no es el Paciente 0.");
            win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+1);
            strcpy(buf, "-Usa las flechas para moverte hacia arriba y abajo.");
            win_write_line_sc2(Interfaz->right_up, buf, FILA_ERROR+1);
            strcpy(buf, "-Cada X te sumara 25 puntos.");
            win_write_line_sc2(Interfaz->right_up, buf, FILA_ERROR+3);
            strcpy(buf, "-Q para salir.");
            win_write_line_sc2(Interfaz->right_up, buf, FILA_ERROR+4);
            ret=birds();
            i_set_score(Interfaz, Interfaz->score+ret);
            _i_redraw(Interfaz);
            if(ret>99){
                /* Prueba de esta habitacion superada*/
                Interfaz->pruebas[pos]=TRUE;
                strcpy(buf, "¡¡Lo has conseguido!!");
                win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+1);
                strcpy(buf, "Cumplire mi palabra, ya puedes descartarlo porque no es el Paciente 0:");
                win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+2);
                win_write_pista_sc4(Interfaz->left_down, SOSPECHOSOS[numSosp], FILA_ERROR+3);
            }
            else{
                strcpy(buf, "Sabia que era demasiado dificil, vuleve a hablar conmigo"
                " si quieres volver a intentarlo.");
                win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+1);
            }
            break;
            /* COLOREAR */
        case 2:
            strcpy(buf, "Uf... creo que mi móvil se ha roto. He oido que se puede arreglar poniendo todas las luces en verde"
            "en menos de 100 pasos. Si me ayudas te digo el nombre de un sospechoso que no es el Paciente 0.");
            win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+1);
            ret=colorear_play(Interfaz);
            i_set_score(Interfaz, Interfaz->score+ret);
            _i_redraw(Interfaz);
            if(ret>=150){
                /* Prueba de esta habitacion superada*/
                Interfaz->pruebas[pos]=TRUE;
                strcpy(buf, "¡¡Lo has conseguido arreglar!!¡¡Muchas gracias!!");
                win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+1);
                strcpy(buf, "Cumplire mi palabra, ya puedes descartarlo porque no es el Paciente 0:");
                win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+2);
                win_write_pista_sc4(Interfaz->left_down, SOSPECHOSOS[numSosp], FILA_ERROR+3);
            }
            else{
                strcpy(buf, "Bueno, supongo que hiciste lo que pudiste... Vuelve a hablar conmigo si quieres"
                " volver a intentarlo.");
                win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+1);
            }
            break;
            /* SNAKE */
        case 3:
            strcpy(buf, "Este juego es un clásico: el snake. No pienso ayudarte si no me demuestras que "
            "valoras los clasicos pasandote este nivel con mas de 100 puntos.");
            win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+1);
            ret=snake();
            i_set_score(Interfaz, Interfaz->score+ret);
            _i_redraw(Interfaz);
            if(ret>99){
                /* Prueba de esta habitacion superada*/
                Interfaz->pruebas[pos]=TRUE;
                strcpy(buf, "¡¡Lo has conseguido!!");
                win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+1);
                strcpy(buf, "Cumplire mi palabra, ya puedes descartarlo porque no es el Paciente 0:");
                win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+2);
                win_write_pista_sc4(Interfaz->left_down, SOSPECHOSOS[numSosp], FILA_ERROR+3);
            }
            else{
                strcpy(buf, "Sabia que era demasiado dificil, vuleve a hablar conmigo cuando quieras"
                " volver a intentarlo.");
                win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+1);
            }
            break;
        default:
            break;
        }
    }
    else if(p==ACERTIJO){
        strcpy(buf, "./aux/AcertijosPistas/acertijoX.txt");
        buf[30]='0'+numP;
        f=fopen(buf, "r");
        win_write_pista_sc4(Interfaz->left_down, "Ayudarte yo quiero, mas regalarte no quiero.", FILA_ERROR+1);
        win_write_pista_sc4(Interfaz->left_down, "Un acertijo has de resolver, si a un inocente quieres conocer.", FILA_ERROR+2);
        win_cls(Interfaz->left_up);
        win_cls(Interfaz->right_up);
        win_write_borders4(Interfaz->left_up, Interfaz->right_up, Interfaz->left_down, Interfaz->right_down);
        printf("%c[%d;%dH", 27, 2, 2);
        i+=win_write_pista_sc4(Interfaz->left_up, "ACERTIJO:", i);
        while(fgets(buf, 499, f)!=NULL){
            i+=win_write_pista_sc4(Interfaz->left_up, buf, i);
            i++;
        }
        fclose(f);
        strcpy(buf,"Resuelve el acertijo e introduce por teclado el nombre del sospechoso"
        " del que sepas que no es el Paciente 0 al resolver el acertijo.");
        win_write_line_sc2(Interfaz->right_up, buf, 2);

        buf[0]=0;
        i=0;

        while(1){
            c=fgetc(stdin);
            if(((c>='0' && c<='z') || c==' ') && i<70){
                /* Pasamos a mayusculas */
                if(c>='a' && c<='z') c=c-'a'+'A';
                buf[i]=c;
                _move_to(Interfaz->left_down, 1, i+2);
                printf("%c",c);
                fflush(stdout);
                i++;
            }
            if(c==10){
                buf[i]=0;
                if(strcmp(buf,SOSPECHOSOS[numSosp])==0){
                    /* Prueba de esta habitacion superada*/
                    Interfaz->pruebas[pos]=TRUE;
                }
                else{
                    /* Prueba de esta habitacion no superada*/
                    /* La ponemos a FALSE para que no se puedan probar todas las convinaciones */
                    Interfaz->pruebas[pos]=FALSE;
                }
                break;
            }
            else if(c==127 && i>=1){
                i--;
                _move_to(Interfaz->left_down, 1, i+2);
                printf(" ");
                fflush(stdout);
            }
        }
        
        _i_redraw(Interfaz);
    }
}

/* Generalmente te muestra la pista del personaje y te da la opcion o no de 
resolver un acertijo o minijuego llamando a prueba() */
int hablar_cmd (void *pt, char *inutil, char **passed_msg, int n){
    WORLD *juego=(WORLD *)pt;
    intrf *Interfaz = (intrf *)juego->interfaz;
    int posicion, ret=2;
    Personaje *personaje;
    char nombre[500], pista[500], c, buf[500];
    Bool a;

    if(juego->mapa_open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[0]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    if(juego->agenda->open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[1]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    if(juego->comandos_info_open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[2]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }

    posicion=mapa_getPos(Interfaz->map);
    /* En el deapacho de Santini necesitan estar encendidas las luces */
    if(posicion==9 && (mapa_areConnected(Interfaz->map,9,9)==FALSE)){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[4]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }

    /* Acceder al personaje */
    personaje=habitacion_getPersonaje(mapa_getHab(Interfaz->map,posicion));
    strcpy(nombre,personaje_getNombre(personaje));
    strcat(nombre,":");

    /* Imprime el nombre del personaje */
    limpiarComandos(Interfaz->left_down);
    win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,nombre);

    /* Caso especial, Santini no tiene pista y pide un objeto */
    if(posicion==9){
        ret+=win_write_pista_sc4(Interfaz->left_down, personaje_getPista(personaje), FILA_ERROR+1);
        ret++;
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }

    /* Imprimir el contnido (pista y desafio) */
    ret+=win_write_pista_sc4(Interfaz->left_down, personaje_getPista(personaje), FILA_ERROR+1);
    ret++;

    if(personaje_getPrueba(personaje)!=PERROR){
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR+ret,COLUMNA_ERROR,"Tengo una prueba para ti. Si quieres aceptarla pulsa s, si no quieres pulsa otra tecla.");
        c=fgetc(stdin);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR+ret,COLUMNA_ERROR,ERROR_ESPACIOS);
        if(c=='s' || c=='S'){
            limpiarComandos(Interfaz->left_down);
            win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,nombre);
            prueba(Interfaz, personaje_getPrueba(personaje),personaje_getNumPrueba(personaje), personaje_getNumSosp(personaje), posicion);
        }
        else if(c=='x' || c=='X'){
            limpiarComandos(Interfaz->left_down);
            win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,nombre);
            /* Prueba de esta habitacion superada*/
            Interfaz->pruebas[posicion]=TRUE;
            strcpy(buf, "¡¡Lo has conseguido!!");
            win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+1);
            strcpy(buf, "Cumplire mi palabra, ya puedes descartarlo porque no es el Paciente 0:");
            win_write_pista_sc4(Interfaz->left_down, buf, FILA_ERROR+2);
            win_write_pista_sc4(Interfaz->left_down, SOSPECHOSOS[personaje_getNumSosp(personaje)], FILA_ERROR+3);
        }
    }
    else{
        /* No hay prueba que superar-->prueba superada */
        Interfaz->pruebas[posicion]=1;
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR+ret,COLUMNA_ERROR,"No tengo ninguna prueba para ti.");
    }

    free_strings_array(passed_msg, n);
    free(inutil);
    return 0;
}

/*Resolver_cmd*/
/****************************************************************/
/****************************************************************/
/****************************************************************/
/****************************************************************/
int resolver_cmd (void *pt, char *inutil, char **passed_msg, int n){
    WORLD *juego=(WORLD *)pt;
    intrf *Interfaz = (intrf *)juego->interfaz;
    int posicion, ret=2, i, puntuacion, paux[20], n_punt, flag=0, contador=0;
    char  c, buf[500], name[500], naux[20][500];
    int numPac;
    FILE *f;

    if(juego->mapa_open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[0]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    if(juego->agenda->open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[1]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    if(juego->comandos_info_open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[2]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }

    posicion=mapa_getPos(Interfaz->map);


    /*Nos aseguramos de estar en la zona de cuarentena*/
    if(posicion==2){
        for(i=0;i<NUM_PRUEBAS;i++){
         if(Interfaz->pruebas[i]!=TRUE){
            limpiarComandos(Interfaz->left_down);
            win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[3]);
            strcpy(buf, "Al menos te falta la habitacion ");
            strcat(buf,habitacion_getNombre(mapa_getHab(Interfaz->map,(long) i)));
            buf[strlen(buf)-4]=0;
            win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR+1,COLUMNA_ERROR,buf);
            free_strings_array(passed_msg, n);
            free(inutil);
            return 0;
        }
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[6]);
    }

        buf[0]=0;
        i=0;


    while(1){
            c=fgetc(stdin);

            if(((c>='0' && c<='z') || c==' ') && i<70){
                /* Pasamos a mayusculas */
                if(c>='a' && c<='z') c=c-'a'+'A';
                buf[i]=c;
                _move_to(Interfaz->left_down, 1, i+2);
                printf("%c",c);
                fflush(stdout);
                i++;
            }
            if(c==10){
                buf[i]=0;
                if(strcmp(buf,SOSPECHOSOS[Interfaz->paciente0])==0){
                    /* Has acertado el nombre del paciente 0; has ganado*/
                    limpiarComandos(Interfaz->left_down);
                    win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[4]);
                    /*PUNTUACION*/
                    /*Guardamos el nombre y la puntuacion del jugador*/
                    puntuacion=Interfaz->score+(Interfaz->max_time-Interfaz->elapsed)*3;
                    strcpy(name, jugador_getNombre(Interfaz->player));
                    /*Incluimos la puntuacion en el fichero de puntuacion*/
                    f=fopen("./aux/puntuacion.txt", "r");
                    /*Guardamos en sendos arrays las puntuaciones y nombres de los jugadores en el ranking*/
                    for(i=0;i<20;i++){
                        fscanf(f, "%d %[^\n]", &paux[i], naux[i]);
                        if(puntuacion>paux[i] && flag==0){
                            n_punt=i;
                            flag=1;
                        }
                    }


                    fclose(f);

                    f=fopen("./aux/puntuacion.txt", "w");;

                    /*Caso en el que tu puntuacion este entre las 20 mas altas*/
                    if(flag==1){
                    for(i=0;contador!=20;i++){
                        if(i==n_punt){
                            fprintf(f, "%d %s\n", puntuacion, name);
                            contador++;
                        }
                        /*Pruebas*/
                   /* strcat(naux[0], "aaa");
                        if(i==0) win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,naux[0]);*/
                        if(contador!=20){
                            fprintf(f, "%d %s\n", paux[i], naux[i]);
                            contador++;
                        }
                    }
                    }

                    fclose(f);

                    /*if(i==0) win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,naux[0]);*/

                    /*Pulsa una tecla y vuelve a la pantalla de inicio*/
                    while(1){
                    if(fgetc(stdin)) break;
                        }
                    free_strings_array(passed_msg, n);
                    free(inutil);
                    return -1;
                }
                else{
                    /*Nos has acertado el nombre del paciente 0; has perdido*/
                    limpiarComandos(Interfaz->left_down);
                    win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[5]);
                    /*Pulsa una tecla y vuelve a la pantalla de inicio*/
                    while(1){
                    if(fgetc(stdin)) break;
                        }
                    free_strings_array(passed_msg, n);
                    free(inutil);
                    return -1;
                }
                
                break;
            }
            else if(c==127 && i>=1){
                i--;
                _move_to(Interfaz->left_down, 1, i+2);
                printf(" ");
                fflush(stdout);
            }
        }


  }

     win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[7]);
    free_strings_array(passed_msg, n);
    free(inutil);
    return 0;
   
}


/* FUNCIONES DE MOVERSE */
/*
Tienen bastantes excepciones pero en gran parte funcionan con restos/modulos y partes enteras/division.
0   1   2
3   4   5   9
6   7   8

El 9 queda como una excepción.
Con resto 0 solo puedes ir a la derecha, con resto 2 solo a la izquierda y con resto 1
puedes moverte a ambos lados.
Para subir y bajar a parte de tener resto 1 depende de la parte entera de posicion/3
*/
int subir_cmd (void *pt, char *inutil, char **passed_msg, int n){
    WORLD *juego=(WORLD *)pt;
    intrf *Interfaz = (intrf *)juego->interfaz;
    int posicion;

    if(juego->mapa_open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[0]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    if(juego->agenda->open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[1]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    if(juego->comandos_info_open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[4]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }


    posicion=mapa_getPos(Interfaz->map);

    /* EASTER EGG */
    if(posicion==9 && mapa_areConnected(Interfaz->map,9,9)==FALSE){
        printf("%c[%d;%dH", 27, 2, 2);
        printf("EATER EGG");
        printf("%c[%d;%dH", 27, 4, 2);
        printf("Introduce una tecla para continuar.");
        fflush(stdout);
        fgetc(stdin);
    }

    if(posicion>2 && posicion%3==1){
        posicion-=3;
        mapa_setPos(Interfaz->map, posicion);
        _i_redraw(Interfaz);
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[3]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }

    limpiarComandos(Interfaz->left_down);
    win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[2]);
    free_strings_array(passed_msg, n);
    free(inutil);
    return 0;
}
int bajar_cmd (void *pt, char *inutil, char **passed_msg, int n){
    WORLD *juego=(WORLD *)pt;
    intrf *Interfaz = (intrf *)juego->interfaz;

    if(juego->mapa_open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[0]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    if(juego->agenda->open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[1]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    if(juego->comandos_info_open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[4]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }

    int posicion;

    posicion=mapa_getPos(Interfaz->map);
    if(posicion<6 && posicion%3==1){
        posicion+=3;
        mapa_setPos(Interfaz->map, posicion);
        _i_redraw(Interfaz);
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[3]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    
    limpiarComandos(Interfaz->left_down);
    win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[2]);
    free_strings_array(passed_msg, n);
    free(inutil);
    return 0;
}
int oeste_cmd (void *pt, char *inutil, char **passed_msg, int n){
    WORLD *juego=(WORLD *)pt;
    intrf *Interfaz = (intrf *)juego->interfaz;

    if(juego->mapa_open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[0]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    if(juego->agenda->open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[1]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    if(juego->comandos_info_open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[4]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }

    int posicion;

    posicion=mapa_getPos(Interfaz->map);
    /* Caso especial, el despacho de Santini */
    if(posicion==9){
        posicion=5;
        mapa_setPos(Interfaz->map, posicion);
        _i_redraw(Interfaz);
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[3]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    if(posicion%3!=0){
        posicion-=1;
        mapa_setPos(Interfaz->map, posicion);
        _i_redraw(Interfaz);
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[3]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    
    limpiarComandos(Interfaz->left_down);
    win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[2]);
    free_strings_array(passed_msg, n);
    free(inutil);
    return 0;
}
int este_cmd (void *pt, char *inutil, char **passed_msg, int n){
    WORLD *juego=(WORLD *)pt;
    intrf *Interfaz = (intrf *)juego->interfaz;
    Objeto *obj;

    if(juego->mapa_open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[0]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    if(juego->agenda->open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[1]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    if(juego->comandos_info_open==ABIERTO){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[6]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }

    int posicion;

    posicion=mapa_getPos(Interfaz->map);
    /* Caso especial, el despacho de Santini */
    if(posicion==9){
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[2]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    /* Caso especial, acceder al despacho de Santini */
    else if(posicion==5){
        if(jugador_getObjeto(Interfaz->player)!=NULL){
            obj=jugador_getObjeto(Interfaz->player);
            if(strcmp(objeto_getNombre(obj), "una llave")==0){
                mapa_setConnections(Interfaz->map,5,9);
                win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR+1,COLUMNA_ERROR,passed_msg[7]);
                jugador_deleteObjeto(Interfaz->player);
            }
        }
        if(mapa_areConnected(Interfaz->map,5,9)==TRUE){
            posicion=9;
            mapa_setPos(Interfaz->map, posicion);
            _i_redraw(Interfaz);
            limpiarComandos(Interfaz->left_down);
            win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[3]);
            free_strings_array(passed_msg, n);
            free(inutil);
            return 0;
        }
        else{
            limpiarComandos(Interfaz->left_down);
            win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[4]);
            free_strings_array(passed_msg, n);
            free(inutil);
            return 0;
        }
    }
    /* Caso especial, acceder a la zona en cuarentena */
    else if(posicion==1){
        if(jugador_getObjeto(Interfaz->player)!=NULL){
            obj=jugador_getObjeto(Interfaz->player);
        if(strcmp(objeto_getNombre(obj), "una mascarilla")==0){
            posicion+=1;
            mapa_setPos(Interfaz->map, posicion);
            _i_redraw(Interfaz);
            limpiarComandos(Interfaz->left_down);
            win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[3]);
            free_strings_array(passed_msg, n);
            free(inutil);
            return 0;
        }
        else{
            limpiarComandos(Interfaz->left_down);
            win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[5]);
            free_strings_array(passed_msg, n);
            free(inutil);
            return 0;
        }
        }
        else{
            limpiarComandos(Interfaz->left_down);
            win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[5]);
            free_strings_array(passed_msg, n);
            free(inutil);
            return 0;
        }
    }
    else if(posicion%3!=2 && posicion!=1){
        posicion+=1;
        mapa_setPos(Interfaz->map, posicion);
        _i_redraw(Interfaz);
        limpiarComandos(Interfaz->left_down);
        win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[3]);
        free_strings_array(passed_msg, n);
        free(inutil);
        return 0;
    }
    
    limpiarComandos(Interfaz->left_down);
    win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[2]);
    free_strings_array(passed_msg, n);
    free(inutil);
    return 0;
}

/*
Imprime el mapa a partir de un fichero de texto y un recuadro rojo en la habitación
que marca tu posición. Al igual que moverse utiliza restos y partes enteras para
pintar la habitación en la que estas a partir de la posicion numerica.
*/
void printMapa(int bckg, int text, intrf *q){
    FILE *f;
    char c;
    int pos, i;
    f=fopen("./aux/plantillas/mapa.txt", "r");
    if(f!=NULL){
        pthread_mutex_lock(&lock);
        _move_to(q->left_up, 0,0);
        while(fscanf(f, "%c", &c)==1){
            if(c=='-' || c==10){
                printf("\x1b[%dm" " ", bckg);
                fflush(stdout);
            }
            else if(c=='*'){
                printf("\x1b[%dm" " ", text+10);
                fflush(stdout);
            }
            else{
                printf("\x1b[%dm" "\x1b[%dm" "%c", bckg, text, c);
                fflush(stdout);
            }
        }
        fclose(f);
        pos=mapa_getPos(q->map);
        printf("\x1b[%dm" "\x1b[%dm", 41, text);
        if(pos==9){
            printf("%c[%d;%dH", 27, 10, 113);
            printf("                  ");
            for(i=0; i<5; i++){
                printf("%c[%d;%dH", 27, 11+i, 113);
                printf("  ");
                printf("%c[%d;%dH", 27, 11+i, 129);
                printf("  ");
            }
            printf("%c[%d;%dH", 27, 16, 113);
            printf("                  ");
        }
        else{
            printf("%c[%d;%dH", 27, 2+(pos/3)*8, 32+(pos%3)*27);
            printf("                  ");
            for(i=0; i<5; i++){
                printf("%c[%d;%dH", 27, 3+(pos/3)*8+i, 32+(pos%3)*27);
                printf("  ");
                printf("%c[%d;%dH", 27, 3+(pos/3)*8+i, 48+(pos%3)*27);
                printf("  ");
            }
            printf("%c[%d;%dH", 27, 8+(pos/3)*8, 32+(pos%3)*27);
            printf("                  ");
        }
        fflush(stdout);

        printf("%c[%d;%dH", 27, 25, 0);
        printf("\x1b[%dm" " ", bckg);
        fflush(stdout);
        printf("%c[%d;%dH", 27, 25, 100);
        printf("\x1b[%dm" " ", bckg);
        fflush(stdout);
        printf("%c[%d;%dH", 27, 25, 145);
        printf("\x1b[%dm" " ", bckg);
        fflush(stdout);
        pthread_mutex_unlock(&lock);
    }
}
/* Imrpime el mapa llamando a printMapa() */
int mapa_cmd (void *pt, char *inutil, char **passed_msg, int n){
    WORLD *juego=(WORLD *)pt;
    intrf *Interfaz = (intrf *)juego->interfaz;

    juego->mapa_open=ABIERTO;
    if(juego->comandos_info_open==ABIERTO || juego->agenda->open==ABIERTO){
        juego->comandos_info_open=CERRADO;
        juego->agenda->open=CERRADO;
        _i_redraw(juego->interfaz);
    }
    limpiarComandos(PANTALLA_LADO(juego));
    printMapa(40, 37, Interfaz);    

    free_strings_array(passed_msg, n);
    free(inutil);
    return 0;
}

int salir_cmd (void *pt, char *inutil, char **passed_msg, int n){
    WORLD *juego=(WORLD *)pt;
    char c=0;
    int ret;

    limpiarComandos(PANTALLA_LADO(juego));
    win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[0]);
    while(c!='s' && c!='n' && c!='S' && c!='n'){
        c=fgetc(stdin);
        if(c=='n' || c=='N'){
            limpiarComandos(PANTALLA_LADO(juego));
            free_strings_array(passed_msg, n);
            free(inutil);
            return 0;
        }
    }
    limpiarComandos(PANTALLA_LADO(juego));
    win_write_line_at(PANTALLA_LADO(juego),FILA_ERROR,COLUMNA_ERROR,passed_msg[1]);
    fflush(stdin);
    c=fgetc(stdin);
    free_strings_array(passed_msg, n);
    free(inutil);
    if(c=='0')
        return -1;
    return (int)(c-'0');
}

int comandos_info_cmd (void *pt, char *inutil, char **passed_msg, int n){
    WORLD *juego=(WORLD *)pt;
    sc_rectangle * frontal = juego->interfaz->left_up;
    FILE *f;
    char c[50];
    int i, len;

    juego->comandos_info_open=ABIERTO;

    if(juego->mapa_open==ABIERTO || juego->agenda->open==ABIERTO){
        juego->mapa_open=CERRADO;
        juego->agenda->open=CERRADO;
        _i_redraw(juego->interfaz);
    }

    preparar_prueba(juego->interfaz);
    f=fopen("./aux/Comandos/comandos_info.txt","r");
    fscanf(f, "%s\n%s", c, c);
    fscanf(f, "%s", c);
    pthread_mutex_lock(&lock);
    win_write_line_at_colour(frontal, 5, 4, c, COLOR_COMANDOS, frontal->bg_color);
    i=0;
    len=0;
    while(strcmp(c,"ESTO_VA_AL_FINAL")!=0){
        fscanf(f, "%s", c);
        if(strcmp(c,";")==0){
            i++;
            len=0;
        }
        else if(strcmp(c,"*")==0){
            fscanf(f, "%s", c);
            win_write_line_at_colour(frontal, 7+i, 4+len, c, COLOR_COMANDOS, frontal->bg_color);
            len += strlen(c)+1;
        }
        else if(strcmp(c,"ESTO_VA_AL_FINAL")!=0){
            win_write_line_at(frontal, 7+i, 4+len, c);
            len += strlen(c)+1;
        }
    }

    pthread_mutex_unlock(&lock);
    fclose(f);
    free_strings_array(passed_msg, n);
    free(inutil);
    return 0;
}
