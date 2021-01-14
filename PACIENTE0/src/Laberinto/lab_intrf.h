/*------------------------------------------------------------------
   Project:  Laberinto
   File:     lab_intrf.h
   Rev.      1.0
   Date:     Nov, 2020

   (C) Alberto Rueda, 2020

    +-----------------------------+----------+
    |                             |          |
    |                             |          |
    |                             |          |
    |                             |          |
    |                             |          |
    |                             |          |
    |                             |          |
    |                             |          |
    |                             |          |
    |                             |          |
    +-----------------------------+----------+

    Funciones que facilitan el desarrollo del juego así como 
    crear la interfaz y su elementos e interactuar con ellos

  -------------------------------------------------------------------
*/

#ifndef INTERFACE_H
#define INTERFACE_H
 
#include <lab_mapa.h>  
#include <lineread.h>


#define BACKGROUND   40  /* Black background */
#define FOREGROUND   32  /* Green foreground... for a "retro" look */

#define NORTH    0
#define SOUTH    1
#define EAST     2
#define WEST     3
#define HERE     4


typedef struct {
  int r_no;                 /* Number of rows in the interface                        */
  int c_no;                 /* Number of columns in the interface                     */
  int field_cols;           /* Number of columns of the playing field                 */
  sc_rectangle *field;      /* rectangle structure for the playing field              */
  sc_rectangle *board;      /* rectangle structure for the score board                */
  int map_rows;             /* Number of rows of the maps (map_rows <= r_no-2)        */
  int map_cols;             /* Number of column of the maps (map_cols <= fields_cols) */
  char **map;               /* Map as drawn in the interface                          */
  char player;              /* Character used to represent the player                 */
  int  p_row, p_col;        /* Current position of the player                         */
  int  ckrow, ckcol;        /* Position of the clock in the score window */
  char *clock_caption;      /* Caption of the clock   */
  char max_time;            /* Max play time */
  int  elapsed;             /* Seconds elapsed so far  */
} intrf;

/*
  lab_i_set_board(intrf *itf, int br, int bc, char **board) 

  Sets the board data

  Parameters
  itf:               the interface which we change the parameters of
  br:                number of rows in the board map
  bc:                number of columns in the board map
  board:             brxbc array with the board.
                     This array will be referred to directly, it will NOT be copied.

  Return:
    1 if all OK
    0 if some parameter error
*/
void lab_i_set_board(intrf *itf, int br, int bc, char **board);

/*
  lab_i_set_play_data(intrf *itf, char player, int rp, int cp, char pellet, int pelno);

  Sets the player and pellet data for the interface.

  Parameters
  itf:               the interface which we change the parameters of
  player:            the character to be used to represente the player
  rp, cp:            initial position of the player. WARNING: no check is made as to 
                     the validity of this position inside the board
  pellet:            character used to represent the pellets
  pelno:             maximum number of pellets that can be present at one time

  Return:
    1 if all OK
    0 if some parameter error
*/
int lab_i_set_play_data(intrf *itf, char player, int rp, int cp);

/*
  intrf *lab_i_create(int rows, int cols, int field);

  Creates an interface with given dimensions. The interface is left
  empty.

  Parameters:
     rows:  number of rows of the interface
     cols:  number of cols of the interface
     (these two values include the border: the usable portion of the 
     interface is (rows-2)x(cols-2))
     field: number of *usable* columns of the field (field<cols-6)

  Returns:
     A pointer to a newly allocated intrf structure. Moreover, this function
     clears the screen and draws the borders of the interface.

     NULL if there was some parameter inconsistency.
*/
intrf *lab_i_create(int rows, int cols, int field);

/*
  int lab_i_draw_board(intrf *itf);

  Draws the board, with the player and all the pellets
*/
int lab_i_draw_board(intrf *itf, int clear);

/*
  Redraws the contours of the interface without touching the contents
  of the inside of the windows
*/
void lab_i_redraw(intrf *q);

/*
  Moves the player in a given direction

  Note that there are five directions: NORTH, SOUTH, EAST, WEST, STILL
  STILL will not move the player, but it will redraw it.

  Returns:
  256*r + c > 0  new encoded position of the player
  -1             player can't move in the given direction
  -2             finds the exit
*/
int lab_i_move(intrf *itf, int dir);

/*
  int lab_i_set_clock_pars(intrf *itf, char *caption, int row, int col)

  Sets the clock parameters and redraws the score window

  Parameters:
  itf:               the interface which we change the parameters of
  caption:           the caption of the clock in the score window
  row, col:          position of the caption. Must be inside the window.
                     The clock will be displayed one row below, left-aligned to the caption
  max_time:          Maximum game time
*/
int lab_i_set_clock_pars(intrf *itf, char *clock_caption, int row, int col, int max_time);

/*
  Sets the elapsed time and returns an indication of whether the game
  has ended or not.

  The function receives an integer indicating the number of seconds
  elapsed from the beginning of the game. If this is different from
  "elapsed", the function updates and prints the clock with the number
  of remaining seconds.

  If the count-down of the remaining time has reached zero, the
  function returns 1, indicating that the game is over, otherwise
  returns zero
*/
int lab_i_set_time(intrf *itf, int elapsed);

/*
  Frees all the memory allocation in itf

  Parameters:
    itf: pointer to the structure we want to free

*/
void lab_free(intrf *itf);

#endif