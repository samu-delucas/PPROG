/**
 * @author Pablo Cuesta Sierra 
 * 
 */

#include "cube.h"
#include <string.h>



/********DECLARATION OF AUXILIAR FUNCTIONS********/

/*function equivalent to: p[]=[a,b,c]*/
void assign(short *p, short a, short b, short c);

/*helps with the initialization: given a piece, it paints it so that the cube is solved when initialized*/

void paint_piece(Piece *pc);

/***************ROTATION OF A PIECE*****************/

/**
 * @brief rotates piece around x axis
 * @param p piece to be rotated
 * @param dir direction in which the rotation is made 
 *      dir=+-1 positive dir means piece is rotated clockwise 
 *      (from the positive x perspective)
 * 
 * dir==1  ==> (x,y,z)>>>(x,z,-y)
 * dir==-1 ==> (x,y,z)>>>(x,-z,y)
*/
void rot_x(Piece *p, short dir);

/**
 * @brief rotates piece around y axis
 * @param p piece to be rotated
 * @param dir direction in which the rotation is made 
 *      dir=+-1 positive dir means piece is rotated clockwise 
 *      (from the positive y perspective)
 * 
 * dir==1  ==> (x,y,z)>>>(-x,y,x)
 * dir==-1 ==> (x,y,z)>>>(x,y,-z)
*/
void rot_y(Piece *p, short dir);

/**
 * @brief rotates piece around z axis
 * @param p piece to be rotated
 * @param dir direction in which the rotation is made 
 *      dir=+-1 positive dir means piece is rotated clockwise 
 *      (from the positive z perspective)
 * 
 * dir==1  ==> (x,y,z)>>>(y,-x,z)
 * dir==-1 ==> (x,y,z)>>>(-y,x,z)
*/
void rot_z(Piece *p, short dir);



/***********MOVES*************/

/* @brief moves the upper layer clockwise */
void m_U(Cube3* c);

/* @brief moves the upper layer counterclockwise */
void m_u(Cube3* c);

/* @brief moves the lower layer clockwise */
void m_D(Cube3* c);

/* @brief moves the lower layer counterclockwise */
void m_d(Cube3* c);

/* @brief moves the right layer clockwise */
void m_R(Cube3* c);

/* @brief moves the right layer counterclockwise */
void m_r(Cube3* c);

/* @brief moves the left layer clockwise */
void m_L(Cube3* c);

/* @brief moves the left layer counterclockwise */
void m_l(Cube3* c);

/* @brief moves the front layer clockwise */
void m_F(Cube3* c);

/* @brief moves the front layer counterclockwise */
void m_f(Cube3* c);

/* @brief moves the back layer clockwise */
void m_B(Cube3* c);

/* @brief moves the back layer counterclockwise */
void m_b(Cube3* c);


/* @brief the layer between L and R, turn direction as L (top-down)*/
void m_M(Cube3* c);

void m_m(Cube3* c);

/* @brief the layer between U and D, turn direction as D (left-right)*/
void m_E(Cube3* c);

void m_e(Cube3* c);

/* @brief the layer between F and B, turn direction as F*/
void m_S(Cube3* c);

void m_s(Cube3* c);



/*the following functions consider different axis than we do in the cube.c for the auxiliary functions. No problem. This will be taken into account*/

/* @brief rotate the entire Cube on R*/
void m_X(Cube3* c);

void m_x(Cube3* c);

/* @brief rotate the entire Cube on U*/
void m_Y(Cube3* c);

void m_y(Cube3* c);

/* @brief rotate the entire Cube on F*/
void m_Z(Cube3* c);

void m_z(Cube3* c);





/***********functions in cube.h***************/

