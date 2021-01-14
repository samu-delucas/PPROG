/*
Prepara los acertijos y pistas para ser usados a partir de sus plantillas.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <acertijosPistas.h>

extern char *SOSPECHOSOS[10];

#define MAXCHAR2 1000

/***********************************************/
/* Función conociendo el paciente 0 sortea un  */
/* inocente que no este en el array inocentes  */
/* para que el conjunto global de pistas,      */
/* acertijos y minijuegos solo deje sin        */
/* descartar al paciente 0.                    */
/* Para esto simplemente llama a aleatNum hasta*/
/* que el numero dado no este en el array.     */
/***********************************************/
int posibleInocente(int *inocentes, int *nInocentes, int paciente0){
    int x, i, estado=1;
    assert(inocentes!=NULL || nInocentes!=NULL);
    assert(*nInocentes>=0 && *nInocentes<9);

    while(estado==1){
        estado=0;
        x=aleat_num(0, 9);
        if(x!=paciente0){
            for(i=0; i<*nInocentes; i++){
                if(inocentes[i]==x) {
                    estado=1;
                    break;
                }
            }
        }
        else estado=1;
    }
    inocentes[*nInocentes]=x;
    *nInocentes=*nInocentes+1;

    return x;
}

/***********************************************/
/* Función que sortea los acertijos y los      */
/* sospechosos que descarta cada acertijo.     */
/* Para esto llama a posibleInocente y una vez */
/* sorteados los acertijos e inocentes hace un */
/* switch para llamar a cada acerijo pasando   */
/* como parametro el sospechoso que descarta   */
/* ese acertijo.                               */
/***********************************************/
Status startAcertijos(FILE *f, int *inocentes, int *nInocentes, int paciente0){
    int i;
    int x1[3], x2[3];

    //Errores que por como utilizamos la funcion nunca se deberian dar
    assert(inocentes!=NULL && nInocentes!=NULL && paciente0>=0 && paciente0<=9 && f!=NULL);
    //srand(time(NULL));

    //Sorteamos los acertijos y los guardamos en el array x1
    //Sorteamos los inocentes que se descubren en cada acertijo y los guardamos en x2
    x1[0]=aleat_num(1, 5);
    x2[0]=posibleInocente(inocentes, nInocentes, paciente0);
    fprintf(f, "%d\t%d\n", x1[0], x2[0]);
    x1[1]=x1[0];
    while(x1[1]==x1[0]){
        x1[1]=aleat_num(1, 5);
    }
    x2[1]=posibleInocente(inocentes, nInocentes, paciente0);
    fprintf(f, "%d\t%d\n", x1[1], x2[1]);
    x1[2]=x1[0];
    while(x1[2]==x1[0] || x1[2]==x1[1]){
        x1[2]=aleat_num(1, 5);
    }
    x2[2]=posibleInocente(inocentes, nInocentes, paciente0);
    fprintf(f, "%d\t%d\n", x1[2], x2[2]);

    //Llamamos a las funciones que preparan los 3 acertijos pasando los sospechosos
    //que descarta cada acertijo como parametro de la funcion
    for(i=0; i<3; i++){
        switch (x1[i])
        {
        case 1:
            startAcertijo1(x2[i]);
            break;
        case 2:
            startAcertijo2(x2[i]);
            break;
        case 3:
            startAcertijo3(x2[i]);
            break;
        case 4:
            startAcertijo4(x2[i]);
            break;
        case 5:
            startAcertijo5(x2[i]);
            break;
        
        default:
            return ERROR;
        }
    }
    return OK;
}

/***********************************************/
/* Funciones que leen la plantilla de cada     */
/* acertijo y preparan el fichero acertijoX.txt*/
/* para ser leido ya con los nombres           */
/* sustituidos y todo listo para ser usado.    */
/***********************************************/
void startAcertijo1(int inocente){
    FILE *f=NULL, *g=NULL;
    char acertijo[MAXCHAR2];
    int x, y, z, t, w;

    x=inocente;
    while(x==inocente){
        x=aleat_num(0,9);
    }
    y=x;
    while(y==x || y==inocente){
      y=aleat_num(0, 9);
    }
    z=x;
    while(z==x || z==y || y==inocente){
      z=aleat_num(0, 9);
    }
    t=x;
    while(t==x || t==y || t==z || y==inocente){
      t=aleat_num(0, 9);
    }
    w=x;
    while(w==x || w==y || w==z || w==t || w==inocente){
      w=aleat_num(0, 9);
    }

    f=fopen("./aux/AcertijosPistas/acertijo1Plantilla.txt", "r");
    g=fopen("./aux/AcertijosPistas/acertijo1.txt", "w");

    fgets(acertijo, MAXCHAR2, f);
    fprintf(g, acertijo, SOSPECHOSOS[x], SOSPECHOSOS[inocente], 10, SOSPECHOSOS[y], SOSPECHOSOS[t], 10, SOSPECHOSOS[z], SOSPECHOSOS[w], 10, 10, 10, SOSPECHOSOS[z], SOSPECHOSOS[x], 10, SOSPECHOSOS[y]);

    fclose(g);
    fclose(f);
}

