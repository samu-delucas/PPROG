/**
 * @author Pablo Cuesta Sierra 
 * @date 2020/11/01
 * 
    This file implemets a solution of a 3x3 (or 2x2) rubiks cube 
    using the 'beginners method'. On a cube as defined in cube.c
    This file is called by cube_interface in order to 
    display the solution of a given cube.

    The main function of the file is char *solve_cube(Cube3*), 
    which allocates memory for and returns a string with the solution 
    to the cube (taking into account if the cube is 2x2 or 3x3).
 * 
 */


#ifndef SOLVER_H_
#define SOLVER_H_

#include "cube.h"

/** @brief allocates memory for and returns a string with the solution 
 * to the cube (taking into account if the cube is 2x2 or 3x3).
 * 
 * Implemets a solution of a 3x3 (or 2x2) rubiks cube 
 * using the 'beginners method'. On a cube as defined in cube.c
 * This file is called by cube_interface in order to 
 * display the solution of a given cube.
*/
char *solve_cube(Cube3* c1);

/******ALL OF THE FOLLOWING FUNCTIONS are to be used by solve_cube: not really intended to be used outside***/

/** @brief functions called by solve_cube: 
 * (these concatenate in sol the solution of 
 * the step and perform it in the cube c)
*/

/** @brief this function solves the bottom cross*/
void solve_cross(Cube3 *c, char *sol);

/** @brief this function solves the bottom corners*/
void solve_d_corners(Cube3* c, char *sol);

/**
 * @brief this function solves the edges of the 
 * second layer when the previous two functions have
 * been performed
*/
void solve_finishF2L(Cube3*c, char *sol);

/** @brief this solves the top  cross (orienting the top edges)*/
void solve_topcross(Cube3* c, char *sol);

/** @brief permutates top edges*/
void solve_topedges(Cube3 *c, char* sol);

/** @brief permutates top corners */
void solve_permcorners(Cube3 *c, char* sol);

/** @brief orients corners - final step*/
void solve_oricorners(Cube3* c, char *sol);

/** @brief solve pll in the 222*/
void solve_PLL222(Cube3* c2, char *sol);

/** @brief gets rid of unnecessary moves (redundancy)*/
void clean_moves(char* m);

/** @brief if it is lowercase, returns uppercase and viceversa*/
char opposite_move(char c);

/** @brief strcat seems not to work*/
void concatenate(char* a, char* b);


#endif