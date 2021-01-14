#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "habpers.h"


/*Quitar la descripcion*/


struct _Habitacion{
	int id;
	char nombre[MAX];
	char desc[MAX];
	Objeto *obj;
	Personaje *pers;
};





/*Inicializa la estructura de la habitacion y devuelve NULL en caso de error*/
Habitacion *habitacion_create(){
	
	Habitacion *h;
	int i;
		
	h=(Habitacion *)malloc(sizeof(Habitacion));
	
	if(h==NULL) return NULL;
	
	h->id=-1;
	strcpy(h->nombre, "");
	strcpy(h->desc, "");
	h->obj=NULL;
	h->pers=NULL;
	
	
	return h;
	
}

/*Libera la memoria reservada para una habitacion*/
void habitacion_free(Habitacion *h){
	
	int i;
	
	if(h==NULL) return;
	
	if(h->obj) objeto_free(h->obj);
	if(h->pers) personaje_free(h->pers);
	free(h);
	
}


/*Modifica la descripcion de una habitacion, devuleve OK o ERROR en caso de error*/
Status habitacion_setId(Habitacion *h, int id){
	
	if(id<0 || h==NULL) return ERROR;
	
	h->id=id;
	
	return OK;
	
}

/*Modifica la descripcion de una habitacion, devuleve OK o ERROR en caso de error*/
Status habitacion_setNombre(Habitacion *h, char *nombre){
	
	if(h==NULL || nombre==NULL) return ERROR;
	
	strcpy(h->nombre, nombre);
	
	return OK;
}

/*Modifica la descripcion de una habitacion, devuleve OK o ERROR en caso de error*/
Status habitacion_setDescripcion(Habitacion *h, char *d){
		
    if(h==NULL || d==NULL) return ERROR;
	
	strcpy(h->desc, d);
	
	return OK;
	
}



/*Borra un objeto de una habitación o devuelve ERROR en caso de error*/
Status habitacion_deleteObjeto(Habitacion *h){
	if(h==NULL)
		return ERROR;

	/*objeto_free(h->obj);*/

	h->obj=NULL;

	return OK;
}


/*A�ade un objeto a la habitacion y devuelve el objeto que estaba anteriormente en la posicion o NULL*/
Status habitacion_addObjeto(Habitacion *h, Objeto *obj){
		
		if(h==NULL || obj==NULL) return ERROR;
		
		
		h->obj=obj;
		
		return OK;		
}


/*A�ade un personaje a la habitacion o devuelve ERROR en caso de error*/
Status habitacion_addPersonaje(Habitacion *h, Personaje *p){
	
	if(h==NULL || p==NULL) return ERROR;
		
		h->pers=p;
		
		return OK;
}


/*Devuelve el id de una habitacion dada o -1 en caso de error*/
int habitacion_getId(Habitacion *h){
	
	if(h==NULL) return -1;
	
	return h->id;
}


/*Devuelve el nombre de una habitacion dada o NULL en caso de error*/
char *habitacion_getNombre(Habitacion *h){
	
	if(h==NULL) return NULL;
	
	return h->nombre;
}

/*Devuelve la descripcion de una habitacion dada o NULL en caso de error*/
char *habitacion_getDescripcion(Habitacion *h){
	
	if(h==NULL) return NULL;
	
	return h->desc;
	
}


/*Devuelve un puntero al objeto */
Objeto* habitacion_getObjeto(Habitacion *h){
		
	if(h==NULL) return NULL;
	
	return h->obj;
	
}




/*Devuelve el personaje*/
Personaje* habitacion_getPersonaje(Habitacion *h){
	
	if(h==NULL) return NULL;

	return h->pers;
}



/*Imprime el nombre de la habitacion*/
int print_habname(Habitacion *h){
    
    int cont=0;
	
	if(h==NULL) return -1;
	
	cont+=fprintf(stdout, "%s\n", h->nombre);
	cont+=fprintf(stdout, "%s\n", h->desc);
	fflush(stdout);
        
        return cont;
}







