#include "stdio.h"
#include <string.h>
#include "definiciones.h"


#define MAXSTRJUGADA 16

/***********************************************************/
/* Función: is_Valid                             
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:
/* m: puntero al movimiento que queremos ver si es válido
/* t: puntero al tablero con la posición en la que queremos hacer el movimiento
/* 
/* Retorno:
/* flag: La función devuelve FALSE si el movimiento no es válido y hay algún error y OK si el movimiento es válido
/*
/* Descripción:
/* Función que comprueba si un movimiento es válido en una posición.
/*
/* Más en detalle: La función genera todos los movimientos posibles para la posición del tablero t y compara si el movimiento está entre ellos.
/***********************************************************/

int is_Valid(MOVE *m,TABLERO *t){
    int count = 0,i,flag = FALSE;
    MOVE **array;

    if(!m||!t) return FALSE;

    array = Generador_Movimientos(t,&count);

    if(!array) return FALSE;

    for(i=0;i<count && flag == FALSE;i++){
        flag = move_cmp(array[i],m);
    }

    for(i=0;i<count;i++){
        free_move(array[i]);
    }
    free(array);

    return flag;

}

/***********************************************************/
/* Función: LeerMovimiento                             
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:
/* entrada: string que contiene la jugada introducida por teclado en formato: 
/*  (P)from(x)to(=C) donde se da que:
/*  - P corresponde a la pieza que se omite si se mueve un peón (en mayúsculas)
/*  - from corresponde a la casilla desde la que se mueve la pieza (en minúsculas)
/*  - x indica si hay captura. Si no hay captura se omite
/*  - to corresponde a la casilla a la que va la pieza (en minúsculas)
/*  - =C se escribe si hay una coronación y en C se escribe la pieza en la que se corona (en mayúsculas)
/*  - En el caso de que el movimiento sea un enroque se escribirá 0-0 para el enroque en el ala de rey y 0-0-0 para el enroque en el ala de dama
/*
/* t: puntero al tablero con la posición en la que queremos hacer el movimiento
/* 
/* Retorno:
/* La función devuelve un puntero a una estructura de movimiento si el movimiento es correcto y NULL si el movimiento es incorrecto 
/*
/* Descripción:
/* Función que toma una cadena de caracteres con un movimiento (que generalmente vendrá de una entrada del teclado) y la transforma en una esturctura de movimiento.
/* Es una función que sirve de traductor entre el humano y el módulo.
/*
/* Más en detalle: La función va separando entre los distintos casos descritos en el formato de entrada.Primero vemos si hay enroque, 
/* después la primera pieza, la casilla from (vemos si es válida), si hay captura, la casilla from (vemos si es válida), la coronación 
/* y el caso en el que se de una captura al paso. Con estos datos podemos crear un movimiento mediante move_insert y llamar a la función 
/* is_Valid para ver si es correcto. En ese caso se devuelve el movimiento en cuestión.
/***********************************************************/