void startAcertijo2(int inocente){
    FILE *f=NULL, *g=NULL;
    char acertijo[MAXCHAR2];
    int x, y, z, t, w;

    x=inocente;
    while(x==inocente){
        x=aleat_num(0,9);
    }
    y=x;
    while(y==x || y==inocente){
      y=aleat_num(0, 9);
    }
    z=x;
    while(z==x || z==y || y==inocente){
      z=aleat_num(0, 9);
    }
    t=x;
    while(t==x || t==y || t==z || y==inocente){
      t=aleat_num(0, 9);
    }
    w=x;
    while(w==x || w==y || w==z || w==t || w==inocente){
      w=aleat_num(0, 9);
    }

    f=fopen("./aux/AcertijosPistas/acertijo2Plantilla.txt", "r");
    g=fopen("./aux/AcertijosPistas/acertijo2.txt", "w");

    fgets(acertijo, MAXCHAR2, f);
    fprintf(g, acertijo, SOSPECHOSOS[x], SOSPECHOSOS[t], 10, SOSPECHOSOS[y], SOSPECHOSOS[inocente], 10, SOSPECHOSOS[z], SOSPECHOSOS[w], 10, 10, SOSPECHOSOS[x], SOSPECHOSOS[y], 10, SOSPECHOSOS[y], SOSPECHOSOS[z], 10, SOSPECHOSOS[z], SOSPECHOSOS[x], SOSPECHOSOS[y]);

    fclose(g);
    fclose(f);
}

void startAcertijo3(int inocente){
    FILE *f=NULL, *g=NULL;
    char acertijo[MAXCHAR2];
    int x, y;

    x=inocente;
    while(x==inocente){
        x=aleat_num(0,9);
    }
    y=x;
    while(y==x || y==inocente){
      y=aleat_num(0, 9);
   }

    f=fopen("./aux/AcertijosPistas/acertijo3Plantilla.txt", "r");
    g=fopen("./aux/AcertijosPistas/acertijo3.txt", "w");

    fgets(acertijo, MAXCHAR2, f);
    fprintf(g, acertijo, SOSPECHOSOS[x], SOSPECHOSOS[y], SOSPECHOSOS[inocente], 10, 10, 10, SOSPECHOSOS[x], SOSPECHOSOS[y], 10);

    fclose(g);
    fclose(f);
}

void startAcertijo4(int inocente){
    FILE *f=NULL, *g=NULL;
    char acertijo[MAXCHAR2];
    int x, y, z;

    x=inocente;
    while(x==inocente){
        x=aleat_num(0,9);
    }
    y=x;
    while(y==x || y==inocente){
      y=aleat_num(0, 9);
   }
   z=x;
    while(z==x || z==y || z==inocente){
      z=aleat_num(0, 9);
   }


    f=fopen("./aux/AcertijosPistas/acertijo4Plantilla.txt", "r");
    g=fopen("./aux/AcertijosPistas/acertijo4.txt", "w");

    fgets(acertijo, MAXCHAR2, f);
    fprintf(g, acertijo, SOSPECHOSOS[x], 10, SOSPECHOSOS[y], 10, 10, SOSPECHOSOS[inocente], 10, SOSPECHOSOS[z]);

    fclose(g);
    fclose(f);
}

void startAcertijo5(int inocente){
    FILE *f=NULL, *g=NULL;
    char acertijo[MAXCHAR2];
    int x, y, z;

    x=inocente;
    while(x==inocente){
        x=aleat_num(0,9);
    }
    y=x;
    while(y==x || y==inocente){
      y=aleat_num(0, 9);
   }
   z=x;
    while(z==x || z==y || z==inocente){
      z=aleat_num(0, 9);
   }


    f=fopen("./aux/AcertijosPistas/acertijo5Plantilla.txt", "r");
    g=fopen("./aux/AcertijosPistas/acertijo5.txt", "w");

    fgets(acertijo, MAXCHAR2, f);
    fprintf(g, acertijo, SOSPECHOSOS[x], SOSPECHOSOS[inocente], SOSPECHOSOS[y], SOSPECHOSOS[z], 10, SOSPECHOSOS[z], SOSPECHOSOS[x], 10, SOSPECHOSOS[y], SOSPECHOSOS[x], 10, SOSPECHOSOS[y], SOSPECHOSOS[inocente], 10, SOSPECHOSOS[inocente], SOSPECHOSOS[z], 10);

    fclose(g);
    fclose(f);
}


