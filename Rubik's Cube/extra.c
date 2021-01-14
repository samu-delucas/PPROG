#include "extra.h"
#include "terminal_funct.h"
#include "interface.h"
#include <termios.h>

extern int fileno(FILE *);
extern int usleep(unsigned int);

Status congratulations(counter_data *dat, char *buff){

    rect **r = NULL, *r_aux,*r_border;
    char c;

    r = dat->rcounter;

    if (!dat||!buff)
        return ERROR;

    if (!(r_aux = rect_init(-2+20, 50+20, 16*8, 9)))
        return ERROR;

    if(!(r_border=rect_expand(r_aux,4,4))){
        rect_free(r_aux);
        return ERROR;
    }

    fade_to_color(GREY);
    print_buffer(buff, -2,r_aux);
    rect_border(r_border);

    bcd_display(dat->sec % 10, r[3]);
    bcd_display(dat->sec / 10, r[2]);
    bcd_display(dat->min % 10, r[1]);
    bcd_display(dat->min / 10, r[0]);
    fflush(stdout);

    usleep(500000); /*wait for half a second, to avoid bugs*/

    tcsetattr(fileno(stdin), TCSAFLUSH, &initial); /*reestablishes the initial terminal */
    _term_init();                                  /*modifies the terminal so that keys can be read without 'enter'*/

    while (scanf("%c", &c) == 0){
        continue;
    }

    rect_free(r_aux);
    rect_free(r_border);
    terminal_clear();

    fflush(stdin);

    usleep(500000); /*wait for half a second, to avoid bugs*/

    tcsetattr(fileno(stdin), TCSAFLUSH, &initial);  /*reestablishes the initial terminal */
    _term_init();                                  /*modifies the terminal so that keys can be read without 'enter'*/
    
    return OK;
}