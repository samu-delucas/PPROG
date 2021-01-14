
/*------------------------------------------------------------------
   Project:  Laberinto
   File:     laberinto.c
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
#include <laberinto.h>
#include <termios.h>
#include <pthread.h>

typedef struct {
  time_t initial; // Initial time at which the function is first called
  intrf  *intrf;    // pointer to the interface where the clock is to be displayed
} clock_data;

pthread_mutex_t lab_lock = PTHREAD_MUTEX_INITIALIZER;

struct termios initial;

int end = 0;

/*
  Initializes the fields of a interface created with malloc
*/
intrf *lab_intrf_init(char *fname) {
  FILE *fp = fopen(fname, "r");
  int i;
  char *buf;

  int rows = atoi(buf = fgetll(fp));
  free(buf);
  int cols = atoi(buf = fgetll(fp));
  free(buf);
  int field = atoi(buf = fgetll(fp)); 
  free(buf);
  intrf *itf = lab_i_create(rows, cols, field);

  int ckrow = atoi(buf = fgetll(fp));
  free(buf);
  int ckcol = atoi(buf = fgetll(fp));
  free(buf);
  int max_time = atoi(buf = fgetll(fp));
  free(buf);
  char *clock_caption = fgetll(fp);
  lab_i_set_clock_pars(itf, clock_caption, ckrow, ckcol, max_time);
 
  char player = (buf = fgetll(fp))[0];
  free(buf);
  int prow = atoi(buf = fgetll(fp));
  free(buf);
  int pcol = atoi(buf = fgetll(fp));
  free(buf);
  lab_i_set_play_data(itf, player, prow, pcol);

  int bdrow = atoi(buf = fgetll(fp));
  free(buf);
  int bdcol = atoi(buf = fgetll(fp));
  free(buf);
  
  char **map = (char **) malloc(bdrow*sizeof(char *));
  for (i=0; i<bdrow; i++) {
    map[i] = fgetll(fp);
  }
  lab_i_set_board(itf, bdrow, bdcol, map);

  fclose(fp);
  return itf;
}

/*
  Read the key entered by the user and return the appropiate value
*/
int lab_read_key() {
  char choice;
  choice = fgetc(stdin);


  if (choice == 27 && fgetc(stdin) == '[') { /* The key is an arrow key */
    choice = fgetc(stdin);

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
  Counter that ends the program when elapsed time is equal to the max time
*/
void *lab_ck_count(void *q) {
  intrf *iq = (intrf *) q;
  time_t itime = time(NULL);

  while(1) {
    time_t t = time(NULL);

    pthread_mutex_lock(&lab_lock);
    if(lab_i_set_time(iq, t-itime)==0){
        iq->elapsed=iq->max_time;
        pthread_mutex_unlock(&lab_lock);
        pthread_exit(NULL);
    }
    pthread_mutex_unlock(&lab_lock);
    
    usleep(10000);
  }
}

/*
  Function that gets the player to the initial position in case the player wants to
*/
void lab_go_ini(void *i){
  intrf *iq = (intrf *)i;
  lab_win_write_char_at(iq->field, iq->p_row, iq->p_col, ' ');
  lab_move_to(iq->field, 1, 1);
  lab_win_write_char_at(iq->field, iq->p_row = 1, iq->p_col = 1, iq->player);
}

/*
  Main funcition that carries the game, initializes the counter and read the keys
*/
int lab_play(void * xq) {
  intrf *iq = (intrf *) xq;
  int res;
  pthread_t pth;
  pthread_create(&pth, NULL, lab_ck_count, (void *) iq);
  while(1) {
    int c = lab_read_key();
    
    if (c == ' '){
      lab_go_ini(xq);
    }

    if (c == 'q' || c=='Q') {
      pthread_cancel(pth);
      pthread_join(pth, NULL);
      lab_free((intrf *)xq);
      return 0;

    }
    else if (c <= 0) {
        if(lab_i_move(iq, -c)==-2){
          pthread_cancel(pth);
          pthread_join(pth, NULL);
          res=iq->max_time-iq->elapsed;
          lab_free((intrf *)xq);
          return 100+res;
        }
      }
      if(iq->elapsed==iq->max_time){
        pthread_cancel(pth);
        pthread_join(pth, NULL);
        lab_free((intrf *)xq);
        return 0;
      }
    }
  return 0;
}

/*
  Function to begin the game, the score is returned in the function play
*/
int laberinto() {
  int n=0;
  intrf *iq=NULL;

  n=aleat_num(1,5);
  switch(n){
    case 1:
      iq = lab_intrf_init("./aux/Laberintos/laberinto1.txt");
      break;
    case 2:
      iq = lab_intrf_init("./aux/Laberintos/laberinto2.txt");
      break;
    case 3:
      iq = lab_intrf_init("./aux/Laberintos/laberinto3.txt");
      break;
    case 4:
      iq = lab_intrf_init("./aux/Laberintos/laberinto4.txt");
      break;
    case 5:
      iq = lab_intrf_init("./aux/Laberintos/laberinto5.txt");
      break;
  }

  lab_i_draw_board(iq, 0);

  return lab_play((void *) iq);
}