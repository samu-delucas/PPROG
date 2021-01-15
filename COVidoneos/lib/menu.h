#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "config.h"
#include "print_helper.h"
#include "input_helper.h"
#include "personaje.h"

int showMenu(Window*);

void cargarPartida(int*, int*, Personaje**);

int guardarPartida(int, int, Personaje*);

#endif