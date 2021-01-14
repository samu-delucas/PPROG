#ifndef SCO_H
#define SCO_H

#include <rectangles.h>
#include <jugador.h>
#include <intrf.h>
#include <cop.h>

#define SALIR_COMANDO -1

typedef struct _Agenda Agenda;
typedef struct _ARGComandos ARGComandos;

/*Importantes*/
/*
Devuelve el Procesador de comandos con asociaciones a partir 
del fichero en el que los comandos hayan sido definidos
Argumento: fichero con comandos
*/
CoP * comandos_set(char *);
/*
Devuelve el argumento 3 de CoP_execute, contiene alguna información
necesaria para imprimir algunos comandos
Args:
-Pantalla donde se imprimirá la agenda/mapa
-Pantalla y línea/col donde saldrán los mensajes simples de
los comandos (ej, Fuiste al Norte...)
-Array con el nombre de cada sospechoso y el número de ellos
(para que te puedan salir en la agenda)
*/
ARGComandos *ARGComandos_create(intrf *, char**nombres, int num_sospechosos);
/*Libera un ARGComandos*/
void ARGComandos_free(ARGComandos* com);
/*ugarda agenda*/
void agenda_save(ARGComandos * com, FILE * f);
ARGComandos *ARGComandos_load(intrf *interfaz, char**nombres, int num_sospechosos, FILE *f);

/*No tan importantes*/
void free_strings_array(char **strings, int num_strings);
Agenda * agenda_create(char**nombres, int num_sospechosos);
void agenda_refrescar(int num_msg, char **msg, char**sosp, int *marca, sc_rectangle * frontal, int num_sospechosos);
int agenda_cmd (void *pt, char *inutil, char **passed_msg, int n);
int cerrar_cmd (void *pt, char *inutil, char **passed_msg, int n);
int escribir_cmd (void *pt, char *mensaje, char **passed_msg, int n);
int tachar_cmd (void *pt, char *mensaje, char **passed_msg, int n);
int error_cmd (void *pt, char *inutil, char **passed_msg, int n);
int hablar_cmd (void *pt, char *inutil, char **passed_msg, int n);
int subir_cmd (void *pt, char *inutil, char **passed_msg, int n);
int bajar_cmd (void *pt, char *inutil, char **passed_msg, int n);
int este_cmd (void *pt, char *inutil, char **passed_msg, int n);
int oeste_cmd (void *pt, char *inutil, char **passed_msg, int n);
int mapa_cmd (void *pt, char *inutil, char **passed_msg, int n);
int resolver_cmd (void *pt, char *inutil, char **passed_msg, int n);
int salir_cmd (void *pt, char *inutil, char **passed_msg, int n);
int coger_cmd (void *pt, char *inutil, char **passed_msg, int n);
int comandos_info_cmd (void *pt, char *inutil, char **passed_msg, int n);

#endif
