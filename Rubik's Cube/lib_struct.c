/**
 * @author Pablo Cuesta Sierra 
 * @date 2020/11/23
 * 
 */

#ifndef _LIB_STRUCTS_
#define _LIB_STRUCTS_

#include "lib_funct.h"

#define U_INDEXES {0, 1, 2, 3, 4, 5, 6, 7, 8}
#define E_INDEXES {9, 10, 11, 12, 13, 14, 15, 16, -1}
#define D_INDEXES {17, 18, 19, 20, 21, 22, 23, 24, 25}
#define R_INDEXES {2, 5, 8, 11, 12, 13, 19, 20, 21 }
#define L_INDEXES { 0, 3, 6, 9, 16, 15, 17, 24, 23 }
#define M_INDEXES {1, 4, 7, 10, 14, 18, 25, 22, -1}
#define F_INDEXES {6, 7, 8, 15, 14, 13, 23, 22, 21 }
#define B_INDEXES { 0, 1, 2, 9, 10, 11, 17, 18, 19 }
#define S_INDEXES {3, 4, 5, 16, 12, 24, 25, 20, -1 }
#define BLACK_INDEXES {3, 4, 5, 16, 12, 24, 25, 20, -1}
#define SIZE_FACE 9

/*matrices of the vertices of the cubes*/
float cube[56][3] = {
        /*Cara 1*/
        {0.6, 0.6, 0.6},
        {0.6, 0.6, 0.2},
        {0.6, 0.6, -0.2},
        {0.6, 0.6, -0.6},
        {0.6, 0.2, -0.6},
        {0.6, -0.2, -0.6},
        {0.6, -0.6, -0.6},
        {0.6, -0.6, -0.2},
        {0.6, -0.6, 0.2},
        {0.6, -0.6, 0.6},
        {0.6, -0.2, 0.6},
        {0.6, 0.2, 0.6},
        {0.6, 0.2, 0.2},
        {0.6, 0.2, -0.2},
        {0.6, -0.2, -0.2},
        {0.6, -0.2, 0.2},

        /*Cara 2*/
        {0.2, 0.6, 0.6},
        {-0.2, 0.6, 0.6},
        {-0.6, 0.6, 0.6},
        {-0.6, 0.6, 0.2},
        {-0.6, 0.6, -0.2},
        {-0.6, 0.6, -0.6},
        {-0.2, 0.6, -0.6}, /*22*/
        {0.2, 0.6, -0.6},
        {0.2, 0.6, -0.2},
        {0.2, 0.6, 0.2},
        {-0.2, 0.6, 0.2},
        {-0.2, 0.6, -0.2}, /*27*/

        /*Cara 3*/
        {-0.6, 0.2, 0.6},
        {-0.6, 0.2, 0.2},
        {-0.6, 0.2, -0.2},
        {-0.6, 0.2, -0.6},
        {-0.6, -0.2, -0.6},
        {-0.6, -0.6, -0.6}, /*33*/
        {-0.6, -0.6, -0.2},
        {-0.6, -0.6, 0.2},
        {-0.6, -0.6, 0.6},
        {-0.6, -0.2, 0.6},
        {-0.6, -0.2, 0.2},
        {-0.6, -0.2, -0.2}, /*39*/

        /*Cara 4*/
        {-0.2, -0.6, 0.6},
        {-0.2, -0.6, 0.2},
        {-0.2, -0.6, -0.2},
        {-0.2, -0.6, -0.6},
        {0.2, -0.6, -0.6}, /*44*/
        {0.2, -0.6, -0.2},
        {0.2, -0.6, 0.2},
        {0.2, -0.6, 0.6}, /*47*/

        /*Cara 5*/
        {0.2, 0.2, 0.6},
        {0.2, -0.2, 0.6},
        {-0.2, -0.2, 0.6},
        {-0.2, 0.2, 0.6},

        /*Cara 6*/
        {0.2, 0.2, -0.6},
        {0.2, -0.2, -0.6},
        {-0.2, -0.2, -0.6},
        {-0.2, 0.2, -0.6}

    };

