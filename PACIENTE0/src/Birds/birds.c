/*
(C) Gregorio Blázquez, 2020
----------------------------------------------------------------------------
Minijuego Birds.


Lo más destacable: lee los datos del mapa y partida de un fichero de texto,
utiliza un pthread para ir leyendo y redibujando el mapa continuamente
(con un tiempo de espera).
----------------------------------------------------------------------------
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

#include <birds_intrf.h>
#include <lineread.h>
#include <types.h>


pthread_mutex_t lock_bird = PTHREAD_MUTEX_INITIALIZER;

char pellet;
int pellet_no;
int ppp;
int exitt=0;
int score = 0;

int print_map(intrf *iq){
  int i,j, ret=0;
  pthread_mutex_lock(&lock_bird);
  for(i=0; i<iq->map_rows;i++){
    for(j=0; j<iq->map_cols; j++){
      if(iq->p_row==i && iq->p_col==j) {
        if(iq->map[iq->p_row][iq->p_col]!=' ' && iq->map[iq->p_row][iq->p_col]!='X') ret=-1;
        else{
          printf("%c[%d;%dH", 27, 2+i, 2+j);
          printf("%c", iq->player);
          fflush(stdout);
          if(iq->map[iq->p_row][iq->p_col]=='X'){
            iq->map[iq->p_row][iq->p_col]=' ';
            ret=1;
          }
        }
      }
      else{
        printf("%c[%d;%dH", 27, 2+i, 2+j);
        printf("%c",iq->map[i][j]);
        fflush(stdout);
      }
    }
  }
  pthread_mutex_unlock(&lock_bird);  
  return ret;
}


/*
  Creates the interface reading the data from a text file (given as a
  parameter), and displays it. Returns the interface handle.
*/
intrf *bird_intrf_init(char *fname) {
  FILE *fp = fopen(fname, "r");
  char line[500];
  char *buf;
  int i, j;

  char *start = fgetll(fp);

  int rows = atoi(buf = fgetll(fp));  free(buf);
  int cols = atoi(buf = fgetll(fp));  free(buf);
  int field = atoi(buf = fgetll(fp));  free(buf);
  intrf *itf = bird_i_create(rows, cols, field);
  itf->start=start;

  char *caption = fgetll(fp);
  int crow = atoi(buf = fgetll(fp));  free(buf);
  int ccol = atoi(buf = fgetll(fp));  free(buf);
  int srow = atoi(buf = fgetll(fp));  free(buf);
  int scol = atoi(buf = fgetll(fp));  free(buf);

  bird_i_set_score_caption(itf, caption, crow, ccol, srow, scol);
 
  char player = (buf = fgetll(fp))[0];  free(buf);
  int prow = atoi(buf = fgetll(fp));  free(buf);
  int pcol = atoi(buf = fgetll(fp));  free(buf);
  
  bird_i_set_play_data(itf, player, prow, pcol);

  int bdrow = atoi(buf = fgetll(fp));  free(buf);
  int bdcol = atoi(buf = fgetll(fp));  free(buf);
  int numMax = atoi(buf = fgetll(fp));  free(buf);
  itf->numMax=numMax;
  
  char **map = (char **) malloc(bdrow*sizeof(char *));
  for (i=0; i<bdrow; i++) {
    map[i] = (char *) malloc(bdcol*sizeof(char));
  }

  /* Iniciamos el mapa */
  for(i=0; i<bdcol; i++){
    fgets(line,499,fp);
    for(j=0;j<bdrow;j++){
      map[j][i]=line[j];
    }
  }

  bird_i_set_board(itf, bdrow, bdcol, map);

  itf->f=fp;

  print_map(itf);
  
  return itf;
}

void bird_interfaz_free(intrf *q){
  int i;

  if(q){
    fclose(q->f);
    win_delete(q->field);
    win_delete(q->board);
    for(i=0; i<q->map_rows; i++){
      free(q->map[i]);
    }
    free(q->map);
    free(q->score_caption);
    free(q->start);
    free(q);
  }
}


