#include "definiciones.h"
#include "stdio.h"

long nHojas;

/***********************************************************/
/* Función: Comprobación                       
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:  
/* prof: profundidad del test                                               
/* tab: puntero a un tablero
/* 
/* Retorno:
/* void
/*
/* Descripción:
/* Test que muestra todas las posiciones posibles para una determianda profundidad. (Función recursiva)
/***********************************************************/

void Comprobacion(int prof, TABLERO *pos) {

    int count=0;
    int aux;


    ASSERT(CheckBoard(pos));  

	if(prof == 0) {
        nHojas++;
        return;
    }	

    MOVE **m=NULL;;
    m=Generador_Movimientos(pos, &count);
    if(!m)return;
      
    int MoveNum = 0;
	for(MoveNum = 1; MoveNum <count; ++MoveNum) {	
       
        if ( !HacerJugada(pos,m[MoveNum]))  {
            free_move(m[MoveNum]);
            continue;
        }
        Comprobacion(prof - 1, pos);
        DeshacerJugada(pos);
        free_move(m[MoveNum]);
    }
    free_move(m[0]);
    free(m);

    return;
}

/***********************************************************/
/* Función: Comprobacióntest                    
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* prof: profundidad del test
/* pos: puntero a un tablero
/*
/* Retorno:
/* void
/*
/* Descripción:
/* Test que muestra todas las posiciones posibles para una determianda profundidad. (Función envolvente)
/***********************************************************/


void Comprobaciontest(int prof, TABLERO *pos) {

    ASSERT(CheckBoard(pos));

	PrintBoard(pos);
	printf("\nEmpezando el test para profundidad:%d\n",prof);	
	nHojas = 0;
	int count=0;

    
    MOVE **m=NULL;
    m=Generador_Movimientos(pos, &count);
    if(!m)return;

    int MoveNum = 0;
	for(MoveNum = 1; MoveNum < count; ++MoveNum) {
        if ( !HacerJugada(pos,m[MoveNum]))  {
            free_move(m[MoveNum]);
            continue;
        }
        long alcanzados = nHojas;

        Comprobacion(prof - 1, pos);
        DeshacerJugada(pos);
                
        long nAnt = nHojas - alcanzados;
        printf("move %d : ",MoveNum);
        PrintMove(m[MoveNum]);
        printf(" : %ld\n", nAnt);
        free_move(m[MoveNum]);
    }
    free_move(m[0]);
	free(m);
	printf("\nTest completado: %ld nodos visitados\n",nHojas);
    
    return;
}