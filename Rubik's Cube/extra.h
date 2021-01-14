#ifndef EXTRA_H
#define EXTRA_H

#include <stdio.h>
#include "bcd.h"
#define CONGRATULATIONS_PATH "./txt_files/congrats.txt"

/**
 * @brief manages all the events when the cube is solved
 * @param dat pointer to counter_data
 * @param buff buffer with the congratulations text
 * @return Ok or ERROR in case of null pointer in argument
 */
Status congratulations(counter_data *dat,char* buff);


#endif