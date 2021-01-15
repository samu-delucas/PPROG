#include "stdio.h"
#include "definiciones.h"
#define SQOFFBOARD(sq) (FILAsBrd[sq]==OFFBOARD)
#define PEONDIR 10

char PceChar2[] = ".PNBRQKPNBRQK";

/***********************************************************/
/* Funcion: create_move                             
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Retorno:
/* Puntero a estructura movimiento
/*
/* Descripción:
/* Reserva memoria para una estructura movimiento
/***********************************************************/

MOVE *create_move(){
    return (MOVE*)malloc(sizeof(MOVE));
}

/***********************************************************/
/* Función: free_move                          
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada: 
/* Puntero a estructura movimiento
/*
/* Descripción:
/* Libera memoria para una estructura movimiento
/***********************************************************/

void free_move(MOVE *m){
    if(m)free(m);
    return;
}

/***********************************************************/
/* Función: move_copy                        
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada: 
/* Puntero a estructura movimiento
/* 
/* Retorno:
/* copy=puntero a estructura movimiento
/*
/* Descripción:
/* Hace una copia de un movimiento
/***********************************************************/

MOVE *move_copy(MOVE*m){
    MOVE *copy;
    if(!m)return NULL;
    if(!(copy = create_move())) return NULL;
    copy->castle=m->castle;
    copy->from = m->from;
    copy->paso = m->paso;
    copy->to = m->to;
    copy->piezas[0] = m->piezas[0];
    copy->piezas[1] = m->piezas[1];
    copy->piezas[2] = m->piezas[2];
    return copy;
}

/***********************************************************/
/* Función: free_UNDO                       
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada: 
/* Puntero a estructura S_UNDO
/*
/* Descripción:
/* Libera memoria para una estructura S_UNDO
/***********************************************************/

void free_UNDO(S_UNDO * u){
    if(u){
        if(u->jugada) free_move(u->jugada);
        if(u->fen) free(u->fen);
        free(u);
    } 
    return;
}

/***********************************************************/
/* Función: create_UNDO                     
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada: 
/* Puntero a estructura movimiento
/* 
/* Retorno:
/* u=puntero a estructura S_UNDO
/*
/* Descripción:
/* Crea una estructura de tipo S_UNDO
/***********************************************************/

S_UNDO *create_UNDO (MOVE *jugada){
    S_UNDO *u;
    if(!(u= (S_UNDO*)malloc(sizeof(S_UNDO)))) return NULL;
    if(!(u->jugada = move_copy(jugada))){
        free_UNDO(u);
        return NULL;
    }
    return u;
}

/***********************************************************/
/* Función: insert_move                     
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada: 
/* castle: entero que indica el permiso de enroque
/* from: entero que indica casilla de salida
/* to: entero que indica casilla de salida
/* pieza: entero que indica la pieza de la que se trata
/* captura: entero indica si se trata de una captura 
/* corona: entero que indica en que pieza se corona
/* paso: entero que indica si se captura al paso
/*
/* Retorno:
/* u=puntero a estructura movimento
/*
/* Descripción:
/* Crea un movimiento e inserta sobre él la información que se le aporta
/***********************************************************/

MOVE* insert_move(int castle, int from, int to, int pieza, int captura, int corona, int paso){
    MOVE  *m;
    m = create_move();

    if (!m) return NULL;        
    m->castle = castle;
    m->from = from;
    m->to =to;
    m->piezas[0] = pieza;
    m->piezas[1] = captura;
    m->piezas[2] = corona;
    m->paso = paso;
    return m;
}

/***********************************************************/
/* Función: move_cmp                
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada: 
/* m1=Puntero a estructura movimiento
/* m2=Puntero a estructura movimiento
/*
/* Retorno:
/* TRUE si son iguales y FALSE si son distintos
/*
/* Descripción:
/* Compara dos movimientos para ver si estos son iguales
/***********************************************************/

int move_cmp(MOVE *m1, MOVE *m2){
    if(!m1||!m2) return FALSE;
    if(m1->castle!=m2->castle) return FALSE;
    if(m1->from!=m2->from) return FALSE;
    if(m1->to!=m2->to) return FALSE;
    if(m1->piezas[0]!=m2->piezas[0]) return FALSE;
    if(m1->piezas[1]!=m2->piezas[1]) return FALSE;
    if(m1->piezas[2]!=m2->piezas[2]) return FALSE;
    if(m1->paso!=m2->paso) return FALSE;
    return TRUE;
}

/***********************************************************/
/* Función: Generador_Movimientos                
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada: 
/* t=puntero a un tablero
/* count=puntero a un entero que cuenta el numero de movimientos
/*
/* Retorno:
/* Array de movimientos con todos los movimientos posibles para una posición dada
/*
/* Descripción:
/* Inicializa un array de movimientos y llama a todos los generadores de movimientos para cada pieza
/***********************************************************/


