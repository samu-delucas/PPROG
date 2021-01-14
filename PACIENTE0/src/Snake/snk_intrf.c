/*------------------------------------------------------------------
   Project:  Snake
   File:     snk_intrf.c
   Rev.      1.0
   Date:     Dic, 2020

   (C) Alberto Rueda, 2020

  -------------------------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>    
#include <stdio.h>    
#include <malloc.h>    
#include <string.h>    
#include "snk_intrf.h"


/*
  Redraws the contours of the interface without touching the contents
  of the inside of the windows
*/
void snk_i_redraw(intrf *q) {
  int k;

  // Draw the top row
  fprintf(stdout, "%c[1;1H", 27); // Move at the top/left of the screen (note: (1,1), and not (0,0)
  fprintf(stdout,"_");

  k = 0;  // Current column for printing
  for (; k<q->field_cols; k++)
    fprintf(stdout,"_");
  fprintf(stdout,"_");
  k++;
  for (; k<q->c_no-1; k++)
    fprintf(stdout,"_");
  fprintf(stdout,"_");

  k = 0;  // Current column for printing
  for (; k<q->field_cols; k++)
    fprintf(stdout,"_");
  fprintf(stdout,"_");
  k++;
  for (; k<q->c_no-1; k++)
    fprintf(stdout,"_");
  fprintf(stdout,"|");


  //Draw the vertical lines
  for (int r=2; r<q->r_no; r++) {
    fprintf(stdout, "%c[%d;1H", 27, r); 
    fprintf(stdout,"|");
    fprintf(stdout, "%c[%d;%dH", 27, r, q->field_cols+2); 
    fprintf(stdout,"|");
  }
  return;
}


