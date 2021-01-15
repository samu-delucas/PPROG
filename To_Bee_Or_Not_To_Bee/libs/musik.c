/**
 * @file    musik.c
 * @author  Bhavuk Sikka and Samuel de Lucas Maroto
 * 
 * @brief   This module handles the functions needed for loading and
 *          playing the background music.
 * */ 


#include "musik.h"

FMOD_SYSTEM *sys = NULL;

status ini_musik(musik **m, char *filename, float vol, bool loop){
    if(!m || !filename) return ERROR;
    
    if(!sys){
        if(FMOD_System_Create(&sys) != FMOD_OK) return ERROR;
        if(FMOD_System_Init(sys, 6, FMOD_INIT_NORMAL, NULL) != FMOD_OK) return -1;
    }
	if(!(*m = (musik *)malloc(sizeof(musik)))) return ERROR;
    
    (*m)->vol=vol;
    (*m)->loop=loop;
    (*m)->system = sys;
    strcpy((*m)->audiofile, filename);
    (*m)->loaded = FALSE;
    (*m)->chan=NULL;
    return OK;
}

void free_musik(musik *m){
    if(m){
        stop_musik(m);
        free(m);
    }
}

void *load_musik(void *mvoid){
    musik *m = (musik *)mvoid;
    if(FMOD_System_CreateSound(m->system, m->audiofile, FMOD_DEFAULT, NULL, &m->sound) != FMOD_OK) return NULL;
    m->loaded = TRUE;
    return NULL;
}

void *play_musik(void *mvoid){
    musik *m = (musik *)mvoid;
    FMOD_BOOL isPlaying;
    
    FMOD_Channel_Stop(m->chan);
    do{
        if(FMOD_Channel_IsPlaying(m->chan, &isPlaying) == FMOD_OK){
            /*Checks every 10 seconds if the music is still playing*/
            sleep(5);
        }else{
            /*If not, it starts the music again*/
            /*if(m->loop==TRUE){
                sleep(rand()%3+1);
            }*/
            
            FMOD_System_PlaySound(m->system, m->sound, NULL, 1, &m->chan);
            FMOD_Channel_SetVolume(m->chan,m->vol);
            FMOD_Channel_SetPaused(m->chan,0);
            FMOD_System_Update(m->system);
        }
    }while(m->loop == TRUE); /*Only loop the ones that must loop*/
    return NULL;
}

void stop_musik(musik *m){
    if(m){
        FMOD_Channel_Stop(m->chan);
    }
}

void end_musik(){
    FMOD_System_Close(sys);
    FMOD_System_Release(sys);
}
