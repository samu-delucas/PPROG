#include <imprimirLab.h>

/*
  Imprime la imagen de los laboratorios, para ello selecciona el fichero y lo lee
  caracter a caracter cambiando el color de fondo en funcion del caracter 
  que lee

  Param:
  
    planta: dependiendo de si es 3 o 7 imprime un laboratorio u otro
*/
void imprimirLab(int lab) {
  FILE *f = NULL;
  char c;
  
  if(lab==3){
    f=fopen("./aux/plantillas/lab3.txt", "r");
  }
  else{
    f=fopen("./aux/plantillas/lab7.txt", "r");
  }
  if(!f) {
    return;
  }
  system("clear");
  printf("%c[%d;%dH", 27, 0, 0);

  while(fscanf(f, "%c", &c)==1){
    if(c=='B'){
      printf("\x1b[48;5;235m" " ");
    } 
    else if(c=='P'){
      printf("\x1b[48;5;247m" " ");
    }
    else if(c=='G'){
      printf("\x1b[48;5;237m" " ");
    } 
    else if(c=='b'){
      printf("\x1b[48;5;251m" " ");
    }
    else if(c=='g'){
      printf("\x1b[48;5;240m" " ");
    }
    else if(c=='R'){
      printf("\x1b[48;5;244m" " ");
    }
    else if(c=='V'){
      printf("\x1b[48;5;46m" " ");
    }
    else if(c=='A'){
      printf("\x1b[48;5;21m" " ");
    }
    else if(c=='r'){
      printf("\x1b[48;5;196m" " ");
    } 
    else if(c=='M'){
      printf("\x1b[48;5;234m" " ");
    }
    else if(c=='m'){
      printf("\x1b[48;5;232m" " ");
    }
    else if(c=='S'){
      printf("\x1b[48;5;242m" " ");
    }
    else if(c=='s'){
      printf("\x1b[48;5;238m" " ");
    }
    else if(c=='F'){
      printf("\x1b[48;5;250m" " ");
    }
    else if(c=='Y'){
      printf("\x1b[48;5;226m" " ");
    }
    else if(c=='y'){
      printf("\x1b[48;5;220m" " ");
    }
    else if(c=='D'){
      printf("\x1b[48;5;88m" " ");
    }
    else if(c=='W'){
      printf("\x1b[48;5;231m" " ");
    }
    else if(c=='w'){
      printf("\x1b[48;5;254m" " ");
    }
    else if(c=='N'){
      printf("\x1b[48;5;202m" " ");
    }
    else if(c=='n'){
      printf("\x1b[48;5;166m" " ");
    } 
    else if(c=='`'){
      printf("\x1b[48;5;180m" " ");
    }
    else if(c=='\n'){
      printf("%c", c);
    }
  }
  
  fclose(f);
}