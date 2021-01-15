/* GRUPO: COVidóneos */

/* FICHERO CON LAS FUNCIONES DE LOS OBJETOS DEL MALETÍN */

#include <stdlib.h>
#include <string.h>
#include "object.h"
#include "config.h"

#define NAME_LEN 32

struct _Object {
    char name[NAME_LEN];
    Bool used;
};

/* En esta función guardamos memoria para el maletín y para cada uno de sus objetos.               */
/* Además, definimos los nombres de estos y que no están usados a través de la estructura _Object. */
Object** setObjects() {
    int i, j;
    Object **maletin;

    maletin = (Object**) malloc(TAM_MALETIN*sizeof(Object*));
    if (maletin==NULL) return NULL;

    i = 0;
    /* Mascarilla */
    maletin[i] = (Object*) malloc(sizeof(Object));
    if (maletin[i]==NULL) {
        free(maletin);
        return NULL;
    }
    if (loadStr(maletin[i]->name, "./res/text/mascarilla.txt")==ERR) {
        free(maletin[i]);
        free(maletin);
        return NULL;
    }
    maletin[i]->used = FALSE;
    i++;

    /* PCR */
    maletin[i] = (Object*) malloc(sizeof(Object));
    if (maletin[i]==NULL) {
        for (j=0; j<i; j++) free(maletin[j]);
        free(maletin);
        return NULL;
    }
    if (loadStr(maletin[i]->name, "./res/text/pcr.txt")==ERR) {
        for (j=0; j<=i; j++) free(maletin[j]);
        free(maletin);
        return NULL;
    }
    maletin[i]->used = FALSE;
    i++;

    /* GUANTES */
    maletin[i] = (Object*) malloc(sizeof(Object));
    if (maletin[i]==NULL) {
        for (j=0; j<i; j++) free(maletin[j]);
        free(maletin);
        return NULL;
    }
    if (loadStr(maletin[i]->name, "./res/text/guantes.txt")==ERR) {
        for (j=0; j<=i; j++) free(maletin[j]);
        free(maletin);
        return NULL;
    }
    maletin[i]->used = FALSE;
    i++;

    /* GEL HIDROALCOHÓLICO */
    maletin[i] = (Object*) malloc(sizeof(Object));
    if (maletin[i]==NULL) {
        for (j=0; j<i; j++) free(maletin[j]);
        free(maletin);
        return NULL;
    }
    if (loadStr(maletin[i]->name, "./res/text/gel.txt")==ERR) {
        for (j=0; j<=i; j++) free(maletin[j]);
        free(maletin);
        return NULL;
    }
    maletin[i]->used = FALSE;
    i++;

    return maletin;
}

/* Función para obtener el nombre de un objeto del maletín */
char* ObjectGetName(Object *obj) {
    if (obj==NULL) return NULL;

    return obj->name;
}

/* Función para declarar que un objeto del maletín se ha usado */
int ObjectUse(Object *obj) {
    if (obj==NULL) return ERR;

    obj->used = TRUE;

    return OK;
}

/* Función para obtener el uso de un objeto del maletín */
Bool ObjectIsUsed(Object *obj) {
    if (obj==NULL) return FALSE;

    return obj->used;
}

/* Función para liberar un objeto */
void ObjectFree(Object *obj) {
    if (obj==NULL) return;

    free(obj);
    return;
}

/* Función para liberar el maletín */
void MaletinFree(Object **maletin) {
    int i;

    if (maletin==NULL) return;

    for (i=0; i<TAM_MALETIN; i++) ObjectFree(maletin[i]);
    free(maletin);

    return;
}