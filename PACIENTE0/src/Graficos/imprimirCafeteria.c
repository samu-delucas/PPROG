#include <imprimirCafeteria.h>

/*
  Imprime la imagen de la Cafeteria, para ello selecciona el fichero y lo lee
  caracter a caracter cambiando el color de fondo en funcion del caracter 
  que lee
*/
void imprimirCafeteria() {
  FILE *f = NULL;
  char c;
  
  f=fopen("./aux/plantillas/cafeteria.txt", "r");
  if(!f) {
    return;
  }
  system("clear");
  printf("%c[%d;%dH", 27, 0, 0);
  while(fscanf(f, "%c", &c)==1){

    if(c=='T'){
      printf("\x1b[48;5;252m" " ");
    }
    else if(c=='t'){
      printf("\x1b[48;5;247m" " ");
    }
    else if(c=='C'){
      printf("\x1b[48;5;221m" " ");
    }
    else if(c=='c'){
      printf("\x1b[48;5;172m" " ");
    }
    else if(c=='F'){
      printf("\x1b[48;5;94m" " ");
    }
    else if(c=='P'){
      printf("\x1b[48;5;250m" " ");
    }
    else if(c=='p'){
      printf("\x1b[48;5;249m" " ");
    }
    else if(c=='B'){
      printf("\x1b[48;5;160m" " ");
    }
    else if(c=='U'){
      printf("\x1b[48;5;243m" " ");
    }
    else if(c=='b'){
      printf("\x1b[48;5;254m" " ");
    }
    else if(c=='M'){
      printf("\x1b[48;5;223m" " ");
    }
    else if(c=='m'){
      printf("\x1b[48;5;228m" " ");
    }
    else if(c=='S'){
      printf("\x1b[48;5;88m" " ");
    }
    else if(c=='s'){
      printf("\x1b[48;5;124m" " ");
    } 
    else if(c=='`'){
      printf("\x1b[48;5;21m" " ");
    }
    else if(c=='\n'){
      printf("%c", c);
    }
    
  }
  fclose(f);
}