Cube3 *c_init(int option){
  Cube3 *c=NULL;
  short i;
  
  if(!(c=(Cube3*)malloc(sizeof(Cube3)))){
    return NULL;
  }

  c->option=option;
  
  /*center pieces:*/
  
  /*white centerpiece*/
  assign(c->pc[0].p,0,0,1);
  /*red centerpiece*/
  assign(c->pc[1].p,0,1,0);
  /*green centerpiece*/
  assign(c->pc[2].p,1,0,0);
  /*yellow centerpiece*/
  assign(c->pc[3].p,0,0,-1);
  /*orange centerpiece*/
  assign(c->pc[4].p,0,-1,0);
  /*blue centerpiece*/
  assign(c->pc[5].p,-1,0,0);
  
  /*edges:*/
  /*wg*/
  assign(c->pc[6].p,1,0,1);
  /*wr*/
  assign(c->pc[7].p,0,1,1);
  /*wb*/
  assign(c->pc[8].p,-1,0,1);
  /*wo*/
  assign(c->pc[9].p,0,-1,1);

  /*gr*/
  assign(c->pc[10].p,1,1,0);
  /*br*/
  assign(c->pc[11].p,-1,1,0);
  /*bo*/
  assign(c->pc[12].p,-1,-1,0);
  /*go*/
  assign(c->pc[13].p,1,-1,0);

  /*yg*/
  assign(c->pc[14].p,1,0,-1);
  /*yr*/
  assign(c->pc[15].p,0,1,-1);
  /*yb*/
  assign(c->pc[16].p,-1,0,-1);
  /*yo*/
  assign(c->pc[17].p,0,-1,-1);
  
  /*corners:*/
  /*wgr*/
  assign(c->pc[18].p,1,1,1);
  /*wbr*/
  assign(c->pc[19].p,-1,1,1);
  /*wbo*/
  assign(c->pc[20].p,-1,-1,1);
  /*wgo*/
  assign(c->pc[21].p,1,-1,1);

  /*ygr*/
  assign(c->pc[22].p,1,1,-1);
  /*ybr*/
  assign(c->pc[23].p,-1,1,-1);
  /*ybo*/
  assign(c->pc[24].p,-1,-1,-1);
  /*ygo*/
  assign(c->pc[25].p,1,-1,-1);

  for(i=0;i<NPC;i++){
    paint_piece(&(c->pc[i]));
  }

  return c;
}

/**
 * saves cube c in (binary) file save_game
 * option is where wither 3 or 2 is passed (for 3x3 or 2x2)
 * 
 * returns ERROR/OK
 **/
int save_cube(Cube3 *c, char *save_game){
    FILE *pf = NULL;
    int ret = 0, i = 0;

    if (!save_game)
        return ERROR;

    if (!(pf = fopen(save_game, "wb")))
        return ERROR;

    /*store the option at the beginning of the file*/
    ret = fwrite(&(c->option), sizeof(int), 1, pf);

    /*store every piece of the cube*/
    for (i = 0; i < NPC && ret != 0; i++)
        ret = fwrite(&(c->pc[i]), sizeof(Piece), 1, pf);

    fclose(pf);

    return ret == 0 ? ERROR : OK;
}

/**
 * reads saved cube into c from (binary) file save_game
 * option is where wither 3 or 2 is stored (for 3x3 or 2x2)
 * 
 * returns ERROR/OK
 **/
int read_saved_cube(Cube3 *c, char *save_game){
    FILE *pf = NULL;
    int ret = 0, i = 0;

    if (!save_game)
        return -1;

    if (!(pf = fopen(save_game, "rb")))
        return -1;

    /*read the option at the beginning of the file*/
    ret = fread(&(c->option), sizeof(int), 1, pf);

    /*store every piece of the cube*/
    for (i = 0; i < NPC && ret != 0; i++)
        ret = fread(&(c->pc[i]), sizeof(Piece), 1, pf);

    fclose(pf);

    return ret == 0 ? ERROR : OK;
}

/*copies cube c1 into c2, and returns the allocated copy*/
Cube3 *c_copy(Cube3*c1){
    Cube3* c2=NULL;
    int i;

    if(!c1){
        return NULL;
    }
    if(!(c2=c_init(c1->option))){
        return NULL;
    }
    
    for (i=0;i<NPC;i++){
        c2->pc[i].c[0]=c1->pc[i].c[0];
        c2->pc[i].c[1]=c1->pc[i].c[1];
        c2->pc[i].c[2]=c1->pc[i].c[2];
        c2->pc[i].p[0]=c1->pc[i].p[0];
        c2->pc[i].p[1]=c1->pc[i].p[1];
        c2->pc[i].p[2]=c1->pc[i].p[2];
    }
    return c2;
}

void c_free(Cube3* c){
  free(c);
}

