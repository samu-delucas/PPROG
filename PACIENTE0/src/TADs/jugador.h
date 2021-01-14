/*Personajes*/


#ifndef JUGADOR_H

#define JUGADOR_H

#include "habpers.h"

/*Define la estructura _Jugador creada en jugador.c*/
typedef struct _Jugador Jugador;

/*Inicializa la estructura del jugador y devuelve NULL en caso de error*/
Jugador* jugador_create();

/*Libera la memoria reservada para un jugador*/
void jugador_free(Jugador *j);

/*Modifica el nombre de un personaje, devuleve OK o ERROR en caso de error*/
Status jugador_setNombre(Jugador *j, char *nombre);

/*A�ade un objeto al jugador o devuelve ERROR en caso de error*/
Status jugador_addObjeto(Jugador *j, Objeto *obj);

/*Elimina un objeto al jugador o devuelve ERROR en caso de error*/
Status jugador_deleteObjeto(Jugador *j);

/*Devuelve el nombre de un jugador dado o NULL en caso de error*/
char *jugador_getNombre(Jugador *j);

/*Devuelve un puntero a la copia del objeto cuyo id es oid*/
Objeto* jugador_getObjeto(Jugador *j);





#endif /*JUGADOR_H*/