/***********************************************/
/* Función que sortea las pistas y los         */
/* sospechosos que descarta cada pista.        */
/* Para esto llama a posibleInocente y una vez */
/* sorteados los acertijos e inocentes hace un */
/* switch para llamar a cada pista pasando     */
/* como parametro el sospechoso que descarta   */
/* esa pista.                                  */
/***********************************************/
Status startPistas(FILE *f, int *inocentes, int *nInocentes, int paciente0){
    int i;
    int x1[3], x2[3];

    //Errores que por como utilizamos la funcion nunca se deberian dar
    assert(inocentes!=NULL && nInocentes!=NULL && paciente0>=0 && paciente0<=9 && f!=NULL);
    //srand(time(NULL));

    //Sorteamos las pistas y los guardamos en el array x1
    //Sorteamos los inocentes que se descubren en cada pista y los guardamos en x2
    x1[0]=aleat_num(1, 6);
    x2[0]=posibleInocente(inocentes, nInocentes, paciente0);
    fprintf(f, "%d\t%d\n", x1[0], x2[0]);
    x1[1]=x1[0];
    while(x1[1]==x1[0]){
        x1[1]=aleat_num(1, 6);
    }
    x2[1]=posibleInocente(inocentes, nInocentes, paciente0);
    fprintf(f, "%d\t%d\n", x1[1], x2[1]);
    x1[2]=x1[0];
    while(x1[2]==x1[0] || x1[2]==x1[1]){
        x1[2]=aleat_num(1, 6);
    }
    x2[2]=posibleInocente(inocentes, nInocentes, paciente0);
    fprintf(f, "%d\t%d\n", x1[2], x2[2]);

    //Llamamos a las funciones que preparan las 3 pistas pasando los sospechosos
    //que descarta cada acertijo como parametro de la funcion
    for(i=0; i<3; i++){
        switch (x1[i])
        {
        case 1:
            startPistas1(x2[i]);
            break;
        case 2:
            startPistas2(x2[i]);
            break;
        case 3:
            startPistas3(x2[i]);
            break;
        case 4:
            startPistas4(x2[i]);
            break;
        case 5:
            startPistas5(x2[i]);
            break;
        case 6:
            startPistas6(x2[i]);
            break;
        
        default:
            return ERROR;
        }
    }
    return OK;
}

/***********************************************/
/* Funciones que leen la plantilla de cada     */
/* pista y preparan el fichero pistaX.txt      */
/* para ser leido ya con los nombres           */
/* sustituidos y todo listo para ser usado.    */
/***********************************************/
void startPistas1(int inocente){
    FILE *f=NULL, *g=NULL;
    char pistas[MAXCHAR2];
    int x, y;

    y=aleat_num(0,9);

    x=inocente;
    while(x==inocente){
        x=aleat_num(0,9);
    }

    if(y%2==0) {
        f=fopen("./aux/AcertijosPistas/pistas1PlantillaA.txt", "r");
        g=fopen("./aux/AcertijosPistas/pistas1.txt", "w");

        fgets(pistas, MAXCHAR2, f);
        fprintf(g, pistas, SOSPECHOSOS[x], SOSPECHOSOS[inocente], 10, 10);

        fclose(g);
        fclose(f);
    }
    else {
        f=fopen("./aux/AcertijosPistas/pistas1PlantillaB.txt", "r");
        g=fopen("./aux/AcertijosPistas/pistas1.txt", "w");

        fgets(pistas, MAXCHAR2, f);
        fprintf(g, pistas, SOSPECHOSOS[inocente], SOSPECHOSOS[x], 10, 10);

        fclose(g);
        fclose(f);
    }
}

void startPistas2(int inocente){
    FILE *f=NULL, *g=NULL;
    char pistas[MAXCHAR2];
    int x, y;

    y=aleat_num(0,9);

    x=inocente;
    while(x==inocente){
        x=aleat_num(0,9);
    }

    if(y%2==0) {
        f=fopen("./aux/AcertijosPistas/pistas2PlantillaA.txt", "r");
        g=fopen("./aux/AcertijosPistas/pistas2.txt", "w");

        fgets(pistas, MAXCHAR2, f);
        fprintf(g, pistas, SOSPECHOSOS[inocente], SOSPECHOSOS[x], 10, 10, SOSPECHOSOS[x]);

        fclose(g);
        fclose(f);
    }
    else {
        f=fopen("./aux/AcertijosPistas/pistas2PlantillaB.txt", "r");
        g=fopen("./aux/AcertijosPistas/pistas2.txt", "w");

        fgets(pistas, MAXCHAR2, f);
        fprintf(g, pistas, SOSPECHOSOS[x], SOSPECHOSOS[inocente], 10, 10, SOSPECHOSOS[x]);

        fclose(g);
        fclose(f);
    }
}

