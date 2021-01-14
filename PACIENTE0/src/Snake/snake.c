
/*------------------------------------------------------------------
   Project:  Snake
   File:     snake.c
   Rev.      1.0
   Date:     Nov, 2020

   (C) Alberto Rueda, 2020

  -------------------------------------------------------------------
*/


#include <stdio.h>
#include <stdlib.h>    
#include <stdio.h>    
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <snk_rectangles.h>
#include <snk_intrf.h>
#include <lineread.h>
#include <snk_qrnd.h>

char pellet;
int pellet_no;
int ppp = 0;

int snk_score = 0;

struct termios initial;

/*
  Structure to send the clock parameters to the function that manages
  the closk1
*/

typedef struct {
  time_t initial; // Initial time at which the function is first called
  intrf  *intrf;    // pointer to the interface where the clock is to be displayed
} clock_data;

pthread_mutex_t time_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t move_lock = PTHREAD_MUTEX_INITIALIZER;

/*
  Creates the interface reading the data from a text file (given as a
  parameter), and displays it. Returns the interface handle.

  Additionally, receives a pointer to an already allocated clock_data
  structure, and fills the field of the structure with the data read
  from the file.

  Vers. 2.0, Oct. 20, 2016
*/
intrf *snk_intrf_init(char *fname) {
  FILE *fp = fopen(fname, "r");

  char *buf;

  int rows = atoi(buf = fgetll(fp));  
  free(buf);
  int cols = atoi(buf = fgetll(fp));  
  free(buf);
  int field = atoi(buf = fgetll(fp));  
  free(buf);
  intrf *itf = snk_i_create(rows, cols, field);

  int ckrow = atoi(buf = fgetll(fp));  
  free(buf);
  int ckcol = atoi(buf = fgetll(fp));  
  free(buf);
  int max_time = atoi(buf = fgetll(fp));  
  free(buf);
  char *clock_caption = NULL;
  itf->clock_caption = NULL;
  clock_caption = fgetll(fp);

  char *caption = NULL;
  itf->score_caption = NULL;
  caption = fgetll(fp);
  int crow = atoi(buf = fgetll(fp));  
  free(buf);
  int ccol = atoi(buf = fgetll(fp));  
  free(buf);
  int srow = atoi(buf = fgetll(fp));  
  free(buf);
  int scol = atoi(buf = fgetll(fp));  
  free(buf);

  snk_i_set_score_caption(itf, caption, crow, ccol, srow, scol);
  snk_i_set_clock_pars(itf, clock_caption, ckrow, ckcol, max_time);
 
  char head = (buf = fgetll(fp))[0];  
  free(buf);
  char body = (buf = fgetll(fp))[0];  
  free(buf);
  int tam = atoi(buf = fgetll(fp));  
  free(buf);
  int dir = atoi(buf = fgetll(fp));  
  free(buf);
  int prow = atoi(buf = fgetll(fp));  
  free(buf);
  int pcol = atoi(buf = fgetll(fp));  
  free(buf);
  pellet = (buf = fgetll(fp))[0];  
  free(buf);
  pellet_no= atoi(buf = fgetll(fp));  
  free(buf);
  ppp= atoi(buf = fgetll(fp));  
  free(buf);
  
  snk_i_set_play_data(itf, head, body, tam, dir, prow, pcol, pellet, pellet_no);

  int bdrow = atoi(buf = fgetll(fp));  
  free(buf);
  int bdcol = atoi(buf = fgetll(fp));  
  free(buf);
  
  char **map = (char **) malloc(bdrow*sizeof(char *));
  for (int i=0; i<bdrow; i++) {
    map[i] = fgetll(fp);
  }
  snk_i_set_board(itf, bdrow, bdcol, map);

  fclose(fp);
  
  return itf;
}


