/**
 * @file    menu.c
 * @author  Samuel de Lucas Maroto and Bhavuk Sikka
 * 
 * @brief   This module has all the functions needed for the game menu. Each of them are
 *          called once the user has chosen an option from the menu.
 * */

#include "menu.h"

extern pthread_mutex_t beemutex;
extern pthread_mutex_t prints;
extern short NUMQUESTS;
extern musik *m[3], *click, *youdead;

/*______PLAY_GAME_______________________*/

status play_game(player *pl){
    int score;
    section *sec=NULL,*sop1=NULL,*sop2=NULL,**sex=NULL, *sect=NULL, *secti=NULL,*schar=NULL,*sname;
    pts *p=NULL,*paux=NULL, *paux2=NULL;
    kolor *k=NULL;
    quest *q=NULL;
    effect *e=NULL;
    pthread_t beethread;
    beestruct beecharacteristics;
    char *text,*op1,*op2,c[3]={'\0','\0','\0'},*neim,beename[NAMELEN]="";
    int column,row;
    short aux = -1;
    /*int prev = 0;*/
    
    if(!pl) return ERROR;
    clear_screen();
    /*initializing everything*/
    if(create_kolor(&k)==ERROR) return ERROR;
    
    if(create_section(&sname)==ERROR) return ERROR;
    if(create_section(&sec)==ERROR) return ERROR;
    if(create_section(&sop1)==ERROR) return ERROR;
    if(create_section(&sop2)==ERROR) return ERROR; 
    if(create_section(&sect)==ERROR) return ERROR;
    if(create_section(&secti)==ERROR) return ERROR;
    if(create_section(&schar)==ERROR) return ERROR;
    if(create_point(&p)==ERROR) return ERROR;
    
    if(ini_quest(&q)==ERROR) return ERROR;
    if(pthread_mutex_init(&beemutex,NULL)!=0) return ERROR;
    
    if(!(sex=paint_layout(2,NULL))) return ERROR;

    

    /*WE SET ALL SECTIONS SIZES AND POSITIONS*/
    srand(time(NULL));
    if(!(paux=get_section_point(sex[0]))) return ERROR;
    row=get_point_row(paux)+get_section_row(sex[0])/5;
    column=get_point_col(paux)+get_section_col(sex[0])/5;
    set_point(p,row,column);
    
    free_point(&paux);
    if(!(paux=get_section_point(sex[1]))) return ERROR;
    
    set_section(sec,p,get_section_row(sex[0])/4, get_section_col(sex[0])-2*get_section_col(sex[0])/5, NULL);
    set_point(p,row+get_section_row(sec)+2,column);
    set_section(sop1,p,4,get_section_col(sex[0])-2*get_section_col(sex[0])/5,NULL);
    set_point(p,get_point_row(p)+5,column);
    set_section(sop2,p,4,get_section_col(sex[0])-2*get_section_col(sex[0])/5,NULL);
    row=get_point_row(p);
    column=get_point_col(p)-3;

    /*SECCION DE LA ABEJA */
    set_kolor(k,YELLOW,BLACK);
    set_point(p,get_point_row(paux)+get_section_row(sex[1])*2/9,get_point_col(paux)+get_section_col(sex[1])/10);
    
    set_section(sect,p,get_section_row(sex[1])*7/9,get_section_col(sex[1])-get_section_col(sex[1])/10-get_section_col(sex[1])/10,k);

    /*EL NOMBRE DE LOS PERSONAJES*/
    set_kolor(k,231,DEFAULT_BG);
    set_point(p,get_point_row(p)*4/7,get_point_col(p));
    set_section(sname,p,2,get_section_col(sect),k);
    free_point(&paux);
    /*Seteando seccion de las barras*/
    if(!(paux=get_section_point(sex[2]))) return ERROR;
    
    set_kolor(k,DEFAULT_FG,DEFAULT_BG);
    set_point(paux, get_point_row(paux), get_point_col(paux)+get_section_col(sex[2])/15);   /*This formula displays the bars centered*/
    set_section(secti,paux, get_section_row(sex[2]), ((int)((14*get_section_col(sex[2]))/15)/4)*4 -1, k);
    /*free_point(&paux);*/
    display_bars(pl, secti, 0);
    beecharacteristics.lang=get_player_lan(pl);
    beecharacteristics.sec=sect;
    beecharacteristics.name=beename;
    beecharacteristics.aux=0;
    pthread_create(&beethread,NULL,&display_bee,&beecharacteristics);

    
    score=0;
    while(is_dead(pl) == 0){

        if(next_quest(q, pl) == ERROR) return ERROR;

        text=get_quest_desc(q);
        op1=get_quest_opt(q,1);
        op2=get_quest_opt(q,2);
        neim=get_quest_char(q);
        
        beecharacteristics.aux=0;
        pthread_mutex_lock(&beemutex);
        strcpy(beename,neim);
        beecharacteristics.aux=1;
        pthread_mutex_unlock(&beemutex);

        clear_section(sname);
        textbox(neim,sname,0,NULL);
        clear_section(sec);
        textbox(text,sec,0,NULL);
        clear_section(sop1);
        textbox(op1,sop1,0,NULL);
        clear_section(sop2);
        textbox(op2,sop2,0,NULL);
        free(text);free(op1);free(op2);free(neim);
    
        /*Esto únicamente servía para debuguear las quest*/

        /*pthread_mutex_lock(&prints);
        move_cursor(3,3);
        printf("Quest: %5d\n",get_quest_num(q));
        move_cursor(4,3);
        printf("Prev: %5d\n",prev);
        prev = get_quest_num(q);
        pthread_mutex_unlock(&prints);*/
        
        /*CHOOSE BETWEEN OPTIONS*/
        set_point(paux,row+5*aux,column);
        draw_line(paux,paux,"⮞",sex[0],NULL,0);
        c[0]=0;
        while(c[0]!='\n' && c[0]!='q' && c[0]!='Q'){
            c[0]=(char)fgetc(stdin);
            if(c[0]==27){
                c[1]=(char)fgetc(stdin);
                c[2]=(char)fgetc(stdin);
                if(c[0]==27 && c[1]==91 && (c[2]==65 || c[2]==66)){ /*If you hit the up/down arrows*/
                    set_point(paux,row+5*aux,column);
                    draw_line(paux,paux," ",sex[0],NULL,0);
                    aux = (aux-1)%2; /*This alternates aux between 0 and -1*/
                   
                    play_musik(click);

                    set_point(paux,row+5*aux,column);
                    draw_line(paux,paux,"⮞",sex[0],NULL,0);
                }
            }
        }
        if(c[0]!='\n'){
            /*If the character isn't '\n' it must be q/Q, so we want to exit*/
            break;
        } else {
            e = get_quest_effect(q, aux+2);
            apply_effect(pl, e);
            if(get_eff_ongoing(e) >= 1) push_continuation(get_quest_num(q), aux+2, get_eff_ongoing(e));
            else score++;
            
            
            /*DISPLAY BARS*/
            clear_section(secti);
            display_bars(pl, secti, 1);
            free_effect(&e);
        }
    }
    
    /*If you are dead then we wait for the user to hit enter so 
     *he/she can see the bars before the game over screen*/
    if(c[0]=='\n'){
        clear_stdin();
        getchar();
    } 
    
    
    beecharacteristics.aux=-1;
    pthread_join(beethread, NULL);
    pthread_mutex_destroy(&beemutex);
    pause_player_thread();

    /*Free operations*/
    free_section(&sec);
    free_section(&sop1);
    free_section(&sop2);
    free_section(&sect);
    free_section(&secti);
    free_section(&schar);
    free_section(&sname);
    free_section(&sex[0]);
    free_section(&sex[1]);
    free_section(&sex[2]);
    free(sex);
    free_point(&p);
    free_point(&paux);
    free_point(&paux2);
    free_kolor(&k);
    free_quest(&q);
    free_effect(&e);
    free_randomizer();

    if(is_dead(pl) != 0){
        int i;
        score*=13;
        
        /*Bump up the counter for the achievments 1 to 4 (the ones that keep track of the times that you have played)*/
        for(i = 1; i <= 4; i++) bump_up_counter(i);
        /*Bump up the counter for the achievments 5,6,7 or 8,9,10 or 11,12,13 or 14,15,16 (the ones that depend on the player's death)*/
        for(i = 2; i < 5; i++) bump_up_counter(3 * is_dead(pl) + i);
        
        FMOD_Channel_SetPaused(m[0]->chan,1);
        FMOD_Channel_SetPaused(m[1]->chan,1);
        play_musik(youdead);
        game_over(is_dead(pl),get_player_lan(pl),score);
        sleep(3);   /*Force the player to read the game over screen. Otherwise it is easily skippable by accident*/
        clear_stdin();
        getchar();
        stop_musik(youdead);
    }
    FMOD_Channel_SetPaused(m[0]->chan,0);
    FMOD_Channel_SetPaused(m[1]->chan,0);
    return OK;
}