void startPistas3(int inocente){
    FILE *f=NULL, *g=NULL;
    char pistas[MAXCHAR2];
    int x, y, z, t;

    x=inocente;
    while(x==inocente){
        x=aleat_num(0,9);
    }
    y=x;
    while(y==x || y==inocente){
      y=aleat_num(0, 9);
    }
    z=x;
    while(z==x || z==y || y==inocente){
      z=aleat_num(0, 9);
    }
    t=x;
    while(t==x || t==y || t==z || y==inocente){
      t=aleat_num(0, 9);
    }

    f=fopen("./aux/AcertijosPistas/pistas3Plantilla.txt", "r");
    g=fopen("./aux/AcertijosPistas/pistas3.txt", "w");

    fgets(pistas, MAXCHAR2, f);
    fprintf(g, pistas, SOSPECHOSOS[x], SOSPECHOSOS[y], SOSPECHOSOS[inocente],  10, SOSPECHOSOS[inocente],  SOSPECHOSOS[z], SOSPECHOSOS[x], 10, SOSPECHOSOS[y], SOSPECHOSOS[t], SOSPECHOSOS[inocente]);

    fclose(g);
    fclose(f);
}

void startPistas4(int inocente){
    FILE *f=NULL, *g=NULL;
    char pistas[MAXCHAR2];
    int x, y;

    y=aleat_num(0,9);

    x=inocente;
    while(x==inocente){
        x=aleat_num(0,9);
    }

    if(y%2==0) {
        f=fopen("./aux/AcertijosPistas/pistas4PlantillaA.txt", "r");
        g=fopen("./aux/AcertijosPistas/pistas4.txt", "w");

        fgets(pistas, MAXCHAR2, f);
        fprintf(g, pistas, 10, SOSPECHOSOS[inocente], SOSPECHOSOS[x], 10, SOSPECHOSOS[x]);

        fclose(g);
        fclose(f);
    }
    else {
        f=fopen("./aux/AcertijosPistas/pistas4PlantillaB.txt", "r");
        g=fopen("./aux/AcertijosPistas/pistas4.txt", "w");

        fgets(pistas, MAXCHAR2, f);
        fprintf(g, pistas, 10, SOSPECHOSOS[x], SOSPECHOSOS[inocente], 10, SOSPECHOSOS[x]);

        fclose(g);
        fclose(f);
    }
}

void startPistas5(int inocente){
    FILE *f=NULL, *g=NULL;
    char pistas[MAXCHAR2];
    int x, y, num;

    num=aleat_num(0,9);

    x=inocente;
    while(x==inocente){
        x=aleat_num(0,9);
    }
    y=x;
    while(y==x || y==inocente){
      y=aleat_num(0, 9);
    }

    if(num%2==0) {
        f=fopen("./aux/AcertijosPistas/pistas5PlantillaA.txt", "r");
        g=fopen("./aux/AcertijosPistas/pistas5.txt", "w");

        fgets(pistas, MAXCHAR2, f);
        fprintf(g, pistas, SOSPECHOSOS[x], SOSPECHOSOS[y], SOSPECHOSOS[inocente], 10, SOSPECHOSOS[x], 10, SOSPECHOSOS[y], SOSPECHOSOS[x]);

        fclose(g);
        fclose(f);
    }
    else {
        f=fopen("./aux/AcertijosPistas/pistas5PlantillaB.txt", "r");
        g=fopen("./aux/AcertijosPistas/pistas5.txt", "w");

        fgets(pistas, MAXCHAR2, f);
        fprintf(g, pistas, SOSPECHOSOS[x], SOSPECHOSOS[inocente], SOSPECHOSOS[y], 10, SOSPECHOSOS[x], 10, SOSPECHOSOS[inocente], SOSPECHOSOS[x]);

        fclose(g);
        fclose(f);
    }
}

void startPistas6(int inocente){
    FILE *f=NULL, *g=NULL;
    char pistas[MAXCHAR2];
    int y;

    y=aleat_num(0,9);

    if(y%2==0) {
        f=fopen("./aux/AcertijosPistas/pistas6PlantillaA.txt", "r");
        g=fopen("./aux/AcertijosPistas/pistas6.txt", "w");

        fgets(pistas, MAXCHAR2, f);
        fprintf(g, pistas, 10, 10, SOSPECHOSOS[inocente]);

        fclose(g);
        fclose(f);
    }
    else {
        f=fopen("./aux/AcertijosPistas/pistas6PlantillaB.txt", "r");
        g=fopen("./aux/AcertijosPistas/pistas6.txt", "w");

        fgets(pistas, MAXCHAR2, f);
        fprintf(g, pistas, 10, 10, SOSPECHOSOS[inocente]);

        fclose(g);
        fclose(f);
    }
}