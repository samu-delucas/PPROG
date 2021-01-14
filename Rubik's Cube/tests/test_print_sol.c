#include "interface.h"
#include <stdio.h>


int main(){

    char *sol[10]="a";
    rect *r;

    r=rect_init(4,10,36,63);

    print_solution(sol,r,4);

    return 0;
}