#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jugador.h"


#define MAX_OBJ 3

//Localizacion nombre reto/pista objetos minijuegos

struct _Jugador{
	char nombre[MAX];
	Objeto *obj;	
};



/*Inicializa la estructura del jugador y devuelve NULL en caso de error*/
Jugador* jugador_create(){
	
	Jugador *j;
		
	j=(Jugador *)malloc(sizeof(Jugador));
	
	if(j==NULL) return NULL;
	
	strcpy(j->nombre, "");
	j->obj=NULL;
	
	return j;
	
}

/*Libera la memoria reservada para un jugador*/
void jugador_free(Jugador *j){
	if(j){
		if(j->obj!=NULL)
			objeto_free(j->obj);
	
		free(j);
	}
    
	return;
}


/*Modifica el nombre de un jugador, devuleve OK o ERROR en caso de error*/
Status jugador_setNombre(Jugador *j, char *n){
	
	if(j==NULL || n==NULL) return ERROR;
	
	strncpy(j->nombre, n, MAX);
	
	return OK;
	
}


/*A�ade un objeto al jugador o devuelve ERROR en caso de error*/

Status jugador_addObjeto(Jugador *j, Objeto *obj){

	Objeto *aux;
		
	if(j==NULL || obj==NULL) return ERROR;
		
	j->obj=obj;	
		
	return OK;		
}



/*Elimina un objeto al jugador o devuelve ERROR en caso de error*/
Status jugador_deleteObjeto(Jugador *j){

	if(j==NULL) return ERROR;

	objeto_free(j->obj);
	j->obj=NULL;	

	return OK;		
}

/*Devuelve el nombre de un jugador dado o NULL en caso de error*/
char *jugador_getNombre(Jugador *j){
	
	if(j==NULL) return NULL;
	
	return j->nombre;
}



/*Devuelve un puntero a la copia del objeto cuyo id es oid*/
Objeto* jugador_getObjeto(Jugador *j){
	
	
	if(j==NULL) return NULL;
	
	return j->obj;
	
}