/*
  Adds a pellet in a random position of the board in which there is a
  blank space
*/
int snk_add_pellet(intrf *itf) {
  rnd_state *rndgen = snk_r_init((long) time(NULL));
  while(1) {
    int r = (int) (snk_i_rnd(rndgen) % itf->map_rows);
    int c = (int) (snk_i_rnd(rndgen) % itf->map_cols);
    int q = snk_i_add_pellet(itf, r, c);
    if (q < 0) {
      snk_r_end(rndgen);
      return 0;
    }
    else if (q > 0) {
      snk_r_end(rndgen);
      return 1;      
    }
  }
}


/*
  Reads a key from the keyboard. If the key is a "regular" key it
  returns its ascii code; if it is an arrow key, it returns one of the
  four interface directions with the "minus" sign
*/
int snk_read_key() {
  char choice;
  choice = getchar();


  if (choice == 27 && getchar() == '[') { /* The key is an arrow key */
    choice = getchar();

    switch(choice) {
    case('A'):
      return -NORTH;
    case('B'):
      return -SOUTH;
    case('C'):
      return -EAST;
    case('D'):
      return -WEST;
    default:
      return -HERE;
    }
  }
  else
    return choice;
}

/*
  Manages the clock, reading time and updating the clock on the screen
*/
void *snk_ck_count(void *q) {
  intrf *iq = (intrf *) q;
  time_t itime = time(NULL);

  while(1) {
    time_t t = time(NULL);
    pthread_mutex_lock(&time_lock);
    if(snk_i_set_time(iq, t-itime)==0){
        iq->elapsed=iq->max_time;
        pthread_mutex_unlock(&time_lock);
        pthread_exit(NULL);
    }
    pthread_mutex_unlock(&time_lock);
    
    usleep(10000);
  }
}

void *snk_move(void *q){
  intrf *iq = (intrf *)q;
  int i=200000, b=1;
  while(i>1000){ 
    pthread_mutex_lock(&move_lock);  
    if(snk_i_move(iq)==-2){
      iq->game_over=1;
      pthread_mutex_unlock(&move_lock);
      pthread_exit(NULL);
    } 
    if (snk_i_is_on_pellet(iq)) {
	    snk_i_remove_pellet(iq, iq->snake[0].x, iq->snake[0].y);
      iq->tam++;
	    snk_add_pellet(iq);
	    snk_i_set_score(iq, iq->score += ppp);
    } 
    pthread_mutex_unlock(&move_lock);
    
    i=i-b*2;
    b++;
    if(iq->snake[0].dir==0 || iq->snake[0].dir==1){
      usleep(i*3/2);
    }
    else{
      usleep(i);
    }
  }
}

/*
  Plays the game
*/
int snk_play(void * xq) {
  intrf *iq = (intrf *) xq;
  pthread_t pth_time;
  pthread_t pth_move;
  pthread_create(&pth_time, NULL, snk_ck_count, (void *) iq);
  pthread_create(&pth_move, NULL, snk_move, (void *) iq);
  snk_win_write_char_at(iq->field, iq->snake[0].x, iq->snake[0].y-iq->tam, ' ');
  while(1) {
    if(iq->game_over==1){
      pthread_join(pth_move, NULL);
      pthread_cancel(pth_time);
      pthread_join(pth_time, NULL);
      snk_free(iq);
      return 0;
    }
    int c = snk_read_key();

    if (c == 'q' || c=='Q') {
      pthread_cancel(pth_time);
      pthread_join(pth_time, NULL);
      pthread_cancel(pth_move);
      pthread_join(pth_move, NULL);
      snk_free(iq);
      return 0;
    }

    else if (c <= 0) {
      iq->snake[0].dir=-c;
    }
    
    if(iq->elapsed==iq->max_time){
      snk_score = iq->score;
      pthread_cancel(pth_move);
      pthread_join(pth_move, NULL);
      pthread_join(pth_time, NULL);
      snk_free(iq);
      return snk_score;
    }
  }
  return 0;
}

int snake() {
  intrf *iq = snk_intrf_init("./aux/Snake/snake1.txt");
  snk_i_draw_board(iq, 0);

  for (int i=0; i<pellet_no; i++)
    snk_add_pellet(iq);

  return snk_play((void *) iq);;
} 