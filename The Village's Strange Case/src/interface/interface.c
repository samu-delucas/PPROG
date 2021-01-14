/************************************************************************/
/*                                                                      */
/* Descripción: Funciones relacionadas con imprimir en la pantalla      */
/*                                                                      */
/* Archivo: interface.c                                                 */
/* Autor: Error 404                                                     */
/*                                                                      */
/************************************************************************/

#include "interface.h"
#define BUFFER_SIZE 1000

/**
 * @brief Imprime en la terminal la imagen deseada en la posición que se indica
 * 
 * @param image imagen a imprimir
 * @param row fila donde se imprime la imagen
 * @param column columna donde se imprime la imagen 
 * 
 * @return Devuelve la anchura de la imagen o error
 */
int print_image(char *image, int row, int column){
    bmp_img img;
    bmp_pixel *pixel;

    int x, y;
    int width, height;
    int r, g, b;

    if (!image) {
        printf("the %s parameter is not ok", image);
        return ERR;
    }

    bmp_img_read(&img, image);
    width = img.img_header.biWidth - 1;
    height = img.img_header.biHeight - 1;

    pthread_mutex_lock(&mutex);

    for (y = 0; y <= height; y++) {
        term_move_to(row + y, column);
        for (x = 0; x <= width; x++) {
            pixel = &(img.img_pixels[y][x]);
            r = pixel->red;
            g = pixel->green;
            b = pixel->blue;
            printf("%c[48;2;%d;%d;%dm", 27, r, g, b);
            printf("  ");
        }

        printf("%c[48;2;%d;%d;%dm", 27, 12, 12, 12);
        printf("%c[38;2;%d;%d;%dm", 27, 204, 204, 204);
        if (y<height) printf("\n");
    }

    pthread_mutex_unlock(&mutex);

    bmp_img_free(&img);
    return width;
}


/**
 * @brief Imprime en pantalla un caracter (mayúsculas, minúsculas, números...)
 * 
 * @param b letra a imprimir leída de un archivo
 * @param i fila complementaria a row
 * @param j columna complementaria a column
 * @param row fila en la que se escribe
 * @param column columna en la que se escribe
 * 
 */
void print_letter(char *b, int *i, int *j, int row, int column) {
    char d[30] = "aux_files/Art/";

    /* Mayúsculas */
    if(b[0] >= 'A' && b[0] <= 'Z') {
        b[0] += 32;
        strcat(d, b);
        strcat(d, b);
        strcat(d, ".bmp");
        *i += 4 + 2*print_image(d, row + *j, column + *i);
        strcpy(d, "aux_files/Art/");
    }

    /* Símbolos de puntuación */
    else if(b[0] == '^');
    else if(b[0] == '.') *i += 4 + 2*print_image("aux_files/Art/point.bmp", row + *j, column + *i);
    else if(b[0] == ';') *i += 4 + 2*print_image("aux_files/Art/dotandcoma.bmp", row + *j, column + *i);
    else if(b[0] == ':') *i += 4 + 2*print_image("aux_files/Art/twopoints.bmp", row + *j, column + *i);
    else if(b[0] == '"') *i += 4 + 2*print_image("aux_files/Art/doblecomilla.bmp", row + *j, column + *i);
    else if(b[0] == '?') *i += 4 + 2*print_image("aux_files/Art/questionmark.bmp", row + *j, column + *i);
    else if(b[0] == '!') *i += 4 + 2*print_image("aux_files/Art/exclamationmark.bmp", row + *j, column + *i);
    else if(b[0] == ',') *i += 4 + 2*print_image("aux_files/Art/coma.bmp", row + *j, column + *i);
    else if(b[0] ==  39) *i += 4 + 2*print_image("aux_files/Art/comilla.bmp", row + *j, column + *i);
    else if(b[0] == '*') *i += 4 + 2*print_image("aux_files/Art/asterisco.bmp", row + *j, column + *i);
    else if(b[0] == '(') *i += 4 + 2*print_image("aux_files/Art/open_parenthesis.bmp", row + *j, column + *i);
    else if(b[0] == ')') *i += 4 + 2*print_image("aux_files/Art/close_parenthesis.bmp", row + *j, column + *i);

    /* Minúsculas y números */
    else if ((b[0] >= 'a' && b[0] <= 'z') || (b[0] >= '0' && b[0] <= '9')) {
        strcat(d, b);
        strcat(d, ".bmp");
        *i += 4 + 2*print_image(d, row + *j, column + *i);
        strcpy(d, "aux_files/Art/");
    }

    else *i+=12;
    usleep(8000);
}