float cube2[56][3]={
    {0.6000, 0.6000, 0.6000},
    {0.6000, 0.6000, 0.0000},
    {0.6000, 0.6000, 0.0000},
    {0.6000, 0.6000, -0.6000},
    {0.6000, 0.0000, -0.6000},
    {0.6000, 0.0000, -0.6000},
    {0.6000, -0.6000, -0.6000},
    {0.6000, -0.6000, 0.0000},
    {0.6000, -0.6000, 0.0000},
    {0.6000, -0.6000, 0.6000},
    {0.6000, 0.0000, 0.6000},
    {0.6000, 0.0000, 0.6000},
    {0.6000, 0.0000, 0.0000},
    {0.6000, 0.0000, 0.0000},
    {0.6000, 0.0000, 0.0000},
    {0.6000, 0.0000, 0.0000},
    {0.0000, 0.6000, 0.6000},
    {0.0000, 0.6000, 0.6000},
    {-0.6000, 0.6000, 0.6000},
    {-0.6000, 0.6000, 0.0000},
    {-0.6000, 0.6000, 0.0000},
    {-0.6000, 0.6000, -0.6000},
    {0.0000, 0.6000, -0.6000},
    {0.0000, 0.6000, -0.6000},
    {0.0000, 0.6000, 0.0000},
    {0.0000, 0.6000, 0.0000},
    {0.0000, 0.6000, 0.0000},
    {0.0000, 0.6000, 0.0000},
    {-0.6000, 0.0000, 0.6000},
    {-0.6000, 0.0000, 0.0000},
    {-0.6000, 0.0000, 0.0000},
    {-0.6000, 0.0000, -0.6000},
    {-0.6000, 0.0000, -0.6000},
    {-0.6000, -0.6000, -0.6000},
    {-0.6000, -0.6000, 0.0000},
    {-0.6000, -0.6000, 0.0000},
    {-0.6000, -0.6000, 0.6000},
    {-0.6000, 0.0000, 0.6000},
    {-0.6000, 0.0000, 0.0000},
    {-0.6000, 0.0000, 0.0000},
    {0.0000, -0.6000, 0.6000},
    {0.0000, -0.6000, 0.0000},
    {0.0000, -0.6000, 0.0000},
    {0.0000, -0.6000, -0.6000},
    {0.0000, -0.6000, -0.6000},
    {0.0000, -0.6000, 0.0000},
    {0.0000, -0.6000, 0.0000},
    {0.0000, -0.6000, 0.6000},
    {0.0000, 0.0000, 0.6000},
    {0.0000, 0.0000, 0.6000},
    {0.0000, 0.0000, 0.6000},
    {0.0000, 0.0000, 0.6000},
    {0.0000, 0.0000, -0.6000},
    {0.0000, 0.0000, -0.6000},
    {0.0000, 0.0000, -0.6000},
    {0.0000, 0.0000, -0.6000},
};



/*
    auxiliary matrices (lines) for the 3x3 
    these are almost exactly like cube[56][3]
    but slightly off so that they can be used to print the lines between the stickers
*/
float cube_p[56][3]={
    {0.6001, 0.6001, 0.6001},
    {0.6001, 0.6001, 0.2100},
    {0.6001, 0.6001, -0.2100},
    {0.6001, 0.6001, -0.6001},
    {0.6001, 0.2100, -0.6001},
    {0.6001, -0.2100, -0.6001},
    {0.6001, -0.6001, -0.6001},
    {0.6001, -0.6001, -0.2100},
    {0.6001, -0.6001, 0.2100},
    {0.6001, -0.6001, 0.6001},
    {0.6001, -0.2100, 0.6001},
    {0.6001, 0.2100, 0.6001},
    {0.6001, 0.2100, 0.2100},
    {0.6001, 0.2100, -0.2100},
    {0.6001, -0.2100, -0.2100},
    {0.6001, -0.2100, 0.2100},
    {0.2100, 0.6001, 0.6001},
    {-0.2100, 0.6001, 0.6001},
    {-0.6001, 0.6001, 0.6001},
    {-0.6001, 0.6001, 0.2100},
    {-0.6001, 0.6001, -0.2100},
    {-0.6001, 0.6001, -0.6001},
    {-0.2100, 0.6001, -0.6001},
    {0.2100, 0.6001, -0.6001},
    {0.2100, 0.6001, -0.2100},
    {0.2100, 0.6001, 0.2100},
    {-0.2100, 0.6001, 0.2100},
    {-0.2100, 0.6001, -0.2100},
    {-0.6001, 0.2100, 0.6001},
    {-0.6001, 0.2100, 0.2100},
    {-0.6001, 0.2100, -0.2100},
    {-0.6001, 0.2100, -0.6001},
    {-0.6001, -0.2100, -0.6001},
    {-0.6001, -0.6001, -0.6001},
    {-0.6001, -0.6001, -0.2100},
    {-0.6001, -0.6001, 0.2100},
    {-0.6001, -0.6001, 0.6001},
    {-0.6001, -0.2100, 0.6001},
    {-0.6001, -0.2100, 0.2100},
    {-0.6001, -0.2100, -0.2100},
    {-0.2100, -0.6001, 0.6001},
    {-0.2100, -0.6001, 0.2100},
    {-0.2100, -0.6001, -0.2100},
    {-0.2100, -0.6001, -0.6001},
    {0.2100, -0.6001, -0.6001},
    {0.2100, -0.6001, -0.2100},
    {0.2100, -0.6001, 0.2100},
    {0.2100, -0.6001, 0.6001},
    {0.2100, 0.2100, 0.6001},
    {0.2100, -0.2100, 0.6001},
    {-0.2100, -0.2100, 0.6001},
    {-0.2100, 0.2100, 0.6001},
    {0.2100, 0.2100, -0.6001},
    {0.2100, -0.2100, -0.6001},
    {-0.2100, -0.2100, -0.6001},
    {-0.2100, 0.2100, -0.6001},
};