/*
  Reads a key from the keyboard. If the key is a "regular" key it
  returns its ascii code; if it is an arrow key, it returns one of the
  four interface directions with the "minus" sign
*/
int bird_read_key() {
  char choice;
  choice = fgetc(stdin);


  if (choice == 27 && fgetc(stdin) == '[') { /* The key is an arrow key */
    choice = fgetc(stdin);

    switch(choice) {
    case('A'):
      return -NORTH;
    case('B'):
      return -SOUTH;
    default:
      return -HERE;
    }
  }
  else
    return choice;
}

/*
  Va actualizando el mapa continuamente.
*/
void *bird_map_redraw(void *q) {
  intrf *iq = (intrf *)q;
  int i=0, col=1, row=1, ret, aleatorio, aleatPoints;
  char nextLine[500];
  /* Lista de barreras donde generar huecos aleatoriamente */
  int barreras[8]={68,88,108,128,148,168,188,208}, j=0;

  aleatPoints=aleat_num(6,12);
  exitt=0;
  while(i<iq->numMax){
    usleep(100000);
    fgets(nextLine,499,iq->f);

    /* Salvo los primeros, los huecos son generados aleatoriamente */
    if(i==barreras[j]){
      aleatorio=aleat_num(2,20);
      nextLine[aleatorio]=' ';
      j++;
      aleatPoints=aleat_num(6,12);
    }
    /* Tambien generamos puntos aleatorios */
    if(i==barreras[j]-aleatPoints){
      aleatorio=aleat_num(2,20);
      nextLine[aleatorio]='X';
    }

    /* Movemos todas las filas 1 a la izquierda */
    for(col=1; col<iq->map_cols; col++){
      for(row=0;row<iq->map_rows; row++){
        iq->map[row][col-1]=iq->map[row][col];
      }
    }

    /* Añadimos la siguiente linea */
    for(row=0; row<iq->map_rows; row++){
      iq->map[row][iq->map_cols-1]=nextLine[row];
    }

    ret=print_map(iq);
    if(ret==-1){
      if(exitt==-1) sleep(1);
      exitt=-2;
      break;
    }
    else if(ret==1){
      bird_i_set_score(iq, iq->score+25);
    }

    i++;
  }

  printf("%c[%d;%dH", 27, 27, 3);
  printf("Has terminado, pulsa una tecla para continuar");
  fflush(stdout);
  exitt++;
  return NULL;
}

/* Play the game and return the score */
int birds() {
  exitt=0;
  pthread_t pth;
  int ret;

  intrf *iq = bird_intrf_init("./aux/Birds/bird.txt");

  fflush(stdin);

  printf("%c[%d;%dH", 27, 27, 3);
  printf("Introduce una tecla para comenzar");
  fflush(stdout);
  fgetc(stdin);
  printf("%c[%d;%dH", 27, 27, 2);
  printf("                                     ");
  fflush(stdout);

  /* Llamamos a la funcion que va actualizando el mapa y empezamos el juego */
  pthread_create(&pth, NULL, bird_map_redraw, (void *) iq);
  while(1) {
    if(exitt==-1){
      pthread_join(pth, NULL);
      bird_interfaz_free(iq);
      printf("%c[%d;%dm", 27, 0, 37);
      printf("%c[%d;%dm", 27, 0, 37);
      fflush(stdout);
      return 0;
    }
    else if(exitt==1){
      pthread_join(pth, NULL);
      bird_interfaz_free(iq);
      printf("%c[%d;%dm", 27, 0, 37);
      fflush(stdout);
      return 50+iq->score;
    }
    int c = bird_read_key();
    if (c == 'q' || c=='Q') {
      pthread_cancel(pth);
      pthread_join(pth, NULL);
      bird_interfaz_free(iq);
      printf("%c[%d;%dm", 27, 0, 37);
      fflush(stdout);
      return 0;
    }
    else if (c <= 0) {
      pthread_mutex_lock(&lock_bird);
      ret=bird_i_move(iq, -c);
      pthread_mutex_unlock(&lock_bird);
      if (ret==-1) {
        exitt=-1;
        pthread_cancel(pth);
        pthread_join(pth, NULL);
        bird_interfaz_free(iq);
        return 0;
      }
      else if(ret==1) {
        bird_i_set_score(iq, iq->score+25);
      }
    }
    
  }
} 
