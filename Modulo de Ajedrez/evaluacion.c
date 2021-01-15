#include <stdio.h>
#include "definiciones.h"
#define SQOFFBOARD(sq) (FILAsBrd[sq] == OFFBOARD)
#define MATERIALFINAL (PieceVal[wR] + 2 * PieceVal[wN] + 2 * PieceVal[wP] + PieceVal[wK])
int PAREJAALFILES = 30;
int colgado =-10;
int doblado =-7;
int PawnPassed[8] = { 0, 5, 10, 20, 35, 60, 100, 200 }; 
int RookOpenFile = 10;
int RookSemiOpenFile = 5;
int QueenOpenFile = 5;
int QueenSemiOpenFile = 3;

/*puntpeones: Array que contiene las puntuaciones para las posiciones de los peones en el tablero*/

int puntpeones[64] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	10, 10, 0, -10, -10, 0, 10, 10,
	5, 0, 0, 5, 5, 0, 0, 5,
	0, 0, 10, 20, 20, 10, 0, 0,
	5, 5, 5, 10, 10, 5, 5, 5,
	10, 10, 10, 20, 20, 10, 10, 10,
	20, 20, 20, 30, 30, 20, 20, 20,
	0, 0, 0, 0, 0, 0, 0, 0};

/*puntcaballo: Array que contiene las puntuaciones para las posiciones de los caballos en el tablero*/

int puntcaballo[64] = {
	0, -10, 0, 0, 0, 0, -10, 0,
	0, 0, 0, 5, 5, 0, 0, 0,
	0, 0, 10, 10, 10, 10, 0, 0,
	0, 0, 10, 20, 20, 10, 5, 0,
	5, 10, 15, 20, 20, 15, 10, 5,
	5, 10, 10, 20, 20, 10, 10, 5,
	0, 0, 5, 10, 10, 5, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0};

/*puntalfil: Array que contiene las puntuaciones para las posiciones de los alfiles en el tablero*/

int puntalfil[64] = {
	0, 0, -10, 0, 0, -10, 0, 0,
	0, 0, 0, 10, 10, 0, 0, 0,
	0, 0, 10, 15, 15, 10, 0, 0,
	0, 10, 15, 20, 20, 15, 10, 0,
	0, 10, 15, 20, 20, 15, 10, 0,
	0, 0, 10, 15, 15, 10, 0, 0,
	0, 0, 0, 10, 10, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0};

/*punttorre: Array que contiene las puntuaciones para las posiciones de las torres en el tablero*/

int punttorre[64] = {
	0, 0, 5, 10, 10, 5, 0, 0,
	0, 0, 5, 10, 10, 5, 0, 0,
	0, 0, 5, 10, 10, 5, 0, 0,
	0, 0, 5, 10, 10, 5, 0, 0,
	0, 0, 5, 10, 10, 5, 0, 0,
	0, 0, 5, 10, 10, 5, 0, 0,
	25, 25, 25, 25, 25, 25, 25, 25,
	0, 0, 5, 10, 10, 5, 0, 0};

/*reynormal: Array que contiene las puntuaciones para las posiciones del rey en el tablero, en un estado 'early-game' o 'mid-game'*/

int reynormal[64] = {
	0, 5, 5, -10, -10, 0, 10, 5,
	-30, -30, -30, -30, -30, -30, -30, -30,
	-50, -50, -50, -50, -50, -50, -50, -50,
	-70, -70, -70, -70, -70, -70, -70, -70,
	-70, -70, -70, -70, -70, -70, -70, -70,
	-70, -70, -70, -70, -70, -70, -70, -70,
	-70, -70, -70, -70, -70, -70, -70, -70,
	-70, -70, -70, -70, -70, -70, -70, -70};

/*reyendgame: Array que contiene las puntuaciones para las posiciones del rey en el tablero, en un estado 'late-game'*/

int reyendgame[64] = {
	-50, -10, 0, 0, 0, 0, -10, -50,
	-10, 0, 10, 10, 10, 10, 0, -10,
	0, 10, 20, 20, 20, 20, 10, 0,
	0, 10, 20, 40, 40, 20, 10, 0,
	0, 10, 20, 40, 40, 20, 10, 0,
	0, 10, 20, 20, 20, 20, 10, 0,
	-10, 0, 10, 10, 10, 10, 0, -10,
	-50, -10, 0, 0, 0, 0, -10, -50};

/***********************************************************/
/* Funcion: Mirror64                           
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parametros de entrada:                                               
/* sq64: entero que indica una casilla del tablero
/* 
/* Retorno:
/* casilla 'opuesta'
/*
/* Descripción:
/* Esta función devuelve la casilla contraria, utilizada para
/* cambiar de blancas a negras en los arrays posicionales
/***********************************************************/

int Mirror64(int sq64)
{
	return 63 - sq64;
}

