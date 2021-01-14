


#ifndef HABPERS_H

#define HABPERS_H

#include "types.h"
#include "objeto.h"

#define MAX 50

/*Define la estructura _Habitacion creada en personaje.c*/
typedef struct _Habitacion Habitacion;

/*Define la estructura _Personaje creada en personaje.c*/
typedef struct _Personaje Personaje;

/*Funciones relativas a la habitacion*/

/*Inicializa la estructura de la habitacion y devuelve NULL en caso de error*/
Habitacion *habitacion_create();

/*Libera la memoria reservada para una habitacion*/
void habitacion_free(Habitacion *h);

/*Modifica la descripcion de una habitacion, devuleve OK o ERROR en caso de error*/
Status habitacion_setId(Habitacion *h, int id);

/*Modifica la descripcion de una habitacion, devuleve OK o ERROR en caso de error*/
Status habitacion_setNombre(Habitacion *h, char *nombre);

/*Modifica la descripcion de una habitacion, devuleve OK o ERROR en caso de error*/
Status habitacion_setDescripcion(Habitacion *h, char *d);

/*A�ade un objeto a la habitacion o devuelve ERROR en caso de error*/
Status habitacion_addObjeto(Habitacion *h, Objeto *obj);

/*Borra un objeto de una habitación o devuelve ERROR en caso de error*/
Status habitacion_deleteObjeto(Habitacion *h);

/*A�ade un personaje a la habitacion o devuelve ERROR en caso de error*/
Status habitacion_addPersonaje(Habitacion *h, Personaje *p);

/*Devuelve el id de una habitacion dada o -1 en caso de error*/
int habitacion_getId(Habitacion *h);

/*Devuelve el nombre de una habitacion dada o NULL en caso de error*/
char *habitacion_getNombre(Habitacion *h);

/*Devuelve la descripcion de una habitacion dada o NULL en caso de error*/
char *habitacion_getDescripcion(Habitacion *h);

//*Devuelve un puntero al objeto o NULL en caso de error*/
Objeto* habitacion_getObjeto(Habitacion *h);

/*Devuelve un puntero a la copia del personaje cuyo id es pid*/
Personaje* habitacion_getPersonaje(Habitacion *p);

/*Imprime el nombre de la habitacion*/
int print_habname(Habitacion *h);

/*Funciones relativas al personaje*/


/*Inicializa la estructura del personaje y devuelve NULL en caso de error*/
Personaje* personaje_create();

/*Libera la memoria reservada para un personaje*/
void personaje_free(Personaje *p);

/*Modifica el id de un personaje, devuelve OK o ERROR en caso de error*/
Status personaje_setId(Personaje *p, int id);

/*Modifica el id de un personaje, devuelve OK o ERROR en caso de error*/
Status personaje_setNumPrueba(Personaje *p, int n);

/*Modifica el numero de Sospechoso que libera un personaje, devuelve OK o ERROR en caso de error*/
Status personaje_setNumSosp(Personaje *p, int n);

/*Modifica el nombre de un personaje, devuleve OK o ERROR en caso de error*/
Status personaje_setNombre(Personaje *p, char *nombre);

/*Modifica el nombre de un personaje, devuleve OK o ERROR en caso de error*/
Status personaje_setPista(Personaje *p, char *pista);

/*Modifica la prueba de un personaje, devuelve OK o ERROR en caso de error*/
Status personaje_setPrueba(Personaje *p, Prueba pr);

/*Devuelve el id de un personaje dado o -1 en caso de error*/
int personaje_getId(Personaje *p);

/*Devuelve el nombre de un personaje dado o NULL en caso de error*/
char *personaje_getNombre(Personaje *p);

/*Devuelve el tipo de prueba de un personaje dado o PERROR en caso de error*/
Prueba personaje_getPrueba(Personaje *p);

/*Devuelve la pista dado o NULL en caso de error*/
char *personaje_getPista(Personaje *p);

/*Imprime las caracteristicas de un personaje o -1 en caso de error*/
int print_pers(Personaje *p);

/*Devuelve el numPrueba de un personaje dado o -1 en caso de error*/
int personaje_getNumPrueba(Personaje *p);

/*Devuelve el numero de sospechoso que libera un personaje dado o -1 en caso de error*/
int personaje_getNumSosp(Personaje *p);

#endif /*HABPERS_H*/
