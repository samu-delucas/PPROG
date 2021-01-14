/**
 * @file    control.h
 * @author  Samuel de Lucas Maroto and Bhavuk Sikka
 * 
 * @brief   This module handles the terminal's cursor positioning, and also 
 *          clearing the screen.
 * 
 *          Almost everything is done using ANSI escape codes. 
 *          Visit https://en.wikipedia.org/wiki/ANSI_escape_code for more info
 * */


#include "control.h"

#define ESC 27


/*_____CLEAR_SCREEN___________________________________*/

void clear_screen(){
    printf("%c[%dJ", ESC, 3);
    printf("%c[%dJ", ESC, 2);
    move_cursor(1, 1);
    fflush(stdout);
}

/*_____CURSOR_APPEARANCE___________________________________*/

void hide_cursor(){
    printf("%c[?25l",ESC);
    fflush(stdout);
}

void show_cursor(){
    printf("%c[?25h",ESC);
    fflush(stdout);
}

/*_____CURSOR_MOVEMENT___________________________________*/

status move_cursor(int n, int m){
    int *size = get_winsize();
    if(n <= 0 || m <= 0 || n > size[0] || m > size[1]){
        free(size);
        return ERROR;
    }
    printf("%c[%d;%dH", ESC, n, m);
    fflush(stdout);
    free(size);
    return OK;
}

status cursor_up(int n){
    int *size = get_winsize();
    if(n <= 0 || n > size[0]){
        free(size);
        return ERROR;
    }
    printf("%c[%dA", ESC, n);
    fflush(stdout);
    free(size);
    return OK;
}

status cursor_down(int n){
    int *size = get_winsize();
    if(n <= 0 || n > size[0]){
        free(size);
        return ERROR;
    }
    printf("%c[%dB", ESC, n);
    fflush(stdout);
    free(size);
    return OK;
}

status cursor_right(int n){
    int *size = get_winsize();
    if(n <= 0 || n > size[0]){
        free(size);
        return ERROR;
    }
    printf("%c[%dC", ESC, n);
    fflush(stdout);
    free(size);
    return OK;
}

status cursor_left(int n){
    int *size = get_winsize();
    if(n <= 0 || n > size[0]){
        free(size);
        return ERROR;
    }
    printf("%c[%dD", ESC, n);
    fflush(stdout);
    free(size);
    return OK;
}