int c_iofPos(Cube3 *c, short x, short y, short z){
  short i;

  for(i=0;i<NPC;i++){
    if(c->pc[i].p[0]==x && c->pc[i].p[1]==y && c->pc[i].p[2]==z){
      return i;
    }
  }
  return -1;
}

int c_iofCol(Cube3 *c, short searchcolours){
  short i;

  for(i=0;i<NPC;i++){
    if(c->pc[i].c[0]+c->pc[i].c[1]+c->pc[i].c[2] == searchcolours){
      return i;
    }
  }
  return -1;
}


int p_print(FILE *f, Piece *p){
    if(!p||!f){
        return -1;
    }
    return fprintf(f,"\nPiece: pos=(%hd,%hd,%hd), col=(%hd,%hd,%hd)\n",p->p[0],p->p[1],p->p[2],p->c[0],p->c[1],p->c[2]);
}

/*Functions that call moves*/

void c_make(Cube3 *c, char l){
  if(l==' '){
    return ;
  }
  switch(l){
    case 'U':   m_U(c);   break;
    case 'u':   m_u(c);   break;
    case 'D':   m_D(c);   break;
    case 'd':   m_d(c);   break;
    case 'R':   m_R(c);   break;
    case 'r':   m_r(c);   break;
    case 'L':   m_L(c);   break;
    case 'l':   m_l(c);   break;
    case 'F':   m_F(c);   break;
    case 'f':   m_f(c);   break;
    case 'B':   m_B(c);   break;
    case 'b':   m_b(c);   break;

    case 'M':   m_M(c);   break;
    case 'm':   m_m(c);   break;
    case 'E':   m_E(c);   break;
    case 'e':   m_e(c);   break;    
    case 'S':   m_S(c);   break;
    case 's':   m_s(c);   break;

    case 'X':   m_X(c);   break;
    case 'x':   m_x(c);   break;
    case 'Y':   m_Y(c);   break;
    case 'y':   m_y(c);   break;    
    case 'Z':   m_Z(c);   break;
    case 'z':   m_z(c);   break;
    default: break;
  }
}

Status c_moves(Cube3 *c, char *s){
  int i, len;

  if(!s||!c){return ERROR;
  }
  
  len=strlen(s);

  for(i=0;i<len;i++){
    c_make(c, s[i]);
  }

  return OK;
}

/*SCRAMBLE*/

Status scramble_cube(Cube3*c, char *filename, char *scramble){
    FILE* f=NULL;
    int i=0, n;

    srand(time(NULL));

    if(!c||!filename){
        return ERROR;
    }

    strncpy(scramble, "", MAX_LINE);
    
    if(!(f=fopen(filename, "r"))){
        return ERROR;
    }
    /*count number of scambles:*/
    for (i=0;fgets(scramble, MAX_LINE, f)!=NULL;i++);
    fclose(f);
    /*take a random number between 0 and number of scrambles-1*/
    n=rand()%i;
    f=fopen(filename, "r");
    for(i=0;i<n;i++){
        fgets(scramble, MAX_LINE, f);
    }
    fclose(f);

    scramble[strlen(scramble)-1]=0;/*remove the \n*/

    return c_moves(c, scramble);
}


