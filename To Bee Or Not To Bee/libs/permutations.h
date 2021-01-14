/**
 * @file    permutations.c
 * @author  Samuel de Lucas Maroto, José Manuel López-Serrano Tapia and Bhavuk Sikka
 * 
 * @brief   This module handles the creation of random numbers, and also 
 *          arrays of random numbers.
 * */

#ifndef PERMUTACIONES_H
#define PERMUTACIONES_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

/**
 * @brief   Routine that generates a random number between two given numbers.
 * @param   inf lower bound
 * @param   sup upper bound
 * @return  random integer between inf and sup
 * */
int random_num(int inf, int sup);

/**
 * @brief   Routine that generates a random permutation of integers.
 *          Reserves memory for this permutation. It must be freed.
 * @param   N number of elements in the permutation
 * @return  pointer to integer array that contains the permitation or NULL in case of error
 * */
int* generate_perm(int N);

/**
 * @brief   Function that generates n_perms random permutations with N elements 
 *          Reserves memory for this permutations. Must be freed.
 * @param   n_perms number of permutations to be generated
 * @param   N number of elements in each permutation
 * @return  Array of pointers to integer that point to each of the permutations or NULL en case of error     
 * */
int** generate_permutations(int n_perms, int N);

#endif
