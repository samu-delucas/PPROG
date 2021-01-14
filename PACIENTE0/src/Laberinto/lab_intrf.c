/*------------------------------------------------------------------
   Project:  Laberinto
   File:     lab_intrf.c
   Rev.      1.0
   Date:     Nov, 2020

   (C) Alberto Rueda, 2020

  -------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>    
#include <stdio.h>    
#include <malloc.h>    
#include <string.h>    
#include <lab_intrf.h>

/*
  Redraws the contours of the interface without touching the contents
  of the inside of the windows
*/
void lab_i_redraw(intrf *q) {
  int k;

  // Draw the top row
  fprintf(stdout, "%c[1;1H", 27); // Move at the top/left of the screen (note: (1,1), and not (0,0)
  fprintf(stdout,"_");
  fflush(stdout);

  k = 0;  // Current column for printing
  for (; k<q->field_cols-3; k++)
    fprintf(stdout,"_");
  fprintf(stdout,"_");
  fflush(stdout);
  k++;
  for (; k<q->c_no-1; k++)
    fprintf(stdout,"_");
  fprintf(stdout,"_");
  fflush(stdout);

  // Draw the bottom row
  fprintf(stdout, "%c[%d;1H", 27, q->r_no); // Move at the bottom/left of the screen
  fprintf(stdout,"|");
  fflush(stdout);

  k = 0;  // Current column for printing
  for (; k<q->field_cols-3; k++)
    fprintf(stdout,"_");
  fprintf(stdout,"_");
  fflush(stdout);
  k++;
  for (; k<q->c_no-2; k++)
    fprintf(stdout,"_");
  fprintf(stdout,"|");
  fflush(stdout); 


  //Draw the vertical lines
  for (int r=2; r<q->r_no; r++) {
    fprintf(stdout, "%c[%d;1H", 27, r); 
    fprintf(stdout,"|");
    fprintf(stdout, "%c[%d;%dH", 27, r, q->field_cols); 
    fprintf(stdout,"|");
    fprintf(stdout, "%c[%d;%dH", 27, r, q->c_no); 
    fprintf(stdout,"|");
  }
  fflush(stdout);
  return;
}

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
intrf *lab_i_create(int rows, int cols, int field) {
  if (rows<=0 || cols<=0 || field<=0 || field>cols-6)
    return NULL;

  intrf *q = (intrf *) malloc(sizeof(intrf));
  q->field = lab_win_new(1, 1, rows-2, field, BACKGROUND, FOREGROUND);
  q->board = lab_win_new(1, field+2, rows-2, cols-2, BACKGROUND, FOREGROUND);
  q->r_no = rows;
  q->c_no = cols;
  q->field_cols = field;
  q->player = 0;

  lab_i_redraw(q);

  return q;
  
}

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
int lab_i_set_play_data(intrf *itf, char player, int rp, int cp) {
  if (rp < 0 || rp > itf->r_no || cp < 0 || cp >= itf->field_cols) return 0;

  itf->player = player;
  itf->p_row = rp;
  itf->p_col = cp;

  return 1;
}

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
void lab_i_set_board(intrf *itf, int br, int bc, char **board) {
  if (br < 0 || br > itf->r_no || bc < 0 || bc  >= itf->field_cols) return;  

  itf->map_rows = br;
  itf->map_cols = bc;
  itf->map = board; 
}

/*
  int i_draw_board(intrf *itf, int clear);

 
  Draws the board, with the player and all the pellets. 
  If clear=0 will draw the board without clearing the window; if clear=1
  it will clear the window and then draw the board
*/
int lab_i_draw_board(intrf *itf, int clear) {
  if (clear)
    lab_win_cls(itf->field);
  
  for (int i=0; i<itf->map_rows; i++) {
    lab_win_write_line_at(itf->field, i, 0, itf->map[i]);
  }
  

  lab_win_write_char_at(itf->field, itf->p_row, itf->p_col, itf->player);
  printf("%c[%d;%dH", 27, itf->ckrow+2, itf->ckcol);
  printf("\x1b[38;5;255m");
  fflush(stdout);
  fprintf(stdout, "Controles:");
  fflush(stdout);
  printf("%c[%d;%dH", 27, itf->ckrow+3, itf->ckcol);
  fflush(stdout);
  fprintf(stdout, "-Flechas para moverse.");
  fflush(stdout);
  printf("%c[%d;%dH", 27, itf->ckrow+4, itf->ckcol);
  fflush(stdout);
  fprintf(stdout, "-ESP para volver al inicio.");
  fflush(stdout);
  printf("%c[%d;%dH", 27, itf->ckrow+5, itf->ckcol);
  fflush(stdout);
  fprintf(stdout, "-Q/q para salir.");
  fflush(stdout);

  return 1;
}