MOVE **Generador_Movimientos(TABLERO *t, int *count){
    MOVE **m;

    ASSERT(CheckBoard(t));

    if(!t) return NULL;

    m = (MOVE**) malloc(sizeof(MOVE*));
    m[0] = insert_move(0,A1,A1,0,0,0,0);
    *count = 1;

    
    m = Generador_Enroques(t, m, count);
    m = Generador_Peones(t, m, count);
    m = Generador_Slide(t, m, count);
    m = Generador_RC(t, m, count);

    return m;
    
} 


/***********************************************************/
/* Función: AddMovePeon                
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:
/* m = array de punteros a movimiento
/* count = puntero al numero de elementos de m
/* cas = casilla de salida del peón
/* to = casilla a la que se mueve el peón
/* to2 = casilla donde está la pieza a la que se comería al paso 
/* captura = la pieza que se captura si no se captura ninugna EMPTY
/* side = El lado al que le toca
/* paso= la casilla a la que se come al paso si no se come EMPTY
/*
/* Retorno:
/* Array de punteros a movimientos actualizado
/*
/* Descripción:
/* Actualiza el array de punteros a movimientos añadiendole jugadas de peón que salen de una misma casilla
/*
/* Más en Datalle: Se mira si están en la penúltima fila para la coronación, si es una captura, si es al paso, etc.
/***********************************************************/


MOVE ** AddMovePeon (MOVE **m,  int *count, int cas, int to, int to2, int captura, int side, int paso){
    if (!m) return NULL;

    if(Cas_Fila(cas) == FILA_7 - side*5){
        
        m = realloc(m,(*count + 4)*sizeof(MOVE*));
        if (!m) return NULL;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, CAMBIO_LADO*side + wN, paso);
        (*count) ++;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, CAMBIO_LADO*side + wB, paso);
        (*count) ++;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, CAMBIO_LADO*side + wR, paso);
        (*count) ++;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, CAMBIO_LADO*side + wQ, paso);
        (*count) ++;
 
    }
    else if (Cas_Fila(cas) == FILA_2 +side*5 && captura == EMPTY && to2 == EMPTY){

        m = realloc(m, (*count + 2)*sizeof(MOVE*));
        if (!m) return NULL;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, EMPTY, paso);
        (*count)++;
        m[*count] = insert_move(EMPTY,cas, to + 10 - 20*side,CAMBIO_LADO*side + wP, captura, EMPTY, paso);
        (*count)++;

    }
    else{
        m = realloc(m, (*count +1)*sizeof(MOVE*));
        if (!m) return NULL;
        m[*count] = insert_move(EMPTY,cas, to,CAMBIO_LADO*side + wP, captura, EMPTY, paso);
        (*count)++;
    }
    return m;
}

/***********************************************************/
/* Función:Generador_Peones                
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:
/* t = puntero al tablero en el que se está trabajando
/* m = array de punteros a movimiento
/* count = puntero al numero de elementos de m
/*
/* Retorno:
/* Array de punteros a movimientos actualizado
/*
/* Descripción:
/* Actualiza el array de punteros a movimientos añadiendole todas las jugadas de peón que se pueden hacer en una posición
/*
/* Más en Datalle: Para cada peón, mira las jugadas de avance, las jugadas de captura y las jugadas de captura al paso.
/***********************************************************/


MOVE ** Generador_Peones(TABLERO *t, MOVE **m, int *count ){

    int i;
    int side;
    int cas = 0, cas_temp=0;
    int pieza = EMPTY;
    short flag = 1;

    if(!m || !t) return NULL;

    side = t->side;

    ASSERT(CheckBoard(t));

    //Las correciones donde hay *side sirven para escoger, dependiendo del lado al que le toque, escoger las características correctas
    for(i=0;i<t->pceNum[CAMBIO_LADO*side + wP]&& flag == 1;i++){
        cas = t->pList[CAMBIO_LADO*side + wP][i];
        ASSERT(cas != OFFBOARD);

        if(t->pieces[cas + 10 - 20*side] == EMPTY){
            m = AddMovePeon(m, count, cas, cas + 10 - 20*side, t->pieces[cas + 20 - 40*side],EMPTY,side,EMPTY);
            if (!m) flag = 0;
        }
        pieza = t->pieces[cas +11 - 20*side];
        if(pieza != EMPTY && pieza != OFFBOARD && pieza != NO_SQ && pieza - (2*side*pieza) > CAMBIO_LADO - 2*side*CAMBIO_LADO && flag == 1){
            m = AddMovePeon(m,count, cas, cas +11 - 20*side, EMPTY,pieza, side, EMPTY);
            if (!m) flag = 0;
        }
        pieza = t->pieces[cas +9 - 20*side];
        if (pieza != EMPTY && pieza != OFFBOARD && pieza != NO_SQ && pieza - (2*side*pieza) > CAMBIO_LADO - 2*side*CAMBIO_LADO && flag == 1){
            m = AddMovePeon(m,count, cas, cas +9 - 20*side, EMPTY,pieza, side, EMPTY);
            if (!m) flag = 0;
        }

        if (t->AlPaso == cas +9 - 20*side && flag == 1 && t->AlPaso != NO_SQ){
            m = AddMovePeon(m,count, cas, cas +9 - 20*side, EMPTY,-(CAMBIO_LADO*side) + bP, side, cas +9 - 20*side);
            if (!m) flag = 0;
        }
        else if (t->AlPaso == cas +11 - 20*side && flag == 1 && t->AlPaso != NO_SQ){
            m = AddMovePeon(m,count, cas, cas +11 - 20*side, EMPTY,-(CAMBIO_LADO*side) + bP, side,cas +11 - 20*side);
            if (!m) flag = 0;
        }
        
    }
    if (flag == 0){
        return NULL;
    }
    return m;
}

