#ifndef ACERTIJOS_H
#define ACERTIJOS_H

#include "types.h"

Status startAcertijos(FILE *f, int *inocentes, int *nInocentes, int paciente0);

void startAcertijo1(int inocente);

void startAcertijo2(int inocente);

void startAcertijo3(int inocente);

void startAcertijo4(int inocente);

void startAcertijo5(int inocente);

Status startPistas(FILE *f, int *inocentes, int *nInocentes, int paciente0);

void startPistas1(int inocente);

void startPistas2(int inocente);

void startPistas3(int inocente);

void startPistas4(int inocente);

void startPistas5(int inocente);

void startPistas6(int inocente);

int posibleInocente(int *inocentes, int *nInocentes, int paciente0);

#endif