/***********************************************************/
/* Funcion: EvalPosition                             
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parametros de entrada:                                               
/* pos: puntero a un tablero
/* 
/* Retorno:
/* Valor de la puntuación evaluacion
/*
/* Descripción:
/* Devuelve la evaluación posicional del bando que indique el tablero
/* 
/* Más en detalle: Esta función se basa en contar el material que tiene cada uno de los
/* bandos y hacer la resta para ver la diferencia, además usando arrays posicionales
/* para cada tipo de pieza, que determinan en qué casilla del tablero es más fuerte cada pieza.
/* Además valora otros factores como la conservación de las parejas de alfiles como algo positivo 
/* o diferencia entre las casillas en las que debe de estar el rey dependiendo del momento de la partida
/***********************************************************/

int MaterialDraw(const TABLERO *pos)
{

	if (!pos->pceNum[wR] && !pos->pceNum[bR] && !pos->pceNum[wQ] && !pos->pceNum[bQ])
	{

		if (!pos->pceNum[bB] && !pos->pceNum[wB])
		{

			if (pos->pceNum[wN] < 3 && pos->pceNum[bN] < 3)
				return TRUE;
		}
		else if (!pos->pceNum[wN] && !pos->pceNum[bN])
		{

			if (((pos->pceNum[wB] - pos->pceNum[bB]) < 2) && ((pos->pceNum[wB] - pos->pceNum[bB]) > -2))
				return TRUE;
		}
		else if ((pos->pceNum[wN] < 3 && !pos->pceNum[wB]) || (pos->pceNum[wB] == 1 && !pos->pceNum[wN]))
		{

			if ((pos->pceNum[bN] < 3 && !pos->pceNum[bB]) || (pos->pceNum[bB] == 1 && !pos->pceNum[bN]))
				return TRUE;
		}
	}
	else if (!pos->pceNum[wQ] && !pos->pceNum[bQ])
	{

		if (pos->pceNum[wR] == 1 && pos->pceNum[bR] == 1)
		{

			if ((pos->pceNum[wN] + pos->pceNum[wB]) < 2 && (pos->pceNum[bN] + pos->pceNum[bB]) < 2)
				return TRUE;
		}
		else if (pos->pceNum[wR] == 1 && !pos->pceNum[bR])
		{

			if ((pos->pceNum[wN] + pos->pceNum[wB] == 0) && (((pos->pceNum[bN] + pos->pceNum[bB]) == 1) || ((pos->pceNum[bN] + pos->pceNum[bB]) == 2)))
				return TRUE;
		}
		else if (pos->pceNum[bR] == 1 && !pos->pceNum[wR])
		{

			if ((pos->pceNum[bN] + pos->pceNum[bB] == 0) && (((pos->pceNum[wN] + pos->pceNum[wB]) == 1) || ((pos->pceNum[wN] + pos->pceNum[wB]) == 2)))
				return TRUE;
		}
	}
	return FALSE;
}


int ** GeneratePawnArrays(const TABLERO *pos){

	int** array;
	int i, j, casilla, casilla64;
	int nWhite = pos->pceNum[wP], nBlack = pos->pceNum[bP];

	array=(int**)malloc(2*sizeof(int*));
	for(i=0;i<2;i++){
	array[i]=(int*)malloc(120*sizeof(int));
	}	

	for (i=0; i<2;i++){
		for(j=0;j<120;j++){
			array[i][j]=0;
		}
	}

	for (i=0;i<nWhite;i++){

		casilla = pos->pList[wP][i];
		array[0][casilla]=1;

	}

	for (i=0;i<nBlack;i++){

		casilla = pos->pList[bP][i];
		array[1][casilla]=1;

	}
	return array;
}

int PassedPawn(int ** pArray, int casilla120, int side){

	int columna=Cas_Col(casilla120), fila=Cas_Fila(casilla120);
	int casilla;
	int contr=side^1, i, j;

	for(i=columna-1;i<=columna+1;i++){
		for(j=fila+1;j<8;j++){
			casilla=((FCCAS(i,j)));
			if(pArray[contr][casilla]==1)return FALSE;
		}
	}
	return TRUE;
}

int IsolatedPawn(int ** pArray, int casilla120, int side){

	int columna=Cas_Col(casilla120);
	int casilla, i, j;

	for(i=columna-1;i<=columna+1;i++){
		if(i==columna)continue;
		for(j=0;j<8;j++){
			casilla=((FCCAS(i,j)));
			if(pArray[side][casilla]==1)return FALSE;
		}
	}
	return TRUE;
}

int DoubledPawn(int ** pArray, int casilla120, int side){

	int columna=Cas_Col(casilla120), i, fila=Cas_Fila(casilla120), casilla;

	for(i=0;i<7;i++){
		if(i==fila)continue;
		casilla=C120a64(((columna,i)));

		if(pArray[side][casilla]==1)return TRUE;
	}
	return FALSE;
}