Status colour_stickers(Cube3* c, short *s){
  int i;
  if(!s||!c){
    return ERROR;
  }
  i=c_iofPos(c, 1, 1, 1);
  s[8]=c->pc[i].c[2];
  s[18]=c->pc[i].c[1];
  s[11]=c->pc[i].c[0];

  i=c_iofPos(c, -1, 1, 1);
  s[2]=c->pc[i].c[2];
  s[20]=c->pc[i].c[1];
  s[27]=c->pc[i].c[0];

  i=c_iofPos(c, -1, -1, 1);
  s[0]=c->pc[i].c[2];
  s[36]=c->pc[i].c[1];
  s[29]=c->pc[i].c[0];

  i=c_iofPos(c, 1, -1, 1);
  s[6]=c->pc[i].c[2];
  s[38]=c->pc[i].c[1];
  s[9]=c->pc[i].c[0];

  i=c_iofPos(c, 1, 1, -1);
  s[47]=c->pc[i].c[2];
  s[24]=c->pc[i].c[1];
  s[17]=c->pc[i].c[0];

  i=c_iofPos(c, -1, 1, -1);
  s[53]=c->pc[i].c[2];
  s[26]=c->pc[i].c[1];
  s[33]=c->pc[i].c[0];

  i=c_iofPos(c, -1, -1, -1);
  s[51]=c->pc[i].c[2];
  s[42]=c->pc[i].c[1];
  s[35]=c->pc[i].c[0];

  i=c_iofPos(c, 1, -1, -1);
  s[45]=c->pc[i].c[2];
  s[44]=c->pc[i].c[1];
  s[15]=c->pc[i].c[0];


  i=c_iofPos(c, 0, 0, 1);
  s[4]=c->pc[i].c[2];

  i=c_iofPos(c, 0, 0, -1);
  s[49]=c->pc[i].c[2];

  i=c_iofPos(c, 0, 1, 0);
  s[22]=c->pc[i].c[1];

  i=c_iofPos(c, 0, -1, 0);
  s[40]=c->pc[i].c[1];

  i=c_iofPos(c, 1, 0, 0);
  s[13]=c->pc[i].c[0];

  i=c_iofPos(c, -1, 0, 0);
  s[31]=c->pc[i].c[0];


  i=c_iofPos(c, 1, 0, 1);
  s[7]=c->pc[i].c[2];
  s[10]=c->pc[i].c[0];

  i=c_iofPos(c, -1, 0, 1);
  s[1]=c->pc[i].c[2];
  s[28]=c->pc[i].c[0];
  
  i=c_iofPos(c, -1, 0, -1);
  s[52]=c->pc[i].c[2];
  s[34]=c->pc[i].c[0];

  i=c_iofPos(c, 1, 0, -1);
  s[46]=c->pc[i].c[2];
  s[16]=c->pc[i].c[0];


  i=c_iofPos(c, 1, 1, 0);
  s[14]=c->pc[i].c[0];
  s[21]=c->pc[i].c[1];

  i=c_iofPos(c, -1, 1, 0);
  s[30]=c->pc[i].c[0];
  s[23]=c->pc[i].c[1];

  i=c_iofPos(c, -1, -1, 0);
  s[32]=c->pc[i].c[0];
  s[39]=c->pc[i].c[1];
  
  i=c_iofPos(c, 1, -1, 0);
  s[12]=c->pc[i].c[0];
  s[41]=c->pc[i].c[1];

    
  i=c_iofPos(c, 0, 1, 1);
  s[5]=c->pc[i].c[2];
  s[19]=c->pc[i].c[1];
    
  i=c_iofPos(c, 0, 1, -1);
  s[50]=c->pc[i].c[2];
  s[25]=c->pc[i].c[1];
    
  i=c_iofPos(c, 0, -1, -1);
  s[48]=c->pc[i].c[2];
  s[43]=c->pc[i].c[1];
    
  i=c_iofPos(c, 0, -1, 1);
  s[3]=c->pc[i].c[2];
  s[37]=c->pc[i].c[1];

  return OK;
}

Bool is_solved(Cube3 *c){
  int i,j;
  short *s=c->stickers;

  colour_stickers(c, s);
 
  for(j=0;j<6;j++){
    i=9*j;
    switch(c->option){
        case 2:{
            if (s[i]==s[i+2] && s[i]==s[i+6] && s[i]==s[i+8]){
                continue;
            }else{
                return FALSE;
            }
            break;
        }

        default:{
            if (s[i]==s[i+1] && s[i]==s[i+2] && s[i]==s[i+3] && s[i]==s[i+4] && s[i]==s[i+5] && s[i]==s[i+6]){
                continue;
            }else{
                return FALSE;
            }
            break;
        }
    }
  }
  
  return TRUE;
}


short cfrom(Cube3* cube, char cp){
    short pos;
    if(!cube){
        return ERROR ;
    }
    switch(cp){
        case 'U':
            pos=c_iofPos(cube, 0,0,1);
            return cube->pc[pos].c[2];

            break;
        case 'D':
            pos=c_iofPos(cube, 0,0,-1);
            return cube->pc[pos].c[2];

            break;
        case 'R':
            pos=c_iofPos(cube, 0,1,0);
            return cube->pc[pos].c[1];

            break;
        case 'L':
            pos=c_iofPos(cube, 0,-1,0);
            return cube->pc[pos].c[1];

            break;
        case 'F':
            pos=c_iofPos(cube, 1,0,0);
            return cube->pc[pos].c[0];

            break;
        case 'B':
            pos=c_iofPos(cube, -1,0,0);
            return cube->pc[pos].c[0];

            break;
        default:
            return ERROR;
    }
}

