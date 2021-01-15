#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "object.h"

typedef struct _Personaje Personaje;

Personaje* startPersonaje(Window* gwin, Window *lwin,Window *bwin, int **f);

void updatePersonaje(Personaje *per, char input);

int hideInventario();

int showInventario(Personaje *per);

int toggleInventario(Personaje *per);

int setSalud(Personaje *per, int s);

int bajarSalud(Personaje *per);

void destroyPersonaje(Personaje *per);

Object* getObject(Personaje *per, ObjectId objId);

int getSalud(Personaje *per);

#endif
