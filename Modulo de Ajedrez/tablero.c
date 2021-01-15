
#include "stdio.h"
#include "definiciones.h"
#define BLANCO "\e[0;30m\e[47m"
#define NEGRO "\e[0;40m\e[37m"
#define RESET "\e[0m"

//char PceChar[] = ".♙♘♗♖♕♔♟♞♝♜♛♚";
char PceChar[] = ".PNBRQKpnbrqk";
char SideChar[] = "wb-";
char RankChar[] = "12345678";
char FileChar[] = "abcdefgh";

int PieceBig[13] = { FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE };
int PieceMaj[13] = { FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE };
int PieceMin[13] = { FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE };
int PieceCol[13] = { BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK };

int FILAsBrd[NUM_CASILLAS];
int COLsBrd[NUM_CASILLAS];






/***********************************************************/
/* Función: InitFILAsCOLsBrd                
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* No recibe ningun parametro de entrada
/* 
/* Retorno:
/* void
/*
/* Descripción:
/* Inicializa los arrays globales
/***********************************************************/

void InitFILAsCOLsBrd() {
	
	int index = 0;
	int col = COL_A;
	int fila = FILA_1;
	int sq = A1;
	int sq64 = 0;
	
	for(index = 0; index < NUM_CASILLAS; ++index) {
		FILAsBrd[index] = OFFBOARD;
		COLsBrd[index] = OFFBOARD;
	}
	
	for(fila = FILA_1; fila <= FILA_8; ++fila) {
		for(col = COL_A; col <= COL_H; ++col) {
			sq = FCCAS(col,fila);
			FILAsBrd[sq] = fila;
			COLsBrd[sq] = col;
		}
	}
}

/***********************************************************/
/* Función: Cas_Fila              
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* cas: casilla del tablero
/* 
/* Retorno:
/* Fila en la que esta la casilla
/***********************************************************/
int Cas_Fila (int cas){
	return FILAsBrd[cas];
}


/***********************************************************/
/* Función: InitFILAsCOLsBrd                
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* cas: casilla del tablero
/* 
/* Retorno:
/* Columna en la que esta la casilla
/***********************************************************/
int Cas_Col (int cas){
	return COLsBrd[cas];
}


/***********************************************************/
/* Función: pieceColour            
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* pce: pieza
/* 
/* Retorno:
/* Devuelve el color de una pieza
/***********************************************************/
int pieceColour(int pce){

    if(pce == wP || pce == wR || pce == wB || pce == wN || pce == wQ || pce == wK) return WHITE;

    else if(pce == bP || pce == bR || pce == bB || pce == bN || pce == bQ || pce == bK) return BLACK;

    return BOTH;
}
/***********************************************************/
/* Función: C64a120                
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* c64: índice en el tablero de 64 casillas
/* 
/* Retorno:
/* retorna el indice en el tablero de 120 casillas
/***********************************************************/

int C64a120 (int c64){
	int resto;
	int cociente;

	resto = c64 % 8;
	cociente = (c64-resto)/8;
	return (cociente*10 + resto + 21);
}


/***********************************************************/
/* Función: C120a64               
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* c64: índice en el tablero de 120 casillas
/* 
/* Retorno:
/* retorna el índice en el tablero de 64 casillas
/***********************************************************/

int C120a64 (int c120){
	int resta, resto, cociente;
	resta = c120- 21;
	if(resta<0 || resta >77 || (resta % 10) >= 8) return OFFBOARD;
	resto = resta % 10;
	cociente = (resta-resto)/10;
	return cociente*8 + resto;
}


/***********************************************************/
/* Función: CheckBoard                     
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* pos: puntero a un tablero
/* 
/* Retorno:
/* TRUE : Si la posicion es correcta
/* FALSE: Si la posición es incorrecta
/*
/* Descripción:
/* Comprueba un tablero
/*
/* Mas en detalle:
/* Función para dada una posicion en el tablero, mediante el uso de una serie de arrays y variables internas, crea una posición "espejo" para 
/* luego comprobar que coincide con la dada y que así se la informacion almacenada en cada una de las martes de estructura 
/* tablero que es redundante es crrecta
/***********************************************************/

