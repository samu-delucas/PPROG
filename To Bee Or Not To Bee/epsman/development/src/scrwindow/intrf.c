/*------------------------------------------------------------------
   Project:  Interactive game demo
   File:     intrf.h
   Rev.      2.0
   Date:     Oct. 1, 2015


   Revisions:
   1.0       Oct. 1, 2015

   (C) Simone Santini, 2015

  -------------------------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>    
#include <stdio.h>    
#include <malloc.h>    
#include <string.h>    
#include <intrf.h>


/*
  Redraws the contours of the interface without touching the contents
  of the inside of the windows
*/
void _i_redraw(intrf *q) {
  int k;

  // Draw the top row
  fprintf(stdout, "%c[1;1H", 27); // Move at the top/left of the screen (note: (1,1), and not (0,0)
  fprintf(stdout,"+");

  k = 0;  // Current column for printing
  for (; k<q->field_cols; k++)
    fprintf(stdout,"-");
  fprintf(stdout,"+");
  k++;
  for (; k<q->c_no-1; k++)
    fprintf(stdout,"-");
  fprintf(stdout,"+");

  // Draw the bottom row
  fprintf(stdout, "%c[%d;1H", 27, q->r_no); // Move at the bottom/left of the screen
  fprintf(stdout,"+");

  k = 0;  // Current column for printing
  for (; k<q->field_cols; k++)
    fprintf(stdout,"-");
  fprintf(stdout,"+");
  k++;
  for (; k<q->c_no-1; k++)
    fprintf(stdout,"-");
  fprintf(stdout,"+");


  //Draw the vertical lines
  for (int r=2; r<q->r_no; r++) {
    fprintf(stdout, "%c[%d;1H", 27, r); 
    fprintf(stdout,"|");
    fprintf(stdout, "%c[%d;%dH", 27, r, q->field_cols+2); 
    fprintf(stdout,"|");
    fprintf(stdout, "%c[%d;%dH", 27, r, q->c_no+1); 
    fprintf(stdout,"|");
  }
  return;
}


