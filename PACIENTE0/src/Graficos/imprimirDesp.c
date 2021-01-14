#include "imprimirDesp.h"

/*
  Imprime la imagen del Despacho de Santini, para ello selecciona el fichero y lo lee
  caracter a caracter cambiando el color de fondo en funcion del caracter 
  que lee
*/
void imprimirDespacho(void) {
  FILE *f = NULL;
  char c;
  
  f=fopen("./aux/plantillas/DespSantini.txt", "r");
  if(!f) {
    return;
  }
  system("clear");
  printf("%c[%d;%dH", 27, 0, 0);

  while(fscanf(f, "%c", &c)==1){
    if(c=='P'){
      printf("\x1b[48;5;232m" " ");
    }
    else if(c=='D'){
      printf("\x1b[48;5;254m" " ");
    }
    else if(c=='c'){
      printf("\x1b[48;5;222m" " ");
    }
    else if(c=='N'){
      printf("\x1b[48;5;221m" " ");
    }
    else if(c=='L'){
      printf("\x1b[48;5;197m" " ");
    }
    else if(c=='E'){
      printf("\x1b[48;5;249m" " ");
    }
    else if(c=='e'){
      printf("\x1b[48;5;250m" " ");
    }
    else if(c=='C'){
      printf("\x1b[48;5;223m" " ");
    }
    else if(c=='A'){
      printf("\x1b[48;5;237m" " ");
    }
    else if(c=='I'){
      printf("\x1b[48;5;235m" " ");
    }
    else if(c=='E'){
      printf("\x1b[48;5;235m" " ");
    }
    else if(c=='e'){
      printf("\x1b[48;5;235m" " ");
    }
    else if(c=='S'){
      printf("\x1b[48;5;234m" " ");
    }
    else if(c=='p'){
      printf("\x1b[48;5;233m" " ");
    }
    else if(c=='R'){
      printf("\x1b[48;5;254m" " ");
    }
    else if(c=='r'){
      printf("\x1b[48;5;253m" " ");
    }
    else if(c=='V'){
      printf("\x1b[48;5;28m" " ");
    }
    else if(c=='M'){
      printf("\x1b[48;5;196m" " ");
    }
    else if(c=='O'){
      printf("\x1b[48;5;52m" " ");
    }
    else if(c=='o'){
      printf("\x1b[48;5;88m" " ");
    }
    else if(c=='X'){
      printf("\x1b[48;5;255m" " ");
    }
    else if(c=='J'){
      printf("\x1b[48;5;179m" " ");
    }
    else if(c=='n'){
      printf("\x1b[48;5;216m" " ");
    }
    else if(c=='l'){
      printf("\x1b[48;5;180m" " ");
    }
    else if(c=='a'){
      printf("\x1b[48;5;226m" " ");
    }
    else if(c=='d'){
      printf("\x1b[48;5;127m" " ");
    }
    else if(c=='`'){
      printf("\x1b[48;5;21m" " ");
    }
    else if(c=='+'){
      printf("\x1b[48;5;33m" " ");
    }
    else if(c=='-'){
      printf("\x1b[48;5;45m" " ");
    }
    else if(c=='^'){
      printf("\x1b[48;5;17m" " ");
    }
    else if(c=='\n'){
      printf("%c", c);
    }
  }
  
  fclose(f);
}