int OpenSemiOpenCol(int ** pArray, int casilla120, int side){

	int columna=Cas_Col(casilla120), i, casilla, flag=2, contr=side^1;

	for(i=0;i<7;i++){

		casilla=C120a64(((columna,i)));

		if(pArray[side][casilla]==1)return FALSE;
		if(pArray[contr][casilla]==1)flag=1;

	}
	return flag;
}

int EvalPosition(const TABLERO *pos)
{

	int casilla, pce, npiezas, casilla64, fila, i, abierta;
	int punt = pos->material[WHITE] - pos->material[BLACK];
	int	** array;

	if (!pos->pceNum[wP] && !pos->pceNum[bP] && MaterialDraw(pos) == TRUE)
	{
		return 0;
	}

	array=GeneratePawnArrays(pos);

	pce = wP;
	for (npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++)
	{
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
		casilla64 = C120a64(casilla);
		punt += puntpeones[casilla64];
		if(PassedPawn(array, casilla, WHITE)==TRUE){

			fila=Cas_Fila(casilla);
			punt+= PawnPassed[fila];
		}

		if(IsolatedPawn(array, casilla, WHITE)) punt+=colgado;	

		if(DoubledPawn(array, casilla, WHITE)) punt+=doblado;
	}

	pce = bP;
	for (npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++)
	{
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
		casilla64 = C120a64(casilla);
		punt -= puntpeones[Mirror64(casilla64)];

		if(PassedPawn(array, casilla, BLACK)==TRUE){

			fila=Cas_Fila(casilla);
			punt-= PawnPassed[7-fila];
		}

		if(IsolatedPawn(array, casilla, BLACK)) punt-=colgado;

		if(DoubledPawn(array, casilla, BLACK)) punt-=doblado;

	}

	pce = wN;
	for (npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++)
	{
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
		casilla64 = C120a64(casilla);
		punt += puntcaballo[casilla64];
	}

	pce = bN;
	for (npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++)
	{
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
		casilla64 = C120a64(casilla);
		punt -= puntcaballo[Mirror64(casilla64)];
	}

	pce = wB;
	for (npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++)
	{
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
		casilla64 = C120a64(casilla);
		punt += puntalfil[casilla64];
	}

	pce = bB;
	for (npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++)
	{
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
		casilla64 = C120a64(casilla);
		punt -= puntalfil[Mirror64(casilla64)];
	}

	pce = wR;
	for (npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++)
	{
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
		casilla64 = C120a64(casilla);
		punt += punttorre[casilla64];

		abierta=OpenSemiOpenCol(array, casilla, WHITE);

		if(abierta==2)punt+=RookOpenFile;
		else if(abierta==1)punt+=RookSemiOpenFile;
	}

	pce = bR;
	for (npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++)
	{
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));
		casilla64 = C120a64(casilla);
		punt -= punttorre[Mirror64(casilla64)];

		abierta=OpenSemiOpenCol(array, casilla, BLACK);

		if(abierta==2)punt-=RookOpenFile;
		else if(abierta==1)punt-=RookSemiOpenFile;
	}

	pce = wQ;

	for (npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++)
	{
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));

		abierta=OpenSemiOpenCol(array, casilla, WHITE);

		if(abierta==2)punt+=QueenOpenFile;
		else if(abierta==1)punt+=QueenSemiOpenFile;
	}

	pce = bQ;

	for (npiezas = 0; npiezas < pos->pceNum[pce]; npiezas++)
	{
		casilla = pos->pList[pce][npiezas];
		ASSERT(!SQOFFBOARD(casilla));

		abierta=OpenSemiOpenCol(array, casilla, BLACK);

		if(abierta==2)punt-=QueenOpenFile;
		else if(abierta==1)punt-=QueenSemiOpenFile;
	}



	pce = wK;
	casilla = pos->pList[pce][0];
	if ((pos->material[BLACK] <= MATERIALFINAL))
	{
		casilla64 = C120a64(casilla);
		punt += reyendgame[(casilla64)];
	}
	else
	{
		punt += reynormal[(casilla64)];
	}

	pce = bK;
	casilla = pos->pList[pce][0];
	if ((pos->material[WHITE] <= MATERIALFINAL))
	{
		casilla64 = C120a64(casilla);
		punt -= reyendgame[Mirror64(casilla64)];
	}
	else
	{
		punt -= reynormal[Mirror64(casilla64)];
	}

	if (pos->pceNum[wB] >= 2)
		punt += PAREJAALFILES;
	if (pos->pceNum[bB] >= 2)
		punt -= PAREJAALFILES;

	if (pos->side == WHITE)
	{
		for(i=0;i<2;i++){
			free(array[i]);
		}
		free(array);
		return punt;
	}
	else
	{
		for(i=0;i<2;i++){
			free(array[i]);
		}
		free(array);
		return -punt;
	}
}