float cube_q[56][3]={
    {0.6001, 0.6001, 0.6001},
    {0.6001, 0.6001, 0.1900},
    {0.6001, 0.6001, -0.1900},
    {0.6001, 0.6001, -0.6001},
    {0.6001, 0.1900, -0.6001},
    {0.6001, -0.1900, -0.6001},
    {0.6001, -0.6001, -0.6001},
    {0.6001, -0.6001, -0.1900},
    {0.6001, -0.6001, 0.1900},
    {0.6001, -0.6001, 0.6001},
    {0.6001, -0.1900, 0.6001},
    {0.6001, 0.1900, 0.6001},
    {0.6001, 0.1900, 0.1900},
    {0.6001, 0.1900, -0.1900},
    {0.6001, -0.1900, -0.1900},
    {0.6001, -0.1900, 0.1900},
    {0.1900, 0.6001, 0.6001},
    {-0.1900, 0.6001, 0.6001},
    {-0.6001, 0.6001, 0.6001},
    {-0.6001, 0.6001, 0.1900},
    {-0.6001, 0.6001, -0.1900},
    {-0.6001, 0.6001, -0.6001},
    {-0.1900, 0.6001, -0.6001},
    {0.1900, 0.6001, -0.6001},
    {0.1900, 0.6001, -0.1900},
    {0.1900, 0.6001, 0.1900},
    {-0.1900, 0.6001, 0.1900},
    {-0.1900, 0.6001, -0.1900},
    {-0.6001, 0.1900, 0.6001},
    {-0.6001, 0.1900, 0.1900},
    {-0.6001, 0.1900, -0.1900},
    {-0.6001, 0.1900, -0.6001},
    {-0.6001, -0.1900, -0.6001},
    {-0.6001, -0.6001, -0.6001},
    {-0.6001, -0.6001, -0.1900},
    {-0.6001, -0.6001, 0.1900},
    {-0.6001, -0.6001, 0.6001},
    {-0.6001, -0.1900, 0.6001},
    {-0.6001, -0.1900, 0.1900},
    {-0.6001, -0.1900, -0.1900},
    {-0.1900, -0.6001, 0.6001},
    {-0.1900, -0.6001, 0.1900},
    {-0.1900, -0.6001, -0.1900},
    {-0.1900, -0.6001, -0.6001},
    {0.1900, -0.6001, -0.6001},
    {0.1900, -0.6001, -0.1900},
    {0.1900, -0.6001, 0.1900},
    {0.1900, -0.6001, 0.6001},
    {0.1900, 0.1900, 0.6001},
    {0.1900, -0.1900, 0.6001},
    {-0.1900, -0.1900, 0.6001},
    {-0.1900, 0.1900, 0.6001},
    {0.1900, 0.1900, -0.6001},
    {0.1900, -0.1900, -0.6001},
    {-0.1900, -0.1900, -0.6001},
    {-0.1900, 0.1900, -0.6001},
};

float cube2_p[56][3]={
    {0.6001, 0.6001, 0.6001},
    {0.6001, 0.6001, 0.0100},
    {0.6001, 0.6001, -0.0100},
    {0.6001, 0.6001, -0.6001},
    {0.6001, 0.0100, -0.6001},
    {0.6001, -0.0100, -0.6001},
    {0.6001, -0.6001, -0.6001},
    {0.6001, -0.6001, -0.0100},
    {0.6001, -0.6001, 0.0100},
    {0.6001, -0.6001, 0.6001},
    {0.6001, -0.0100, 0.6001},
    {0.6001, 0.0100, 0.6001},
    {0.6001, 0.0100, 0.0100},
    {0.6001, 0.0100, -0.0100},
    {0.6001, -0.0100, -0.0100},
    {0.6001, -0.0100, 0.0100},
    {0.0100, 0.6001, 0.6001},
    {-0.0100, 0.6001, 0.6001},
    {-0.6001, 0.6001, 0.6001},
    {-0.6001, 0.6001, 0.0100},
    {-0.6001, 0.6001, -0.0100},
    {-0.6001, 0.6001, -0.6001},
    {-0.0100, 0.6001, -0.6001},
    {0.0100, 0.6001, -0.6001},
    {0.0100, 0.6001, -0.0100},
    {0.0100, 0.6001, 0.0100},
    {-0.0100, 0.6001, 0.0100},
    {-0.0100, 0.6001, -0.0100},
    {-0.6001, 0.0100, 0.6001},
    {-0.6001, 0.0100, 0.0100},
    {-0.6001, 0.0100, -0.0100},
    {-0.6001, 0.0100, -0.6001},
    {-0.6001, -0.0100, -0.6001},
    {-0.6001, -0.6001, -0.6001},
    {-0.6001, -0.6001, -0.0100},
    {-0.6001, -0.6001, 0.0100},
    {-0.6001, -0.6001, 0.6001},
    {-0.6001, -0.0100, 0.6001},
    {-0.6001, -0.0100, 0.0100},
    {-0.6001, -0.0100, -0.0100},
    {-0.0100, -0.6001, 0.6001},
    {-0.0100, -0.6001, 0.0100},
    {-0.0100, -0.6001, -0.0100},
    {-0.0100, -0.6001, -0.6001},
    {0.0100, -0.6001, -0.6001},
    {0.0100, -0.6001, -0.0100},
    {0.0100, -0.6001, 0.0100},
    {0.0100, -0.6001, 0.6001},
    {0.0100, 0.0100, 0.6001},
    {0.0100, -0.0100, 0.6001},
    {-0.0100, -0.0100, 0.6001},
    {-0.0100, 0.0100, 0.6001},
    {0.0100, 0.0100, -0.6001},
    {0.0100, -0.0100, -0.6001},
    {-0.0100, -0.0100, -0.6001},
    {-0.0100, 0.0100, -0.6001},
};