/*
  Redraws the caption window. This function erases the current score.
*/
void _cap_redraw(intrf *itf) {
  win_bgcol(itf->board, BACKGROUND);
  win_fgcol(itf->board, FOREGROUND);
  win_cls(itf->board);

  if (itf->score_caption) {
    win_write_line_at(itf->board, itf->r_cap, itf->c_cap, itf->score_caption);
    i_set_score(itf, itf->score);
  }
  if (itf->clock_caption) {
    win_write_line_at(itf->board, itf->ckrow, itf->ckcol, itf->clock_caption);
    i_set_time(itf, 0);
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
intrf *i_create(int rows, int cols, int field) {
  if (rows<=0 || cols<=0 || field<=0 || field>cols-6)
    return NULL;

  intrf *q = (intrf *) malloc(sizeof(intrf));
  q->field = win_new(1, 1, rows-2, field, BACKGROUND, FOREGROUND);
  q->board = win_new(1, field+2, rows-2, cols-field-2, BACKGROUND, FOREGROUND);
  q->r_no = rows;
  q->c_no = cols;
  q->field_cols = field;
  q->player = 0;
  q->score = 0;
  q->pellet_no = q->pellet_space = 0;

  fprintf(stdout, "%c[2J", 27);   // Clear the whole screen
  _i_redraw(q);

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
int i_set_score_caption(intrf *itf, char *caption, int r_cap, int c_cap, int r_score, int c_score) {
  if (r_cap<0 || r_cap >= itf->r_no || c_cap<0 || c_cap >= itf->c_no)  return 0;
  if (r_score<0 || r_score>=itf->r_no || c_score<0 || c_score >= itf->c_no)  return 0;

  itf->score_caption = caption;
  itf->r_cap = r_cap;
  itf->c_cap = c_cap;
  itf->r_score = r_score;
  itf->c_score = c_score;

  _cap_redraw(itf);
  win_cls(itf->field);


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
int i_set_clock_pars(intrf *itf, char *caption, int row, int col, int max_time) {
  itf->ckrow = row;
  itf->ckcol = col;
  itf->clock_caption = caption;
  itf->max_time = max_time;
  itf->elapsed = -1;

  _cap_redraw(itf);

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
int i_set_play_data(intrf *itf, char player, int rp, int cp, char pellet, int pelno) {
  if (rp < 0 || rp > itf->r_no || cp < 0 || cp >= itf->field_cols) return 0;

  itf->player = player;
  itf->p_row = rp;
  itf->p_col = cp;

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
int i_set_board(intrf *itf, int br, int bc, char **board) {
  if (br < 0 || br > itf->r_no || bc < 0 || bc  >= itf->field_cols) return 0;  

  itf->map_rows = br;
  itf->map_cols = bc;
  itf->map = board; 
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
int  i_add_pellet(intrf *itf, int r, int c) {
  if (itf->pellet_no >= itf->pellet_space) return -itf->pellet_no;
  if (r<0 || r >= itf->map_rows || c<0 || c >= itf->map_cols) return 0;
  if (itf->map[r][c] != ' ')  return 0;
  for (int i=0; i<itf->pellet_no; i++) {
    if (itf->pel_row[i] == r && itf->pel_col[i] == c) 
      return 0;
  }

  itf->pel_row[itf->pellet_no] = r;
  itf->pel_col[itf->pellet_no] = c;
  win_write_char_at(itf->field, r, c, itf->pellet_char);

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
  -1                 there is no pellet at the given location               
*/
int i_remove_pellet(intrf *itf, int r, int c) {
  if (r<0 || r >= itf->map_rows || c<0 || c >= itf->map_cols) return 0;
  
  for (int i=0; i<itf->pellet_no; i++) {
    if (itf->pel_row[i] == r && itf->pel_col[i] == c) {
      if (i < itf->pellet_no-1) {
	itf->pel_row[i] = r; 
	itf->pel_col[i] = c;
	win_write_char_at(itf->field, r, c, ' ');
      }
      /* Redraw the player: if it was on top of the pellet we have erased it */
      win_write_char_at(itf->field, itf->p_row, itf->p_col, itf->player);      
      return --itf->pellet_no;
    }
  }
  
  return -1;
}


/*
  int i_draw_board(intrf *itf, int clear);

 
  Draws the board, with the player and all the pellets. 
  If clear=0 will draw the board without clearing the window; if clear=1
  it will clear the window and then draw the board
*/
int i_draw_board(intrf *itf, int clear) {
  if (clear)
    win_cls(itf->field);
  
  for (int i=0; i<itf->map_rows; i++) {
    win_write_line_at(itf->field, i, 0, itf->map[i]);
  }
  
  for (int i=0; i<itf->pellet_no; i++) {
    win_write_char_at(itf->field, itf->pel_row[i], itf->pel_col[i], itf->pellet_char);
  }

  win_write_char_at(itf->field, itf->p_row, itf->p_col, itf->player);
  return 1;
}


/*
  Sets the score to a given value, and prints it

  Returns the score
*/
int i_set_score(intrf *itf, int score) {
  char buf[10];
  itf->score = score;
  win_write_line_at(itf->board, itf->r_score, itf->c_score, "    ");  
  sprintf(buf, "%3d", itf->score);
  win_write_line_at(itf->board, itf->r_score, itf->c_score, buf);  

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
int i_set_time(intrf *itf, int elapsed) {
  char buf[20];
  if (elapsed==itf->elapsed)
    return 1;
  itf->elapsed = elapsed;
  int remaining = itf->max_time - itf->elapsed;
  remaining = (remaining < 0) ? 0 : remaining;

  win_write_line_at(itf->board, itf->ckrow+1, itf->ckcol, "     ");  
  sprintf(buf, "%3d", remaining);
  win_write_line_at(itf->board, itf->ckrow+1, itf->ckcol, buf);  

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
int i_move(intrf *itf, int dir) {
  if (dir < 0 || dir > 4) return -1;

  int r = itf->p_row + Dr[dir];
  int c = itf->p_col + Dc[dir];
  if (r<0 || c<0 || r >= itf->r_no || c >= itf->field_cols) return -1;
  if (itf->map[r][c] != ' ') return -1;
  win_write_char_at(itf->field, itf->p_row, itf->p_col, ' ');
  win_write_char_at(itf->field, itf->p_row = r, itf->p_col = c, itf->player);
  return r<<8 + c;
}


/*
  Returns true if the player is on a pellet
*/
int i_is_on_pellet(intrf *itf) {
  for (int i=0; i<itf->pellet_no; i++) {
    if (itf->pel_row[i] == itf->p_row && itf->pel_col[i] == itf->p_col) 
      return 1;
  }
  return 0;
}
