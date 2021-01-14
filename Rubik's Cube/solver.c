/**
 * @author Pablo Cuesta Sierra 
 * @date 2020/11/01
 * 
    This file implemets a solution of a 3x3 (or 2x2) rubiks cube 
    using the 'beginners method'. On a cube as defined in cube.c
    This file is called by cube_interface in order to 
    display the solution of a given cube.

    The main function of the file is char *solve_cube(Cube3*), 
    which allocates memory for an returns a string with the solution 
    to the cube (taking into account if the cube is 2x2 or 3x3).
 * 
 */

#include "solver.h"
#include <string.h>

#define MAX_MOV 10000



char *solve_cube(Cube3* c1){
    char *sol=NULL;
    Cube3 *c2=NULL;
    
    if(!c1){
        return NULL;
    }
    if(!(c2=c_copy(c1))){
        return NULL;
    }
    if(!(sol = (char*)calloc(MAX_MOV, sizeof(char)))){
        c_free(c2);
        return NULL;
    }
    sol[0]='\0';
    /*resolvemos la cruz*/
    if(c1->option!=2)
        solve_cross(c2, sol);

    /*resolver las esquinas inferiores*/
    solve_d_corners(c2, sol);

    /*solve the second layer*/
    if(c1->option!=2)
        solve_finishF2L(c2, sol);
    
    /*solve the top cross*/
    if(c1->option!=2)
        solve_topcross(c2, sol);

    /*permutate top edges*/
    if(c1->option!=2)
        solve_topedges(c2, sol);

    /*permutate top corners*/
    if(c1->option!=2)
        solve_permcorners(c2, sol);

    /*orient the top corners*/
    solve_oricorners(c2, sol);

    if(c1->option==2)
        solve_PLL222(c2, sol);
    
    c_free(c2);
    clean_moves(sol);
    return sol;
}

void solve_cross(Cube3 *c, char *sol){
    short cd, cf, i, pos;
    char cross[101]="";
    Piece *p;

    cd=cfrom(c, 'D');
    
    /*solve each cross piece*/
    for(i=0;i<4;i++){
        cross[0]=0;
        cf=cfrom(c, 'F');

        /*search for the piece that goes in FD*/
        pos=c_iofCol(c, cd+cf);
        p=&(c->pc[pos]);

        /*first we put our piece in UF*/

        if(p->p[2]==-1){
        /*  it is in the D layer */
            if(p->p[1]==1){
                /*it is in DR*/
                strncat(cross, "RRU",100);
            }else if(p->p[1]==-1){
                /*it is in DL*/
                strncat(cross, "LLu",100);
            }else if(p->p[0]==1){
                /*it is in DF*/
                strncat(cross, "FF",100);
            }else{
                /*it is in DB*/
                strncat(cross, "BBUU",100);
            }
        }else if(p->p[2]==0){
            /*it is in the middle layer*/            
            if(p->p[1]==1){
                /*it is in R*/
                if(p->p[0]==1){
                    /*it is in RF*/
                    strncat(cross, "f",100);
                }else{
                    /*it is in RB*/
                    strncat(cross, "rUR",100);
                }
            }else{
                /*it is in L*/
                if(p->p[0]==1){
                    /*it is in LF*/
                    strncat(cross, "F",100);
                }else{
                    /*it is in LB*/
                    strncat(cross, "Lul",100);
                }
            }
            
        }else{
            /*it is in the top layer*/
            if(p->p[0]==1){
                /*it is in the correct spot*/
            }else if(p->p[0]==-1){
                /*it is in UB*/
                strncat(cross, "UU",100);
            }else if(p->p[1]==1){
                /*it is in UR*/
                strncat(cross, "U",100);
            }else {
                /*it is in UL*/
                strncat(cross, "u",100);
            }
        }
        c_moves(c,cross);/*make the moves*/
        
        /*now that the piece is in UF*/
        pos=c_iofPos(c, 1, 0, 1);
        p=&(c->pc[pos]);
        if(p->c[2]==cd){
            /*facing up*/
            strncat(cross, "FF",100);
            c_moves(c,"FF");
        }else{
            /*facing front*/
            strncat(cross, "urFR",100);
            c_moves(c,"urFR");

        }
        if(i<3){
            /*rotate cube to find next piece*/
            strncat(cross, "Y", 100);
            c_moves(c, "Y");
        }
        strncat(sol, cross, 100);
    }
}