/***********************************************************/
/* Función: Generador_RC               
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada: 
/* t=puntero a un tablero
/* count=puntero a un entero que cuenta el numero de movimientos
/* m=array que contiene los movimientos anteriores y al que se irán añadiendo los nuevos
/*
/* Retorno:
/* Array de movimientos con todos los movimientos posibles de caballo y rey, más los que éste contuviese, para una posición dada
/*
/* Descripción:
/* Utilizando arrays de direcciones, mira todos los movimientos para todos los caballos que haya y para el rey
/*
/* Más en detalle: Mediante el uso de la piece List y del array de pieces, partiendo de la casilla en la que se sitúa cada uno de los caballos y el rey,
/* mira sumando las direcciones a la casilla de salida y mira los todos los movimientos posibles, tomando solo los legales (rey no queda en jaque tras el movimiento)
/***********************************************************/

MOVE** Generador_RC(TABLERO *t, MOVE **m, int *count){
    int i,j;
    int dir;
    int side;
    int cas = 0, cas_temp=0;
    int cas_aux;
    int pce_cas_aux;
    int pieza = EMPTY;
    short flag = 1;
    int dircaballo[8]={-8,-19,-21,-12,8,19,21,12};
    int dirrey[8]={-1,-10,1,10,-9,-11,9,11};
    
    if(!m || !t) return NULL;

	side=t->side;

    ASSERT(CheckBoard(t));
   
   
    //bucle caballo blanco

   if(side==WHITE){
	for(i=0; i< t->pceNum[2] ; ++i) {
			
            cas = t->pList[2][i];
            
			
			for(j = 0; j < 8;j++) {
				dir = dircaballo[j];
				cas_aux = cas + dir;
                

				
				if(!SQOFFBOARD(cas_aux)) {	
                     
                    pce_cas_aux=t->pieces[cas_aux];
                    if(pce_cas_aux==EMPTY || pce_cas_aux==bP || pce_cas_aux==bN || pce_cas_aux== bB || pce_cas_aux== bR || pce_cas_aux==bQ || pce_cas_aux==bK){
                       
                        m = realloc(m, (*count +1)*sizeof(MOVE*));
                        if (!m) return NULL;
                      

                        m[*count] = insert_move(EMPTY,cas,cas_aux,2, pce_cas_aux, EMPTY, EMPTY);
                        (*count)++;
                       
				}
                }
				
			}
		}
   }


    //bucle caballo negro
     if(side==BLACK){

    for(i=0; i< t->pceNum[bN]; i++) {
			cas = t->pList[bN][i];
			
			
			for(j = 0; j < 8; ++j) {
				dir = dircaballo[j];
				cas_aux = cas + dir;
				
				if((!SQOFFBOARD(cas_aux))) {	
                    pce_cas_aux=t->pieces[cas_aux];
                    if(pce_cas_aux==EMPTY || pce_cas_aux==wP || pce_cas_aux==wN || pce_cas_aux== wB || pce_cas_aux== wR || pce_cas_aux==wQ || pce_cas_aux==wK){
                        m = realloc(m, (*count +1)*sizeof(MOVE*));
                        if (!m) return NULL;
                      

                        m[*count] = insert_move(EMPTY,cas,cas_aux,bN, pce_cas_aux, EMPTY, EMPTY);
                        (*count)++;
				}
				}
				
			}
		}
     }

        //bucle rey blanco
      if(side==WHITE){
        for(i=0; i< t->pceNum[wK]; i++) {
			cas = t->pList[wK][i];
			
			
			for(j = 0; j < 8; ++j) {
				dir = dirrey[j];
				cas_aux = cas + dir;
                pce_cas_aux=t->pieces[cas_aux];
				
				if((!SQOFFBOARD(cas_aux))) {	
                    if(pce_cas_aux==EMPTY || pce_cas_aux==bP || pce_cas_aux==bN || pce_cas_aux== bB || pce_cas_aux== bR || pce_cas_aux==bQ || pce_cas_aux==bK){
                        m = realloc(m, (*count +1)*sizeof(MOVE*));
                        if (!m) return NULL;
                      

                        m[*count] = insert_move(EMPTY,cas,cas_aux,wK, pce_cas_aux, EMPTY, EMPTY);
                        (*count)++;
				}
            }
				
			}
		}
      }

        //bucle rey negro
         if(side==BLACK){
           for(i=0; i< t->pceNum[bK]; i++) {
			cas = t->pList[bK][i];
		
			
			for(j = 0; j < 8; ++j) {
				dir = dirrey[j];
				cas_aux = cas + dir;
                pce_cas_aux=t->pieces[cas_aux];
				
				if((!SQOFFBOARD(cas_aux))) {	
                    if(pce_cas_aux==EMPTY || pce_cas_aux==wP || pce_cas_aux==wN || pce_cas_aux== wB || pce_cas_aux== wR || pce_cas_aux==wQ || pce_cas_aux==wK){
                        m = realloc(m, (*count +1)*sizeof(MOVE*));
                        if (!m) return NULL;
                      

                        m[*count] = insert_move(EMPTY,cas,cas_aux,bK, pce_cas_aux, EMPTY, EMPTY);
                        (*count)++;
				}
            }
				
			}
		}
         }
			
return m;
}

