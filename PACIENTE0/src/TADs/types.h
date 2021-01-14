/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   types.h
 * Author: e422993
 *
 * Created on 4 de febrero de 2020, 11:03
 */



/*** @file types.h* @author Profesores Programación 2* @date 2 February 2020* @brief ADT Boolean and Status** @details Here typically goes a more extensive explanation of what the header* defines. Doxygens tags are words preceeded by @.** @see*/
#ifndef TYPES_H_
#define TYPES_H_
/*** @brief ADT Boolean*/

typedef enum {
    FALSE = 0,/*!< False value */
    TRUE = 1 /*!< True value  */
} Bool; /*** @brief ADT Status*/

typedef enum {
    ERROR = 0, /*!< To codify an ERROR output */
    OK = 1 /*!< OK output */
} Status;

typedef enum{
	ACERTIJO = 1,
	MINIJUEGO = 2,
	PERROR = -1
}Prueba;

int aleat_num(int inf, int sup);

#endif /* TYPES_H_ */
