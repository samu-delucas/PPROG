/*
(C) Gregorio Blázquez, 2020
------------------------------------------------------------------
Interfaz del minijuego Birds

La definicion de la estructura interfaz esta en el .h
Contiene algunas funciones básicas para el minijuego Birds.
--------------------------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>    
#include <stdio.h>    
#include <malloc.h>    
#include <string.h>
    
#include <birds_intrf.h>


/*
  Redraws the caption window. This function erases the current score.
*/
void bird_cap_redraw(intrf *itf) {
  win_bgcol(itf->board, BACKGROUND);
  win_fgcol(itf->board, FOREGROUND);
  win_cls(itf->board);

  if (itf->score_caption) {
    win_write_line_at(itf->board, itf->r_cap, itf->c_cap, itf->score_caption);
    bird_i_set_score(itf, itf->score);
  }
  return;
}


/*

*/
intrf *bird_i_create(int rows, int cols, int field) {
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
  q->numMax=0;
  q->map=NULL;

  return q;
  
}

/*
  
*/
int bird_i_set_score_caption(intrf *itf, char *caption, int r_cap, int c_cap, int r_score, int c_score) {
  if (r_cap<0 || r_cap >= itf->r_no || c_cap<0 || c_cap >= itf->c_no)  return 0;
  if (r_score<0 || r_score>=itf->r_no || c_score<0 || c_score >= itf->c_no)  return 0;

  itf->score_caption = caption;
  itf->r_cap = r_cap;
  itf->c_cap = c_cap;
  itf->r_score = r_score;
  itf->c_score = c_score;

  bird_cap_redraw(itf);
  win_cls(itf->field);


  fflush(stdout);
  return 1;
}



/*
  
*/
int bird_i_set_play_data(intrf *itf, char player, int rp, int cp) {
  if (rp < 0 || rp > itf->r_no || cp < 0 || cp >= itf->field_cols) return 0;

  itf->player = player;
  itf->p_row = rp;
  itf->p_col = cp;

  return 1;
}


/*
  
*/
int bird_i_set_board(intrf *itf, int br, int bc, char **board) {
  if (br < 0 || br > itf->r_no || bc < 0 || bc  >= itf->field_cols) return 0;  

  itf->map_rows = br;
  itf->map_cols = bc;
  itf->map = board; 
}


/*
  Sets the score to a given value, and prints it

  Returns the score
*/
int bird_i_set_score(intrf *itf, int score) {
  char buf[10];
  itf->score = score;
  win_write_line_at(itf->board, itf->r_score, itf->c_score, "    ");  
  sprintf(buf, "%3d", itf->score);
  win_write_line_at(itf->board, itf->r_score, itf->c_score, buf);  

  return itf->score;
}


/*
  Moves the player in a given direction
*/
static int Dr[5] = {-1, 1, 0, 0, 0};
static int Dc[5] = {0, 0, 1, -1, 0}; 
int bird_i_move(intrf *itf, int dir) {
  int ret=0;
  if (dir < 0 || dir > 4) return 0;

  int r = itf->p_row + Dr[dir];
  int c = itf->p_col + Dc[dir];
  if (r<0 || c<0 || r >= itf->r_no || c >= itf->field_cols) return 0;
  if (itf->map[r][c] == '|') ret=-1;
  if(itf->map[r][c] == 'X') {
    ret=1;
    itf->map[r][c] = ' ';
  }
  win_write_char_at(itf->field, itf->p_row, itf->p_col, ' ');
  win_write_char_at(itf->field, itf->p_row = r, itf->p_col = c, itf->player);
  return ret;
}