/***********************************************************/
/* Función: Generador_Enroques              
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada: 
/* t=puntero a un tablero
/* count=puntero a un entero que cuenta el numero de movimientos
/* m=array que contiene los movimientos anteriores y al que se irán añadiendo los nuevos
/*
/* Retorno:
/* Array de movimientos con todos los movimientos posibles de enroque, más los que éste contuviese, para una posición dada
/*
/* Descripción:
/* Utilizando el permiso de enroque de la estructura tablero, busca a ver si son legales los movimientos de enroque y si esto es así los añade a la lista
/*
/***********************************************************/


MOVE ** Generador_Enroques(TABLERO *t, MOVE **m, int *count ){

    int side;
    int sq, entresq;
    int i, flag=1;
    int aux;

    if(!t||!m)return NULL;

    ASSERT(CheckBoard(t));

    side=t->side;
    
    aux=t->enroque;
    

    if(side==WHITE){

        sq = t->pList[wK][0];

        if(SqAttacked(sq, BLACK,t))return m;

        if((aux%2)==1){
            aux-=1;
            for(i=1;i<=2&&flag==1;i++){

                entresq=sq+i;
                flag=((t->pieces[entresq] == EMPTY && !SqAttacked(entresq, BLACK,t)));
                
            }
            if(flag==1){

                m = realloc(m,(*count + 1)*sizeof(MOVE*));
				m[*count] = insert_move(WKCA,EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY);
                (*count) ++;

            }

        }
        flag=1;
        aux=aux/2;
        if(aux%2==1){
            for(i=1;i<=2&&flag==1;i++){

                entresq=sq-i;
                flag=((t->pieces[entresq] == EMPTY && !SqAttacked(entresq, BLACK,t)));
                
            }
            if(flag == 1) flag = (t->pieces[sq-3] == EMPTY);
            if(flag==1){

                m = realloc(m,(*count + 1)*sizeof(MOVE*));
				m[*count] = insert_move(WQCA,EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY);
                (*count) ++;

            }

        }

    }
    if(side==BLACK){
        aux=t->enroque;
        

        sq = t->pList[bK][0];

        if(SqAttacked(sq, WHITE,t))return m;


        aux-=BQCA;
        if((aux)>=0){
            for(i=1;i<=2&&flag==1;i++){

                entresq=sq-i;
                flag=((t->pieces[entresq] == EMPTY && !SqAttacked(entresq, WHITE,t)));

            }
            if(flag == 1) flag = (t->pieces[sq-3] == EMPTY);
            if(flag==1){

                m = realloc(m,(*count + 1)*sizeof(MOVE*));
				m[*count] = insert_move(BQCA,EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY);
                (*count) ++;
                

            }

        }

        flag=1;
        if(aux<0)aux=t->enroque;

        aux-=BKCA;

        if(aux>=0){
            for(i=1;i<=2&&flag==1;i++){

                entresq=sq+i;
                flag=((t->pieces[entresq] == EMPTY && !SqAttacked(entresq, WHITE,t)));

            }
            if(flag==1){

                m = realloc(m,(*count + 1)*sizeof(MOVE*));
				m[*count] = insert_move(BKCA,EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY);
                (*count) ++;

            }

        }

    }

    return m;
}

