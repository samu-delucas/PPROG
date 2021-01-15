/**
 * @file    body.c
 * @author  Bhavuk Sikka
 * 
 * @brief   This module handles the drawing of the bees
 * */

#ifndef _BODY
#define _BODY

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include "grafiks.h"
#include "control.h"
#include "kolors.h"
#include "quest.h"
#include "types.h"

typedef struct body body;

typedef struct beestruct{
    section *sec;
    char *name;
    int lang;
    int aux;
} beestruct;


/** @brief      Creates a body
 *  @param f1   Address of a pointer of type body
 *  @return     Returns ERROR in case of error and OK if everything goes well */
status create_body(body **b);

/** @brief      Sets the body's parameters
 *  @param f1   Pointer of body
 *  @param f2   Tallness of the bee (0-2)
 *  @param f3   Fatness of the bee (0-2)
 *  @param f4   Angryness of the bee (0-2)
 *  @param f5   Hair tallness (0-10)
 *  @param f6   Wing size (0-3)
 *  @return     Returns ERROR in case of error and OK if everything goes well */
status set_body(body *b, const short tall, const short fat, const short angry, const short hair);

/** @brief      Gets body's tallness
 *  @param f1   Pointer of body
 *  @return     Returns -1 in case of error. Otherwise, it returns tallness parameter */
short get_body_tall(body *b);

/** @brief      Gets body's fatness
 *  @param f1   Pointer of body
 *  @return     Returns -1 in case of error. Otherwise, it returns fatness parameter */
short get_body_fat(body *b);

/** @brief      Gets body's angryness
 *  @param f1   Pointer of body
 *  @return     Returns -1 in case of error. Otherwise, it returns angryness parameter */
short get_body_angry(body *b);

/** @brief      Gets body's hair growth
 *  @param f1   Pointer of body
 *  @return     Returns -1 in case of error. Otherwise, it returns hair parameter */
short get_body_hair(body *b);

/** @brief      Gets body's wings
 *  @param f1   Pointer of body
 *  @return     Returns -1 in case of error. Otherwise, it returns wings parameter */
short get_body_wings(body *b);

/** @brief      Frees the body
 *  @param f1   Address of pointer of body*/
void free_body(body **b);

/** @brief      Draws the bee in a section
 *  @param f1   Pointer of section
 *  @param f2   Name of the character
 *  @param f3   Language of the name of the character
 *  @return     Returns ERROR in case of error and OK if everything goes well*/
status draw_bee(section *sec,char *name,short lang);

/**
 *  @brief      Adds two path characters, among these: "┓", "┏", "┛", "┗", "━", "┃", "┣", "┫", "┻", "┳", "╋"
 *  @param f1   First path character
 *  @param f2   Second path character
 *  @return     Returns the resulting character of combining two path characters
*/
int res(int a, int b);

void *display_bee(void *voidb);
#endif /*_BODY*/
