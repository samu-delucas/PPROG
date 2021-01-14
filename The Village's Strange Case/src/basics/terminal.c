/************************************************************************/
/*                                                                      */
/* Descripción: Funciones necesarias para trabajar con la terminal      */
/*                                                                      */
/* Archivo: terminal.c                                                  */
/* Autor: Error 404                                                     */
/*                                                                      */
/************************************************************************/

#include "terminal.h"

/**
 * @brief Inicializa la terminal con todas las banderas necesarias.
 */
void term_init(){
  struct termios new;

  tcgetattr(fileno(stdin), &initial);
  new = initial;
  new.c_lflag &= ~ICANON;
  new.c_lflag &= ~ECHO;
  new.c_cc[VMIN] = 1;
  new.c_cc[VTIME] = 0;
  new.c_lflag &= ~ISIG;
  tcsetattr(fileno(stdin), TCSANOW, &new);

  term_clear();
  term_set_size(345, 600);
  term_hide_cursor();
}

/**
 * @brief Termina el uso de la terminal.
 */
void term_end(){
  term_clear();
  term_move_to(1,1);
  term_show_cursor();
  fprintf(stdout, "%c[0", 27);
  fflush(stdout);

	tcsetattr(fileno(stdin), TCSANOW, &initial);
}

/**
 * @brief Limpia la terminal.
 */
void term_clear(){
  fprintf(stdout, "%c[2J", 27);
  fflush(stdout);
}

/**
 * @brief Establece el tamaño indicado a la terminal del usuario.
 * 
 * @param height Altura de la terminal.
 * @param width Anchura de la terminal.
 */
void term_set_size(int height, int width){
  fprintf(stdout, "%c[8;%d;%dt", 27, height, width);
  fflush(stdout);
}

/**
 * @brief Mueve el cursor a la posición indicada.
 * 
 * @param row Fila 
 * @param col Columna
 * 
 */
void term_move_to(int row, int col){
  fprintf(stdout, "%c[%d;%df", 27, row, col);
  fflush(stdout);
}

/**
 * @brief Oculta el cursor.
 */
void term_hide_cursor(){
  fprintf(stdout, "%c[?25l", 27);
  fflush(stdout);
}

/**
 * @brief Muestra el cursor.
 */
void term_show_cursor(){
  fprintf(stdout, "%c[?25h", 27);
  fflush(stdout);
}