/*
    Aquí están la mayor parte de las funciones, la definición de la estructura interfaz 
    está en el .h para poder accerder a la estructura desde cualquier fichero que contenga
    a intrf.h.
*/
#include <stdio.h>
#include <stdlib.h>    
#include <stdio.h>    
#include <malloc.h>    
#include <string.h>   
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

#include <intrf.h>
#include <rectangles.h>  
#include <jugador.h>
#include <mapa.h>
#include <imprimirBiblioteca.h>
#include <imprimirHall.h>
#include <imprimirLab.h>
#include <imprimirCafeteria.h>
#include <imprimirEscaleras.h>
#include <imprimirAula10.h>
#include <imprimirDesp.h>
#include <imprimirZonaCuarentena.h>

extern pthread_mutex_t lock;

void print_score(intrf *itf) {
  char buf[10];
  win_write_line_at(itf->right_down, itf->r_score, itf->c_score+13, "                 ");
   win_write_line_at(itf->right_down, itf->r_score, itf->c_score+13, itf->score_caption);
  sprintf(buf, "%3d", itf->score);
  win_write_line_at(itf->right_down, itf->r_score, itf->c_score+21, buf);  
}

void print_time(intrf *itf) {
  int seconds, minutes;
  char buf[20];
  int remaining = itf->max_time - itf->elapsed;
  remaining = (remaining < 0) ? 0 : remaining;

  /* Pasamos el tiempo a formato 00:00 */
  seconds=remaining%60;
  minutes=remaining/60;
  buf[0]='0'+minutes/10;
  buf[1]='0'+minutes%10;
  buf[2]=':';
  buf[3]='0'+seconds/10;
  buf[4]='0'+seconds%10;
  buf[5]=0;

  win_write_line_at(itf->right_down, itf->ckrow, itf->ckcol+13, "                 ");  
  win_write_line_at(itf->right_down, itf->ckrow, itf->ckcol+13, itf->clock_caption);  
  win_write_line_at(itf->right_down, itf->ckrow, itf->ckcol+21, buf);
}


/* Función privada de _i_redraw */
void print_fijo(intrf *interfaz){
  char buf[50];

  win_write_borders4(interfaz->left_up, interfaz->right_up, interfaz->left_down, interfaz->right_down);
  print_time(interfaz);
  print_score(interfaz);

  strcpy(buf,"COMANDOS para abrir la guia de comandos");
  win_write_line_at(interfaz->right_down,1,1,buf);
  strcpy(buf,"MAPA para abrir el mapa");
  win_write_line_at(interfaz->right_down,2,1,buf);
  strcpy(buf,"AGENDA para abrir la agenda");
  win_write_line_at(interfaz->right_down,3,1,buf);
  strcpy(buf,"CERRAR para salir del mapa y agenda");
  win_write_line_at(interfaz->right_down,4,1,buf);
  strcpy(buf,"SALIR para abrir el menu para salir");
  win_write_line_at(interfaz->right_down,5,1,buf);
}
/* Función privada de _i_redraw */
void print_Nofijo(intrf *interfaz){
  int i=1, pos;
  char buf[200], direccion[100]="./aux/Habitaciones/";
  FILE *f=NULL;
  char *nombrePersonaje=NULL, *nombreObjeto=NULL;
  Objeto *obj=NULL;
  Bool a;

  pos=mapa_getPos(interfaz->map);

  /* En la habitación de Santini necesitamos dar la luz */
  if(pos==9 && mapa_areConnected(interfaz->map,9,9)==FALSE){
    obj=jugador_getObjeto(interfaz->player);
    if(obj!=NULL && strcmp(objeto_getNombre(obj),"una linterna")==0){
      print_fijo(interfaz);
      strcpy(buf,"Con la linterna consigues en encontrar el interruptor y "
      "enciendes la luz. De repente Santini grita ¡¡BU!!. Te asustas. "
      "Cuando te hayas recuperado pulsa una tecla.");
      win_write_line_sc2(interfaz->right_up, buf, i);
      jugador_deleteObjeto(interfaz->player);
      fgetc(stdin);
      mapa_setConnections(interfaz->map,9,9);
      win_cls(interfaz->right_up);
      win_write_borders4(interfaz->left_up, interfaz->right_up, interfaz->left_down, interfaz->right_down);
    }
    else{
      strcpy(buf,"La luz esta apagada y no puedes ver nada, necesitaras algo para "
      "poder ver y encender la luz.");
      win_write_line_sc2(interfaz->right_up, buf, i);
      return;
    }
  }

  printf("%c[%d;%dH", 27, 0, 0);
  switch (pos)
  {
  case 0:
    imprimirBiblioteca();
    break;
  case 1:
    imprimirEscaleras(1);
    break;
  case 2:
    imprimirZonaCuarentena();
    break;
  case 3:
    imprimirLab(3);
    break;
  case 4:
    imprimirEscaleras(4);
    break;
  case 5:
    imprimirLab(7);
    break;
  case 6:
    imprimirAula10();
    break;
  case 7:
    imprimirHall();
    break;
  case 8:
    imprimirCafeteria();
    break;
  case 9:
    imprimirDespacho();
    break;
  default:
    break;
  }

  strcat(direccion,habitacion_getNombre(mapa_getHab(interfaz->map,mapa_getPos(interfaz->map))));

  f=fopen(direccion,"r");
  if(f==NULL){
    printf("ERROR AL ABRIR EL ARCHIVO");
    return;
  }
  while(fgets(buf, 199, f)!=NULL){
    i+=win_write_line_sc2(interfaz->right_up, buf, i);
    if(i==2) i++;
  }

  strcpy(buf,"Aqui se encuentra ");
  nombrePersonaje=personaje_getNombre(habitacion_getPersonaje(mapa_getHab(interfaz->map,pos)));
  strcat(buf, nombrePersonaje);
  i++;
  i+=win_write_line_sc2(interfaz->right_up, buf, i);
  i++;

  if(habitacion_getObjeto(mapa_getHab(interfaz->map, pos))==NULL){
    i+=win_write_line_sc2(interfaz->right_up, "No hay ningun objeto en esta habitacion.", i);
  }
  else{
    strcpy(buf,"Si te fijas bien puedes ver ");
    nombreObjeto=objeto_getNombre(habitacion_getObjeto(mapa_getHab(interfaz->map, pos)));
    strcat(buf, nombreObjeto);
    i++;
    i+=win_write_line_sc2(interfaz->right_up, buf, i);
  }

  fclose(f);
}