void solve_d_corners(Cube3* c, char *sol){
    short cd, cf, cr, i, pos;
    char cor[101]="";
    Piece *p;

    cd=cfrom(c, 'D');
    
    /*solve each cross piece*/
    for(i=0;i<4;i++){
        cor[0]=0;
        cf=cfrom(c, 'F');
        cr=cfrom(c, 'R');

        /*search for the piece that goes in FD*/
        pos=c_iofCol(c, cd+cf+cr);
        p=&(c->pc[pos]);

        /*first we put our piece in URF*/

        if(p->p[2]==-1){
        /*  it is in the D layer */
            if(p->p[1]==1){
                /*it is in DRx */
                if(p->p[0]==1){
                    /*DRF*/
                    strncat(cor, "Rur", 100);
                }else{
                    /*DRB*/
                    strncat(cor, "rURU", 100);
                }
            }else{
                /*it is in DLx */
                if(p->p[0]==1){
                    /*DLF */
                    strncat(cor, "luL",100);
                }else{
                    /*DLB*/
                    strncat(cor, "Luul",100);
                }
            }
        }else{
        /*  it is in the U layer */
            if(p->p[1]==1){
                /*it is in URx */
                if(p->p[0]==1){
                    /*URF --  nothing to be done*/
                }else{
                    /*URB*/
                    strncat(cor, "U", 100);
                }
            }else{
                /*it is in ULx */
                if(p->p[0]==1){
                    /*ULF */
                    strncat(cor, "u",100);
                }else{
                    /*ULB*/
                    strncat(cor, "UU",100);
                }
            }
        }
        c_moves(c,cor);/*make the moves*/
        
        /*now that the piece is in URF*/
        pos=c_iofPos(c, 1, 1, 1);
        p=&(c->pc[pos]);
        if(p->c[2]==cd){
            /*facing up*/
            strncat(cor, "RuuruRUr",100);
            c_moves(c,"RuuruRUr");
        }else if(p->c[1]==cd){
            /*facing left*/
            strncat(cor, "RUr",100);
            c_moves(c,"RUr");
        }else {
            /*facing front*/
            strncat(cor, "URur",100);
            c_moves(c,"URur");
        }
        if(i<3){
            /*rotate cube to find next piece*/
            strncat(cor, "Y", 100);
            c_moves(c, "Y");
        }
        strncat(sol, cor, 100);
    }
}

void solve_finishF2L(Cube3*c, char *sol){
    short cf, cr, i, pos;
    char cor[101]="";
    Piece *p;
    
    /*solve each second layer piece*/
    for(i=0;i<4;i++){
        cor[0]=0;
        cf=cfrom(c, 'F');
        cr=cfrom(c, 'R');

        /*search for the piece that goes in FD*/
        pos=c_iofCol(c, cf+cr);
        p=&(c->pc[pos]);

        /*first we put our piece in UF*/

        if(p->p[2]==0){
        /*  it is in the middle layer */
            if(p->p[1]==1){
                /*it is in R */
                if(p->p[0]==1){
                    /*RF*/
                    if(p->c[0]==cf){
                        /*solved*/
                        strncat(sol, "Y", 100);
                        c_moves(c, "Y");
                        continue;
                    }else{
                        /*only orientation*/
                        strncat(sol, "RUUrURUUrUYluL", 100);
                        c_moves(c, "RUUrURUUrUYluL");
                        continue;
                    }
                }else{
                    /*RB*/
                    strncat(cor, "rURbRBr", 100);
                }
            }else{
                /*it is in L */
                if(p->p[0]==1){
                    /*LF */
                    strncat(cor, "lULfLFluu",100);
                }else{
                    /*LB*/
                    strncat(cor, "LulBlbL",100);
                }
            }
        }else{
        /*  it is in the U layer */
            if(p->p[0]==1){
                /*it is in UF --nothing to do*/
            }else if(p->p[1]==1){
                /*UR*/
                strncat(cor, "U", 100);
            }else if (p->p[1]==-1){
                /*it is in UL */
                strncat(cor, "u",100);
            }else{
                /*UB*/
                strncat(cor, "UU",100);
            }
        }
        
        c_moves(c,cor);/*make the moves*/
        
        /*now that the piece is in URF*/
        pos=c_iofPos(c, 1, 0, 1);
        p=&(c->pc[pos]);
        if(p->c[2]==cf){
            /*no mathc with center*/
            strncat(cor, "urfRURurF",100);
            c_moves(c,"urfRURurF");
        }else {
            /*matches with center*/
            strncat(cor, "URurFrfR",100);
            c_moves(c,"URurFrfR");
        }
        if(i<3){
            /*rotate cube to find next piece*/
            strncat(cor, "Y", 100);
            c_moves(c, "Y");
        }
        strncat(sol, cor, 100);
    }
}

