#ifndef PRINT_C_H
#define PRINT_C_H

#include "cube.h"
#include "types.h"
#include "interface.h"

#define CUBE_1 "./txt_files/cubo.txt"
#define CUBE_2 "./txt_files/cubo2.txt"
#define CUBE_3 "./txt_files/cubo3.txt"
#define CUBE_222 "./txt_files/cubo222.txt"


typedef int (*cprint_from_stickers)(FILE*, short*);
typedef int (*cprint_from_stickers2)(short *,short*, rect *, rect *, int); 
typedef int (*cprint_from_stickers3)(short *,short*, rect *, char *, int); 
/*pointers to rect*/


int c_print(FILE *f, short *s);

/**
 * @brief : prints cube in FILE making use of the function passed in the last parameter
 * 
 * This function calls colour_stickers(Cube3*, short *) and frees all memory allocated within it
 * 
 * @returns ERROR/OK
*/
Status refresh_cube(Cube3*, FILE*, cprint_from_stickers);

Status refresh_cube2(Cube3 *, rect *,rect*, cprint_from_stickers2);

Status refresh_cube3(Cube3 *c, rect *r1, char *buf, int size, cprint_from_stickers3 print_cube);

/**
 * @brief ALLOCATES an array. Translates the array of colors of the stickers into the color code for ANSI ESC
 * @param s pointer to array of stickers (s[i]=MACRO of a color defined in cubo.c )
 * @return an array with the color for ansi code 
 */
Status sticker_to_color(short *s,short *c);

/**
 * @brief ALLOCATES a matrix. Translates the array of colors of the stickers into the color code for SLD ESC
 * @param s pointer to array of stickers (s[i]=MACRO of a color defined in cubo.c )
 * @return a 56x3 matrix with the rgb code 
 */
double** sticker_colorSDL_init();/*allocates the pointer used in the next function*/
double **sticker_colorSDL(short *s, double **c);

/**
 * @brief Frees the array allocated by the function sticker_colorSDL
 * @param s matrix to be freed
 */
void colorSDL_free(double** s);

/*passes the cube stickers to the rgb matrix stickers*/
void cube_to_SDL(Cube3 *c, double **stickers);

/**
 * @brief prints a cube
 * @param f, file to print in
 * @param s, array of stickers (it is translated with sticker_to_color funct)
 * @return 
 */ 
int c_print2(FILE *f, short *s,short*col);

/**
 * @brief makes the moves in the input waiting delay seconds befor making each move
 * @param c, cube
 * @param pf, file to print in
 * @param print_cube function used to print the cube 
 * @param moves -moves to be made to the cube
 * @param delay -used to regulate the time that it waits
 * @return 
 */ 
Status slow_moves(Cube3* c, cprint_from_stickers2 print_cube, char *moves, int usec, rect*r1, rect*r2);

/*same as slow_moves, but with buffer*/
Status slow_moves2(Cube3* c, cprint_from_stickers3 print_cube, char *moves, int usec,rect* r1, char *buf, int size);

int c_print3(short *s,short *col,rect *r1, rect *r2, int option);

/**
 * @brief cube print with buffers 
 * @param size size of the buffer
 * 
 */
int c_print4(short *s, short *col, rect *r1, char *buf, int size);

#endif 