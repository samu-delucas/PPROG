/*------------------------------------------------------------------
   Project:  Snake
   File:     snake_intrf.h
   Rev.      1.0
   Date:     Dic, 2020

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
 
#include "snk_rectangles.h"  


#define BACKGROUND   40  /* Black background */
#define FOREGROUND   37  /* White foreground */
#define MAX_TAM      100 /* Maximum size of the snake*/

#define NORTH    0
#define SOUTH    1
#define EAST     2
#define WEST     3
#define HERE     4

typedef struct {
  int x,y;                  /* Coordinate of the part of the snake*/
  int dir;                  /* Direction of the part of the snake*/
  char image;               /* Character used to represent the part of the snake*/
} snk;

typedef struct {
  int r_no;                 /* Number of rows in the interface */
  int c_no;                 /* Number of columns in the interface */
  int field_cols;           /* Number of columns of the playing field */
  sc_rectangle *field;      /* rectangle structure for the playing field */
  sc_rectangle *board;      /* rectangle structure for the score board */
  int map_rows;             /* Number of rows of the maps (map_rows <= r_no-2) */
  int map_cols;             /* Number of column of the maps (map_cols <= fields_cols) */
  char **map;               /* Map as drawn in the interface */
  int tam;                  /* Size of the snake*/
  snk snake[MAX_TAM];       /* Snake*/
  char pellet_char;         /* Character used to represent a pellet */
  int  pellet_no;           /* Number of pellets  */
  int  pellet_space;        /* Space actually allocated for the pellet array */
  int  *pel_row, *pel_col;  /* Position of the pellets in the board */
  char *score_caption;      /* Caption used in the score window */
  int  r_cap, c_cap;        /* Position of the score caption in the score window */
  int  r_score, c_score;    /* Position of the score in the score window */
  int  score;               /* The current score                         */
  int  ckrow, ckcol;        /* Position of the clock in the score window */
  char *clock_caption;      /* Caption of the clock   */
  char max_time;            /* Max play time */
  int  elapsed;             /* Seconds elapsed so far  */
  int game_over;            /* Flag to see if the game is over*/
} intrf;


/*
  intrf *i_create(int rows, int cols, int field);

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
intrf *snk_i_create(int rows, int cols, int field);

/*
  int i_set_score_caption(intrf *itf, char *caption, int r_cap, int c_cap, int r_score, int c_score)

  Sets the score window parameters of teh interface and redraws it
  (just the score window)

  Parameters
  itf:               the interface which we change the parameters of
  caption:           the caption in the score window
  r_cap, c_cap:      position of the caption. Must be inside teh window
  r_score, c_score:  position of the score. Must be inside teh window

  Return:
    1 if all OK
    0 if some parameter error
*/
int snk_i_set_score_caption(intrf *itf, char *caption, int r_cap, int c_cap, int r_score, int c_score);


/*
  int i_set_clock_pars(intrf *itf, char *caption, int row, int col)

  Sets the clock parameters and redraws the score window

  Parameters:
  itf:               the interface which we change the parameters of
  caption:           the caption of the clock in the score window
  row, col:          position of the caption. Must be inside the window.
                     The clock will be displayed one row below, left-aligned to the caption
  max_time:          Maximum game time
*/
int snk_i_set_clock_pars(intrf *itf, char *caption, int row, int col, int max_time);



/*
  i_set_play_data(intrf *itf, char player, int rp, int cp, char pellet, int pelno);

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
int snk_i_set_play_data(intrf *itf, char head, char body, int tam, int dir, int rp, int cp, char pellet, int pelno);


/*
  i_set_board(intrf *itf, int br, int bc, char **board) 

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
int snk_i_set_board(intrf *itf, int br, int bc, char **board);


/*
  i_add_pellet(intrf *itf, int r, int c)

  Adds a pellet in a given position of the board

  Parameters
  itf:               the interface which we add the pellet to
  int r, c:          the position at which we add the pellet

  Returns
  n > 0              number of pellets currently on the board
  n < 0              the board contains -n pellets, and is full
  0                  the place at which the pellet should be added is not a free spacee
*/
int snk_i_add_pellet(intrf *itf, int r, int c);


/*
  i_remove_pellet(intrf *itf, int r, int c)

  Removes a pellet given its position on board

  Parameters
  itf:               the interface which we remove the pellet from
  int r, c:          the position at which the pellet is located

  Returns
  n >= 0              number of pellets remaining on the board
  -1                 there is no pellet at the given location               
*/
int snk_i_remove_pellet(intrf *itf, int r, int c);

/*
  int i_draw_board(intrf *itf);

  Draws the board, with the player and all the pellets
*/
int snk_i_draw_board(intrf *itf, int clear);

/*
  Sets the score to a given value, and prints it

  Returns the score
*/
int snk_i_set_score(intrf *itf, int score);


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
int snk_i_set_time(intrf *itf, int elapsed);


/*
  Moves the player in a given direction

  Note that there are five directions: NORTH, SOUTH, EAST, WEST, STILL
  STILL will not move the player, but it will redraw it.

  Returns:
  256*r + c > 0  new encoded position of the player
  -1             player can't move in the given direction
  -2             game over
*/
int snk_i_move(intrf *itf);


/*
  Returns true if the player is on a pellet
*/
int snk_i_is_on_pellet(intrf *itf);

/*
  Frees all the memory allocation in itf

  Parameters:
    itf: pointer to the structure we want to free

*/
void snk_free(intrf *itf);

#endif
