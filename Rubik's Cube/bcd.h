#ifndef BCD_H
#define BCD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "interface.h"

typedef struct _counter_data counter_data;

struct _counter_data{
    int sec, min, mode, display; /*Mode=0: show time 00:00. Mode=-1 Counter stopped. Mode=1 Counter running*/
    rect *rcounter[4];           /*Array of counters where to print min(r[0],r[1]) and secs(r[2],r[3])*/
};

/**
 * @brief Allocates a new counter_data structure
 * @return Pointer to counter_data or NULL 
 */
counter_data* counter_data_init();

/**
 * @brief Frees the counter_data strcuture
 */
void counter_data_free(counter_data *dat);

Status counter_data_set_time(counter_data* dat,int min, int sec);
Status counter_data_set_mode(counter_data* dat, int mode);
Status counter_data_set_display(counter_data*dat, int display);
/**
 * @brief Stablish the data of each rectangle of the struct. Should be called with l=15,h=17
 */
Status counter_data_set_rects(counter_data* dat,int line, int column,int l,int h);
rect **counter_data_get_rect(counter_data* dat);
int counter_data_get_min(counter_data*dat);
int counter_data_get_sec(counter_data *dat);
int counter_data_get_mode(counter_data *dat);

/**Prints the segmenst of a bcd. Arguments line and column define the upper left corner**/
void bcd_a(int line, int column);
void bcd_b(int line, int column);
void bcd_c(int line, int column);
void bcd_d(int line, int column);
void bcd_e(int line, int column);
void bcd_f(int line, int column);
void bcd_g(int line, int column);

int bcd_display(int n, rect *r);


/**
* @brief implements a counter to be used in a thread.
* @param a Pointer to a structure of type counter_data_set_time
* 
**/
void *counter_bcd(void*a);

#endif