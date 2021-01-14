#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "jugador.h"
#include "mapa.h"

int main(){

	Mapa *m=NULL;
	char n[50];
	int i;
	Habitacion *h=NULL;
	Personaje *p=NULL;
	Objeto *o=NULL;

	srand(time(NULL));

	m=mapa_init();

	for(i=0;i<10;i++){
	h=mapa_getHab (m, i);

	p=habitacion_getPersonaje(h);
	o=habitacion_getObjeto(h);	

	strcpy(n,habitacion_getNombre(h));

	printf("%s\n", n);
	fflush(stdout);

	if(p!=NULL){
	strcpy(n, personaje_getNombre(p));

	printf("%s\n", n);
	fflush(stdout);
	}

	if(o!=NULL){
	strcpy(n, objeto_getNombre(o));


	printf("%s\n", n);
	fflush(stdout);
	}



	}

	mapa_free(m);
        
	
	return 0;
}