/*
  Moves the player in a given direction

  Note that there are five directions: NORTH, SOUTH, EAST, WEST, STILL
  STILL will not move the player, but it will redraw it.

  Returns:
  256*r + c > 0  new encoded position of the player
  -1             player can't move in the given direction
  -2             finds the exit
*/
static int Dr[5] = {-1, 1, 0, 0, 0};
static int Dc[5] = {0, 0, 1, -1, 0}; 
int lab_i_move(intrf *itf, int dir) {
  if (dir < 0 || dir > 4) return -1;

  int r = itf->p_row + Dr[dir];
  int c = itf->p_col + Dc[dir];
  if (r<0 || c<0 || r >= itf->r_no || c >= itf->field_cols) return -1;
  if (itf->map[r][c] == 'S') return -2;
  if (itf->map[r][c] != ' ') return -1;
  lab_win_write_char_at(itf->field, itf->p_row, itf->p_col, ' ');
  lab_win_write_char_at(itf->field, itf->p_row = r, itf->p_col = c, itf->player);
  return (r<<8) + c;
}

/*
  Redraws the caption window. This function erases the current score.
*/
void lab_cap_redraw(intrf *itf) {
  lab_win_bgcol(itf->board, BACKGROUND);
  lab_win_fgcol(itf->board, FOREGROUND);

  if (itf->clock_caption) {
    printf("%c[%d;%dH", 27, itf->ckrow, itf->ckcol);
    fflush(stdout);
    fprintf(stdout, "%s", itf->clock_caption);
    fflush(stdout);
    lab_i_set_time(itf, 0);
  }
  return;
}

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
int lab_i_set_clock_pars(intrf *itf, char *clock_caption, int row, int col, int max_time) {
  itf->clock_caption = clock_caption;
  itf->ckrow = row;
  itf->ckcol = col;
  itf->max_time = max_time;
  itf->elapsed = -1;

  lab_cap_redraw(itf);

  return 1;
}

/*
  Sets the elapsed time and returns an indication of whether the game
  has ended or not.

  The function receives an integer indicating the number of seconds
  elapsed from the beginning of the game. If this is different from
  "elapsed", the function updates and prints the clock with the number
  of remaining seconds.

  If the count-down of the remaining time has reached zero, the
  function returns 0, indicating that the game is over, otherwise
  returns the remaining time
*/
int lab_i_set_time(intrf *itf, int elapsed) {
  int seconds, minutes;
  char buf[20];
  if (itf->elapsed==elapsed){
    return 1;
  }
  itf->elapsed = elapsed;
  int remaining = itf->max_time - itf->elapsed;
  remaining = (remaining < 0) ? 0 : remaining;

  seconds=remaining%60;
  minutes=remaining/60;
  buf[0]='0'+minutes/10;
  buf[1]='0'+minutes%10;
  buf[2]=':';
  buf[3]='0'+seconds/10;
  buf[4]='0'+seconds%10;
  buf[5]=0;
  
  printf("%c[%d;%dH", 27, 4, 90);
  fflush(stdout);
  fprintf(stdout, "%s", buf);
  fflush(stdout);
  return remaining;
}

/*
  Frees all the memory allocation in itf

  Parameters:
    itf: pointer to the structure we want to free

*/
void lab_free(intrf *itf){
  if(itf){
    free(itf->clock_caption);
    for (int i=0; i<itf->map_rows; i++) {
      free(itf->map[i]);
    }
    free(itf->map);
    lab_win_delete(itf->field);
    lab_win_delete(itf->board);
    free(itf);
  }
}