float cube2_q[56][3]={
    {0.6001, 0.6001, 0.6001},
    {0.6001, 0.6001, -0.0100},
    {0.6001, 0.6001, 0.0100},
    {0.6001, 0.6001, -0.6001},
    {0.6001, -0.0100, -0.6001},
    {0.6001, 0.0100, -0.6001},
    {0.6001, -0.6001, -0.6001},
    {0.6001, -0.6001, 0.0100},
    {0.6001, -0.6001, -0.0100},
    {0.6001, -0.6001, 0.6001},
    {0.6001, 0.0100, 0.6001},
    {0.6001, -0.0100, 0.6001},
    {0.6001, -0.0100, -0.0100},
    {0.6001, -0.0100, 0.0100},
    {0.6001, 0.0100, 0.0100},
    {0.6001, 0.0100, -0.0100},
    {-0.0100, 0.6001, 0.6001},
    {0.0100, 0.6001, 0.6001},
    {-0.6001, 0.6001, 0.6001},
    {-0.6001, 0.6001, -0.0100},
    {-0.6001, 0.6001, 0.0100},
    {-0.6001, 0.6001, -0.6001},
    {0.0100, 0.6001, -0.6001},
    {-0.0100, 0.6001, -0.6001},
    {-0.0100, 0.6001, 0.0100},
    {-0.0100, 0.6001, -0.0100},
    {0.0100, 0.6001, -0.0100},
    {0.0100, 0.6001, 0.0100},
    {-0.6001, -0.0100, 0.6001},
    {-0.6001, -0.0100, -0.0100},
    {-0.6001, -0.0100, 0.0100},
    {-0.6001, -0.0100, -0.6001},
    {-0.6001, 0.0100, -0.6001},
    {-0.6001, -0.6001, -0.6001},
    {-0.6001, -0.6001, 0.0100},
    {-0.6001, -0.6001, -0.0100},
    {-0.6001, -0.6001, 0.6001},
    {-0.6001, 0.0100, 0.6001},
    {-0.6001, 0.0100, -0.0100},
    {-0.6001, 0.0100, 0.0100},
    {0.0100, -0.6001, 0.6001},
    {0.0100, -0.6001, -0.0100},
    {0.0100, -0.6001, 0.0100},
    {0.0100, -0.6001, -0.6001},
    {-0.0100, -0.6001, -0.6001},
    {-0.0100, -0.6001, 0.0100},
    {-0.0100, -0.6001, -0.0100},
    {-0.0100, -0.6001, 0.6001},
    {-0.0100, -0.0100, 0.6001},
    {-0.0100, 0.0100, 0.6001},
    {0.0100, 0.0100, 0.6001},
    {0.0100, -0.0100, 0.6001},
    {-0.0100, -0.0100, -0.6001},
    {-0.0100, 0.0100, -0.6001},
    {0.0100, 0.0100, -0.6001},
    {0.0100, -0.0100, -0.6001},
};

