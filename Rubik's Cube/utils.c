/**
 * @file utils.c
 * @author Pablo Cuesta, Álvaro Zamanillo
 * @brief functions to read files of the project to buffers
 *        to avoid constant file reading
 */



#include "utils.h"
#include <stdlib.h>

extern char *strndup(const char *, size_t );

/**
 * @brief This function reads the whole file with name 
 * in string file and stores it in a new char with size
 * just equal to the size of the data in the file. The
 * string will have a '\0' at the end. This function
 * allocates memory for the new string.
 * If the file does not exist, it returns NULL, 
 * and does not create it.
 * The maximum size to be read is MAX_BUF
*/
int ftobuffer(char *file, char **res){
    FILE *pfrom = NULL;
    char buf[MAX_BUF], *new = NULL;
    int count;

    if (file == NULL)
        return -1;

    if (!(pfrom = fopen(file, "r"))){
        printf("Could not open %s\n", file);
        return -1;
    }
    count = fread(buf, sizeof(char), MAX_BUF, pfrom);

   
    new = strndup(buf, count);

    fclose(pfrom);
    *res = new;

    if (res == NULL)
        return -1;

    return count;
}

int random_num(int inf, int sup){
    double t;

    if (inf > sup){
        return -1;
    }

    t = (double)rand() / ((double)RAND_MAX + 1.);

    return inf + ((int)(t * (sup - inf + 1)));
}


int get_linesfromterm(){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return  w.ws_row; 
}

int get_columnsfromterm(){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}