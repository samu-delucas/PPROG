#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "habpers.h"

//Localizacion nombre reto/pista objetos minijuegos

struct _Personaje{
	int id;
	char nombre[MAX];
	Prueba prueba;
	int numPrueba;
	int numSosp;
	char pista[500];
};



/*Inicializa la estructura del personaje y devuelve NULL en caso de error*/
Personaje* personaje_create(){
	
	Personaje *p;
		
	p=(Personaje *)malloc(sizeof(Personaje));
	
	if(p==NULL) return NULL;
	
	p->id=-1;
	strcpy(p->nombre, "");
	p->prueba=PERROR;
	p->numPrueba=-1;
	p->numSosp=-1;
	strcpy(p->pista, "");
	
	return p;
	
}

/*Libera la memoria reservada para un personaje*/
void personaje_free(Personaje *p){
	
	if(!p) return;
	
	free(p);
	
	return;
}

/*Modifica el numPrueba de un personaje, devuelve OK o ERROR en caso de error*/
Status personaje_setNumPrueba(Personaje *p, int n){
	
	if(!p) return ERROR;
	
	p->numPrueba=n;
	
	return OK;
}

/*Modifica el numero de Sospechoso que libera un personaje, devuelve OK o ERROR en caso de error*/
Status personaje_setNumSosp(Personaje *p, int n){
	
	if(!p || n<0) return ERROR;
	
	p->numSosp=n;
	
	return OK;
}

/*Modifica la pista de un personaje, devuelve OK o ERROR en caso de error*/
Status personaje_setPista(Personaje *p, char *pista){
	
	if(!p || pista==NULL) return ERROR;
	
	strcpy(p->pista, pista);

	return OK;
}

/*Modifica el id de un personaje, devuelve OK o ERROR en caso de error*/
Status personaje_setId(Personaje *p, int id){
	
	if(!p || id<0) return ERROR;
	
	p->id=id;
	
	return OK;
}

/*Modifica el nombre de un personaje, devuleve OK o ERROR en caso de error*/
Status personaje_setNombre(Personaje *p, char *n){
	
	if(p==NULL || n==NULL) return ERROR;
	
	strcpy(p->nombre, n);
	
	return OK;
	
}

/*Modifica la prueba de un personaje, devuelve OK o ERROR en caso de error*/
Status personaje_setPrueba(Personaje *p, Prueba pr){
	
	if(p==NULL || (!(pr==ACERTIJO) && !(pr==MINIJUEGO) && !(pr==PERROR))) return ERROR;
	
	p->prueba=pr;
	
	return OK;
	
}
/*Devuelve el id de un personaje dado o -1 en caso de error*/
int personaje_getId(Personaje *p){
	
	if(!p) return -1;
	
	return p->id;	
}

/*Devuelve el numPrueba de un personaje dado o -1 en caso de error*/
int personaje_getNumPrueba(Personaje *p){
	
	if(!p) return -1;
	
	return p->numPrueba;	
}

/*Devuelve el numero de sospechoso que libera un personaje dado o -1 en caso de error*/
int personaje_getNumSosp(Personaje *p){
	
	if(!p) return -1;
	
	return p->numSosp;	
}

/*Devuelve el nombre de un personaje dado o NULL en caso de error*/
char *personaje_getNombre(Personaje *p){
	
	if(!p) return NULL;
	
	return p->nombre;
}

/*Devuelve la pista dado o NULL en caso de error*/
char *personaje_getPista(Personaje *p){
	
	if(!p) return NULL;
	
	return p->pista;
}

/*Devuelve el tipo de prueba de un personaje dado o PERROR en caso de error*/
Prueba personaje_getPrueba(Personaje *p){
	
	if(!p) return PERROR;
	
	return p->prueba;
}



/*Imprime las caracteristicas de un personaje o -1 en caso de error*/
int print_pers(Personaje *p){
	
	int cont=0;
	
	if(p==NULL) return -1;
	
	cont+=fprintf(stdout, "%s\n", p->nombre);
	
	return cont;
}


