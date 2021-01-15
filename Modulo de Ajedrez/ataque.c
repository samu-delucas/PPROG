
#include "stdio.h"
#include "definiciones.h"
#define SQOFFBOARD(sq) (FILAsBrd[sq]==OFFBOARD)

const int KnDir[8] = { -8, -19,	-21, -12, 8, 19, 21, 12 };
const int RkDir[4] = { -1, -10,	1, 10 };
const int BiDir[4] = { -9, -11, 11, 9 };
const int KiDir[8] = { -1, -10,	1, 10, -9, -11, 11, 9 };

/***********************************************************/
/* Funcion: SqAttacked                           
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parametros de entrada:                                               
/* sq: entero que indica una casilla del tablero
/* side: bando que se quiere ver si ataca la casilla
/* pos: puntero al tablero
/*
/* Retorno:
/* TRUE en caso de que la casilla esté atacada por el bando en cuestión y FALSE en caso contrario
/*
/* Descripción:
/* Esta función mira una por una cada pieza del bando side para ver si atacan a la casilla en cuestión
/*
/* Más en detalle: Mediante e uso de los arrays definididos al comienzo que determinan las direcciones de cada pieza, 
/* Mediante el array pieces del tablero se accede a cada una de las piezas de cada tipo y se va mirando, sumando las direcciones a las casillas
/* en las que estas se encuentran si están o no atacando la casilla en cuestión.
/***********************************************************/


int SqAttacked(const int sq, const int side, const TABLERO *pos) {

	int pce,index,t_sq,dir;

	ASSERT(!SQOFFBOARD(sq));
	
	// reyes
	for(index = 0; index < 8; ++index) {		
		pce = pos->pieces[sq + KiDir[index]];
		if((pce == wK || pce == bK) && pieceColour(pce)==side) {
			return TRUE;
		}
	}


	// pawns
	if(side == WHITE) {
		if(pos->pieces[sq-11] == wP || pos->pieces[sq-9] == wP) {
			return TRUE;
		}
	} else {
		if(pos->pieces[sq+11] == bP || pos->pieces[sq+9] == bP) {
			return TRUE;
		}	
	}
	
	// caballos
	for(index = 0; index < 8; ++index) {		
		pce = pos->pieces[sq + KnDir[index]];
		if((pce == wN || pce == bN) && pieceColour(pce)==side) {
			return TRUE;
		}
	}
	
	// torres, damas
	for(index = 0; index < 4; ++index) {		
		dir = RkDir[index];
		t_sq = sq + dir;
		pce = pos->pieces[t_sq];
		while(pce != OFFBOARD) {
			if(pce != EMPTY) {
				if((pce == bR || pce == bQ || pce == wQ || pce == wR) && pieceColour(pce) == side) {

					return TRUE;
				}
				break;
			}
			t_sq += dir;
			pce = pos->pieces[t_sq];
		}
	}
	
	// alfiles y damas
	for(index = 0; index < 4; ++index) {		
		dir = BiDir[index];
		t_sq = sq + dir;
		pce = pos->pieces[t_sq];
		while(pce != OFFBOARD) {
			if(pce != EMPTY) {
				if((pce == bB || pce == bQ || pce == wQ || pce == wB) && pieceColour(pce) == side) {
					return TRUE;
				}
				break;
			}
			t_sq += dir;
			pce = pos->pieces[t_sq];
		}
	}
	return FALSE;
}

	