/**
 * @brief Imprime en pantalla el texto de un archivo
 * 
 * @param file Contiene el texto que se va a imprimir en pantalla 
 * 
 */
int print_text(Global* global, char *file){
    FILE *f = fopen(file, "r");
    char b[2];
    int column, row, i, j;

    for (i = 0, j = 0, b[1] = '\0'; (b[0] = fgetc(f)) != EOF; ) {

        /* Diálogo de personaje no jugable */
        if(b[0]=='-') {
            print_image("aux_files/Art/blank_text2.bmp", 50, 300);
            row=50;
            column=300;
            i=0;
            j=0;
        }
        
        /* Diálogo del jugador */
        else if(b[0]=='#') {
            print_image("aux_files/Art/blank_text.bmp", 289, 50);
            row=289;
            column=50;
            i=0;
            j=0;
        }
     
        else if(b[0]=='/'){
            b[0] = fgetc(f);
            print_string(global_get_set(global, b[0]), row+j, column+i+4);
            i+=10+8*strlen(global_get_set(global, b[0]));
            b[0]=' ';
        }
        
        /* Comentario en el texto */
        else if(b[0] == '@'){
            do { b[0] = fgetc(f);
            } while(b[0] != '@');  
        }

        /* Borra y empieza a escribir desde el (1,1) */
        else if (b[0] == '<') {
            wait_for_input(" ");
            print_image("aux_files/Art/blank_text.bmp", 289, 50);
            print_image("aux_files/Art/blank_text2.bmp", 30, 300);
            i = 0;
            j = 0;
        }

        /* Salto de línea */
        else if (b[0] == '>') {
            i = 0;
            if(row==289) j += 8;
            else j+=16;
        }
        
        /* Otros */
        
        print_letter(b, &i, &j, row, column);

    }

    fclose(f);
}


/**
 * @brief Imprime en pantalla el texto enviado en el parámetro *string en la posición indicada
 * 
 * @param string Texto a imprimir
 * @param row Fila donde se imprime
 * @param column Columna donde se imprime
 * 
 */
void print_string(char *string, int row, int column){
    char b[2];
    int i, j, k;
    
    if(!string) return;
    term_move_to(row,column);

    b[1]='\0';

    for(i=k=j=0; (b[0]=string[k])!='\0'; k++){
        if (b[0] == '<') {
            wait_for_input(" ");
            print_image("aux_files/Art/blank_text.bmp", 289, 50);
            print_image("aux_files/Art/blank_text2.bmp", 50, 300);
            i = 0;
            j = 0;
        }

        /* Salto de línea */
        else if (b[0] == '>') {
            i = 0;
            if(row==289) j += 8;
            else j+=16;
        }
        
        print_letter(b, &i, &j, row, column);
    }
}


void* myscanf_animation(void *i){
    while(1){
        print_image("aux_files/Art/barrabaja.bmp", 303,  80+(*((int*)i))*20);
        usleep(500000);
        print_image("aux_files/Art/barrabaja_blank.bmp", 303,  80+(*((int*)i))*20);
        usleep(500000);
    }
}

/**
 * @brief Lee lo que se escribe por pantalla 
 * 
 * @param row Fila en la que se escribe
 * @param column Columna en la que se escribe
 * @param result Donde se guarda el texto
 * 
 */
