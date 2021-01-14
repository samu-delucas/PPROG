#ifndef _LIB_FUNCT_
#define _LIB_FUNCT_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glut.h>

#include "cube.h"

#ifndef NPC
#define NPC 26
#endif

/*STRUCTURES to make "printing" easier*/

typedef struct _sSticker{
    int num; /*index in the rgb matrix, -1 if there is no sticker*/
    int mat[4]; /*index of vertices in vertices matrix (sorted)*/
}sSticker;

typedef struct _sPiece{
    sSticker st[3]; /*stickers in directions x, y, z (same directions 
                    as described by the structures in cube.h), if the 
                    piece has no sticker in direction i=0,1,2; then 
                    st[i].num==-1*/
}sPiece;

typedef struct _sCube{
    sPiece pc[NPC];/*26 pieces that make the cube, ordered and static*/
}sCube;


/** @brief initializes the whole static sCube structure, 
 * to be used in order for the processes of colouring the 
 * cube in the library's window easier
 * 
 * (allocates memory that can be freed with free_sCube)
 **/
sCube *sCube_init();

/** @brief frees the memory allocated by sCube_init()*/
void free_sCube(sCube *p);

/** @brief wrapper of the functions used to give color to the 
 * sticker s, using the matrix cube and the colours in rgb
 * It does not paint non existing stickers (those with s->num==-1)
 * */
void paint_sSticker(sSticker *s, int option, double **rgb);

/** @brief wrapper of the functions used to give color to the piece p (its three (or less) stickers), using the option (2/3) and the colours in rgb*/
void paint_sPiece(sPiece *p, int option, double **rgb);

/** @brief this function returns 1 if 
 * sPiece i (in cube sCube) is 
 * in face c='R','U','D','L','F','B','M','E','S'
 * and 0 otherwise
 **/
int is_in_face(int i, char c);


void quit(int rc);

/** @brief separates a string into three (maximum)-to be used in SDL_DisplayText*/
char ** separarCadenas(char*cad);

/** @brief discplays the string 'text' in a new window (with a maximum of three lines)*/
void SDL_DisplayText(SDL_Renderer *renderer, char *text, TTF_Font *font);

/** @brief uses SDL_DisplayText to print in a new window the string solution using a font*/
void SDL_DisplayTextWRAPPER(SDL_Window **_window2, char *solution, SDL_Renderer **_renderer, TTF_Font *_font);

void Render(double **s, int option);

/**
 * @brief Render_wr
 * @author Pablo Cuesta
 * @date 2020/11/27
 * wrapper of Render with preparations used in prueba.c 
 *   
 **/
void Render_wr(int *w, int *h, double **stickers, SDL_Window *_window, SDL_GLContext _ctx, int option);

/** @brief rotation of a layer*/
void Rot_layer(float _angle, double **s, char c, int option);

/** @brief rotation of the whole cube*/
void Rot_cube(float _angle, double **s, char c, int option);

/**
 * @brief Rot
 * @author Pablo Cuesta
 * @date 2020/11/27
 * wrapper of Rot_layer/Rot_cube 
 **/
void Rot(float _angle, double **s, char c, int option);

/**
 * @brief Rot_full_move
 * @author Pablo Cuesta
 * @date 2020/11/27
 * wrapper of Rot with modification of the angle inside to 
 * display the full move in char move 
 *   
 **/
void Rot_full_move(int *w, int *h, double **stickers, char move, SDL_Window *_window, SDL_GLContext _ctx, int option);

/**
 * @brief SlowMoveRot
 * @author Pablo Cuesta
 * @date 2020/11/27
 * Displays in screen the full string of moves in 
 * char *moves, made in cube c
 *   
 **/
void SlowMoveRot(Cube3* c, int *w, int *h, double **stickers, char *moves, SDL_Window *_window, SDL_GLContext _ctx, int option);




#endif