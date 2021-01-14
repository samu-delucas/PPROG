#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "bcd.h"

#define GROSOR 3 /*grosor del tramo*/
#define LONG 5   /*longitud del tramo*/
#define CHAR 32  /*char used to draw bcd*/
#define COLOR 7   /*Color of the segments in the bcd*/

/*

          aaaaa
         f     b
         f     b
         f     b
          ggggg
         e     c      
         e     c 
         e     c 
          dddd
*/



counter_data *counter_data_init(){
    counter_data *dat;
    int i,j;

    if(!(dat=(counter_data*)calloc(1,sizeof(counter_data))))
        return NULL;

    for(i=0;i<4;i++){
        if(!(dat->rcounter[i]=rect_init(1,1,1,1))){
            for(j=0;j<i;j++){
                rect_free(dat->rcounter[j]);
            }
            free(dat);
            return NULL;
        }
    }

    dat->min=0;
    dat->sec=0;
    dat->mode=0;
    dat->display=0;

    return dat;
}

void counter_data_free(counter_data *dat){
    int i;

    if(dat){
        for(i=0;i<4;i++){
            rect_free(dat->rcounter[i]);
        }
        free(dat);
    }
        
}
Status counter_data_set_mode(counter_data *dat, int mode){
    if(!dat||mode>1||mode<-1)
        return ERROR;
    
    dat->mode=mode;
    return OK;
}

Status counter_data_set_display(counter_data *dat, int display){
    if(!dat||display>1||display<0)
        return ERROR;
    
    dat->display=display;
    return OK;
}

Status counter_data_set_time(counter_data* dat, int min, int sec){
    if(!dat||min>59||sec>59||min<0||sec<0)
        return ERROR;
    
    dat->min=min;
    dat->sec=sec;
    return OK;
}

Status counter_data_set_rects(counter_data *dat, int line, int column, int l, int h){
    int i,j;

    if(!dat||line<1||column<1||l<0||h<0)
        return ERROR;


    for(i=0;i<2;i++){
        rect_setline(dat->rcounter[i],line);
        rect_setcolumn(dat->rcounter[i],column+i*20);
        rect_setheight(dat->rcounter[i],h);
        rect_setlength(dat->rcounter[i],l);
    }

    column+=20*2+5; /*20 for each rect, 5 for the separation*/

    for (i = 2,j=0; i < 4; i++,j++){
        rect_setline(dat->rcounter[i], line);
        rect_setcolumn(dat->rcounter[i], column + j * 20);
        rect_setheight(dat->rcounter[i], h);
        rect_setlength(dat->rcounter[i], l);
    }

    return OK;
}

rect **counter_data_get_rect(counter_data *dat){
    if(!dat)
        return NULL;
    
    return dat->rcounter;
}

int counter_data_get_mode(counter_data *dat){
    if(!dat)
        return -2;
    
    return dat->mode;
}

int counter_data_get_min(counter_data *dat){
    if (!dat)
        return -1;

    return dat->min;
}

int counter_data_get_sec(counter_data *dat){
    if (!dat)
        return -1;

    return dat->sec;
}

void bcd_a(int l, int c)
{
    int line, column, i, j;
    line = l;
    column = c;

    printf("%c[%i;%iH", 27, line + 1, column + GROSOR);
    for (i = 0; i < GROSOR - 1; i++)
    {
        printf("%c[%i;%iH", 27, line + 1 + i, column + GROSOR);
        for (j = 0; j < LONG; j++)
        {
            printf("%c[;;%im", 27, COLOR);
            printf("%c", CHAR);
        }
    }
    printf("%c[0m", 27); /*reset color*/
    fflush(stdout);
}

void bcd_b(int l, int c)
{
    int line, column, i, j;
    line = l;
    column = c;

    printf("%c[%i;%iH", 27, line + GROSOR, column + GROSOR + LONG);
    for (i = 0; i < LONG; i++)
    {
        printf("%c[%i;%iH", 27, line + GROSOR + i, column + GROSOR + LONG);
        for (j = 0; j < GROSOR; j++)
        {
            printf("%c[;;%im", 27, COLOR);
            printf("%c", CHAR);
        }
    }
    printf("%c[0m", 27);
    fflush(stdout);
}

void bcd_c(int l, int c)
{
    int line, column, i, j;
    line = l;
    column = c;

    printf("%c[%i;%iH", 27, line + 2 * GROSOR + LONG - 1, column + LONG + GROSOR);
    for (i = 0; i < LONG; i++)
    {
        printf("%c[%i;%iH", 27, line + 2 * GROSOR + LONG - 1 + i, column + LONG + GROSOR);
        for (j = 0; j < GROSOR; j++)
        {
            printf("%c[;;%im", 27, COLOR);
            printf("%c", CHAR);
        }
    }
    printf("%c[0m", 27);
    fflush(stdout);
}

void bcd_d(int l, int c)
{
    int line, column, i, j;
    line = l;
    column = c;

    printf("%c[%i;%iH", 27, line + 2 * LONG + 2 * GROSOR - 1, column + GROSOR);
    for (i = 0; i < GROSOR - 1; i++)
    {
        printf("%c[%i;%iH", 27, line + i + 2 * LONG + 2 * GROSOR - 1, column + GROSOR);
        for (j = 0; j < LONG; j++)
        {
            printf("%c[;;%im", 27, COLOR);
            printf("%c", CHAR);
        }
    }
    printf("%c[0m", 27);
    fflush(stdout);
}

