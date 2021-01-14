/**
 * @file utils.h
 * @author Pablo Cuesta, Álvaro Zamanillo
 */

#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define MAX_BUF (15000)


/*C ANSI CODES FOR COLOR TEXT*/
/*The %c is for the escape, number 27 in decimal in ansi*/
#define HBLK "%c[0;90m"
#define HRED "%c[0;91m"
#define HGRN "%c[0;92m"
#define HYEL "%c[0;93m"
#define HBLU "%c[0;94m"
#define HMAG "%c[0;95m"
#define HCYN "%c[0;96m"
#define HWHT "%c[0;97m"
#define color_reset "%c[0m"

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
int ftobuffer(char *file, char **res);


/**
 * @brief Generates a pseudo-random number between inf and sup
 */
int random_num(int inf, int sup);


/***Return the number of lines & columns of the terminal**/
int get_linesfromterm();
int get_columnsfromterm();
#endif