/*
  Redraws the caption window. This function erases the current score.
*/
void snk_cap_redraw(intrf *itf) {
  snk_win_bgcol(itf->board, BACKGROUND);
  snk_win_fgcol(itf->board, FOREGROUND);
  snk_win_cls(itf->board);

  if (itf->score_caption) {
    snk_win_write_line_at(itf->board, itf->r_cap, itf->c_cap, itf->score_caption);
    snk_i_set_score(itf, 0);
  }
  if (itf->clock_caption) {
    snk_win_write_line_at(itf->board, itf->ckrow, itf->ckcol, itf->clock_caption);
    snk_i_set_time(itf, 0);
  }
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
intrf *snk_i_create(int rows, int cols, int field) {
  if (rows<=0 || cols<=0 || field<=0 || field>cols-6)
    return NULL;

  intrf *q = (intrf *) malloc(sizeof(intrf));
  q->field = snk_win_new(1, 1, rows-2, field, BACKGROUND, FOREGROUND);
  q->board = snk_win_new(1, field+2, rows-2, cols-field-2, BACKGROUND, FOREGROUND);
  q->r_no = rows;
  q->c_no = cols;
  q->field_cols = field;
  q->tam = 0;
  q->score = 0;
  q->pellet_no = q->pellet_space = 0;
  q->game_over = 0;

  snk_i_redraw(q);

  return q;
  
}

/*
  int i_set_score_caption(intrf *itf, char *caption, int r_cap, int c_cap, int r_score, int c_score)

  Sets the score window parameters of teh interface and redraws it
  (just the score window)

  Parameters
  itf:               the interface which we change the parameters of
  caption:           the caption in the score window. Note: the caption is referred to in the 
                     interface, it is NOT copied. The parameter should not be freed.
  r_cap, c_cap:      position of the caption. Must be inside the window
  r_score, c_score:  position of the score. Must be inside the window

  Return:
    1 if all OK
    0 if some parameter error
*/
int snk_i_set_score_caption(intrf *itf, char *caption, int r_cap, int c_cap, int r_score, int c_score) {
  if (r_cap<0 || r_cap >= itf->r_no || c_cap<0 || c_cap >= itf->c_no)  return 0;
  if (r_score<0 || r_score>=itf->r_no || c_score<0 || c_score >= itf->c_no)  return 0;

  itf->score_caption = caption;
  itf->r_cap = r_cap;
  itf->c_cap = c_cap;
  itf->r_score = r_score;
  itf->c_score = c_score;

  snk_cap_redraw(itf);
  snk_win_cls(itf->field);

  fflush(stdout);
  return 1;
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
int snk_i_set_clock_pars(intrf *itf, char *caption, int row, int col, int max_time) {
  itf->ckrow = row;
  itf->ckcol = col;
  itf->clock_caption = caption;
  itf->max_time = max_time;
  itf->elapsed = -1;

  snk_cap_redraw(itf);

  fflush(stdout);
  return 1;
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
int snk_i_set_play_data(intrf *itf, char head, char body, int tam, int dir, int rp, int cp, char pellet, int pelno) {
  if (rp < 0 || rp > itf->r_no || cp < 0 || cp >= itf->field_cols) return 0;

  itf->snake[0].image = head;
  for(int i=1; i<MAX_TAM; i++){
    itf->snake[i].image = body;
  }
  itf->snake[0].x = rp;
  itf->snake[0].y = cp;
  for(int i=1; i<MAX_TAM; i++){
    itf->snake[i].x = itf->snake[i-1].x;
    itf->snake[i].y = itf->snake[i-1].y-1;
  }
  itf->tam = tam;
  for(int i=0; i<MAX_TAM; i++){
    itf->snake[i].dir = dir;
  }
  
  itf->pellet_char = pellet;
  itf->pellet_space = pelno;
  itf->pel_row = (int *) malloc(pelno*sizeof(int));
  itf->pel_col = (int *) malloc(pelno*sizeof(int));
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
int snk_i_set_board(intrf *itf, int br, int bc, char **board) {
  if (br < 0 || br > itf->r_no || bc < 0 || bc  >= itf->field_cols) return 0;  

  itf->map_rows = br;
  itf->map_cols = bc;
  itf->map = board;
  return 1; 
}


/*
  i_add_pellet(intrf *itf, int r, int c)

  Adds a pellet in a given position of the board

  Parameters
  itf:               the interface which we add the pellet to
  int r, c:          the position at which we add the pellet

  Returns
  n > 0              number of pellets currently on the board
  n < 0              the board contains -n pellets, and is full
  0                  the place at which the pellet should be added is not a free space
*/
int  snk_i_add_pellet(intrf *itf, int r, int c) {
  if (itf->pellet_no >= itf->pellet_space) return -itf->pellet_no;
  if (r<0 || r >= itf->map_rows || c<0 || c >= itf->map_cols) return 0;
  if (itf->map[r][c] != ' ')  return 0;

  itf->map[r][c]=itf->pellet_char;
  snk_win_write_char_at(itf->field, r, c, itf->pellet_char);

  return ++itf->pellet_no;
}


/*
  i_remove_pellet(intrf *itf, int r, int c)

  Removes a pellet given its position on board

  Parameters
  itf:               the interface which we remove the pellet from
  int r, c:          the position at which the pellet is located

  Returns
  n >= 0              number of pellets remaining on the board
  -1                  there is no pellet at the given location               
*/
int snk_i_remove_pellet(intrf *itf, int r, int c) {
  if (r<0 || r >= itf->map_rows || c<0 || c >= itf->map_cols) return 0;
  
  itf->map[r][c]=' ';
  snk_win_write_char_at(itf->field, r, c, ' ');
  
  return --itf->pellet_no;
}


/*
  int i_draw_board(intrf *itf, int clear);

 
  Draws the board, with the player and all the pellets. 
  If clear=0 will draw the board without clearing the window; if clear=1
  it will clear the window and then draw the board
*/
int snk_i_draw_board(intrf *itf, int clear) {
  if (clear)
    snk_win_cls(itf->field);
  
  for (int i=0; i<itf->map_rows; i++) {
    snk_win_write_line_at(itf->field, i, 0, itf->map[i]);
  }
  
  for (int i=0; i<itf->pellet_no; i++) {
    snk_win_write_char_at(itf->field, itf->pel_row[i], itf->pel_col[i], itf->pellet_char);
  }

  snk_win_write_char_at(itf->field, itf->snake[0].x, itf->snake[0].y, itf->snake[0].image);
  for(int i=1; i<=itf->tam; i++){
    snk_win_write_char_at(itf->field, itf->snake[0].x, itf->snake[0].y-i, itf->snake[i].image);
  }
  snk_win_write_line_at(itf->board, 5, 1, "Controles:");
  fflush(stdout);
  snk_win_write_line_at(itf->board, 6, 1, "-Flechas para moverse.");
  fflush(stdout);
  snk_win_write_line_at(itf->board, 7, 1, "-Q/q para salir.");
  fflush(stdout);
  return 1;
}


/*
  Sets the score to a given value, and prints it

  Returns the score
*/
int snk_i_set_score(intrf *itf, int score) {
  char buf[10];
  itf->score = score;
  snk_win_write_line_at(itf->board, itf->r_score, itf->c_score, "    ");  
  sprintf(buf, "%3d", itf->score);
  snk_win_write_line_at(itf->board, itf->r_score, itf->c_score, buf);  

  return itf->score;
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
int snk_i_set_time(intrf *itf, int elapsed) {
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
  
  printf("%c[%d;%dH", 27, 4, 82);
  fflush(stdout);
  fprintf(stdout, "%s", buf);
  fflush(stdout);
  return remaining;
}

/*
  Moves the player in a given direction

  Note that there are five directions: NORTH, SOUTH, EAST, WEST, STILL
  STILL will not move the player, but it will redraw it.

  Returns:
  256*r + c > 0  new encoded position of the player
  -1             player can't move in the given direction
*/
static int Dr[5] = {-1, 1, 0, 0, 0};
static int Dc[5] = {0, 0, 1, -1, 0}; 
int snk_i_move(intrf *itf) {
  int r = itf->snake[0].x + Dr[itf->snake[0].dir];
  int c = itf->snake[0].y + Dc[itf->snake[0].dir];
  int rf = itf->snake[itf->tam-1].x;
  int cf = itf->snake[itf->tam-1].y;
  if (r<0 || c<0 || r >= itf->r_no || c >= itf->field_cols) return -1;
  if (itf->map[r][c] == '=' || itf->map[r][c] == '|' || itf->map[r][c] == '-' || itf->map[r][c] == '+') {
    itf->game_over=1;
    return -2;
  }
  for(int i=1; i<itf->tam; i++){
    if(r == itf->snake[i].x && c == itf->snake[i].y){
      itf->game_over=1;
      return -2;
    }
  }
  if (itf->map[r][c] != ' ' && itf->map[r][c] != itf->pellet_char){
    return -1;
  }
  for(int i=itf->tam-1; i>0; i--){
    itf->snake[i].x = itf->snake[i-1].x;
    itf->snake[i].y = itf->snake[i-1].y;
  }
  itf->snake[0].x = r;
  itf->snake[0].y = c;
  for(int i=0; i<itf->tam; i++){
    snk_win_write_char_at(itf->field, itf->snake[i].x, itf->snake[i].y, itf->snake[i].image);
  }
  snk_win_write_char_at(itf->field, rf, cf, ' ');
  return (r<<8) + c;
}


/*
  Returns true if the player is on a pellet
*/
int snk_i_is_on_pellet(intrf *itf) {
  if(itf->map[itf->snake[0].x][itf->snake[0].y] == itf->pellet_char){
    return 1;
  }
  return 0;
}

/*
  Frees all the memory allocation in itf

  Parameters:
    itf: pointer to the structure we want to free

*/
void snk_free(intrf *itf){
  if(itf){
    free(itf->clock_caption);
    free(itf->score_caption);
    free(itf->pel_row);
    free(itf->pel_col);
    for (int i=0; i<itf->map_rows; i++) {
      free(itf->map[i]);
    }
    free(itf->map);
    snk_win_delete(itf->field);
    snk_win_delete(itf->board);
    free(itf);
  }
}