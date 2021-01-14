#include <imprimirAula10.h>

/*
  Imprime la imagen del Aula 10, para ello selecciona el fichero y lo lee
  caracter a caracter cambiando el color de fondo en funcion del caracter 
  que lee
*/
void imprimirAula10() {
  FILE *f = NULL;
  char c;
  
  f=fopen("./aux/plantillas/aula10.txt", "r");
  if(!f) {
    return;
  }
  system("clear");
  printf("%c[%d;%dH", 27, 0, 0);
  while(fscanf(f, "%c", &c)==1){

    if(c=='P'){
      printf("\x1b[48;5;22m" " ");
    }
    else if(c=='B'){
      printf("\x1b[48;5;231m" " ");
    }
    else if(c=='M'){
      printf("\x1b[48;5;222m" " ");
    }
    else if(c=='S'){
      printf("\x1b[48;5;221m" " ");
    }
    else if(c=='G'){
      printf("\x1b[48;5;247m" " ");
    }
    else if(c=='N'){
      printf("\x1b[48;5;232m" " ");
    }
    else if(c=='A'){
      printf("\x1b[48;5;226m" " ");
    }
    else if(c=='Z'){
      printf("\x1b[48;5;17m" " ");
    }
    else if(c=='U'){
      printf("\x1b[48;5;251m" " ");
    }
    else if(c=='C'){
      printf("\x1b[48;5;88m" " ");
    }
    else if(c=='I'){
      printf("\x1b[48;5;236m" " ");
    }
    else if(c=='H'){
      printf("\x1b[48;5;160m" " ");
    }
    else if(c=='T'){
      printf("\x1b[48;5;238m" " ");
    }
    else if(c=='b'){
      printf("\x1b[48;5;252m" " ");
    }
    else if(c=='h'){
      printf("\x1b[48;5;124m" " ");
    }
    else if(c=='s'){
      printf("\x1b[48;5;220m" " ");
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