void bcd_e(int l, int c)
{
    int line, column, i, j;
    line = l;
    column = c;

    printf("%c[%i;%iH", 27, line + 2 * GROSOR + LONG - 1, column);
    for (i = 0; i < LONG; i++)
    {
        printf("%c[%i;%iH", 27, line + 2 * GROSOR + LONG - 1 + i, column);
        for (j = 0; j < GROSOR; j++)
        {
            printf("%c[;;%im", 27, COLOR);
            printf("%c", CHAR);
        }
    }
    printf("%c[0m", 27);
    fflush(stdout);
}

void bcd_f(int l, int c)
{
    int line, column, i, j;
    line = l;
    column = c;

    printf("%c[%i;%iH", 27, line + GROSOR, column);
    for (i = 0; i < LONG; i++)
    {
        printf("%c[%i;%iH", 27, line + GROSOR + i, column);
        for (j = 0; j < GROSOR; j++)
        {
            printf("%c[;;%im", 27, COLOR);
            printf("%c", CHAR);
        }
    }
    printf("%c[0m", 27);
    fflush(stdout);
}

void bcd_g(int l, int c)
{
    int line, column, i, j;
    line = l;
    column = c;

    printf("%c[%i;%iH", 27, line + LONG + GROSOR, column + GROSOR);
    for (i = 0; i < GROSOR - 1; i++)
    {
        printf("%c[%i;%iH", 27, line + i + LONG + GROSOR, column + GROSOR);
        for (j = 0; j < LONG; j++)
        {
            printf("%c[;;%im", 27, COLOR);
            printf("%c", CHAR);
        }
    }
    printf("%c[0m", 27);
    fflush(stdout);
}

int bcd_display(int n, rect *r)
{

    int l, c;

    if (!r || n < 0 || n > 9)
        return -1;

    rect_clear(r); /*Prepare area*/

    l = rect_getline(r);
    c = rect_getcolumn(r);

    switch (n)
    {
    case 0:
        bcd_a(l, c);
        bcd_b(l, c);
        bcd_c(l, c);
        bcd_d(l, c);
        bcd_e(l, c);
        bcd_f(l, c);
        break;

    case 1:
        bcd_b(l, c);
        bcd_c(l, c);
        break;
    case 2:
        bcd_a(l, c);
        bcd_b(l, c);
        bcd_d(l, c);
        bcd_e(l, c);
        bcd_g(l, c);
        break;
    case 3:
        bcd_a(l, c);
        bcd_b(l, c);
        bcd_d(l, c);
        bcd_c(l, c);
        bcd_g(l, c);
        break;
    case 4:
        bcd_c(l, c);
        bcd_g(l, c);
        bcd_b(l, c);
        bcd_f(l, c);
        break;
    case 5:
        bcd_a(l, c);
        bcd_f(l, c);
        bcd_g(l, c);
        bcd_c(l, c);
        bcd_d(l, c);
        break;
    case 6:
        bcd_a(l, c);
        bcd_f(l, c);
        bcd_g(l, c);
        bcd_c(l, c);
        bcd_d(l, c);
        bcd_e(l, c);
        break;
    case 7:
        bcd_a(l, c);
        bcd_b(l, c);
        bcd_c(l, c);
        break;
    case 8:
        bcd_a(l, c);
        bcd_b(l, c);
        bcd_c(l, c);
        bcd_d(l, c);
        bcd_e(l, c);
        bcd_f(l, c);
        bcd_g(l, c);
        break;
    case 9:
        bcd_a(l, c);
        bcd_b(l, c);
        bcd_c(l, c);
        bcd_f(l, c);
        bcd_g(l, c);
        break;
    }

    return 0;
}

void *counter_bcd(void *dat){
    rect **r;
    counter_data *d;
    int blank=0,stop=0;

    /*rs1 = rect_init(2, 160, 15, 17);
    rs2 = rect_init(2, 180, 15, 17);
    rmin1 = rect_init(2, 115, 15, 17);
    rmin2 = rect_init(2, 135, 15, 17);*/

    d = (counter_data *)dat;

    r = d->rcounter;

    while(1){

    while(d->mode==0){ /*Print time 0*/
        if(blank==0){
            bcd_display(0, r[3]);
            bcd_display(0, r[2]);
            bcd_display(0, r[1]);
            bcd_display(0, r[0]);
            blank=1;
        }
        
    }


    while(d->mode==-1){/*Counter stopped*/
        if(stop==0){
            bcd_display(d->sec % 10, r[3]);
            bcd_display(d->sec / 10, r[2]);
            bcd_display(d->min % 10, r[1]);
            bcd_display(d->min / 10, r[0]);
            stop=1;
        }
   
    }


    while (d->mode==1)/*Counter running*/
    {

        bcd_display(d->min % 10, r[1]);
        bcd_display(d->min / 10, r[0]);
        bcd_display(d->sec % 10, r[3]);
        bcd_display(d->sec / 10, r[2]);

        d->sec++;

        if (d->sec == 60)
        {
            bcd_display(d->min % 10, r[1]);
            bcd_display(d->min / 10, r[0]);
            d->min += 1;
            d->sec = 0;
        }
        if (d->min == 60)
        {
            d->min = 0;
        }
        sleep(1);
    }

    }

    return 0;
}
