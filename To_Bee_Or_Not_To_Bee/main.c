/**
 * @file    main.c
 * @author  Samuel de Lucas Maroto and Bhavuk Sikka
 * 
 * @brief   This file only calls the main functions and handles the music.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "libs/control.h"
#include "libs/grafiks.h"
#include "libs/kolors.h"
#include "libs/quest.h"
#include "libs/body.h"
#include "libs/player.h"
#include "libs/musik.h"
#include "libs/randomizer.h"
#include "libs/types.h"
#include "libs/screen.h"
#include "libs/menu.h"
#include "libs/achievement.h"

musik *m[3], *click, *youdead;

int main(){
    player *pl = NULL;
    FMOD_BOOL isPlaying;
    pthread_t load_threads[3], play_threads[3], loadingthread;
    bool loaded=FALSE;
    int i, choice;
    
    /*setting up the terminal*/
    init_terminal();
    clear_screen();
    hide_cursor();
    srand(time(NULL));
    

    /*initializing everything*/
    if(ini_musik(&(m[0]), "audio/SpaceAudio.mp3", 0.05, TRUE) == ERROR) return ERROR;
    if(ini_musik(&(m[1]), "audio/NatureAudio.mp3", 0.05, TRUE) == ERROR) return ERROR;
    if(ini_musik(&(m[2]), "audio/WooshDelPrincipio.mp3", 0.7, FALSE) == ERROR) return ERROR;
    if(ini_musik(&(click), "audio/Click.mp3", 0.4, FALSE) == ERROR) return ERROR;
    if(ini_musik((&youdead),"audio/Gameover.mp3",0.1, FALSE) == ERROR) return ERROR;

    /*Load the music*/
    load_musik(click);/*Really short sound so no thread is required*/
    load_musik(youdead);/*Really short sound so no thread is required*/
    for(i = 0; i < 3; i++){
        pthread_create(&load_threads[i], NULL, &load_musik, m[i]);
    }
    
    while(m[2]->loaded == FALSE){
        /*Waiting for the initial sound to load*/
        usleep(10);
    }
    pthread_create(&play_threads[2], NULL, &play_musik, m[2]);

    /*Loading screen (waiting for the music to load while playing the initial sound)*/
    pthread_create(&loadingthread, NULL, &loading_screen, &loaded);
    do{
        FMOD_Channel_IsPlaying(m[2]->chan, &isPlaying);
        usleep(10);
    }while(m[0]->loaded == FALSE || m[1]->loaded == FALSE || isPlaying);
    loaded=TRUE;
    pthread_join(loadingthread, NULL);
    
    /*Play the music*/
    pthread_create(&play_threads[0],NULL,play_musik,m[0]);
    pthread_create(&play_threads[1],NULL,play_musik,m[1]);

    /*Display menu*/
    do{
        if(ini_player(&pl)==ERROR) return ERROR;
        /*we initialize a new player every time so settings get updated*/
        clear_stdin();
        choice = display_menu(get_player_lan(pl), click);
        clear_screen();
        
        switch(choice){
            case 0:
                if(play_game(pl)==ERROR) return ERROR;
                break;
            case 1:
                if(how_to(get_player_lan(pl))==ERROR) return ERROR;
                break;
            case 2:
                if(achievments(get_player_lan(pl))==ERROR) return ERROR;
                break;
            case 3:
                if(settings(get_player_lan(pl))==ERROR) return ERROR;
                break;
            default:/*Exit option or error, both lead to an exit*/
                break;
        }
        
        free_player(&pl);
    } while (choice >= 0 && choice <= 3);
    
    /*Free operations*/
    for(i = 0; i < 3; i++){
        free_musik(m[i]);
    }
    
    free_musik(click);
    free_musik(youdead);
    end_musik();
    free_player(&pl);
    clear_screen();
    close_terminal();
    return OK;
}
