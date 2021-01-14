/**
 * @file    musik.h
 * @author  Bhavuk Sikka and Samuel de Lucas Maroto
 * 
 * @brief   This module handles the functions needed for loading and
 *          playing the background music.
 * */  


#ifndef _MUSIK
#define _MUSIK

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "types.h"
#include "fmod/api/core/inc/fmod.h"

/**
 * @struct  musik
 * @brief   struct that contains all the information needed for loading
 *          and playing a particular audio file. This information is the 
 *          name of the file, a variable that indicates if it has been loaded
 *          correctly, and other stuff needed for FMOD
 * */
typedef struct musik{
    FMOD_SYSTEM *system;
    FMOD_SOUND *sound;
    FMOD_CHANNEL *chan;
    char audiofile[50];
    float vol;
    bool loaded;
    bool loop;
} musik;

/**
 * @brief   function that initialices the music structure values. Allocates memory.
 * @param   m struct to be initialiced
 * @param   filename name of the audio file (path)
 * @param   vol volume from 0 to 1 of sound
 * @return  OK if no errors ocurred ERROR otherwise
 * */
status ini_musik(musik **m, char * filename, float vol, bool loop);

/**
 * @brief   function that frees the memory allocated for the musik structure m
 * @param   m struct to be initialiced
 * */
void free_musik(musik *m);

/**
 * @brief   function that loads an audio file. We use it in a thread
 * @param   m struct with the audio file
 * */
void *load_musik(void *mvoid);

/**
 * @brief   function that plays the music. We use it in a thread
 * @param   m struct with the audio file
 * */
void *play_musik(void *mvoid);

void stop_musik(musik *musik);

/**
 * @brief   function that stops the music and handles some FMOD variables
 * */
void end_musik();

#endif /*_MUSIK*/ 