/*
    auxiliary matrices, for the internal cube(s), this is the same thing as cube[56][3] 
    but all of the stickers are closer to the center of the cube (it is like the 
    internal side of each sticker)
*/
float cube_internal[56][3]={
    {0.5999, 0.5999, 0.5999},
    {0.5999, 0.5999, 0.2000},
    {0.5999, 0.5999, -0.2000},
    {0.5999, 0.5999, -0.5999},
    {0.5999, 0.2000, -0.5999},
    {0.5999, -0.2000, -0.5999},
    {0.5999, -0.5999, -0.5999},
    {0.5999, -0.5999, -0.2000},
    {0.5999, -0.5999, 0.2000},
    {0.5999, -0.5999, 0.5999},
    {0.5999, -0.2000, 0.5999},
    {0.5999, 0.2000, 0.5999},
    {0.5999, 0.2000, 0.2000},
    {0.5999, 0.2000, -0.2000},
    {0.5999, -0.2000, -0.2000},
    {0.5999, -0.2000, 0.2000},
    {0.2000, 0.5999, 0.5999},
    {-0.2000, 0.5999, 0.5999},
    {-0.5999, 0.5999, 0.5999},
    {-0.5999, 0.5999, 0.2000},
    {-0.5999, 0.5999, -0.2000},
    {-0.5999, 0.5999, -0.5999},
    {-0.2000, 0.5999, -0.5999},
    {0.2000, 0.5999, -0.5999},
    {0.2000, 0.5999, -0.2000},
    {0.2000, 0.5999, 0.2000},
    {-0.2000, 0.5999, 0.2000},
    {-0.2000, 0.5999, -0.2000},
    {-0.5999, 0.2000, 0.5999},
    {-0.5999, 0.2000, 0.2000},
    {-0.5999, 0.2000, -0.2000},
    {-0.5999, 0.2000, -0.5999},
    {-0.5999, -0.2000, -0.5999},
    {-0.5999, -0.5999, -0.5999},
    {-0.5999, -0.5999, -0.2000},
    {-0.5999, -0.5999, 0.2000},
    {-0.5999, -0.5999, 0.5999},
    {-0.5999, -0.2000, 0.5999},
    {-0.5999, -0.2000, 0.2000},
    {-0.5999, -0.2000, -0.2000},
    {-0.2000, -0.5999, 0.5999},
    {-0.2000, -0.5999, 0.2000},
    {-0.2000, -0.5999, -0.2000},
    {-0.2000, -0.5999, -0.5999},
    {0.2000, -0.5999, -0.5999},
    {0.2000, -0.5999, -0.2000},
    {0.2000, -0.5999, 0.2000},
    {0.2000, -0.5999, 0.5999},
    {0.2000, 0.2000, 0.5999},
    {0.2000, -0.2000, 0.5999},
    {-0.2000, -0.2000, 0.5999},
    {-0.2000, 0.2000, 0.5999},
    {0.2000, 0.2000, -0.5999},
    {0.2000, -0.2000, -0.5999},
    {-0.2000, -0.2000, -0.5999},
    {-0.2000, 0.2000, -0.5999},
};

float cube2_internal[56][3]={
    {0.5999, 0.5999, 0.5999},
    {0.5999, 0.5999, 0.0000},
    {0.5999, 0.5999, 0.0000},
    {0.5999, 0.5999, -0.5999},
    {0.5999, 0.0000, -0.5999},
    {0.5999, 0.0000, -0.5999},
    {0.5999, -0.5999, -0.5999},
    {0.5999, -0.5999, 0.0000},
    {0.5999, -0.5999, 0.0000},
    {0.5999, -0.5999, 0.5999},
    {0.5999, 0.0000, 0.5999},
    {0.5999, 0.0000, 0.5999},
    {0.5999, 0.0000, 0.0000},
    {0.5999, 0.0000, 0.0000},
    {0.5999, 0.0000, 0.0000},
    {0.5999, 0.0000, 0.0000},
    {0.0000, 0.5999, 0.5999},
    {0.0000, 0.5999, 0.5999},
    {-0.5999, 0.5999, 0.5999},
    {-0.5999, 0.5999, 0.0000},
    {-0.5999, 0.5999, 0.0000},
    {-0.5999, 0.5999, -0.5999},
    {0.0000, 0.5999, -0.5999},
    {0.0000, 0.5999, -0.5999},
    {0.0000, 0.5999, 0.0000},
    {0.0000, 0.5999, 0.0000},
    {0.0000, 0.5999, 0.0000},
    {0.0000, 0.5999, 0.0000},
    {-0.5999, 0.0000, 0.5999},
    {-0.5999, 0.0000, 0.0000},
    {-0.5999, 0.0000, 0.0000},
    {-0.5999, 0.0000, -0.5999},
    {-0.5999, 0.0000, -0.5999},
    {-0.5999, -0.5999, -0.5999},
    {-0.5999, -0.5999, 0.0000},
    {-0.5999, -0.5999, 0.0000},
    {-0.5999, -0.5999, 0.5999},
    {-0.5999, 0.0000, 0.5999},
    {-0.5999, 0.0000, 0.0000},
    {-0.5999, 0.0000, 0.0000},
    {0.0000, -0.5999, 0.5999},
    {0.0000, -0.5999, 0.0000},
    {0.0000, -0.5999, 0.0000},
    {0.0000, -0.5999, -0.5999},
    {0.0000, -0.5999, -0.5999},
    {0.0000, -0.5999, 0.0000},
    {0.0000, -0.5999, 0.0000},
    {0.0000, -0.5999, 0.5999},
    {0.0000, 0.0000, 0.5999},
    {0.0000, 0.0000, 0.5999},
    {0.0000, 0.0000, 0.5999},
    {0.0000, 0.0000, 0.5999},
    {0.0000, 0.0000, -0.5999},
    {0.0000, 0.0000, -0.5999},
    {0.0000, 0.0000, -0.5999},
    {0.0000, 0.0000, -0.5999},
};