int CheckBoard(const TABLERO *pos) {   
	
 
	int esp_pceNum[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	int esp_material[2] = { 0, 0};
	
	int sq64,esp_piece,esp_pce_num,sq120,colour;

	for(esp_piece = wP; esp_piece <= bK; ++esp_piece) {
		for(esp_pce_num = 0; esp_pce_num < pos->pceNum[esp_piece]; ++esp_pce_num) {
			sq120 = pos->pList[esp_piece][esp_pce_num];
			ASSERT(pos->pieces[sq120]==esp_piece);
		}	
	}


	for(sq64 = 0; sq64 < 64; ++sq64) {
		sq120 = C64a120(sq64);
		esp_piece = pos->pieces[sq120];
		esp_pceNum[esp_piece]++;
		colour = pieceColour(esp_piece);
		
		esp_material[colour] += PieceVal[esp_piece];
	}
	
	for(esp_piece = wP; esp_piece <= bK; ++esp_piece) {
		ASSERT(esp_pceNum[esp_piece]==pos->pceNum[esp_piece]);	
	}
	

	ASSERT(esp_material[WHITE]==pos->material[WHITE] && esp_material[BLACK]==pos->material[BLACK]);

	
	ASSERT(pos->side==WHITE || pos->side==BLACK);

	
	ASSERT(pos->AlPaso==NO_SQ || ( FILAsBrd[pos->AlPaso]==FILA_6 && pos->side == WHITE)
		 || ( FILAsBrd[pos->AlPaso]==FILA_3 && pos->side == BLACK));
	
	ASSERT(pos->pieces[pos->KingSq[WHITE]] == wK);
	ASSERT(pos->pieces[pos->KingSq[BLACK]] == bK);
		 
	return TRUE;	
}

/***********************************************************/
/* Función: UpdateListsMaterial                       
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* pos: puntero a un tablero
/* 
/* Retorno:
/* void
/*
/* Descripción:
/* Actualiza el material 
/*
/* Mas en detalle:
/* Recorre el array de piezas para ir almacenando el material en los distintos campos de la estructura tablero
/***********************************************************/



void UpdateListsMaterial(TABLERO *pos) {	
	
	int pieza,cas,i,color;
	
	pos->material[0] = 0;
	pos->material[1] = 0;

	for (i = 0; i < 13 ;i++) {
		pos->pceNum[i] = 0;
	}

	for(i = 0; i < NUM_CASILLAS; i++) {
		cas = i;
		pieza = pos->pieces[i];

		if(pieza!=OFFBOARD && pieza!= EMPTY) {
			if(pieza <= wK) color = WHITE;
			else color = BLACK;

			pos->material[color] += PieceVal[pieza];

			pos->pList[pieza][pos->pceNum[pieza]] = cas;

			pos->pceNum[pieza]++;

			if(pieza==wK) pos->KingSq[WHITE] = cas;
			if(pieza==bK) pos->KingSq[BLACK] = cas;	
			
		}
	}
}
/***********************************************************/
/* Función: LeerFen                          
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:    
/* fen: cadena de caracteres con el fen de una posicion guardado
/* pos: puntero a un tablero
/* 
/* Retorno:
/* retorna OK o ERR si ha fallado algo
/*
/* Descripción:
/* Recibe una cadena guardada en un fen y la carga en el tablero
/*
/*Mas en Detalle:
/* Recibe una cadena guardada en un fen y la carga en el tablero. Para ello se resetea el tablero y se va almacenando la informacion del fen en el array de piezas
/* y en los distintos campos de ña estructura tablero. Finalmente se actualiza el material del tablero para que la posición quede completamente cargada.
/***********************************************************/


int LeerFen(char *fen, TABLERO *pos) {
	
	ASSERT(fen!=NULL);
	ASSERT(pos!=NULL);
	
	int  fila = FILA_8, col = COL_A, piece = 0, count = 0, i = 0,  c64 = 0, c120 = 0;
	short flag =0;
	
	ResetBoard(pos);
	
	while ((fila >= FILA_1) && *fen) {
	    count = 1;
		switch (*fen) {
            case 'p': piece = bP; break;
            case 'r': piece = bR; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'k': piece = bK; break;
            case 'q': piece = bQ; break;
            case 'P': piece = wP; break;
            case 'R': piece = wR; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'K': piece = wK; break;
            case 'Q': piece = wQ; break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                piece = EMPTY;
                count = *fen - '0';
                break;

            case '/':
            case ' ':
                fila--;
                col = COL_A;
                count=0;
                break;              

            default:
                printf("FEN error \n");
                return -1;
        }		
		
		for (i = 0; i < count; i++) {			
            c64 = fila * 8 + col;
			c120 = C64a120(c64);
            if (piece != EMPTY) {
                pos->pieces[c120] = piece;
            }
			col++;
        }
		fen++;
	}
	ASSERT(*fen == 'w' || *fen == 'b');
	
	if(*fen == 'w') pos->side = WHITE;
	else pos->side = BLACK;
	fen += 2;
	
	pos->enroque = 0;
	for (i = 0, flag = 1; i < 4 && flag == 1; i++) {

		switch(*fen) {
			case 'K': pos->enroque += WKCA; fen++; break;
			case 'Q': pos->enroque += WQCA; fen++; break;
			case 'k': pos->enroque += BKCA; fen++; break;
			case 'q': pos->enroque += BQCA; fen++; break;
			default: flag = 0;
        }
	}
	fen++;
	
	ASSERT(pos->enroque>=0 && pos->enroque <= 15);
	
	if (*fen != '-') {        
		col = fen[0] - 'a';
		fila = fen[1] - '1';
		
		ASSERT(col>=COL_A && col <= COL_H);
		ASSERT(fila>=FILA_1 && fila <= FILA_8);
		
		pos->AlPaso = FCCAS(col,fila);
		fen++;	
    }
	fen += 2;

	if(fen[1] == ' '){
		pos->fiftyMove = fen[0] - '0';
	}
	else{
		pos->fiftyMove = (fen[0] - '0')*10 + (fen[1] - '0');
		fen++;
	}
	fen+=2;

	if(fen[1] == '\0'){
		pos->j_real = 2*(fen[0]-'0') + pos->side-1;
	}
	else if(fen[2] == '\0'){
		pos->j_real = 2*((fen[1]-'0')*10 + (fen[0]-'0')) + pos->side-1;
	}
	else if(fen[3] == '\0'){
		pos->j_real = 2*((fen[2]-'0')*100 + (fen[1]-'0')*10 + (fen[0]-'0')) + pos->side-1;
	}
	else if (fen[4] == '\0'){
		pos->j_real = 2*((fen[3]-'0')*1000 + (fen[2]-'0')*100 + (fen[1]-'0')*10 + (fen[0]-'0')) + pos->side -1;
	}
	else return -1;
	
	UpdateListsMaterial(pos);
	
	return 0;
}


/***********************************************************/
/* Función: ResetBoard                         
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* pos: puntero a un tablero
/* 
/* Retorno:
/* void
/*
/* Descripción:
/* Devuelve un tablero sin piezas con todas las estructuras inicializadas
/***********************************************************/

void ResetBoard(TABLERO *pos) {

	int i = 0;
	int j = 0;

	for(i = 0; i < NUM_CASILLAS; ++i) {
		pos->pieces[i] = OFFBOARD;
	}

	for(i = 0; i < 64; ++i) {
		pos->pieces[C64a120(i)] = EMPTY;
	}

	pos->KingSq[WHITE] = pos->KingSq[BLACK] = NO_SQ;

	pos->side = BOTH;
	pos->AlPaso = NO_SQ;
	pos->fiftyMove = 0;
	
	pos->histcont = 0;
	pos->j_real = 0;
	
	pos->enroque = 0;
	pos->material[0] = 0;
	pos->material[1] = 0;
	
	
	for(i = 0; i < 12; ++i) {
		pos->pceNum[i] = 0;
	}
	for(i=0;i<13;i++){
		for(j=0;j<10;j++){

			pos->pList[i][j]=0;	
		}
	}
	pos->KingSq[0]=0;
	pos->KingSq[1]=0;

	
}

/***********************************************************/
/* Función: PrintBoard                           
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* pos: puntero a un tablero
/* 
/* Retorno:
/* void
/*
/* Descripción:
/* Imprime el tablero contenido en pos
/*
/*Mas en Detalle: 
/* Recorre el array de piezas e imprime la numeración de las filas y las casillas junto con las piezas almacenadas en este
/* usando los codigos de Ascii para cambiar los colores de la fuente y el fondo en la terminal.
/***********************************************************/

void PrintBoard(const TABLERO *pos) {
	
	int pieza,fila,col,piezan;
	int cont=0;
	

	printf("\nGame Board:\n\n");
	
	for(fila = FILA_8; fila >= FILA_1; fila--) {
		printf("%d  ",fila+1);
		for(col = COL_A; col <= COL_H; col++ ,cont ++) {
			pieza = FCCAS(col,fila);
			piezan = pos->pieces[pieza];
			if(cont%2==0)  printf("\e[1;33m\e[47m");
			if(cont%2==1) 	printf("\e[1;33m\e[45m");
			printf(" ");
            if(piezan==0 )
			printf(" ");
			if(piezan==1)
			printf("♟");
			if(piezan==2)
			printf("♞");
			if(piezan==3)
			printf("♝");
			if(piezan==4)
			printf("♜");
			if(piezan==5)
			printf("♛");
			if(piezan==6)
			printf("♚");
			printf("\e[0m");
			if(cont%2==0)  printf("\e[1;30m\e[47m");
			if(cont%2==1) 	printf("\e[1;30m\e[45m");
			
			if(piezan==7)
			printf("♟");
			if(piezan==8)
			printf("♞");
			if(piezan==9)
			printf("♝");
			if(piezan==10)
			printf("♜");
			if(piezan==11)
			printf("♛");
			if(piezan==12)
			printf("♚");
			printf(" ");

			printf("\e[0m");

		}
		cont+=1;
		printf("\n");
	}
	printf("\e[0m");
	printf("\n ");
	printf(" ");
	for(col = COL_A; col <= COL_H; col++) {
		printf("%3c",'a'+col);	
	}
	printf("\n");
	printf("side:%c\n",SideChar[pos->side]);
	if(pos->AlPaso==99)printf("AlPaso:NO\n");
	else printf("AlPaso:%d\n",pos->AlPaso);
	printf("castle:%c%c%c%c\n",
			pos->enroque & WKCA ? 'K' : '-',
			pos->enroque & WQCA ? 'Q' : '-',
			pos->enroque & BKCA ? 'k' : '-',
			pos->enroque & BQCA ? 'q' : '-'	
			);
	
}
/***********************************************************/
/* Función: Create_tablero                         
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* No recibe Parámetros de entrada
/* 
/* Retorno:
/* Estructura tablero creada
/*
/* Descripción:
/* Crea una estructura tablero
/***********************************************************/

TABLERO* Create_tablero(){

	TABLERO *tab=NULL;
	int i;


	tab=(TABLERO*)malloc(sizeof(TABLERO));
	if(!tab) 
		return NULL;

	tab->KingSq=(int*)malloc(2*sizeof(int));
	tab->pceNum=(int*)malloc(13*sizeof(int));
	tab->material=(int*)malloc(2*sizeof(int));
	tab->pieces=(int*)malloc(NUM_CASILLAS*sizeof(int));

	if(!tab->material||!tab->pceNum||!tab->KingSq){
		
		
		Free_tablero(tab);
		return NULL;
	}

	tab->pList=(int**)malloc(13*sizeof(int*));
	if(!tab->pList){

		Free_tablero(tab);
		return NULL;
	}

	for(i=0;i<13;i++){

	tab->pList[i]=(int*)malloc(10*sizeof(int));

	if(!tab->pList[i]){

		Free_tablero(tab);
		return NULL;

	}
	}
	//Podria cambiarse por realloc
	tab->history = (S_UNDO**) malloc(MAXGAMEMOVES*sizeof(S_UNDO*));

	if(!tab->history){
		Free_tablero(tab);
		return NULL;
	}

	return tab;
}

/***********************************************************/
/* Función: Free_tablero                          
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* tab: puntero a un tablero
/* 
/* Retorno:
/* void
/*
/* Descripción:
/* Libera una estructura de datos tablero
/***********************************************************/

void Free_tablero(TABLERO *tab){
	int i;
	if(tab){
		if(tab->pieces)free(tab->pieces);


		if(tab->KingSq)free(tab->KingSq);

		if(tab->material)free(tab->material);

		if(tab->pceNum)free(tab->pceNum);

		if(tab->pList){
			
			for(i=0;i<13;i++){
				if(tab->pList[i])free(tab->pList[i]);
			}
			free(tab->pList);
		}

		if(tab->history){
			for(i=0;i<tab->histcont;i++){
				free_UNDO(tab->history[i]);
			}
			free(tab->history);
		}
		free(tab);
	}
	return;

}

/***********************************************************/
/* Función: EscribirFen                      
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* t: puntero a un tablero
/* 
/* Retorno:
/* cadena de caracteres con el fen de la posicion del tablero
/*
/* Descripción:
/* Dado un tablero guarda en una cadena el fen asociado a la posicion
/***********************************************************/


char * EscribirFen(TABLERO *t){
	char *fen;
	int i=0,ln=0,a=0;
	int pieza,cont=0;
	char aux;
	short flag = TRUE;


	if(!t) return NULL;

	if(!(fen = (char*)malloc(MAXFEN*sizeof(char)))) return NULL;

	for(i=64;i>0 && flag == TRUE;i--){

		a = i + 6 -2*((i-1) %8);
		pieza = t->pieces[C64a120(a)];
		if(i!= 64 && i%8 ==0){
			if(cont!= 0){
				fen[ln] = '0' + cont;
				ln ++;
				cont = 0;
			}
			fen[ln] = '/';
			ln++;
		}
		switch (pieza)
		{
		case EMPTY:
			cont++;
			break;
		case OFFBOARD: 

			flag = FALSE;
			break;

		default:
			if(cont!= 0){
				fen[ln] = '0' + cont;
				ln ++;
				cont = 0;
			}

			fen[ln] = PceChar[pieza]; 
			ln++; 
			break;
		}
	}

	fen[ln] = ' ';
	ln++;
	if(t->side == WHITE) fen[ln] = 'w';
	else fen[ln] = 'b';
	ln++;

	fen[ln] = ' ';
	ln++;
	cont = t->enroque;

	if(cont % 2 == 1){
		fen[ln] = 'K';
		ln++;
	}

	cont = (cont- cont %2)/2;

	if(cont % 2 == 1){
		fen[ln] = 'Q';
		ln++;
	}

	cont = (cont- cont %2)/2;

	if(cont % 2 == 1){
		fen[ln] = 'k';
		ln++;
	}

	cont = (cont- cont %2)/2;

	if(cont % 2 == 1){
		fen[ln] = 'q';
		ln++;
	}

	fen[ln] = '\0';
	ln++;

	return fen;
} 

/***********************************************************/
/* Función: Repetida                    
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* tab: puntero a un tablero
/* times: número de veces que se repite la última jugada
/*
/* Retorno:
/* valor TRUE/FALSE si la jugada está repetida o no
/*
/* Descripción:
/* Determina una jugada está repetida o no y cuenta el número de veces que esta está repetida
/***********************************************************/

int Repetida(TABLERO *tab, int *times){

	int i=0;
	char *aux=NULL, *fen=NULL;

	ASSERT(tab!=NULL);

	(*times)=1;

	if(tab->histcont==0)return FALSE;

	fen=tab->history[tab->histcont -1]->fen;
	
	for(i=tab->histcont - tab->fiftyMove; i<(tab->histcont);i++){

		aux=tab->history[i]->fen;
		
		if(strcmp(aux, fen)==0){
			(*times)++;
		
		} 
	

		if((*times)==3)return TRUE;

	}
	if((*times)>1)return TRUE;

	return FALSE;

}
/***********************************************************/
/* Función: esTablas                    
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* tab: puntero a un tablero
/* 
/* Retorno:
/* valor TRUE/FALSE 
/*
/* Descripción:
/* Determina si la posicion de un tablero es tablas o no
/***********************************************************/

int esTablas(TABLERO *tab){

	int flag=0, flag1=0, cont=0;

	ASSERT(tab!=NULL);

	if(tab->fiftyMove >= 100)return TRUE;
	
	flag=Repetida(tab, &cont);
	
	

	flag1=InsufMat(tab);

	
	if(flag1==TRUE)return TRUE;

	if(flag==FALSE||(flag==TRUE && cont<3)) return FALSE;
	
	
	return TRUE;

}

/***********************************************************/
/* Función: InsufMat                          
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* tab: puntero a un tablero
/* 
/* Retorno:
/* valor TRUE/FALSE
/*
/* Descripción:
/* Comprueba si hay material suficiente para seguir jugando
/***********************************************************/

int InsufMat(TABLERO *tab){

	ASSERT(tab!=NULL);

	if(tab->material[BLACK]==50000 && tab->material[WHITE]==50000)return TRUE;
	if(tab->material[WHITE]==50325 && tab->material[BLACK]==50000)return TRUE;
	if(tab->material[BLACK]==50325 && tab->material[WHITE]==50000)return TRUE; 

	return FALSE;

}

/***********************************************************/
/* Función: InsufMat                          
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:                                                 
/* tab: puntero a un tablero
/* 
/* Retorno:
/* valor TRUE/FALSE/TABLA
/*
/* Descripción:
/* Decide si se puede continuar la partida o ya se ha acabado.
/*
/* Mas en Detalle:
/* Para determinar si se acaba la partida se comprueba si es Tablas, o si no hay jugadas posibles y ademas el rey está atacado por lo que seria jaque mate
/***********************************************************/

int FinPartida(TABLERO *tab){
	MOVE **m;
	int count=0,i;
	int flag = TRUE;
	if(!tab) return FALSE;
	if(esTablas(tab)== TRUE) return TABLAS;

	m = Generador_Movimientos(tab,&count);

	for(i=1;i<count && flag == TRUE;i++){
		if(HacerJugada(tab,m[i])==TRUE){
			DeshacerJugada(tab);
			flag = FALSE;
		}
	}

	for(i=0;i<count;i++){
		free_move(m[i]);
	}
	free(m);
	
	
	if(flag == TRUE){
		if(SqAttacked(tab->KingSq[tab->side],1-tab->side,tab)==TRUE) return GANAN_NEGRAS + 2 * tab->side;
		else return TABLAS;
	}

	return FALSE;
	
}