MOVE *LeerMovimiento(char *entrada, TABLERO *t){
    MOVE *m;
    char *aux = entrada;
    int from,to,pieza= EMPTY;
    int captura = EMPTY,corona=EMPTY;
    int paso =EMPTY;

    if(!aux || !t) return NULL;


    if(*aux == '0'){
        if(strncmp("0-0-0",aux,5) == 0){
            m=insert_move(WQCA + t->side*6, EMPTY,EMPTY,EMPTY, EMPTY,EMPTY, EMPTY);
            if(is_Valid(m,t)) return m;
            free_move(m);
            return NULL;
        }
        else if(strncmp("0-0",aux,3) == 0){

            m=insert_move(WKCA + t->side*3, EMPTY,EMPTY,EMPTY, EMPTY,EMPTY, EMPTY);
            if(is_Valid(m,t)) return m;
            free_move(m);
            return NULL;
        }
        else return NULL;
    }
    else if (*aux >= 'a' && *aux <= 'h'){
        pieza = wP +t->side*(CAMBIO_LADO);

    }

    switch (*aux)
    {
        case 'N': pieza = wN + t->side*(CAMBIO_LADO);aux++; break;
        case 'B': pieza = wB + t->side*(CAMBIO_LADO);aux++; break;
        case 'R': pieza = wR + t->side*(CAMBIO_LADO);aux++; break;
        case 'Q': pieza = wQ + t->side*(CAMBIO_LADO);aux++; break;
        case 'K': pieza = wK + t->side*(CAMBIO_LADO);aux++; break;
        
        default:
            break;
    }


    if(pieza == EMPTY) return NULL;
    if ((*aux) < 'a' || (*aux) > 'h') return NULL;
    if(*(aux+1) < '1' || *(aux+1) > '8') return NULL;


    from = FCCAS(*aux-'a',*(aux+1)-'1');
    aux++;

    if(from < 0 || from >= 120) return NULL;
    if(t->pieces[from] == OFFBOARD) return NULL;
    aux++;

    if(*aux == 'x'){
        captura = 1;
        aux++;
    }

    if ((*aux) < 'a' || (*aux) > 'h') return NULL;

    
    if(*(aux+1) < '1' || *(aux+1) > '8') return NULL;

    to = FCCAS(*aux-'a', *(aux+1) -'1');
    aux++;
    if(to < 0 || to >= 120) return NULL;

    if(t->pieces[to] == OFFBOARD) return NULL;
    aux++;

    if(*aux == '=' && pieza ==  wP +t->side*(CAMBIO_LADO)){

        aux++;

        switch (*aux){
            case 'N': corona = wN + CAMBIO_LADO*(t->side); break;
            case 'B': corona = wB + CAMBIO_LADO*(t->side); break;
            case 'R': corona = wR + CAMBIO_LADO*(t->side); break;
            case 'Q': corona = wQ + CAMBIO_LADO*(t->side); break;
            default: return NULL;
        }
    }
    if(captura == 1){
        captura = t->pieces[to];
        if(captura == EMPTY){
            captura = bP -CAMBIO_LADO*t->side;
            paso = to;
        }
    }

    m = insert_move(EMPTY, from,to,pieza, captura,corona, paso);

    if(is_Valid(m,t)){

        
        return m;
    }
    free_move(m);
    return NULL;

}

void JuegaModulo(TABLERO *tab){
    MOVE *jugada=NULL;
    INFO info;
    info.depth = PROFUNDIDAD;
    jugada = SearchPosition(tab,&info);
    HacerJugada(tab,jugada);
    PrintBoard(tab);
    printf("El módulo ha jugado ");
    PrintMove(jugada);
    printf("\n\n");
    free_move(jugada);
}


int ObtenerJugada(TABLERO *tab){
    int flag = OK;
    char entradajugada[MAXSTRJUGADA]="\0";
    MOVE *jugada=NULL;

    do{
        printf("Introduzca su jugada:\n");
        fflush(stdin);
        if(fgets(entradajugada, sizeof(entradajugada),stdin) == NULL) flag = ERR;

        if(flag != ERR){
            if(strncmp(entradajugada,"exit",4)==0){
                flag = EXIT;
                break;
            }
            if(strncmp(entradajugada,"mod",3)==0){
                JuegaModulo(tab);
                if (FinPartida(tab) != FALSE)break;
                flag = ObtenerJugada(tab);
                break;
            }
            jugada = LeerMovimiento(entradajugada,tab);
            if(!jugada) printf("Jugada inválida.\n");
            else{
                if(HacerJugada(tab, jugada) == FALSE){
                printf("jugada inválida.\n");
                free_move(jugada);
                jugada = NULL;
                }
            }
        }

    }while(flag == OK && !jugada);

    
    free_move(jugada);
    return flag;
}