/*aux function to paint the lines of the stiker*/
void paint_sStickerLines(sSticker *s, int option, double **rgb){
    
    if(option==2){
        /*lines*/
        glColor3f(0, 0, 0);
        glVertex3fv(cube2_q[s->mat[0]]);
        glVertex3fv(cube2_q[s->mat[1]]);
        glVertex3fv(cube2_p[s->mat[1]]);
        glVertex3fv(cube2_p[s->mat[0]]);

        glColor3f(0, 0, 0);
        glVertex3fv(cube2_q[s->mat[1]]);
        glVertex3fv(cube2_q[s->mat[2]]);
        glVertex3fv(cube2_p[s->mat[2]]);
        glVertex3fv(cube2_p[s->mat[1]]);

        glColor3f(0, 0, 0);
        glVertex3fv(cube2_q[s->mat[2]]);
        glVertex3fv(cube2_q[s->mat[3]]);
        glVertex3fv(cube2_p[s->mat[3]]);
        glVertex3fv(cube2_p[s->mat[2]]);

        glColor3f(0, 0, 0);
        glVertex3fv(cube2_q[s->mat[3]]);
        glVertex3fv(cube2_q[s->mat[0]]);
        glVertex3fv(cube2_p[s->mat[0]]);
        glVertex3fv(cube2_p[s->mat[3]]);
        return;
    }
    /*lines*/
    glColor3f(0, 0, 0);
    glVertex3fv(cube_q[s->mat[0]]);
    glVertex3fv(cube_q[s->mat[1]]);
    glVertex3fv(cube_p[s->mat[1]]);
    glVertex3fv(cube_p[s->mat[0]]);

    glColor3f(0, 0, 0);
    glVertex3fv(cube_q[s->mat[1]]);
    glVertex3fv(cube_q[s->mat[2]]);
    glVertex3fv(cube_p[s->mat[2]]);
    glVertex3fv(cube_p[s->mat[1]]);

    glColor3f(0, 0, 0);
    glVertex3fv(cube_q[s->mat[2]]);
    glVertex3fv(cube_q[s->mat[3]]);
    glVertex3fv(cube_p[s->mat[3]]);
    glVertex3fv(cube_p[s->mat[2]]);

    glColor3f(0, 0, 0);
    glVertex3fv(cube_q[s->mat[3]]);
    glVertex3fv(cube_q[s->mat[0]]);
    glVertex3fv(cube_p[s->mat[0]]);
    glVertex3fv(cube_p[s->mat[3]]);
}


void paint_sSticker(sSticker *s, int option, double **rgb)
{
    

    if ((s->num) == -1){
        return;
    }
    switch(option){
        case 2:{
            glColor3f(rgb[s->num][0], rgb[s->num][1], rgb[s->num][2]);
            glVertex3fv(cube2[s->mat[0]]);
            glVertex3fv(cube2[s->mat[1]]);
            glVertex3fv(cube2[s->mat[2]]);
            glVertex3fv(cube2[s->mat[3]]);

            /*paint internal side of the sticker black*/
            glColor3f(0, 0, 0);
            glVertex3fv(cube2_internal[s->mat[0]]);
            glVertex3fv(cube2_internal[s->mat[1]]);
            glVertex3fv(cube2_internal[s->mat[2]]);
            glVertex3fv(cube2_internal[s->mat[3]]);
            break;
        }
        default:{
            glColor3f(rgb[s->num][0], rgb[s->num][1], rgb[s->num][2]);
            glVertex3fv(cube[s->mat[0]]);
            glVertex3fv(cube[s->mat[1]]);
            glVertex3fv(cube[s->mat[2]]);
            glVertex3fv(cube[s->mat[3]]);

            /*paint internal side of the sticker black*/
            glColor3f(0, 0, 0);
            glVertex3fv(cube_internal[s->mat[0]]);
            glVertex3fv(cube_internal[s->mat[1]]);
            glVertex3fv(cube_internal[s->mat[2]]);
            glVertex3fv(cube_internal[s->mat[3]]);
        }
    }

    /*paint the lines*/
    paint_sStickerLines(s, option, rgb);
    
}

void paint_sPiece(sPiece *p, int option, double **rgb)
{
    paint_sSticker(&(p->st[0]), option, rgb);
    paint_sSticker(&(p->st[1]), option, rgb);
    paint_sSticker(&(p->st[2]), option, rgb);
}