/***********************************************************/
/* Función: Generador_Slide              
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada: 
/* t=puntero a un tablero
/* count=puntero a un entero que cuenta el numero de movimientos
/* m=array que contiene los movimientos anteriores y al que se irán añadiendo los nuevos
/*
/* Retorno:
/* Array de movimientos con todos los movimientos posibles de torre, alfil y dama, más los que éste contuviese, para una posición dada
/*
/* Descripción:
/* Utilizando arrays de direcciones, mira todos los movimientos para los alfiles, torres y damas
/*
/* Más en detalle: Mediante el uso de la piece List y del array de pieces, partiendo de la casilla en la que se sitúa cada uno de los alfiles, torres y damas,
/* mira sumando las direcciones a la casilla de salida y mira los todos los movimientos posibles, tomando solo los legales (rey no queda en jaque tras el movimiento)
/***********************************************************/


MOVE ** Generador_Slide(TABLERO *t, MOVE **m, int *count ){

    ASSERT(CheckBoard(t));

    int pceIndex, pce, pceNum, side, index, sq, t_sq, dir;
    int PceDirSlide[8][8] = {
	{0,0,0,0,0,0,0,0},
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
    {0,0,0,0,0,0,0,0},
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 }

    };


    int NumDirSlide[13] = {
    0,0,0,4,4,8,0,0,0,4,4,8,0
    };

    int LoopSlidePce[8] = {
    wB, wR, wQ, 0, bB, bR, bQ, 0
    };

    int LoopSlideIndex[2] = { 0, 4 };


    if(!t||!m)return NULL;

    side=t->side;

	pceIndex = LoopSlideIndex[side];
	pce = LoopSlidePce[pceIndex];
    pceIndex++;
	while( pce != 0) {	
		
		for(pceNum = 0; pceNum < t->pceNum[pce]; ++pceNum) {
			sq = t->pList[pce][pceNum];
			if(SQOFFBOARD(sq)) return NULL;
			
			for(index = 0; index < NumDirSlide[pce]; ++index) {
				dir = PceDirSlide[pceIndex][index];
				t_sq = sq + dir;
				
				while(!SQOFFBOARD(t_sq)) {				
					
					if(t->pieces[t_sq] != EMPTY) {
						if( pieceColour(t->pieces[t_sq]) !=side) {
                            
                            m = realloc(m,(*count + 1)*sizeof(MOVE*));
							m[*count] = insert_move(EMPTY,sq, t_sq, pce, t->pieces[t_sq], EMPTY, EMPTY);
                            (*count) ++;
						}
						break;
					}	
					m = realloc(m,(*count + 1)*sizeof(MOVE*));
					m[*count] = insert_move(EMPTY,sq, t_sq, pce, EMPTY, EMPTY, EMPTY);
                    
                    (*count) ++;
					t_sq += dir;
				}
			}
		}
		
		pce = LoopSlidePce[pceIndex++];

}
    return m;

}

/***********************************************************/
/* Función: PrintMove            
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada: 
/* 
/* mt=puntero a un movimiento
/*
/* Retorno:
/* -1 en caso de error o OK en caso de que todo haya ido de manera correcta
/*
/* Descripción:
/* Imprime por pantalla un movimiento mediante la sintaxis: 
/* (P)from(x)to(=C) donde se da que:
/*  - P corresponde a la pieza que se omite si se mueve un peón (en mayúsculas)
/*  - from corresponde a la casilla desde la que se mueve la pieza (en minúsculas)
/*  - x indica si hay captura. Si no hay captura se omite
/*  - to corresponde a la casilla a la que va la pieza (en minúsculas)
/*  - =C se escribe si hay una coronación y en C se escribe la pieza en la que se corona (en mayúsculas)
/*  - En el caso de que el movimiento sea un enroque se escribirá WKCA, WQCA, BKCA o BQCA dependiendo del tipo de enroque y del bando
/*
/***********************************************************/