/*
  Reimprime la pantalla con el contenido y los bordes.
*/
void _i_redraw(intrf *interfaz) {
  pthread_mutex_lock(&lock);
  system("clear");
  
  /***IMPRIMIR CONTENIDO PANTALLAS NO FIJAS***/
  print_Nofijo(interfaz);

  /***IMPRIMIR CONTENICO PANTALLA FIJA***/
  print_fijo(interfaz);

  /* Movemos el puntero de la terminal a su pantalla */
  _move_to(interfaz->left_down, 0, 1);
  pthread_mutex_unlock(&lock);

  return;
}


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
intrf *i_create(Jugador *jug, int puntuacion, int tiempo, int paciente0) {
  intrf *interfaz;
  sc_rectangle *x, *y, *z, *w;
  int i;

  if(jug==NULL || puntuacion<0 || tiempo<0) return NULL;

  interfaz=(intrf *)malloc(sizeof(intrf));
  if(interfaz==NULL) return NULL;

  /* DATOS DE LA PARTIDA */
  interfaz->paciente0=paciente0;
  interfaz->score=puntuacion;
  interfaz->elapsed=tiempo-1;
  interfaz->player=jug;
  /* PANTALLAS (siempre igual) */
  interfaz->left_up=win_new(0, 0, 25, 100, BACKGROUND, FOREGROUND);
  win_cls(interfaz->left_up);
  interfaz->right_up=win_new(0, 100, 25, 45, BACKGROUND, FOREGROUND);
  win_cls(interfaz->right_up);
  interfaz->left_down=win_new(25, 0, 10, 100, BACKGROUND, FOREGROUND);
  win_cls(interfaz->left_down);
  interfaz->right_down=win_new(25, 100, 10, 45, BACKGROUND, FOREGROUND);
  win_cls(interfaz->right_down);
  /* Posiciones del tiempo y puntución y limites */
  interfaz->r_score=7;
  interfaz->c_score=1;
  interfaz->ckrow=8;
  interfaz->ckcol=1;
  interfaz->max_time=1500;
  /* Empezamos con 0 pruebas superadas */
  for(i=0; i<NUM_PRUEBAS; i++){
    interfaz->pruebas[i]=FALSE;
  }
  /* Tetxo Puntuacion y Tiempo */
  strcpy(interfaz->clock_caption, "Time:");
  strcpy(interfaz->score_caption, "Score:");


  /**MAPA**/
  interfaz->map=mapa_init();

  /* Prepara la pantalla */
  printf("%c[8;%d;%dt",  27, 35, 145);
  system("clear");
  _i_redraw(interfaz);

  return interfaz;
  
}

/*
  Libera la interfaz llamando a las funciones correspondientes.
*/
void interfaz_free(intrf *q){
  if(q){
    mapa_free(q->map);
    jugador_free(q->player);
    win_delete(q->left_up);
    win_delete(q->right_up);
    win_delete(q->left_down);
    win_delete(q->right_down);
    free(q);
  }
}


/*
  Sets the score to a given value, and prints it

  Returns the score
*/
int i_set_score(intrf *itf, int score) {
  char buf[10];
  itf->score = score;
  win_write_line_at(itf->right_down, itf->r_score, itf->c_score+13, "                 ");
   win_write_line_at(itf->right_down, itf->r_score, itf->c_score+13, itf->score_caption);
  sprintf(buf, "%3d", itf->score);
  win_write_line_at(itf->right_down, itf->r_score, itf->c_score+21, buf);  

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
  int seconds, minutes;
  char buf[20];
  if (itf->elapsed==elapsed)
    return 1;
  itf->elapsed = elapsed;
  int remaining = itf->max_time - itf->elapsed;
  remaining = (remaining < 0) ? 0 : remaining;

  /* Pasamos el tiempo a formato 00:00 */
  seconds=remaining%60;
  minutes=remaining/60;
  buf[0]='0'+minutes/10;
  buf[1]='0'+minutes%10;
  buf[2]=':';
  buf[3]='0'+seconds/10;
  buf[4]='0'+seconds%10;
  buf[5]=0;

  win_write_line_at(itf->right_down, itf->ckrow, itf->ckcol+13, "                 ");  
  win_write_line_at(itf->right_down, itf->ckrow, itf->ckcol+13, itf->clock_caption);  
  win_write_line_at(itf->right_down, itf->ckrow, itf->ckcol+21, buf);

  return remaining;
}