/**
 * this function returns 1 if 
 * sPiece i (in cube sCube) is 
 * in face c='R','U','D','L','F','B','M','E','S'
 * and 0 otherwise
 **/
int is_in_face(int i, char c)
{
    char inU[] = U_INDEXES, inD[] = D_INDEXES, inE[] = E_INDEXES;
    char inR[] = R_INDEXES, inL[] = L_INDEXES, inM[] = M_INDEXES;
    char inF[] = F_INDEXES, inB[] = B_INDEXES, inS[] = S_INDEXES;
    char pc[SIZE_FACE];
    int j = 0; /*because memcpy works with char*/

    if (i < 0 || i >= 26)
        return 0;

    switch (c)
    {
    case 'U':
    case 'u':
        memcpy(pc, inU, SIZE_FACE);
        break;
    case 'D':
    case 'd':
        memcpy(pc, inD, SIZE_FACE);
        break;
    case 'E':
    case 'e':
        memcpy(pc, inE, SIZE_FACE);
        break;
    case 'R':
    case 'r':
        memcpy(pc, inR, SIZE_FACE);
        break;
    case 'L':
    case 'l':
        memcpy(pc, inL, SIZE_FACE);
        break;
    case 'M':
    case 'm':
        memcpy(pc, inM, SIZE_FACE);
        break;
    case 'B':
    case 'b':
        memcpy(pc, inB, SIZE_FACE);
        break;
    case 'F':
    case 'f':
        memcpy(pc, inF, SIZE_FACE);
        break;
    case 'S':
    case 's':
        memcpy(pc, inS, SIZE_FACE);
        break;
    default:
        return 0;
    }
    for (j = 0; j < SIZE_FACE; j++)
    {
        if ((char)i == pc[j])
            return 1;
    }
    return 0;
}

/*aux. function to declare the cube more easily (used in sCube_init() )*/
void declare_sticker(sCube *sc, int pindex, int sindex, int num, int v1, int v2, int v3, int v4)
{
    sc->pc[pindex].st[sindex].num = num;
    sc->pc[pindex].st[sindex].mat[0] = v1;
    sc->pc[pindex].st[sindex].mat[1] = v2;
    sc->pc[pindex].st[sindex].mat[2] = v3;
    sc->pc[pindex].st[sindex].mat[3] = v4;
}

