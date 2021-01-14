/**
 * @author Pablo Cuesta Sierra 
 * 
 */

#ifndef CUBE_H_
#define CUBE_H_

#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include "types.h"


/* 
COLORS: (standard) defined so that the id of a piece is c[0]+c[1]+c[2]
*/

#define W 1 
#define Y 2 
#define R 10 
#define O 20
#define G 100
#define B 200
#define N 0 

#define NPC 26 /*number of pieces*/

#define MAX_LINE 1000

typedef struct{
  short p[3];/*position x,y,z for x,y,z in {-1,0,1}*/
  short c[3];/*color {0...6}  [color axis x, c. axis y, color axis z]*/
  /*the id of a piece is c[0]+c[1]+c[2]*/
}Piece;

typedef struct{
  Piece pc[NPC];
  short stickers[54];  /*Code of color of each sticker*/
  short colorsESC[54]; /*C ANSI ESC COde color*/
  int option; /*2 or 3: 2x2 of 3x3*/
}Cube3;

/**
 * @brief creates new cube alredy solved: 
 *         white on top, green in the front, red to the right
 * @return the cube created
*/
Cube3 *c_init(int option);

/** @brief frees a cube (entered as parameter)*/
void c_free(Cube3*);

/** @brief copies cube c1 into c2, and returns the allocated copy*/
Cube3 *c_copy(Cube3*c1);

/*returns index of piece in pos (x,y,z)*/
int c_iofPos(Cube3 *c, short x, short y, short z);

/*returns index of piece whose id is searchcolors=color1+color2+color3*/
int c_iofCol(Cube3 *c, short searchcolours);

/***************Functions that call moves***************/

/** @brief performs move l in cube c*/
void c_make(Cube3 *c, char l);

/** @brief performs set of moves in string in order */
Status c_moves(Cube3 *c, char *s);


/** @brief returns array of stickers's colours as shown in picture STICKERS.jpeg*/
Status colour_stickers(Cube3* c, short *s);


/** @brief scrambles a cube using a random scramble from a file with 
 * name filename, puts the scramble string in scramble 
 * (without allocating new memory, scramble is suposed 
 * to be big enough to receive the string: 1000 Bytes)*/
Status scramble_cube(Cube3*c, char *filename, char *scramble);


/** @brief prints info of a piece (for testing)*/
int p_print(FILE *, Piece *);

/** @brief Checks if a cube is solved*/
Bool is_solved(Cube3*c);

/** @brief function that returns the color from the centerpiece in position cp: RLFBUD*/
short cfrom(Cube3* cube, char cp);

/** @brief saves cube c in (binary) file save_game
 * option is where wither 3 or 2 is passed (for 3x3 or 2x2)
 * 
 * returns ERROR/OK
 **/
int save_cube(Cube3 *c, char* save_game);

/** @brief reads saved cube into c from (binary) file save_game
 * option is where wither 3 or 2 is stored (for 3x3 or 2x2)
 * 
 * returns ERROR/OK
*/
int read_saved_cube(Cube3 *c, char* save_game);

#endif
