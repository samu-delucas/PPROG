//GRAPH.C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "mapa.h"


#define MAX_HAB 10
#define MAX_OBJ 4
#define MAX_PERS 10

extern char *SOSPECHOSOS[10];

struct _Mapa {
    Habitacion *hab[MAX_HAB]; //Array with the nodes in the graph
    Bool connections[MAX_HAB][MAX_HAB]; //Adjacency matrix
	int posicion;
};

int aleat_num_mod(int inf, int sup, int *num, int n)
{
  int aleatorio, flag, i;

	/*Si no queremos que haya negativos y que la difererncia sea menor de 1*/
	do{
	flag=0;
  	if(inf<0 ||sup-inf<0) return -1;
  	aleatorio=(inf+(int)(sup-inf+1.0)*(rand()/(RAND_MAX+1.0)));
  
  	for(i=0;i<n;i++){
  		if(aleatorio==num[i]) flag=1;
  		}
	}while(flag==1);
	
  return aleatorio;
}


Mapa * mapa_init() {
    Mapa *m=NULL;
    Objeto *o[MAX_OBJ];
    Personaje *p[MAX_PERS];
    int i, j, aux1[MAX_OBJ],aux2[MAX_PERS-1], n, k;
	FILE *f;
	int pistas[3], acertijos[3], minijuegos[3], acertijosSosp[3], minijuegosSosp[3];
	char basura[100], pistasC[9][500], fichero[500], buf[500];

    m = (Mapa *) malloc(sizeof (Mapa));
    if (m==NULL) {
        return NULL;
    }
    
    /*Inicializamos las habitaciones*/
    for(i=0;i<MAX_HAB;i++){
    	m->hab[i]=habitacion_create();
    	if(!m->hab[i]) return NULL;
	}
	
	/*Inicializamos los  objetos*/
    for(i=0;i<MAX_OBJ;i++){
    	o[i]=objeto_create();
    	if(!o[i]) return NULL;
	}

	/*Inicializamos los personajes*/
    for(i=0;i<MAX_PERS;i++){
    	p[i]=personaje_create();
    	if(!p[i]) return NULL;
	}

	/*Inicializamos los ids de personajes*/
	for(i=0;i<MAX_PERS;i++){
		personaje_setId(p[i],i+1);
	}

	for(i=0;i<MAX_PERS;i++){
		aux2[i]=-1;
	}
	
	/*Inicializamos los nombres, pruebas y pistas de los personajes y los metemos en las habitaciones*/
	f=fopen("./aux/start.txt", "r");
	if(f==NULL) return NULL;
	fgets(basura, 10,f);
	/* Leemos de un archivo que pistas, acertijos y minijuegos han tocado */
	for(i=0; i<3; i++){ /* Acertijos */
		fscanf(f, "%d\t%d\n", &acertijos[i],&acertijosSosp[i]);
	}
	for(i=0; i<3; i++){ /* Minijuegos */
		fscanf(f, "%d\t%d\n", &minijuegos[i],&minijuegosSosp[i]);
	}
	for(i=0; i<3; i++){ /* Pistas */
		fscanf(f, "%d%s", &pistas[i],basura);
	}
	fclose(f);
	/* Guardamos las pistas (3 frases por cada 1) */
	strcpy(fichero, "./aux/AcertijosPistas/pistasX.txt");
	fichero[28]='0'+pistas[0];
	f=fopen(fichero, "r");
	for(i=0; i<3; i++){
		fgets(pistasC[i],499,f);
		pistasC[i][strlen(pistasC[i])-1]=0;
	}
	fclose(f);
	fichero[28]='0'+pistas[1];
	f=fopen(fichero, "r");
	for(i=0; i<3; i++){
		fgets(pistasC[i+3],499,f);
		pistasC[i+3][strlen(pistasC[i+3])-1]=0;
	}
	fclose(f);
	fichero[28]='0'+pistas[2];
	f=fopen(fichero, "r");
	for(i=0; i<3; i++){
		fgets(pistasC[i+6],499,f);
		pistasC[i+6][strlen(pistasC[i+6])-1]=0;
	}
	fclose(f);
	for(i=0, j=0, k=0;i<MAX_HAB-1;i++){ 
		n=aleat_num_mod(0,MAX_PERS-2,aux2, MAX_PERS);
		personaje_setNombre(p[i], SOSPECHOSOS[n]); /*Elegimos un personaje aleatorio dentro de la lista de sospechosos quedando uno fuera*/
		if(n%3==0){
			personaje_setPrueba(p[i],ACERTIJO);
			personaje_setNumPrueba(p[i], acertijos[j]);
			personaje_setNumSosp(p[i],acertijosSosp[j]);
			j++;
		}
		else if(n%3==1){
			personaje_setPrueba(p[i],MINIJUEGO);
			personaje_setNumPrueba(p[i], minijuegos[k]);
			personaje_setNumSosp(p[i],minijuegosSosp[k]);
			k++;
		}
		else{
			personaje_setPrueba(p[i],PERROR);
		}
		personaje_setPista(p[i],pistasC[n]);
		habitacion_addPersonaje(m->hab[i], p[i]);
		aux2[i]=n;
	}

	/*Incluimos el personaje que esta siempre en la misma habitacion*/
	personaje_setNombre(p[9], "Santini");
	strcpy(buf,"Estoy tan viciado a un nuevo juego llamado Paciente 0 que ni quiero salir del"
	" despacho. Te dejare que te lleves la mascarilla si encuentras mi ordenador "
	"para que pueda seguir jugando.");
	personaje_setPista(p[9],buf);
	habitacion_addPersonaje(m->hab[9], p[9]);


	/*Inicializamos los ids de objetos*/
	for(i=0;i<MAX_OBJ;i++){
		objeto_setId(o[i],i+1);
	}
	
	/*Inicializamos nombres de los objetos*/
	objeto_setNombre(o[0],"una linterna");
	objeto_setNombre(o[1],"un portatil");
	objeto_setNombre(o[2],"una llave");
	objeto_setNombre(o[3],"una mascarilla");
	
	for(i=0;i<MAX_OBJ-1;i++){
		aux1[i]=-1;
	}
	
	for(i=0;i<MAX_OBJ-1;i++){
		do{
		n=aleat_num_mod(0,MAX_HAB-2,aux1, MAX_OBJ-1);
		}while(n==2); /*No queremos objetos en la zona de cuarentena*/
		habitacion_addObjeto(m->hab[n], o[i]);
		aux1[i]=n;
	}	

	printf("\n");

	/*Añadimos el objeto fijo en DespSantini*/
	habitacion_addObjeto(m->hab[9], o[3]);
	
	/*Inicializamos ids de habitaciones*/
	for(i=0;i<MAX_HAB;i++){
		habitacion_setId(m->hab[i], i);
	}
	
	/*Inicializamos nombres de las habitaciones*/
	habitacion_setNombre(m->hab[0],"biblioteca.txt");
	habitacion_setNombre(m->hab[1],"escalerasP2.txt");
	habitacion_setNombre(m->hab[2],"ZonaCuarentena.txt");
	habitacion_setNombre(m->hab[3],"lab3.txt");
	habitacion_setNombre(m->hab[4],"escalerasP1.txt");
	habitacion_setNombre(m->hab[5],"lab7.txt");
	habitacion_setNombre(m->hab[6],"aula10.txt");
	habitacion_setNombre(m->hab[7],"hall.txt");
	habitacion_setNombre(m->hab[8],"cafeteria.txt");
	habitacion_setNombre(m->hab[9],"DespSantini.txt");
	
	for(i=0;i<MAX_HAB;i++){
		for(j=0;j<MAX_HAB;j++){
			m->connections[i][j]=FALSE;
		}
	}

	/*Planta 3*/
	m->connections[0][1]=TRUE;
	m->connections[1][0]=TRUE;
	/* CONEXION CON LA ZONA EN CUARENTENA */
	/*m->connections[1][2]=TRUE;
	m->connections[2][1]=TRUE;*/
	
	/*Planta 2*/
	m->connections[3][4]=TRUE;
	m->connections[4][3]=TRUE;
	m->connections[4][5]=TRUE;
	m->connections[5][4]=TRUE;
	
	/* CONEXION DE SANTINI ESTA DE PRUEBA EN TRUE */
	/* PERO LUEGO HABRA QUE PONERLA EN FALSE */
	/*m->connections[9][5]=TRUE;
	m->connections[5][9]=TRUE;*/
	
	/*Planta 3*/
	m->connections[6][7]=TRUE;
	m->connections[7][6]=TRUE;
	m->connections[7][8]=TRUE;
	m->connections[8][7]=TRUE;

	/*Conexiones entre plantas*/
	m->connections[1][4]=TRUE;
	m->connections[4][1]=TRUE;
	m->connections[4][7]=TRUE;
	m->connections[7][4]=TRUE;

	/* Empezamos en la posicion Hall (7) */
	m->posicion=7;

    return m;
}


