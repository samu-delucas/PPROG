#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "objeto.h"

//Localizacion nombre reto/pista objetos minijuegos

struct _Objeto{
	int id;
	char nombre[50];
	char desc[50];
	Bool posesion;
		
};



/*Inicializa la estructura del objeto y devuelve NULL en caso de error*/
Objeto* objeto_create(){
	
	Objeto *o;
		
	o=(Objeto*)malloc(sizeof(Objeto));
	
	if(o==NULL) return NULL;
	
	o->id=-1;
	strcpy(o->nombre, "");
	strcpy(o->desc, "");
	o->posesion=FALSE;
	
	return o;
	
}

/*Libera la memoria reservada para un objeto*/
void objeto_free(Objeto *o){
	
	if(o==NULL) return;
	
	free(o);
	return;
}

/*Modifica el id de un objeto, devuelve OK o ERROR en caso de error*/
Status objeto_setId(Objeto *o, int oid){
	
	if(o==NULL || oid<0) return ERROR;
	
	o->id=oid;
	
	return OK;
}

/*Modifica el nombre de un objeto, devuleve OK o ERROR en caso de error*/
Status objeto_setNombre(Objeto *o, char *n){
	
	if(o==NULL || n==NULL) return ERROR;
	
	strcpy(o->nombre, n);
	
	
	return OK;
	
}


/*Modifica la descripcion de un objeto, devuleve OK o ERROR en caso de error*/
Status objeto_setDescripcion(Objeto *o, char *d){
	
	if(o==NULL || d==NULL) return ERROR;
	
	strcpy(o->desc, d);
	
	return OK;
	
}

/*A�ade una posesion al objeto o devuelve ERROR en caso de error*/
Status objeto_setPosesion(Objeto *o, Bool pos){
	
	if(o==NULL || !(pos==TRUE || pos==FALSE)) return ERROR;
	
	o->posesion=pos;
	
	return OK;
}

/*Devuelve el id de un objeto dada o -1 en caso de error*/
int objeto_getId(Objeto *o){
	
	if(o==NULL) return -1;
	
	return o->id;
}


/*Devuelve el nombre de un objeto dado o NULL en caso de error*/
char *objeto_getNombre(Objeto *o){
	
	if(o==NULL) return NULL;
	
	return o->nombre;
}

/*Devuelve la descripcion de una habitacion dada o NULL en caso de error*/
char *objeto_getDescripcion(Objeto *o){
	
	if(o==NULL) return NULL;
	
	return o->desc;
}

/*Devuelve TRUE en caso de que el objeto est� en posesion y FALSE en caso contrario o en caso de error*/
Bool objeto_getPosesion(Objeto *o){
	
	if(o==NULL) return FALSE;
	
	return o->posesion;
		
}