/*declares pieces as in piezas_lib_struct.jpeg*/
sCube *sCube_init()
{
    sCube *sc = NULL;

    if ((sc = (sCube *)calloc(1, sizeof(sCube))) == NULL)
        return NULL;

    /*declaration of the whole cube: */
    /*piece: UBL */
    declare_sticker(sc, 0, 0, 29, 44, 45, 7, 6);
    declare_sticker(sc, 0, 1, 36, 44, 6, 5, 53);
    declare_sticker(sc, 0, 2, 0, 5, 6, 7, 14);
    /*piece: UB */
    declare_sticker(sc, 1, 0, 28, 45, 46, 8, 7);
    declare_sticker(sc, 1, 1, -1, 44, 6, 5, 53);
    declare_sticker(sc, 1, 2, 1, 7, 8, 15, 14);
    /*piece: UBR */
    declare_sticker(sc, 2, 0, 27, 46, 47, 9, 8);
    declare_sticker(sc, 2, 1, 20, 9, 47, 49, 10);
    declare_sticker(sc, 2, 2, 2, 8, 9, 10, 15);
    /*piece: UL */
    declare_sticker(sc, 3, 0, -1, 44, 45, 7, 6);
    declare_sticker(sc, 3, 1, 37, 53, 5, 4, 52);
    declare_sticker(sc, 3, 2, 3, 4, 5, 14, 13);
    /*piece: U */
    declare_sticker(sc, 4, 0, -1, 44, 45, 7, 6);
    declare_sticker(sc, 4, 1, -1, 44, 6, 5, 53);
    declare_sticker(sc, 4, 2, 4, 12, 13, 14, 15);
    /*piece: UR */
    declare_sticker(sc, 5, 0, -1, 44, 45, 7, 6);
    declare_sticker(sc, 5, 1, 19, 10, 49, 48, 11);
    declare_sticker(sc, 5, 2, 5, 10, 11, 12, 15);
    /*piece: UFL */
    declare_sticker(sc, 6, 0, 9, 2, 3, 23, 24);
    declare_sticker(sc, 6, 1, 38, 52, 4, 3, 23);
    declare_sticker(sc, 6, 2, 6, 2, 3, 4, 13);
    /*piece: UF */
    declare_sticker(sc, 7, 0, 10, 1, 2, 24, 25);
    declare_sticker(sc, 7, 1, -1, 44, 6, 5, 53);
    declare_sticker(sc, 7, 2, 7, 1, 2, 13, 12);
    /*piece: UFR */
    declare_sticker(sc, 8, 0, 11, 0, 1, 25, 16);
    declare_sticker(sc, 8, 1, 18, 11, 48, 16, 0);
    declare_sticker(sc, 8, 2, 8, 0, 1, 12, 11);

    /*piece: BL */
    declare_sticker(sc, 9, 0, 32, 43, 42, 45, 44);
    declare_sticker(sc, 9, 1, 39, 43, 44, 53, 54);
    declare_sticker(sc, 9, 2, -1, 5, 6, 7, 14);
    /*piece: B */
    declare_sticker(sc, 10, 0, 31, 42, 41, 46, 45);
    declare_sticker(sc, 10, 1, -1, 44, 6, 5, 53);
    declare_sticker(sc, 10, 2, -1, 5, 6, 7, 14);
    /*piece: BR */
    declare_sticker(sc, 11, 0, 30, 41, 40, 47, 46);
    declare_sticker(sc, 11, 1, 23, 47, 40, 50, 49);
    declare_sticker(sc, 11, 2, -1, 5, 6, 7, 14);
    /*piece: R */
    declare_sticker(sc, 12, 0, -1, 44, 45, 7, 6);
    declare_sticker(sc, 12, 1, 22, 51, 50, 49, 48);
    declare_sticker(sc, 12, 2, -1, 5, 6, 7, 14);
    /*piece: RF */
    declare_sticker(sc, 13, 0, 14, 25, 16, 17, 26);
    declare_sticker(sc, 13, 1, 21, 48, 51, 17, 16);
    declare_sticker(sc, 13, 2, -1, 5, 6, 7, 14);
    /*piece: F */
    declare_sticker(sc, 14, 0, 13, 24, 25, 26, 27);
    declare_sticker(sc, 14, 1, -1, 44, 6, 5, 53);
    declare_sticker(sc, 14, 2, -1, 5, 6, 7, 14);
    /*piece: FL */
    declare_sticker(sc, 15, 0, 12, 23, 24, 27, 22);
    declare_sticker(sc, 15, 1, 41, 55, 52, 23, 22);
    declare_sticker(sc, 15, 2, -1, 5, 6, 7, 14);
    /*piece: L */
    declare_sticker(sc, 16, 0, -1, 44, 45, 7, 6);
    declare_sticker(sc, 16, 1, 40, 53, 54, 55, 52);
    declare_sticker(sc, 16, 2, -1, 5, 6, 7, 14);

    /*piece: DBL */
    declare_sticker(sc, 17, 0, 35, 33, 34, 42, 43);
    declare_sticker(sc, 17, 1, 42, 33, 43, 54, 32);
    declare_sticker(sc, 17, 2, 51, 32, 39, 34, 33);
    /*piece: DB */
    declare_sticker(sc, 18, 0, 34, 34, 35, 41, 42);
    declare_sticker(sc, 18, 1, -1, 44, 6, 5, 53);
    declare_sticker(sc, 18, 2, 52, 39, 38, 35, 34);
    /*piece: DBR */
    declare_sticker(sc, 19, 0, 33, 35, 36, 40, 41);
    declare_sticker(sc, 19, 1, 26, 40, 36, 37, 50);
    declare_sticker(sc, 19, 2, 53, 38, 37, 36, 35);
    /*piece: DR */
    declare_sticker(sc, 20, 0, -1, 44, 45, 7, 6);
    declare_sticker(sc, 20, 1, 25, 50, 37, 28, 51);
    declare_sticker(sc, 20, 2, 50, 29, 28, 37, 38);
    /*piece: DFR */
    declare_sticker(sc, 21, 0, 17, 26, 17, 18, 19);
    declare_sticker(sc, 21, 1, 24, 51, 28, 18, 17);
    declare_sticker(sc, 21, 2, 47, 18, 19, 29, 28);
    /*piece: DF */
    declare_sticker(sc, 22, 0, 16, 27, 26, 19, 20);
    declare_sticker(sc, 22, 1, -1, 44, 6, 5, 53);
    declare_sticker(sc, 22, 2, 46, 19, 20, 30, 29);
    /*piece: DFL */
    declare_sticker(sc, 23, 0, 15, 22, 27, 20, 21);
    declare_sticker(sc, 23, 1, 44, 31, 55, 22, 21);
    declare_sticker(sc, 23, 2, 45, 20, 21, 31, 30);
    /*piece: DL */
    declare_sticker(sc, 24, 0, -1, 44, 45, 7, 6);
    declare_sticker(sc, 24, 1, 43, 32, 54, 55, 31);
    declare_sticker(sc, 24, 2, 48, 31, 30, 39, 32);
    /*piece: D */
    declare_sticker(sc, 25, 0, -1, 44, 45, 7, 6);
    declare_sticker(sc, 25, 1, -1, 44, 6, 5, 53);
    declare_sticker(sc, 25, 2, 49, 30, 29, 38, 39);

    return sc;
}

void free_sCube(sCube *p)
{
    if (p != NULL)
        free(p);
}

#endif