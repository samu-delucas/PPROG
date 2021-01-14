#include <imprimirZonaCuarentena.h>

/*
  Imprime la imagen de la Zona de Cuarentena, para ello selecciona el fichero y lo lee
  caracter a caracter cambiando el color de fondo en funcion del caracter 
  que lee
*/
void imprimirZonaCuarentena() {
  FILE *f = NULL;
  char c;
  
  f=fopen("./aux/plantillas/ZonaCuarentena.txt", "r");
  if(!f) {
    return;
  }
  system("clear");
  printf("%c[%d;%dH", 27, 0, 0);
  while(fscanf(f, "%c", &c)==1){

    if (c=='N'){
      printf("\x1b[48;5;232m" " ");
    }
    if (c=='B'){
      printf("\x1b[48;5;255m" " ");
    }
    else if (c=='n'){
      printf("\x1b[48;5;235m" " ");
    } 
    else if (c=='R'){
      printf("\x1b[48;5;196m" " ");
    }
    else if (c=='A'){
      printf("\x1b[48;5;226m" " ");
    }
    else if (c=='Z'){
      printf("\x1b[48;5;18m" " ");
    } 
    else if (c=='p'){
      printf("\x1b[48;5;248m" " ");
    } 
    else if(c=='`'){
      printf("\x1b[48;5;250m" " ");
    }
    else if(c=='\n'){
      printf("%c", c);
    }
    
  }
  fclose(f);
}