/*______HOW_TO____________________________*/

status how_to(int lan){
    section **s = NULL;
    pts *p;
    kolor *k;
    int r, c, aux;
    int len = 82;           /*Length of the title*/
    int buf = len * 10;     /*Buffer size*/
    char *str;       /*Buffers to read the lines of the file and the user input*/
    char howto_path[19];
    FILE *pf = NULL;
    
    /*Initialize everithing*/
    clear_screen();
    if(create_kolor(&k) == ERROR) return ERROR;
    if(set_kolor(k,BORDERKOLOR,DEFAULT_BG) == ERROR) return ERROR;
    if(!(s = paint_layout(1, k))) return ERROR;
    r = get_section_row(s[0]);
    c = get_section_col(s[0]);
    
    if(create_point(&p) == ERROR) return ERROR;
    if(set_point(p, (int)(r*0.25), (int)(c/2 - len/2)) == ERROR) return ERROR;

    /*Open the how to file depending on the language*/
    if(lan == 1) strcpy(howto_path, "extra/howtoplay_es");
    else strcpy(howto_path, "extra/howtoplay_en");
    if(!(pf = fopen(howto_path, "r"))) return ERROR;
    
    /*Allocate the buffer*/
    if(!(str = (char *)calloc(buf, sizeof(char)))) return ERROR;
        
    /*Read and print the howto file*/
    aux = 0;
    if(set_kolor(k,226,DEFAULT_BG) == ERROR) return ERROR;
    while(getline(&str, (size_t *)(&buf), pf) > 0){
        if(draw_line(p, p, str, s[0], k, 0)) return ERROR;
        if(set_point(p, get_point_row(p) + 1, get_point_col(p)) == ERROR) return ERROR;
        aux++;
        if(aux == 12) set_kolor(k,DEFAULT_FG,DEFAULT_BG); /*Change the color for the text (we print the title in yellow and then the text in white)*/
    }
    free(str);
    free_section(&s[0]);
    free(s);
    free_point(&p);
    free_kolor(&k);
    fclose(pf);
    clear_stdin();
    getchar();
    return OK;
}