int PrintMove(MOVE *mt){
    
    int col, fila;

    if (!mt) return -1;
    
        //primero ver el enroque
        //Después vemos las piezas:
        if(mt->piezas[0] == wP || mt->piezas[0] == bP){
            col = Cas_Col(mt->from);
            fila = Cas_Fila(mt->from);
            printf("%c%d", 'a'+col, fila +1);

            //Captura
            if(mt->piezas[1] != EMPTY && mt->piezas[1] != OFFBOARD) printf("x");

            col = Cas_Col(mt->to);
            fila = Cas_Fila(mt->to);
            printf("%c%d", 'a'+col, fila +1);

            //Coronación
            if(mt->piezas[2] != EMPTY){
                switch (mt->piezas[2])
                {
                case wN:
                case bN:
                    printf("=N");
                    break;
                case wB:
                case bB:
                    printf("=B");
                    break;
                case wR:
                case bR:
                    printf("=R");
                    break;
                case wQ:
                case bQ:
                    printf("=Q");
                    break;
                default:
                    break;
                }
            }
        }
        else{
            if(mt->castle==EMPTY){
            printf("%c", PceChar2[mt->piezas[0]]);
            col = Cas_Col(mt->from);
            fila = Cas_Fila(mt->from);
            printf("%c%d", 'a'+col, fila +1);

            //Captura
            if(mt->piezas[1] != EMPTY && mt->piezas[1] != OFFBOARD) printf("x");

            col = Cas_Col(mt->to);
            fila = Cas_Fila(mt->to);
            printf("%c%d", 'a'+col, fila +1);
            }
            else{
                if(mt->castle==WKCA)printf("WKCA");
                else if(mt->castle==WQCA)printf("WQCA");
                else if(mt->castle==BKCA)printf("BKCA");
                else if(mt->castle==BQCA)printf("BQCA");

            }
            
        }
        return OK;
    }

/***********************************************************/
/* Función: print_moves            
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada: 
/* 
/* m=array de movimientos
/* count= numero de movimientos que contiene el array
/* Retorno:
/* -1 en caso de error o OK en caso de que todo haya ido de manera correcta
/*
/* Descripción:
/* Imprime por pantalla todos los movimientos del array mediante la siguiente la sintaxis: 
/* (P)from(x)to(=C) donde se da que:
/*  - P corresponde a la pieza que se omite si se mueve un peón (en mayúsculas)
/*  - from corresponde a la casilla desde la que se mueve la pieza (en minúsculas)
/*  - x indica si hay captura. Si no hay captura se omite
/*  - to corresponde a la casilla a la que va la pieza (en minúsculas)
/*  - =C se escribe si hay una coronación y en C se escribe la pieza en la que se corona (en mayúsculas)
/*  - En el caso de que el movimiento sea un enroque se escribirá WKCA, WQCA, BKCA o BQCA dependiendo del tipo de enroque y del bando
/*
/***********************************************************/


int print_moves(MOVE **m, int count){
    int i;
    int col, fila;
    MOVE *mt;

    if (!m) return -1;
    
    for(i=0; i<count; i++){
        printf("Jugada %d: ",i);
        mt = m[i];
        //primero ver el enroque
        //Después vemos las piezas:
        if(mt->piezas[0] == wP || mt->piezas[0] == bP){
            col = Cas_Col(mt->from);
            fila = Cas_Fila(mt->from);
            printf("%c%d", 'a'+col, fila +1);

            //Captura
            if(mt->piezas[1] != EMPTY && mt->piezas[1] != OFFBOARD) printf("x");

            col = Cas_Col(mt->to);
            fila = Cas_Fila(mt->to);
            printf("%c%d", 'a'+col, fila +1);

            //Coronación
            if(mt->piezas[2] != EMPTY){
                switch (mt->piezas[2])
                {
                case wN:
                case bN:
                    printf("=N");
                    break;
                case wB:
                case bB:
                    printf("=B");
                    break;
                case wR:
                case bR:
                    printf("=R");
                    break;
                case wQ:
                case bQ:
                    printf("=Q");
                    break;
                default:
                    break;
                }
            }
        }
        else{
            if(mt->castle==EMPTY){
            printf("%c", PceChar2[mt->piezas[0]]);
            col = Cas_Col(mt->from);
            fila = Cas_Fila(mt->from);
            printf("%c%d", 'a'+col, fila +1);

            //Captura
            if(mt->piezas[1] != EMPTY && mt->piezas[1] != OFFBOARD) printf("x");

            col = Cas_Col(mt->to);
            fila = Cas_Fila(mt->to);
            printf("%c%d", 'a'+col, fila +1);
            }
            else{
                if(mt->castle==WKCA)printf("WKCA");
                else if(mt->castle==WQCA)printf("WQCA");
                else if(mt->castle==BKCA)printf("BKCA");
                else if(mt->castle==BQCA)printf("BQCA");

            }
            
        }
        printf("\n");
    }
    return OK;
}

/***********************************************************/
/* Función: HacerJugada           
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada: 
/* 
/* m=puntero a un movimiento
/* t=puntero al tablero
/*
/* Retorno:
/* FALSE si algo falla o la jugada introducida es jaque y TRUE si todo va correctamente
/*
/* Descripción:
/* Dada una jugada válida, la mueve en el tablero y actualiza sus parámetros.
/*
/* Más en detalle: La entrada debe de ser una jugada válida o una jugada de jaque. Si no se introduce una de estas jugadas,
/* la función dará error y puede llevar a cambios irreparables en el tablero. La función separa en casos en función de la juagada intoducida
/* Si la función es un enroque se presume que es válido y se mueven las piezas necesarias. En caso contrario, se comprueba si la jugada es jaque utilizando
/* la función SqAttacked para ver si la casilla del rey está atacada. Si no está atacada, sustituye las pieza  from a to eliminando o añadiendo las piezas corresondientes.
/* Se llama a la función Update list Material para actualizar el estado de las informaciones de los parámetros de tablero. Por último,
/* además se incrementan los contadores de jugadas y se crea una estructura UNDO que se guarda para poder deshacer el movimiento. 
/***********************************************************/


