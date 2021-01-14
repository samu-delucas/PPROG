/*
(C) Gregorio Blázquez, 2020
----------------------------------------------------------------------------
Minijuego Birds.
----------------------------------------------------------------------------
*/

#ifndef INTERFACE_H
#define INTERFACE_H
 
#include <rectangles.h>  


#define BACKGROUND   40  /* Black background */
#define FOREGROUND   37  /* White foreground */

#define NORTH    0
#define SOUTH    1
#define EAST     2
#define WEST     3
#define HERE     4



typedef struct {
  char *start;              /* Start prhase */
  int r_no;                 /* Number of rows in the interface */
  int c_no;                 /* Number of columns in the interface */
  int field_cols;           /* Number of columns of the playing field */
  sc_rectangle *field;      /* rectangle structure for the playing field */
  sc_rectangle *board;      /* rectangle structure for the score board */
  int map_rows;             /* Number of rows of the maps (map_rows <= r_no-2) */
  int map_cols;             /* Number of column of the maps (map_cols <= fields_cols) */
  char **map;               /* Map as drawn in the interface */
  char player;              /* Character used to represent the player */
  int  p_row, p_col;        /* Current position of the player */
  char *score_caption;      /* Caption used in the score window */
  int  r_cap, c_cap;        /* Position of the score caption in the score window */
  int  r_score, c_score;    /* Position of the score in the score window */
  int  score;               /* The current score                         */
  FILE *f;                  /* File with the map */
  int numMax;               /* Max Number of lines to read */
} intrf;


intrf *bird_i_create(int rows, int cols, int field);
int bird_i_set_score_caption(intrf *itf, char *caption, int r_cap, int c_cap, int r_score, int c_score);
int bird_i_set_play_data(intrf *itf, char player, int rp, int cp);
int bird_i_set_board(intrf *itf, int br, int bc, char **board);
int bird_i_draw_board(intrf *itf, int clear);
int bird_i_set_score(intrf *itf, int score);
int bird_i_move(intrf *itf, int dir);

#endif