status settings(int lan){
    section **sex, *slang[3], *rst, *notes;
    pts *p1, *p2, *p3, *p4, *paux;
    FILE *f;
    kolor *k;
    char *buf;
    int i, aux=0;
    char crst[]="RESET GAME";
    char ch[75],c[3];
    char flecha[4]="⮞";
    char idioma[]="Language / Idioma";
    const int bufs = 256;

    clear_screen();
    if(!(sex=paint_layout(1,NULL))) return ERROR;
    for (i=0;i<3;i++){
        if(create_section(&slang[i])==ERROR) return ERROR;
    }
    if(create_section(&rst)==ERROR) return ERROR;
    if(create_section(&notes)==ERROR) return ERROR;
    if(create_point(&p1)==ERROR) return ERROR;
    if(create_point(&p2)==ERROR) return ERROR;
    if(create_point(&p3)==ERROR) return ERROR;
    if(create_point(&p4)==ERROR) return ERROR;
    if(create_point(&paux)==ERROR) return ERROR;
    if(create_kolor(&k)==ERROR) return ERROR;

    set_kolor(k,226,DEFAULT_BG);
    if(lan == 0) f=fopen("extra/settings_en","r");
    else f=fopen("extra/settings_es","r");
    
    if(!f){
        /*Only displayed if there is an error opening the file (backup plan)*/
        set_point(paux, get_section_row(sex[0])/4, (get_section_col(sex[0]) - 8)/2); /*The word SETTINGS occupies 8 length*/
        draw_line(paux,paux,"SETTINGS",sex[0],k,0);
    }else{
        if(!(buf=(char*)malloc(bufs))){
            fclose(f);
            return ERROR;
        }
        set_point(paux, get_section_row(sex[0])/4, (get_section_col(sex[0])-60)/2); /*The settings' banner occupies 60 length*/
        while(getline(&buf, (size_t *)(&bufs), f) > 0){
            draw_line(paux,paux,buf,sex[0],k,0);
            set_point(paux,get_point_row(paux)+1,get_point_col(paux));
        }
        free(buf);
        fclose(f);
    }

    set_point(p1, get_section_row(sex[0])/2, (get_section_col(sex[0])-strlen(idioma))/2);
    set_point(p2, get_point_row(p1) + 2, get_section_col(sex[0])/2-6);
    set_point(p3, get_point_row(p1) + 2, get_section_col(sex[0])/2+4);
    set_section(slang[0],p1,1,strlen(idioma)+1,NULL);
    set_section(slang[1],p2,1,3,NULL);
    set_section(slang[2],p3,1,3,NULL);

    textbox(idioma, slang[0], 0, NULL);
    textbox("EN", slang[1], 0, NULL);
    textbox("ES", slang[2], 0, NULL);
    
    set_point(p4,get_section_row(sex[0])/2 +8, (get_section_col(sex[0])-strlen(crst))/2);
    set_section(rst,p4,1,strlen(crst)+1,NULL);
    textbox(crst, rst, 0, NULL);    /*RESET GAME button*/
    
    set_point(paux, get_section_row(sex[0]), 2);
    set_section(notes,paux,1,get_section_col(sex[0]),NULL);
    if(lan==0) strcpy(ch,"Press Q to save changes and return to menu");
    else strcpy(ch, "Presiona Q para guardar los cambios y volver al menú");
    textbox(ch,notes,0,NULL);
    
    set_point(p1, get_point_row(p1), get_point_col(p1)-2);
    set_point(p2, get_point_row(p2), get_point_col(p2)-2);
    set_point(p3, get_point_row(p3), get_point_col(p3)-2);
    set_point(p4, get_point_row(p4), get_point_col(p4)-2);
    
    draw_line(p1,p1,flecha,sex[0],NULL,0);
    clear_stdin();
    do{
        c[0]=0;
        while(c[0]!='\n' && c[0]!='q' && c[0]!='Q'){
            c[0]=(char)fgetc(stdin);
            if(c[0]==27){
                c[1]=(char)fgetc(stdin);
                c[2]=(char)fgetc(stdin);
                if(c[0]==27 && c[1]==91 && (c[2]=='A' || c[2]=='B')){ /*If you hit the up/down arrows*/
                    aux = (aux+1)%2;
                    if(aux){ 
                        draw_line(p1,p1," ",sex[0],NULL,0);
                        draw_line(p4,p4,"⮞",sex[0],NULL,0);
                    }else{
                        draw_line(p1,p1,"⮞",sex[0],NULL,0);
                        draw_line(p4,p4," ",sex[0],NULL,0);
                    }
                    play_musik(click);
                }
            }
        }
        draw_line(p1,p1," ",sex[0],NULL,0);
        draw_line(p4,p4," ",sex[0],NULL,0);
        
        if(c[0]=='\n'){
            c[0]=0;
            switch (aux){
            case 0:
                if(lan==0) draw_line(p2,p2,"⮞",sex[0],NULL,0);
                else draw_line(p3,p3,"⮞",sex[0],NULL,0);
                while(c[0]!='\n' && c[0]!='q' && c[0]!='Q'){
                    c[0]=(char)fgetc(stdin);
                    if(c[0]==27){
                        c[1]=(char)fgetc(stdin);
                        c[2]=(char)fgetc(stdin);
                        if(c[0]==27 && c[1]==91 && (c[2]=='C' || c[2]=='D')){ /*If you hit the up/down arrows*/
                            lan = (lan+1)%2;
                            if(lan){ 
                                draw_line(p2,p2," ",sex[0],NULL,0);
                                draw_line(p3,p3,"⮞",sex[0],NULL,0);
                            }else{
                                draw_line(p2,p2,"⮞",sex[0],NULL,0);
                                draw_line(p3,p3," ",sex[0],NULL,0);
                            }
                            play_musik(click);
                        }
                    }
                }
                if(c[0]=='\n')  update_config(lan,-1);
                draw_line(p1,p1,"⮞",sex[0],NULL,0);
                break;
            default:
                reset_all_achievements();
                update_config(-1,0);
                c[0]='q';
                break;
            }
            draw_line(p2,p2," ",sex[0],NULL,0);
            draw_line(p3,p3," ",sex[0],NULL,0);
            
        }

    }while(c[0]!='q' && c[0]!='Q');



    free_section(&sex[0]);
    free(sex);
    for (i=0;i<3;i++){
        free_section(&slang[i]);
    }
    free_section(&rst);
    free_section(&notes);
    free_point(&p1);
    free_point(&p2);
    free_point(&p3);
    free_point(&p4);
    free_point(&paux);
    free_kolor(&k);

    return OK;
}