/*Introducir una jugada que sea válida o jaque*/
int HacerJugada(TABLERO *t,MOVE *m){
    S_UNDO *u;
    int aux,ksq;



    if(!t||!m)return FALSE;
    aux=t->enroque;


    if(!(u = create_UNDO(m))) return FALSE;
    u->AlPaso = t->AlPaso;
    u->fiftyMove = t->fiftyMove;
    u->enroque = t->enroque;
    u->fen = EscribirFen(t);
    if(m->castle!=EMPTY){

        if(m->castle==WKCA){

            t->pieces[E1]=EMPTY;
            t->pieces[H1]=EMPTY;
            t->pieces[G1]=wK;
            t->pieces[F1]=wR;
            if(aux%2==1){

                t->enroque-=1;
                aux-=1;
            }
            aux/=2;
            if(aux%2==1){
                t->enroque-=2;
            }
        }
        if(m->castle==WQCA){

            t->pieces[E1]=EMPTY;
            t->pieces[A1]=EMPTY;
            t->pieces[C1]=wK;
            t->pieces[D1]=wR;
            if(aux%2==1){

                t->enroque-=1;
                aux-=1;
            }
            aux/=2;
            if(aux%2==1){
                t->enroque-=2;
            }
        }
        if(m->castle==BKCA){

            t->pieces[E8]=EMPTY;
            t->pieces[H8]=EMPTY;
            t->pieces[G8]=bK;
            t->pieces[F8]=bR;
            aux-=BQCA;
            if(aux>=0){
                t->enroque-=8;
            }
            else if(aux<0)aux=t->enroque;

            aux-=BKCA;
            if(aux>=0)t->enroque-=4;
        }
        if(m->castle==BQCA){

            t->pieces[E8]=EMPTY;
            t->pieces[A8]=EMPTY;
            t->pieces[C8]=bK;
            t->pieces[D8]=bR;
            aux-=BQCA;
            if(aux>=0){
                t->enroque-=8;
            }
            else if(aux<0)aux=t->enroque;

            aux-=BKCA;
            if(aux>=0)t->enroque-=4;
    }

    }

    else{
            t->pieces[m->from] = EMPTY;
            t->pieces[m->to] = m->piezas[0];
            if(m->piezas[0]==wK || m->piezas[0]== bK) ksq = m->to;
            else ksq = t->KingSq[t->side];
            if(m->paso != EMPTY && m->paso != NO_SQ && m->paso!= OFFBOARD){
                t->pieces[m->paso -10 +20*(t->side)] = EMPTY;
            }
            if(SqAttacked(ksq ,1 - t->side,t)){
                t->pieces[m->from] = t->pieces[m->to];
                t->pieces[m->to] = m->piezas[1];
                if (m->paso!= EMPTY && m->paso!= NO_SQ){
                    t->pieces[m->to -10 +20*t->side]=bP - (CAMBIO_LADO)*(t->side);
                    t->pieces[m->to] = EMPTY;
                } 
                free_UNDO(u);
                return FALSE;
            }

            if (m->piezas[2] != EMPTY) t->pieces[m->to] = m->piezas[2];

    } 

    UpdateListsMaterial(t);


    CheckBoard(t);
    
    if(t->side==WHITE){
        aux-=BQCA;
        if(m->to==A8){
                if(aux>=0){                   
                    t->enroque-=8;
                }

                else if(aux<0)aux=t->enroque;

            }
            else if(m->to==H8){
                if(aux<0)aux=t->enroque;

                aux-=BKCA;
                if(aux>=0)t->enroque-=4;
             }
        aux = t->enroque;
        if(t->pieces[m->to]==wK){

            if(aux%2==1){

                t->enroque-=1;
                aux-=1;
            }
            aux/=2;
            if(aux%2==1){
                t->enroque-=2;
            }

        }
        else if(t->pieces[m->to]==wR){

            if(m->from==H1){

                if(aux%2==1)t->enroque-=1;
            }
            else if(m->from==A1){

                aux=(aux-(aux%2))/2;

                if(aux%2==1) t->enroque-=2;                    
            }
        }
    }
    else if(t->side==BLACK){
        if(m->to==H1){

                if(aux%2==1)t->enroque-=1;
            }
            else if(m->to==A1){

                aux=(aux-(aux%2))/2;

                if(aux%2==1) t->enroque-=2;                    
            }
        aux = t->enroque;
        if(t->pieces[m->to]==bK){

            aux-=BQCA;
            if(aux>=0){
                t->enroque-=8;
            }
            else if(aux<0)aux=t->enroque;

            aux-=BKCA;
            if(aux>=0)t->enroque-=4;
        }

        else if(t->pieces[m->to]==bR){

            aux-=BQCA;
            if(m->from==A8){

                if(aux>=0){                   
                    t->enroque-=8;
                }

                else if(aux<0)aux=t->enroque;

            }
            else if(m->from==H8){
                if(aux<0)aux=t->enroque;

                aux-=BKCA;
                if(aux>=0)t->enroque-=4;
            }
        }

    }



    if(m->piezas[0] == wP + CAMBIO_LADO*t->side && (m->to - m->from) == 20 -40*t->side){
        t->AlPaso = m->to -10 +20*t->side;
    }
    else{
        t->AlPaso = NO_SQ;
    }

    if(m->piezas[1] == EMPTY && m->piezas[0] != wP + CAMBIO_LADO*t->side){
        t->fiftyMove++;
    }
    else{
        t->fiftyMove = 0;
    }
    t->j_real++;


    t->history[t->histcont] = u;
    t->histcont++;
    t->side = 1- t->side;
    return TRUE;

}