/*CUBE MOVES*/

void m_U(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[2]==1){
      rot_z(&c->pc[i], 1);
    }
  }
}


void m_u(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[2]==1){
      rot_z(&c->pc[i], -1);
    }
  }
}


void m_D(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[2]==-1){
      rot_z(&c->pc[i], -1);
    }
  }
}


void m_d(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[2]==-1){
      rot_z(&c->pc[i], 1);
    }
  }
}


void m_R(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[1]==1){
      rot_y(&c->pc[i], 1);
    }
  }
}


void m_r(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[1]==1){
      rot_y(&c->pc[i], -1);
    }
  }
}


void m_L(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[1]==-1){
      rot_y(&c->pc[i], -1);
    }
  }
}


void m_l(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[1]==-1){
      rot_y(&c->pc[i], 1);
    }
  }
}


void m_F(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[0]==1){
      rot_x(&c->pc[i], 1);
    }
  }
}


void m_f(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[0]==1){
      rot_x(&c->pc[i], -1);
    }
  }
}


void m_B(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[0]==-1){
      rot_x(&c->pc[i], -1);
    }
  }
}


void m_b(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[0]==-1){
      rot_x(&c->pc[i], 1);
    }
  }
}


/* the layer between L and R, turn direction as L (top-down)*/
void m_M(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[1]==0){
      rot_y(&c->pc[i], -1);
    }
  }
}

void m_m(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[1]==0){
      rot_y(&c->pc[i], 1);
    }
  }
}

/* the layer between U and D, turn direction as D (left-right)*/
void m_E(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[2]==0){
      rot_z(&c->pc[i], -1);
    }
  }
}


void m_e(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[2]==0){
      rot_z(&c->pc[i], 1);
    }
  }
}

/* the layer between F and B, turn direction as F*/
void m_S(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[0]==0){
      rot_x(&c->pc[i], 1);
    }
  }
}

void m_s(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    if(c->pc[i].p[0]==0){
      rot_x(&c->pc[i], -1);
    }
  }
}



/*the following functions consider different axis than we do in the cube.c for the auxiliary functions. This will be taken into account*/

/* rotate the entire Cube on R*/
void m_X(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    rot_y(&c->pc[i], 1);
  }
}

void m_x(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    rot_y(&c->pc[i], -1);
  }
}

/* rotate the entire Cube on U*/
void m_Y(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    rot_z(&c->pc[i], 1);
  }
}

void m_y(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    rot_z(&c->pc[i], -1);
  }
}

/* rotate the entire Cube on F*/
void m_Z(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    rot_x(&c->pc[i], 1);
  }
}

void m_z(Cube3* c){
  short i;

  for (i=0;i<NPC;i++){
    rot_x(&c->pc[i], -1);
  }
}



/***********AUXILIARY FUNTIONS***********/

void paint_piece(Piece *pc){
  short aux[3][2]={{G,B},{R,O},{W,Y}},i;

  for(i=0;i<3;i++){
    switch(pc->p[i]){
      case 1:
        pc->c[i]=aux[i][0];
        break;
      case -1:
        pc->c[i]=aux[i][1];
        break;
      default:
        pc->c[i]=N;
    }
  }

}

void assign(short *p, short a, short b, short c){
  p[0]=a;
  p[1]=b;
  p[2]=c;
}

/******PIECE ROTATION********/

void rot_x(Piece *p, short dir){
  short y=p->p[1], z=p->p[2], temp;

  p->p[1]=z*dir;
  p->p[2]=-y*dir;

  temp=p->c[1];
  p->c[1]=p->c[2];
  p->c[2]=temp;
}

void rot_y(Piece *p, short dir){
  short x=p->p[0], z=p->p[2], temp;

  p->p[2]=x*dir;
  p->p[0]=-z*dir;

  temp=p->c[0];
  p->c[0]=p->c[2];
  p->c[2]=temp;
}

void rot_z(Piece *p, short dir){
  short x=p->p[0], y=p->p[1], temp;

  p->p[0]=y*dir;
  p->p[1]=-x*dir;

  temp=p->c[0];
  p->c[0]=p->c[1];
  p->c[1]=temp;
}
