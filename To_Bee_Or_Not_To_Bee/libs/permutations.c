/**
 * @file    permutations.c
 * @author  Samuel de Lucas Maroto, José Manuel López-Serrano Tapia and Bhavuk Sikka
 * 
 * @brief   This module handles the creation of random numbers, and also 
 *          arrays of random numbers.
 * */

#include "permutations.h"

/***************************************************/
/* Function: random_num     Date: 23/09/2020       */
/* Authors: José Manuel López-Serrano Tapia        */
/*          Samuel de Lucas Maroto                 */
/*                                                 */
/* Routine that generates a random number          */
/* between two given numbers                       */
/*                                                 */
/* Input:                                          */
/* int inf: lower limit                            */
/* int sup: upper limit                            */
/* Output:                                         */
/* int: random number                              */
/***************************************************/
int random_num(int inf, int sup)
{
    int diff = sup - inf + 1;
    return inf + (int)((double)(diff)*rand()/(RAND_MAX+1.0));
}

/***************************************************/
/* Function: generate_perm  Date: 23/09/2020       */
/* Authors: José Manuel López-Serrano Tapia        */
/*          Samuel de Lucas Maroto                 */
/*                                                 */
/* Routine that generates a random permutation     */
/*                                                 */
/* Input:                                          */
/* int n: number of elements in the permutation    */
/* Output:                                         */
/* int *: pointer to integer array                 */
/* that contains the permitation                   */
/* or NULL in case of error                        */
/***************************************************/
int* generate_perm(int N)
{
    int* perm = NULL;
    int i;
    
    if(!(perm = (int *)calloc(sizeof(int), N))){
        return NULL;
    }
    
    for(i = 1; i <= N; i++){
        perm[i-1] = i;
    }
    
    for(i = 0; i < N; i++){
        int rand_index = random_num(0, N-1);
        int aux;
        if(rand_index < 0 || rand_index > N-1){
            free(perm);                                     
            return NULL;
        }
        aux = perm[i];
        perm[i] = perm[rand_index];
        perm[rand_index] = aux;
    }
    return perm;
}

/***************************************************/
/* Function: generate_permutations Date: 23/09/2020*/
/* Authors: José Manuel López-Serrano Tapia        */
/*          Samuel de Lucas Maroto                 */
/*                                                 */
/* Function that generates n_perms random          */
/* permutations with N elements                    */
/*                                                 */
/* Input:                                          */
/* int n_perms: Number of permutations             */
/* int N: Number of elements in each permutation   */
/* Output:                                         */
/* int**: Array of pointers to integer that point  */
/* to each of the permutations                     */
/* NULL en case of error                           */
/***************************************************/
int** generate_permutations(int n_perms, int N)
{
    int **arr = NULL;
    int i, failed = 0;
    
    if(!(arr = (int **)calloc(sizeof(int*), n_perms))){
        return NULL;
    }

    for(i=0; i < n_perms && !failed; i++){
        arr[i] = generate_perm(N);
        if(!arr[i])
            failed = 1;
    }

    if(failed){
        for(i = 0; i < n_perms; i++){
            if(arr[i]) free(arr[i]); 
        }
        free(arr);
        return NULL;
    }

    return arr;
}
