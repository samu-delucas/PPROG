#include <colorear.h>

/*Mejoraré algunas cosas*/

#define NO_COMPLETADO 0
#define SEMICOMPLETADO 1
#define COMPLETADO 2
#define COLOR_ESQUELETO 47 /*Blanco*/
#define COLOR_NO_COMPLETADO 41 /*Rojo*/
#define COLOR_SEMICOMPLETADO 43 /*Amarillo*/
#define COLOR_COMPLETADO 42 /*Verde*/
#define LONG_ESQUELETO_COLOREAR "                                                                         "
#define LONG_MENU_COLOREAR "                           "
#define LONG_CASILLA "       "
#define ALTO_CASILLA 4
#define LARGO_CASILLA 8
#define MARCADOR " ô-ô "
#define COLOR_MARCADOR 44 /*Azul*/
#define COLOR_LETRAS_MARCADOR 40 /*Negro*/

void colorear_generar(int filas, int columnas, int matriz[filas][columnas]){
    int i,j;
    for(i=0;i<filas;i++){
        for(j=0;j<columnas;j++){
            matriz[i][j]=aleat_num(NO_COMPLETADO,COMPLETADO);
        }
    }
    return;
}

int colorear_read_key(int filas, int columnas, int *row, int *col, int matriz[filas][columnas]){
  char choice;

  choice = fgetc(stdin);

  if (choice==' ' || (choice == 27 && fgetc(stdin) == '[')) { // The key is an arrow key 
    
    if(choice == ' '){
    matriz[(*row+1)%filas][(*col)]=(matriz[(*row+1)%filas][(*col)]+1)%3;
    matriz[(*row-1+filas)%filas][(*col)]=(matriz[(*row-1+filas)%filas][(*col)]+1)%3;
    matriz[(*row)][(*col+1)%columnas]=(matriz[(*row)][(*col+1)%columnas]+1)%3;
    matriz[(*row)][(*col-1+columnas)%columnas]=(matriz[(*row)][(*col-1+columnas)%columnas]+1)%3;
    return 1;
  }

    choice = fgetc(stdin);

    switch(choice) {
    case('A'): /*ARRIBA*/
        (*row)=(*row-1+filas)%filas;
        matriz[(*row)][(*col)]=(matriz[(*row)][(*col)]+1)%3;
        
        return 1;
    case('B'): /*ABAJO*/
        (*row)=(*row+1+filas)%filas;
        matriz[(*row)][(*col)]=(matriz[(*row)][(*col)]+1)%3;
        return 1;
    case('C'): /*Derecha*/
        (*col)=(*col+1+columnas)%columnas;
        matriz[(*row)][(*col)]=(matriz[(*row)][(*col)]+1)%3;
        return 1;
    case('D'):
        /*Izquierda*/
        (*col)=(*col-1+columnas)%columnas;
        matriz[(*row)][(*col)]=(matriz[(*row)][(*col)]+1)%3;
        return 1;

    default:
      return 0;
    }
  }
}

void colorear_print_esqueleto(sc_rectangle *frontal){
    int i, j;

    win_write_line_at_colour(frontal,5,82,"OBJETIVOS",40, frontal->bg_color);
    win_write_line_at_colour(frontal,7,81,"OK",40, frontal->bg_color);
    win_write_line_at_colour(frontal,9,81,"CONTROLES",40, frontal->bg_color);
    win_write_line_at_colour(frontal,11,79," ^",40, frontal->bg_color);
    win_write_line_at_colour(frontal,12,77," <   >   ESPACIO",40, frontal->bg_color);
    win_write_line_at_colour(frontal,13,77,"   v",40, frontal->bg_color);
    win_write_line_at_colour(frontal,15,81,"MOVIMIENTOS",40, frontal->bg_color);
    
    for(i=2;i<30;i+=4)
        win_write_line_at_colour(frontal,i,2,LONG_ESQUELETO_COLOREAR,COLOR_ESQUELETO, COLOR_ESQUELETO);

    for(i=2;i<80;i+=8){
        for(j=2; j<22;j++){
            win_write_line_at_colour(frontal,j,i," ",COLOR_ESQUELETO, COLOR_ESQUELETO);
        }
    }

    win_write_line_at_colour(frontal,2,70,LONG_MENU_COLOREAR,COLOR_ESQUELETO, COLOR_ESQUELETO);
    win_write_line_at_colour(frontal,22,70,LONG_MENU_COLOREAR,COLOR_ESQUELETO, COLOR_ESQUELETO);
    for(j=2; j<23;j++){
            win_write_line_at_colour(frontal,j,97," ",COLOR_ESQUELETO, COLOR_ESQUELETO);
        }

    win_write_line_at_colour(frontal,7,78,"  ",COLOR_COMPLETADO, COLOR_COMPLETADO);

    return;
}

int colorear_print_casillas(sc_rectangle *frontal, int row, int col, int matriz[row][col]){
    int i, j, k;
    int color, retorno=COMPLETADO;

    for(i=0;i<row;i++){
        for(j=0;j<col;j++){
            if(matriz[i][j]==NO_COMPLETADO){
                color=COLOR_NO_COMPLETADO;
                retorno=NO_COMPLETADO;
            }
            else if (matriz[i][j]==SEMICOMPLETADO){
                color=COLOR_SEMICOMPLETADO; 
                retorno=NO_COMPLETADO;
            }
            else{
                color=COLOR_COMPLETADO;
            }


            for (k=0;k<ALTO_CASILLA-1;k++)
                win_write_line_at_colour(frontal,3+i*ALTO_CASILLA+k, 3+j*LARGO_CASILLA, LONG_CASILLA, color, color);
        }

    }


    return retorno;
}

int colorear_play(intrf *interfaz) {
    int matriz[5][9], i , j, filas=5, columnas=9, row, col, movs=0;
    
    colorear_generar(filas, columnas, matriz); 

    row=0;
    col=0;
    colorear_generar(filas,columnas,matriz);
    
    win_cls(interfaz->left_up);
    win_write_border(interfaz->left_up);
    colorear_print_esqueleto(interfaz->left_up);


    while(colorear_print_casillas(interfaz->left_up, 5, 9, matriz)==NO_COMPLETADO && movs<=100){
        win_write_line_at_colour(interfaz->left_up,3+row*ALTO_CASILLA+1, 3+ col*LARGO_CASILLA+1, MARCADOR, COLOR_MARCADOR, COLOR_MARCADOR);
        movs+=colorear_read_key(filas, columnas, &row, &col, matriz);
        win_write_int_at(interfaz->left_up, 17, 82, movs);
    }

    win_cls(interfaz->left_up);
    _i_redraw(interfaz);
    
    if(movs>100){
        return 0;
    }

    return 250-movs;
}