void mapa_setConnections(Mapa *m, long nId1, long nId2){
	if(m){
		m->connections[nId1][nId2]=TRUE;
		m->connections[nId2][nId1]=TRUE;
	}
}

void mapa_free(Mapa *m) {
    int i;
    
    //Comprobacion
    if(!m){
        return;
    }
    //Free nodes
    for (i = 0; i < MAX_HAB; i++) {
        habitacion_free(m->hab[i]);
    }

    free(m);
    return;
}

int find_hab_index(const Mapa *m, long nId1) {
    int i;
    if (!m) return -1;
    for (i = 0; i < MAX_HAB; i++) {
        if (habitacion_getId(m->hab[i]) == nId1)
            return i;
    }
    // ID not found
    return -1;
}

Habitacion *mapa_getHab(const Mapa *m, long nId) {
    
    int x=find_hab_index(m, nId);
    
    if (!m || x == -1){
        return NULL;
    }
        
    return m->hab[x];
}





Bool mapa_areConnected(const Mapa *m, const long nId1, const long nId2) {
    //Control de errores
    if (!m|| find_hab_index(m, nId1) == -1 || find_hab_index(m, nId2) == -1 ) {
        return FALSE;
    }

    return m->connections[nId1][nId2];
}


int mapa_getPos(const Mapa *m){
	if(m!=NULL) return m->posicion;
}
int mapa_setPos(Mapa *m, int a){
	if(m==NULL) return -1;
	m->posicion=a;
	return 1;
}
int mapa_sumPos(Mapa *m, int a){
	if(m==NULL) return -1;
	m->posicion+=a;
	return 1;
}

