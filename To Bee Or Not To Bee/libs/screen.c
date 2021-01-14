/**
 * @file    screen.c
 * @author  Bhavuk Sikka and Samuel de Lucas Maroto
 * 
 * @brief   This module has all the functions related to specific screens that are displayed during
 *          the game (i.e. loading screen, menu screen, game over screen, etc)
 * */

#include "screen.h"
#define M_PI		3.14159265358979323846


/*______LOADING_SCREEN__________________________*/

#include "body.h"
void *loading_screen(void *l){ 
	bool *loaded = (bool *)l;
    double i=0,x,y;
	short colors[] = {202,208,214,220,226,227,190,184,172,178};
    int min, j=0, c=0;
	pts *p1, *p2;
	section **sec = NULL;
	kolor *k;

    /*Initialize everything*/
    clear_screen();
    if(create_kolor(&k) == ERROR) return NULL;
    set_kolor(k,BORDERKOLOR,DEFAULT_BG);
	if(!(sec = paint_layout(1, k))) return NULL;
	if(!(p1=get_section_point(sec[0]))) return NULL;
    if(create_point(&p2) == ERROR) return NULL;
    set_point(p1,get_section_row(sec[0])/2+get_point_row(p1),get_section_col(sec[0])/2+get_point_col(p1));
    min=get_section_col(sec[0])*(get_section_row(sec[0])<get_section_col(sec[0]))+get_section_row(sec[0])*(get_section_row(sec[0])>=get_section_col(sec[0]));
    
    while(*loaded==FALSE || j != 200){
        x=sin(i)*min;
        y=cos(i)*min;
        set_point(p2,x+get_point_row(p1),y+get_point_col(p1));
        set_kolor(k,colors[(c++)%10],DEFAULT_BG);
        if(j<100){
            draw_line(p1,p2,NULL,sec[0],k,0);
        }else if(j==100||j==200){
            i=0;
        }else{
            draw_line(p1,p2," ",sec[0],NULL,0);
        }
        if(*loaded==FALSE){
            usleep(20000);
        }else{
            usleep(5000);
        }
        /*clear_section(sec[0]);*/
        j=(j+1)%201;
        i+=M_PI/50;
    }
	usleep(50000);
	clear_screen();
	free_point(&p1);
    free_point(&p2);
    free_kolor(&k);
	free_section(&sec[0]);
	free(sec);
    return NULL;
}

/*______DISPLAY_MENU__________________________*/

int display_menu(int lan, musik *click){
    section **s = NULL;
    pts *p;
    kolor *k;
    int r, c, aux;
    int len = 82;           /*Length of the title*/
    int buf = len * 10;     /*Buffer size*/
    char *str, ch[3];       /*Buffers to read the lines of the file and the user input*/
    char menu_path[14];
    FILE *pf = NULL;
    
    /*Initialize everithing*/
    clear_screen();
    if(create_kolor(&k) == ERROR) return -1;
    
    if(set_kolor(k,BORDERKOLOR,DEFAULT_BG) == ERROR) return -1;
    if(!(s = paint_layout(1, k))) return -1;
    r = get_section_row(s[0]);
    c = get_section_col(s[0]);
    
    if(create_point(&p) == ERROR) return -1;
    if(set_point(p, (int)(r*0.25), (int)(c/2 - len/2)) == ERROR) return -1;

    /*Open the menu file depending on the language*/
    if(lan == 1) strcpy(menu_path, "extra/menu_es");
    else strcpy(menu_path, "extra/menu_en");
    
    if(!(pf = fopen(menu_path, "r"))) return -1;

    /*Allocate the buffer*/
    if(!(str = (char *)calloc(buf, sizeof(char)))) return -1;
    
    /*Read and print the menu file*/
    aux = 0;
    if(set_kolor(k,226,DEFAULT_BG) == ERROR) return -1;
    while(getline(&str, (size_t *)(&buf), pf) > 0){
        if(draw_line(p, p, str, s[0], k, 0)) return -1;
        if(set_point(p, get_point_row(p) + 1, get_point_col(p)) == ERROR) return -1;
        aux++;
        if(aux == 16) set_kolor(k,DEFAULT_FG,DEFAULT_BG); /*Change the color for the text (we print the title in yellow and then the text in white)*/
    }
    free(str);
    
    /*Draw the arrow and read the user input*/
    aux = 0;    /*This variable just indicates the current option that the user has chosen*/
    do{
        if(set_point(p, (int)(r*0.25) + 20 + 2*aux, (int)(c/2 - 7)) == ERROR) return -1;        
        if(draw_line(p, p, "⮞", s[0], k, 0)) return -1;        
        
        /*This is how the arrow keys are codified*/
        ch[0]=(char)fgetc(stdin);
        if(ch[0]==27){
            ch[1]=(char)fgetc(stdin);
            if(ch[1] == 91){
                ch[2]=(char)fgetc(stdin);
                switch(ch[2]){
                    case 65:
                        aux += 5; /*This just assures that 'aux' will be positive after the next operation*/
                        aux = (aux-1)%5;
                        play_musik(click);
                        break;
                    case 66:
                        aux = (aux+1)%5;
                        play_musik(click);
                        break;
                    default:
                        break;
                }
                if(draw_line(p, p, " ", s[0], k, 0) == ERROR) return -1;
            }
        }
    } while(ch[0]!='\n');   /*Exit when the user hits enter*/
    
    free_section(&s[0]);
    free(s);
    free_point(&p);
    free_kolor(&k);
    fclose(pf);
    return aux;
}
 
