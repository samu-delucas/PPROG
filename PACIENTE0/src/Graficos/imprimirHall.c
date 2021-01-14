#include "imprimirHall.h"

/*
  Imprime la imagen del Hall, para ello selecciona el fichero y lo lee
  caracter a caracter cambiando el color de fondo en funcion del caracter 
  que lee
*/
void imprimirHall(void) {
  FILE *f = NULL;
  char c;
  
  f=fopen("./aux/plantillas/hall.txt", "r");
  if(!f) {
    return;
  }
  system("clear");
  printf("%c[%d;%dH", 27, 0, 0);

  while(fscanf(f, "%c", &c)==1){
    if(c=='m'){
      printf("\x1b[48;5;215m" " ");
    }
    else if(c=='M'){
      printf("\x1b[48;5;216m" " ");
    }
    else if(c=='G'){
      printf("\x1b[48;5;243m" " ");
    }
    else if(c=='g'){
      printf("\x1b[48;5;255m" " ");
    }
    else if(c=='r'){
      printf("\x1b[48;5;217m" " ");
    }
    else if(c=='R'){
      printf("\x1b[48;5;224m" " ");
    } 
    else if(c=='N'){
      printf("\x1b[48;5;217m" " ");
    }
    else if(c=='J'){
      printf("\x1b[48;5;225m" " ");
    } 
    else if(c=='B'){
      printf("\x1b[48;5;235m" " ");
    } 
    else if(c=='V'){
      printf("\x1b[48;5;34m" " ");
    } 
    else if(c=='v'){
      printf("\x1b[48;5;41m" " ");
    } 
    else if(c=='A'){
      printf("\x1b[48;5;22m" " ");
    } 
    else if(c=='a'){
      printf("\x1b[48;5;23m" " ");
    } 
    else if(c=='p'){
      printf("\x1b[48;5;247m" " ");
    } 
    else if(c=='n'){
      printf("\x1b[48;5;17m" " ");
    } 
    else if(c=='s'){
      printf("\x1b[48;5;251m" " ");
    } 
    else if(c=='`'){
      printf("\x1b[48;5;21m" " ");
    }
    else if(c=='L'){
      printf("\x1b[48;5;195m" " ");
    }
    else if(c=='l'){
      printf("\x1b[48;5;231m" " ");
    }
    else if(c=='j'){
      printf("\x1b[48;5;28m" " ");
    }
    else if(c=='k'){
      printf("\x1b[48;5;52m" " ");
    }
    else if(c=='K'){
      printf("\x1b[48;5;160m" " ");
    }
    else if(c=='\n'){
      printf("%c", c);
    }
  }
  
  fclose(f);
}