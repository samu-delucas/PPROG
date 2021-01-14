#include "imprimirEscaleras.h"

/*
  Imprime la imagen de las Escaleras, para ello selecciona el fichero y lo lee
  caracter a caracter cambiando el color de fondo en funcion del caracter 
  que lee

  Param:
  
    planta: dependiendo de si es 1 o 2 imprime una planta u otra
*/
void imprimirEscaleras(int planta) {
  FILE *f = NULL;
  char c;
  
  if(planta==1){
    f=fopen("./aux/plantillas/escalerasP3.txt", "r");
  }
  else{
    f=fopen("./aux/plantillas/escalerasP2.txt", "r");
  }
  if(!f) {
    return;
  }
  system("clear");
  printf("%c[%d;%dH", 27, 0, 0);

  while(fscanf(f, "%c", &c)==1){
    if(c=='P'){
      printf("\x1b[48;5;16m" " ");
    }
    else if(c=='V'){
      printf("\x1b[48;5;46m" " ");
    }
    else if(c=='A'){
      printf("\x1b[48;5;244m" " ");
    }
    else if(c=='a'){
      printf("\x1b[48;5;247m" " ");
    } 
    else if(c=='S'){
      printf("\x1b[48;5;238m" " ");
    }
    else if(c=='b'){
      printf("\x1b[48;5;249m" " ");
    }
    else if(c=='B'){
      printf("\x1b[48;5;18m" " ");
    }
    else if(c=='c'){
      printf("\x1b[48;5;17m" " ");
    }
    else if(c=='H'){
      printf("\x1b[48;5;255m" " ");
    }
    else if(c=='C'){
      printf("\x1b[48;5;166m" " ");
    }
    else if(c=='M'){
      printf("\x1b[48;5;172m" " ");
    }
    else if(c=='R'){
      printf("\x1b[48;5;160m" " ");
    }
    else if(c=='v'){
      printf("\x1b[48;5;28m" " ");
    }
    else if(c=='m'){
      printf("\x1b[48;5;88m" " ");
    }
    else if(c=='n'){
      printf("\x1b[48;5;124m" " ");
    } 
    else if(c=='`'){
      printf("\x1b[48;5;252m" " ");
    }
    else if(c=='\n'){
      printf("%c", c);
    }
  }
  
  fclose(f);
}