void myscanf(int row, int column, char *result){
    char b[2], d[30] = "aux_files/Art/"; /*para cargar las imágenes de las letras*/
    int k=0;
    b[1] ='\0';

    pthread_create(&animation_pth, NULL, myscanf_animation, (void*) &k);

    print_image("aux_files/Art/barrabaja.bmp", 303,  80+k*20);
    while(b[0] = fgetc(stdin)) {
        if(b[0] == 10){
            pthread_cancel(animation_pth);
            return;
        }
        
        /* Uso de la tecla de retroceso */
        if((b[0] == 8 || b[0]==127) && k!=0) {
            print_image("aux_files/Art/backspace.bmp", row, column + k*20);
            result[--k]='\0';
            print_image("aux_files/Art/backspace.bmp", row, column + k*20);
        }
        
        else if(k!=0 || b[0]!=' '){
            print_image("aux_files/Art/backspace.bmp", row, column + k*20);
            result[k] = b[0];

            if(b[0] >= 'A' && b[0] <= 'Z') {
                b[0] += 32;
                strcat(d, b);
                strcat(d, b);
                strcat(d, ".bmp");
                print_image(d, row, column + k*20);
                strcpy(d, "aux_files/Art/");
            }

            /* Símbolos de puntuación */
            else if(b[0] == '.') print_image("aux_files/Art/point.bmp", row, column + k*20);
            else if(b[0] == ';') print_image("aux_files/Art/dotandcoma.bmp", row, column + k*20);
            else if(b[0] == ':') print_image("aux_files/Art/twopoints.bmp", row, column + k*20);
            else if(b[0] == '"') print_image("aux_files/Art/doblecomilla.bmp", row, column + k*20);
            else if(b[0] == '?') print_image("aux_files/Art/questionmark.bmp", row, column + k*20);
            else if(b[0] == '!') print_image("aux_files/Art/exclamationmark.bmp", row, column + k*20);
            else if(b[0] == ',') print_image("aux_files/Art/coma.bmp", row, column + k*20);
            else if(b[0] ==  39) print_image("aux_files/Art/comilla.bmp", row, column + k*20);
            else if(b[0] == '*') print_image("aux_files/Art/asterisco.bmp", row, column + k*20);
            else if(b[0] == '(') print_image("aux_files/Art/open_parenthesis.bmp", row, column + k*20);
            else if(b[0] == ')') print_image("aux_files/Art/close_parenthesis.bmp", row, column + k*20);

            /* Minúsculas y números */
            else if ((b[0] >= 'a' && b[0] <= 'z') || (b[0] >= '0' && b[0] <= '9')) {
                strcat(d, b);
                strcat(d, ".bmp");
                print_image(d, row, column + k*20);
                strcpy(d, "aux_files/Art/");
            }
            k++;
            print_image("aux_files/Art/barrabaja.bmp", 303,  80+k*20);
        }
    }
}

int print_intro(char *file){
    FILE *f = fopen(file, "r");
    char b[2];
    int column, row, i, j;

    for (i = 0, j = 0, b[1] = '\0'; (b[0] = fgetc(f)) != EOF; ) {
    
        /* Escribimos en el centro*/
        if(b[0]=='$') {
            print_image("aux_files/Art/full_blank.bmp", 1, 1);
            row= 120;
            column= 90;
            i=0;
            j=0;
        }

        /* Diálogo del mayor*/
        if(b[0]=='-') {
            wait_for_input(" ");
            if(row==120){
                row=40;
                j=0;
                print_image("aux_files/Art/full_blank.bmp", 1, 1);
            }
            row+=20+j;
            column=30;
            i=0;
            j=0;
        }
        
        /* Diálogo del jugador */
        else if(b[0]=='#') {
            wait_for_input(" ");
            
            row+=20+j;
            column=350;
            i=0;
            j=0;
        }

        /* Comentario en el texto */
        else if(b[0] == '@'){
            do { b[0] = fgetc(f);
            } while(b[0] != '@');  
        }

        /* Salto de línea */
        else if (b[0] == '>') {
            i = 0;
            j += 8;
        }
        
        /* Otros */
        print_letter(b, &i, &j, row, column);
    }
    
    fclose(f);
}
