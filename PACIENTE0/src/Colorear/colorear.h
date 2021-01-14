#ifndef COLOREAR_H
#define COLOREAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <intrf.h>

int colorear_play(intrf *interfaz);
void colorear_print_esqueleto(sc_rectangle *frontal);
int colorear_print_casillas(sc_rectangle *frontal, int row, int col, int matriz[row][col]);
void colorear_generar(int filas, int columnas, int matriz[filas][columnas]);

#endif
