#include <imprimirBiblioteca.h>

/*
  Imprime la imagen de la Biblioteca, para ello selecciona el fichero y lo lee
  caracter a caracter cambiando el color de fondo en funcion del caracter 
  que lee
*/
void imprimirBiblioteca() {
  FILE *f = NULL;
  char c;
  
  f=fopen("./aux/plantillas/biblioteca.txt", "r");
  if(!f) {
    return;
  }
  system("clear");
  printf("%c[%d;%dH", 27, 0, 0);
  while(fscanf(f, "%c", &c)==1){

    if(c=='?'){
      printf("\x1b[48;5;179m" " ");
    }
    else if(c=='p'){
      printf("\x1b[48;5;52m" " ");
    }
    else if(c=='P'){
      printf("\x1b[48;5;94m" " ");
    }
    else if(c=='m'){
      printf("\x1b[48;5;88m" " ");
    }
    else if(c=='M'){
      printf("\x1b[48;5;124m" " ");
    }
    else if(c=='E'){
      printf("\x1b[48;5;130m" " ");
    }
    else if(c=='e'){
      printf("\x1b[48;5;131m" " ");
    }
    else if(c=='G'){
      printf("\x1b[48;5;243m" " ");
    }
    else if(c=='A'){
      printf("\x1b[48;5;229m" " ");
    }
    else if(c=='B'){
      printf("\x1b[48;5;255m" " ");
    }
    else if(c=='b'){
      printf("\x1b[48;5;252m" " ");
    }
    else if(c=='r'){
      printf("\x1b[48;5;160m" " ");
    }
    else if(c=='R'){
      printf("\x1b[48;5;196m" " ");
    }
    else if(c=='v'){
      printf("\x1b[48;5;28m" " ");
    }
    else if(c=='V'){
      printf("\x1b[48;5;76m" " ");
    }
    else if(c=='g'){
      printf("\x1b[48;5;239m" " ");
    }
    else if(c=='a'){
      printf("\x1b[48;5;226m" " ");
    }
    else if(c=='N'){
      printf("\x1b[48;5;202m" " ");
    }
    else if(c=='Z'){
      printf("\x1b[48;5;18m" " ");
    }
    else if(c=='z'){
      printf("\x1b[48;5;17m" " ");
    }
    else if(c=='C'){
      printf("\x1b[48;5;223m" " ");
    }
    else if(c=='c'){
      printf("\x1b[48;5;222m" " ");
    }
    else if(c=='L'){
      printf("\x1b[48;5;232m" " ");
    }
    else if(c=='X'){
      printf("\x1b[48;5;89m" " ");
    }
    else if(c=='x'){
      printf("\x1b[48;5;53m" " ");
    }
    else if(c=='k'){
      printf("\x1b[48;5;236m" " ");
    }
    else if(c=='K'){
      printf("\x1b[48;5;238m" " ");
    }
    else if(c=='h'){
      printf("\x1b[48;5;171m" " ");
    }
    else if(c=='H'){
      printf("\x1b[48;5;207m" " ");
    }
    else if(c=='s'){
      printf("\x1b[48;5;20m" " ");
    }
    else if(c=='S'){
      printf("\x1b[48;5;21m" " ");
    }
    else if(c=='d'){
      printf("\x1b[48;5;46m" " ");
    }
    else if(c=='D'){
      printf("\x1b[48;5;48m" " ");
    }
    else if(c=='F'){
      printf("\x1b[48;5;101m" " ");
    }
    else if(c=='f'){
      printf("\x1b[48;5;100m" " ");
    }
    else if(c=='\n'){
      printf("%c", c);
    }
    
  }
  fclose(f);
}