#ifndef ESCENAS_H
#define ESCENAS_H
#include "types.h"
#include "config.h"
#include "print_helper.h"
#include "input_helper.h"
#include "personaje.h"


#define ESCENAS 14
#ifndef STR_LEN
#define STR_LEN 256
#endif

void dibujarEscena0(int*, Window*, Window*, Window*);

void updateEscena0(int*, char*, Window*, Window*, Window*, Personaje*);


void dibujarEscena1(int*, Window*, Window*, Window*);

void updateEscena1(int*, char*, Window*, Window*, Window*, Personaje*);


void dibujarEscena2(int*, Window*, Window*, Window*);

void updateEscena2(int*, char*, Window*, Window*, Window*, Personaje*);


void dibujarEscena3(int*, Window*, Window*, Window*);

void updateEscena3(int*, char*, Window*, Window*, Window*, Personaje*);


void dibujarEscena4(int *iescena, Window *game, Window *lateral, Window *title);

void updateEscena4(int*, char*, Window*, Window*, Window*, Personaje*);


void dibujarEscena5(int *iescena, Window *game, Window *lateral, Window *title);

void updateEscena5(int*, char*, Window*, Window*, Window*, Personaje*);


void dibujarEscena6(int *iescena, Window *game, Window *lateral, Window *title);

void updateEscena6(int*, char*, Window*, Window*, Window*, Personaje*);


void dibujarEscena7(int *iescena, Window *game, Window *lateral, Window *title);

void updateEscena7(int*, char*, Window*, Window*, Window*, Personaje*);


void dibujarEscena8(int *iescena, Window *game, Window *lateral, Window *title);

void updateEscena8(int*, char*, Window*, Window*, Window*, Personaje*);


void dibujarEscena9(int *iescena, Window *game, Window *lateral, Window *title);

void updateEscena9(int*, char*, Window*, Window*, Window*, Personaje*);


void dibujarEscena10(int *iescena, Window *game, Window *lateral, Window *title);

void updateEscena10(int*, char*, Window*, Window*, Window*, Personaje*);


void dibujarEscena11(int *iescena, Window *game, Window *lateral, Window *title);

void updateEscena11(int*, char*, Window*, Window*, Window*, Personaje*);


void dibujarEscena12(int *iescena, Window *game, Window *lateral, Window *title);

void updateEscena12(int*, char*, Window*, Window*, Window*, Personaje*);


void dibujarEscena13(int *iescena, Window *game, Window *lateral, Window *title);

void updateEscena13(int*, char*, Window*, Window*, Window*, Personaje*);


#endif 