/***********************************************************/
/* Función: DeshacerJugada           
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada: 
/* 
/* t=puntero al tablero
/*
/* Retorno:
/* ninguno
/*
/* Descripción:
/* Dado un tablero, deshace la última juga hecha.
/*
/* Más en detalle: Para deshacer la jugada se utiliza la estructura de S_UNDO que tiene el tablero. Se escoge su último elemento y
/* se guardan sus parámetros en los parámetros del tablero. Gracias a su información se puede recuperar de donde vino la pieza que 
/* se movió en la última jugada, si capturo una pieza cuál fue, si se comió al paso... El proceso simplemente es el de sobreescribir la información guardada en 
/* la del tablero y liberar este último elemento de el array de S_UNDO
/***********************************************************/


void DeshacerJugada(TABLERO *t) {
    int col,pce,flag,i,captura,promo;
    MOVE * move;
	int from;
    int to;
	
    ASSERT(CheckBoard(t));
	
	t->histcont--; //histcont  hisply
    t->j_real--;  //j_real ply
	
    move = t->history[t->histcont]->jugada;
    from = move->from;
    to = move->to;	
	


    t->enroque = t->history[t->histcont]->enroque;
    t->fiftyMove = t->history[t->histcont]->fiftyMove;
    t->AlPaso = t->history[t->histcont]->AlPaso;



    
    t->side=1-t->side;
	
	if(move->paso!=EMPTY&&move->paso!= OFFBOARD && move->paso != NO_SQ) {
        if(t->side == WHITE) {
            t->pieces[to-10]=bP;
        } else {
            t->pieces[to+10]=wP;
        }
    } else if(move->castle==WKCA  || move->castle==BKCA || move->castle==WQCA || move->castle==BQCA) {
        switch(move->castle) {
           
            
            case WKCA:  
                t->pieces[E1]=wK;
                t->pieces[H1]=wR;
                t->pieces[G1]=EMPTY;
                t->pieces[F1]=EMPTY;
                break;
            case WQCA: 
                t->pieces[E1]=wK;
                t->pieces[A1]=wR;
                t->pieces[C1]=EMPTY;
                t->pieces[D1]=EMPTY;
                break;
            case BKCA: 
                t->pieces[E8]=bK;
                t->pieces[H8]=bR;
                t->pieces[G8]=EMPTY;
                t->pieces[F8]=EMPTY;
                break;
            case BQCA: 
                t->pieces[E8]=bK;
                t->pieces[A8]=bR;
                t->pieces[C8]=EMPTY;
                t->pieces[D8]=EMPTY;

                break;
            default: ASSERT(FALSE);     
                break;
        }
    }
	
	
	//move piece to-from
	pce = t->pieces[to];	
    t->pieces[to] = EMPTY;
	t->pieces[from] = pce;
     
    
	
	captura = move->piezas[1];
    if(captura != EMPTY && (move->paso == EMPTY || move->paso == NO_SQ)) {
        t->pieces[to] = captura;
    }

    promo=move->piezas[2];
	
	if(promo != EMPTY)   {
        
        t->pieces[from]=EMPTY;
        t->pieces[from] = CAMBIO_LADO*t->side +wP;

    }    
    free_UNDO(t->history[t->histcont]);
    UpdateListsMaterial(t);
  
	
    ASSERT(CheckBoard(t));
    return;

}