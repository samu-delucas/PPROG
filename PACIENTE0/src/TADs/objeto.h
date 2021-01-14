/*Objeto*/


#ifndef OBJETO_H

#define OBJETO_H

#include "types.h"

/*Define la estructura _Habitacion creada en personaje.c*/
typedef struct _Objeto Objeto;

/*Inicializa la estructura de la habitacion y devuelve NULL en caso de error*/
Objeto *objeto_create();

/*Libera la memoria reservada para una habitacion*/
void objeto_free(Objeto *o);

/*Modifica el id de un objeto, devuleve OK o ERROR en caso de error*/
Status objeto_setId(Objeto *o, int oid);

/*Modifica el nombre de un objeto, devuleve OK o ERROR en caso de error*/
Status objeto_setNombre(Objeto *o, char *nombre);

/*Modifica la descripcion de un objeto, devuleve OK o ERROR en caso de error*/
Status objeto_setDescripcion(Objeto *o, char *d);

/*A�ade una posesion al objeto o devuelve ERROR en caso de error*/
Status objeto_setPosesion(Objeto *o, Bool pos);

/*Devuelve el id de un objeto dado o -1 en caso de error*/
int objeto_getId(Objeto *o);

/*Devuelve el nombre de una habitacion dada o NULL en caso de error*/
char *objeto_getNombre(Objeto *o);

/*Devuelve la descripcion de un objeto dado o NULL en caso de error*/
char *objeto_getDescripcion(Objeto *o);

/*Devuelve TRUE en caso de que el objeto est� en posesion y FALSE en caso contrario o en caso de error*/
Bool objeto_getPosesion(Objeto *o);



#endif /*OBJETO_h*/