status achievments(int lan){
    achievement **a;
    section **s, *notes;
    pts *p;
    char note[75];
    kolor *k;
    int i, c, t;
    char buf[512];
    
    if(!(s = paint_layout(1,NULL))) return ERROR;
    if(create_point(&p)==ERROR) return ERROR;
    if(create_kolor(&k)==ERROR) return ERROR;
    if(read_all_achievements(lan, &a) == ERROR) return ERROR;
    if(create_section(&notes)==ERROR) return ERROR;

    if(set_point(p, (get_section_row(s[0]) - 2*(MAX_N+2.))/2, get_section_col(s[0])/2 - 44) == ERROR) return ERROR;
    for(i = 0; i < MAX_N; i++){
        if(set_point(p, get_point_row(p)+2, get_point_col(p)) == ERROR) return ERROR;
        c = get_a_counter(a[i]);
        t = get_a_total(a[i]);
        sprintf(buf, "%-20s %-60s \t %2d / %2d", get_a_title(a[i]), get_a_description(a[i]), c, t);
        
        if(c == t) set_kolor(k, 76, DEFAULT_BG);
        else set_kolor(k, DEFAULT_FG, DEFAULT_BG);
        draw_line(p, p, buf, s[0], k, 0);
    }

    if(lan==0) strcpy(note,"Press any key to return to menu");
    else strcpy(note, "Presiona cualquier tecla para volver al menú");
    set_point(p,get_section_row(s[0]),2);
    set_section(notes,p,1,get_section_col(s[0]),NULL);
    textbox(note,notes,0,NULL);
    clear_stdin();
    getchar();

    
    end_all_achievements(a);
    free_section(s);
    free(s);
    free_section(&notes);
    free_kolor(&k);
    free_point(&p);
    return OK;
}