void solve_topcross(Cube3* c, char *sol){
    short cu, pos;
    Piece *p1=NULL, *p2=NULL, *p3=NULL, *p4=NULL;
    char moves[101]="";
    int i, count, line=0;

    cu=cfrom(c, 'U');

    /*count oriented edges on the top*/
    for (i=count=0;i<4;i++){
        pos=c_iofPos(c, 1, 0, 1);
        if(cu==c->pc[pos].c[2]){
            count++;
        }
        c_moves(c, "U");
    }

    switch(count){
        case 0:{
            /*no edges oriented*/
            strncat(moves, "FRUruSRUrufs", 100);
        }
            break;
        case 2:{
            /*track the four edges on top*/
            pos=c_iofPos(c, 1, 0, 1);
            p1=&(c->pc[pos]);
            pos=c_iofPos(c, 0, 1, 1);
            p2=&(c->pc[pos]);
            pos=c_iofPos(c, -1, 0, 1);
            p3=&(c->pc[pos]);
            pos=c_iofPos(c, 0, -1, 1);
            p4=&(c->pc[pos]);
            /*look at all of the cases: */
            if(cu==p1->c[2]){
                if(cu==p3->c[2]){
                    line=1;
                }else if(cu==p2->c[2]){
                    line=-2;
                }else{
                    line=-1;
                }
            }else if(cu==p2->c[2]){
                if(cu==p4->c[2]){
                    line=2;
                }else{
                    line=-3;
                }
            }
            switch(line){
                case 1:
                    strncat(moves, "U", 100); /*no break on purpose*/
                case 2:
                    strncat(moves, "FRUruf", 100);
                    break;
                case -1:
                    strncat(moves, "u", 100); /*no break on purpose*/
                case -2:
                    strncat(moves, "u", 100); /*no break on purpose*/
                case -3:
                    strncat(moves, "u", 100); /*no break on purpose*/
                default:
                    strncat(moves, "FURurf", 100); /*no break on purpose*/
                    break;

            }
        }
            break;
        default:
            /* all oriented*/
            break;
    }
    c_moves(c, moves);
    strncat(sol, moves, 101);
}


void solve_topedges(Cube3 *c, char* sol){
    short cf, cr, cl, pos;
    Piece *p1 = NULL, *p2 = NULL;
    char moves[101]="";

    /*store the colors: */
    cf = cfrom(c, 'F');
    cr = cfrom(c, 'R');
    cl = cfrom(c, 'L');

    pos = c_iofPos(c, 1, 0, 1);

    while( c->pc[pos].c[0] != cf ){
        c_make(c, 'U');
        strncat(sol, "U", MAX_MOV);
        pos = c_iofPos(c, 1, 0, 1);
    }
    /*right piece*/
    pos = c_iofPos(c, 0, 1, 1);
    p1 = &(c->pc[pos]);
    /*left piece*/
    pos = c_iofPos(c, 0, -1, 1);
    p2 = &(c->pc[pos]);

    if(p1->c[1]==cl){
        if(p2->c[1]==cr){
            /*permutate right to left, left to right*/
            strncat(moves, "RUrurFRRuruRUrf", 100);
        }else{
            /*U perm, right opposite*/
            strncat(moves, "RUUruRur", 100);
        }
    }else if(p2->c[1]==cr){
        /*U perm, right opposite*/
        strncat(moves, "RUrURUUr", 100);
    }else if(p1->c[1]==cr){
        /*right good*/
        if(p2->c[1]==cl){
            /*left good, ie. all good*/
        }else{
            strncat(moves, "yRUrURUUrU", 100);
        }
    }else{
        /*left good, right not*/
        strncat(moves, "YRUUruRuru", 100);
    }
    c_moves(c, moves);
    strncat(sol, moves, MAX_MOV);
}

void solve_permcorners(Cube3 *c, char *sol){
    short pos1, pos2, cf, cl, cu, cb, cr, i, flag=0;
    char moves[101]="";

    /*store the colors: */
    cf = cfrom(c, 'F');
    cl = cfrom(c, 'L');
    cu = cfrom(c, 'U');

    pos1 = c_iofCol(c, cf+cl+cu);
    pos2 = c_iofPos(c, 1, -1, 1);

    i=0;
    while(pos1!=pos2){
        if(i==4){
            flag=1;
            break;
        }
        c_make(c, 'Y');
        strncat(sol, "Y", MAX_MOV);
        cf = cfrom(c, 'F');
        cl = cfrom(c, 'L');
        pos1 = c_iofCol(c, cf+cl+cu);
        pos2 = c_iofPos(c, 1, -1, 1);
        i++;
    }

    cb = cfrom(c, 'B');
    cr = cfrom(c, 'R');
    cl = cfrom(c, 'L');
    
    pos2 = c_iofPos(c, 1, 1, 1);

    if(flag==1){
        /*special case*/
        if(pos2==c_iofCol(c, cu+cb+cl)){
            /*h perm*/
            strncat(moves, "mmUmmUUmmUmmUU", 100);
        }else if(pos2==c_iofCol(c, cu+cb+cr)){
            /*e perm*/
            strncat(moves, "rurdRurDRUrdRUrDRR", 100);
        }else{
            /*U+e perm*/
            strncat(moves, "UrurdRurDRUrdRUrDRRu", 100);
        }
    }else{
        if(pos2==c_iofCol(c, cb+cr+cu)){
            strncat(moves, "RulUruLU", 100);
        }else if(pos2==c_iofCol(c, cb+cl+cu)){
            strncat(moves, "ulURuLUr", 100);
        }else{
            /*all good*/
        }
    }
    c_moves(c, moves);
    strncat(sol, moves, MAX_MOV);
}