/*______GAME_OVER__________________________*/

void game_over(int n, int lang, int score){
    section **sex, *sec, *sect, *stext;
    pts *paux, *paux2, *paux3;
    kolor *k;
    FILE *f;
    char gameover[700];
    char es[32]="Puntuación Final: ";
    char en[32]="Final Score: ";
    char *caux;
    int *rc, r, c;

    /*Initialize everything*/
    clear_screen();
    clear_stdin();
    if(!(sex=paint_layout(1,NULL))) return;
    if(!(paux=get_section_point(sex[0]))) return;
    if(create_point(&paux2)==ERROR) return;
    if(create_section(&sec)==ERROR) return;
    if(create_section(&sect)==ERROR) return;
    if(create_section(&stext)==ERROR) return;
    if(create_kolor(&k)==ERROR) return;
    if(!(rc=get_winsize())) return;
    
    set_point(paux2,rc[0]*2/7,rc[1]/10);
    set_kolor(k,YELLOW,BLACK);
    set_section(sect,paux2,rc[0]*5/7,rc[1]*7/19,k);
    
    set_point(paux2,get_point_row(paux2)+1,get_point_col(paux2)+get_section_col(sect)+rc[1]/10);
    set_kolor(k,DEFAULT_FG,DEFAULT_BG);
    set_section(stext,paux2,get_section_row(sect)*5/7,get_section_col(sect)*5/7,k);

    set_point(paux,rc[0]/10,(rc[1]-49)/2);
    set_section(sec,paux,5,49,k);

    set_kolor(k,RED,DEFAULT_BG);
    set_point(paux,get_point_row(paux)+(get_section_row(sec)-5)/2,get_point_col(paux)+(get_section_col(sec)-50)/2);
    if(!(f=fopen("extra/gameover","r"))){
        draw_line(paux,paux,"GAME OVER",sec,k,0);
    }else{
        set_section(sec,paux,get_section_row(sec),51,k);
        while (fscanf(f," %[^\n]",gameover)!=EOF){
            draw_line(paux,paux,gameover, sec, k, 0);
            set_point(paux,get_point_row(paux)+1,get_point_col(paux));
            set_section(sec,paux,get_section_row(sec),get_section_col(sec),k);
        }
        fclose(f);
    }
    draw_bee(sect,"Dylan",lang);    /*Dylan is the bee that appears in the game over screen*/
    
    if(!(paux3=get_section_point(stext))) return;
    r=get_point_row(paux3);
    c=get_point_col(paux3);
    /*En las siguientes lineas se dibuja el contorno del texto alrededor de la seccion stext (para mas claridad, ver el juego)*/
    set_point(paux,r-1,c+get_section_col(stext));
    set_point(paux3,r-1,c-1);
    draw_line(paux,paux3,NULL,sex[0],NULL,0);

    set_point(paux3,r+get_section_row(stext),c+get_section_col(stext));
    draw_line(paux,paux3,NULL,sex[0],NULL,0);

    set_point(paux,r+get_section_row(stext),c-1);
    draw_line(paux,paux3,NULL,sex[0],NULL,0);

    set_point(paux3,r+2*get_section_row(stext)/3,c-1);
    draw_line(paux,paux3,NULL,sex[0],NULL,0);
    
    set_point(paux2,r+get_section_row(stext)/2,c-rc[1]/8);
    draw_line(paux2,paux3,NULL,sex[0],NULL,0);
    
    set_point(paux2,get_point_row(paux2)-1,get_point_col(paux2));
    set_point(paux,r+get_section_row(stext)/3,c-1);
    draw_line(paux2,paux,NULL,sex[0],NULL,0);

    set_point(paux3,r-1,c-1);
    draw_line(paux,paux3,NULL,sex[0],NULL,0);
    if(lang==0){
        sprintf(gameover,"quests/en/death%d.txt",n);
    }else if(lang==1){
        sprintf(gameover,"quests/es/muerte%d.txt",n);
    }
    if(!(f=fopen(gameover,"r"))){
        set_point(paux,r,c);
        if(lang==0) strcpy(gameover,"Ummm... we are dead.");
        if(lang==1) strcpy(gameover,"Esto... estamos muertos.");  /*Este texto solo se muestra si hay errores abriendo el archivo*/
    }else{
        fscanf(f,"%[^\n]",gameover);
        fclose(f);
    }
    free_point(&paux3);
    paux3=get_section_point(stext);
    if(lang==0) caux=en;
    else caux=es;
    sprintf(&caux[strlen(caux)],"%d",score);
    set_point(paux3,get_point_row(paux3)+get_section_row(stext)*2/3+3,get_point_col(paux3)+(get_section_col(stext)-strlen(caux))/2);
    draw_line(paux3,paux3,caux,sex[0],NULL,0);
    free_point(&paux3);
    paux3=get_section_point(stext);
    set_point(paux3,get_point_row(paux3)+get_section_row(stext)/6,get_point_col(paux3)+get_section_col(stext)/6);
    set_section(stext,paux3,get_section_row(stext)*2/3,get_section_col(stext)*2/3,NULL);
    textbox(gameover,stext,0,NULL);

    
    free(rc);
    free_kolor(&k);
    free_section(&stext);
    free_section(&sect);
    free_section(&sec);
    free_point(&paux3);
    free_point(&paux2);
    free_point(&paux);
    free(sex[0]);
    free(sex);
}
