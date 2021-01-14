#ifndef INTERFACE_H
#define INTERFACE_H
 
#include <rectangles.h>  
#include <jugador.h>
#include <mapa.h>


#define BACKGROUND   40  /* Black background */
#define FOREGROUND   37  /* Green foreground... for a "retro" look */
#define NUM_PRUEBAS 9    /* Numero de pruebas a resolver para poder acabar el juego */


typedef struct {
  sc_rectangle *left_up;      /* Pantalla de imagenes y auxiliares */
  sc_rectangle *right_up;     /* Pantalla de texto sobre la habitacion */
  sc_rectangle *left_down;    /* Pantalla de entrada y salida de texto */
  sc_rectangle *right_down;   /* Pantalla fija de opciones */
  char score_caption[20];     /* Caption used in the score window */
  int  r_score, c_score;      /* Posicion de la puntiacion en right_down */
  int  score;                 /* Puntuacion */
  int  ckrow, ckcol;          /* Posicion del reloj en right_down */
  char clock_caption[20];     /* Caption of the clock   */
  long max_time;              /* Tiempo limite */
  int  elapsed;               /* Tiempo transcurrido */
  int pruebas[NUM_PRUEBAS];   /* Numero de pruebas superadas (max 9) */
  Jugador *player;            /* Estructura con los datos de la partida */
  Mapa *map;                  /* Estructura con el mapa y la posicion del jugador */

  int paciente0;
} intrf;

/*
  Reimprime la pantalla con el contenido y los bordes.
*/
void _i_redraw(intrf *q);


/*

  intrf *i_create(Jugador *jug, int puntuacion, int tiempo);

  Crea e inicializa una interfaz con los parametros pasados y prepara la pantalla.

  Parameters:
     jug: puntero a la estructura jugador con la infromacion del juego al iniciar.
     puntuacion: puntuacion al iniciar.
     tiempo: tiempo de inicio.

  Returns:
     Un puntero a la estructura inicializada.

     NULL si algun paso ha fallado.
*/
intrf *i_create(Jugador *jug, int puntuacion, int tiempo, int paciente0);


void interfaz_free(intrf *q);


/*
  Sets the score to a given value, and prints it

  Returns the score
*/
int i_set_score(intrf *itf, int score);


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
int i_set_time(intrf *itf, int elapsed);

#endif