void solve_oricorners(Cube3* c, char *sol){
    short cu, i, pos;
    char moves[101]="";

    cu=cfrom(c, 'U');

    /*categorize the cases: */
    for(i=0; i<4; i++){
        pos = c_iofPos(c, 1, 1, 1);
        if(c->pc[pos].c[2]==cu){
            strncat(moves, "U", 100);
        }else if(c->pc[pos].c[1]==cu){
            strncat(moves, "rdRDrdRDU", 100);
        }else{
            strncat(moves, "drDRdrDRU", 100);
        }
        c_make(c, 'U');/*4 made, after loop the cube is untouched*/
    }
    c_moves(c, moves);
    strncat(sol, moves, MAX_MOV);
}


void solve_PLL222(Cube3*c, char *sol){
    int found=0, i=0;
    short pos1, pos2, cf;
    char moves[101]="";

    while (found==0 && i<4){
        /*front top corners*/
        pos1=c_iofPos(c, 1, 1, 1);
        pos2=c_iofPos(c, 1, -1, 1);
        if(c->pc[pos1].c[0]==c->pc[pos2].c[0]){
            found=1;
        }
        else{
            strncat(moves, "U", 100);
            c_make(c, 'U');
            i++;
        }
    }
    /*make perms*/
    if(found==1){
        /*top back corners*/
        pos1=c_iofPos(c, -1, -1, 1);
        pos2=c_iofPos(c, -1, 1, 1);
        if(c->pc[pos1].c[0]!=c->pc[pos2].c[0]){
            /*tperm*/
            strncat(moves, "rUrFFRfrFFRR", 100);
            c_moves(c, "rUrFFRfrFFRR");
        }
    }
    else{
        /*vperm*/
        strncat(moves, "FRuruRUrfRUrurFRf", 100);
        c_moves(c, "FRuruRUrfRUrurFRf");
    }
    /*adjust*/
    found=0;
    i=0;
    pos2=c_iofPos(c, 1, 1, -1);
    cf=c->pc[pos2].c[0];
    while (found==0 && i<4){
        /*front top corners*/
        pos1=c_iofPos(c, 1, 1, 1);
        if(cf==c->pc[pos1].c[0]){
            found=1;
        }
        else{
            strncat(moves, "U", 100);
            c_make(c, 'U');
            i++;
        }
    }
    strncat(sol, moves, MAX_MOV);
}

/* auxiliary functions: */

/**
 * Returns the opposite move (same axis of rotation, 
 * opposite direction) of entered move
 **/
char opposite_move(char c){
    if(c>='a' && c<='z'){
        return (c-'a'+'A');
    }else if(c>='A' && c<='Z'){
        return (c-'A'+'a');
    }else{
        return -1;
    }
}

void clean_moves(char* m){
    int i;
    if(!m){
        return ;
    }
    i=0;
    while(m[i]!=0){
        if(m[i]==m[i+1] && m[i]==m[i+2] && m[i]==m[i+3]){
            /*4 moves that are the same are useless*/
            m[i]=0;
            concatenate(m, m+i+4);
            i=0;
        }else if(m[i]==m[i+1] && m[i]==m[i+2] && m[i]!=m[i+3]){
            /*3 moves that are the same is equivalent to the opposite*/
            m[i]=opposite_move(m[i]);
            m[i+1]=0;
            concatenate(m, m+i+3);
            i=0;
        }else if(m[i]==opposite_move(m[i+1])){
            /*two opposite moves together are useless*/
            m[i]=0;
            concatenate(m, m+i+2);
            i=0;
        }else {
            i++;
        }
    }
}

/**
 * This function should be the same as strcat(), but said 
 * function seems not to do the job properly
 **/

void concatenate(char *a, char *b){

    for(; *a != 0; a++);

    for( ; *b != 0 ; *(a++) = *(b++) );

    (*a) = 0;
}