/***********************************************************/
/* Función: Menu_juego                             
/* Autores: Omicron: Pablo Soto, Sergio Leal, Raúl Díaz                                  
/*                                                         
/* Parámetros de entrada:
/* t: puntero al tablero con el que se va a jugar
/* 
/* Retorno:
/* La función devuelve ERR si se ha producido algún error en el menú y OK si todo ha ido de manera correcta
/*
/* Descripción:
/* Función que ejecuta un menú básico de juego
/*
/* Más en detalle: La función pregunta al usuario en que bando quiere jugar. Una vez que lo decide dependiendo de quien sea el turno,
/* llama al módulo para que haga una jugada o al usuario a que introduzca una jugada por teclado. Cuando la introduce el usuario,
/* llama a la función LeerMovimiento para comprobar si es correcta. Cuando le toca al módulo, llama a la función SearchPosition
/* que devuelve la jugada que hace el módulo. Una vez tenemos una jugada correcta, llamamos a HacerJugada para moverla y cambiamos a 
/* quien le toca. Se saldrá de la función cuando se llegue a una posición de tablas, o mate de una de las dos partes.
/***********************************************************/


int Menu_juego(TABLERO *tab){
    int flag = OK;
    int flag1;

    char bando='\0';
    char entradajugada[MAXSTRJUGADA]="\0";

    int acabar = FALSE;

    printf("¡¡¡Bienvenido a nuestro modulo de ajedrez!!!\n\n");

    do{
        printf("Seleccione el bando con el que quiera jugar (w,b):\n");
        fflush(stdin);
        flag1 = scanf("%c",&bando);
        getchar();
        if(flag1 == EOF) return ERR;
        if(bando != 'w' && bando != 'b') printf("Error al introducir el bando.\n");
    }while(bando != 'w'&& bando != 'b'&&flag == OK);
    if (flag == ERR) return ERR;

    printf("Empezamos la partida!\n");

    if(bando == 'w'){
        PrintBoard(tab);

        if((flag = ObtenerJugada(tab))==EXIT) acabar =EXIT;
    }
    
    if(acabar != EXIT) acabar = FinPartida(tab);
    if(acabar != FALSE){
        switch (acabar){
            case GANAN_NEGRAS: printf("Las negras han ganado esta partida. Bien jugado\n"); break;
            case TABLAS: printf("Esta partida ha sido tablas. Bien jugado\n"); break;
            case GANAN_BLANCAS: printf("Las blancas han ganando esta partida. Bien jugado\n"); break;
            case EXIT: printf("Se ha cancelado la partida. Hasta luego\n"); break;
            default:break;
        }
    }
    PrintBoard(tab);
    while(flag == OK && acabar == FALSE){
        JuegaModulo(tab);

        acabar = FinPartida(tab);
        if(acabar != FALSE){
            switch (acabar){
                case GANAN_NEGRAS: printf("Las negras han ganado esta partida. Bien jugado\n"); break;
                case TABLAS: printf("Esta partida ha sido tablas. Bien jugado\n"); break;
                case GANAN_BLANCAS: printf("Las blancas han ganando esta partida. Bien jugado\n"); break;
                case EXIT: printf("Se ha cancelado la partida. Hasta luego\n"); break;
                default:break;
            }
        }
        else{
            if((flag = ObtenerJugada(tab))==EXIT) acabar =EXIT;

            }

        PrintBoard(tab);
        if(acabar != EXIT) acabar = FinPartida(tab);
        if(acabar != FALSE){
            switch (acabar){
                case GANAN_NEGRAS: printf("Las negras han ganado esta partida. Bien jugado\n"); break;
                case TABLAS: printf("Esta partida ha sido tablas. Bien jugado\n"); break;
                case GANAN_BLANCAS: printf("Las blancas han ganando esta partida. Bien jugado\n"); break;
                case EXIT: printf("Se ha cancelado la partida. Hasta luego\n"); break;
                default:break;
            }
        }
        
    }
    return OK;

}

