/**
 * @file    grafiks.h
 * @author  Bhavuk Sikka
 * 
 * @brief   This module handles various things. Creation of points and sections,
 * 			initializing terminal's parameters, drawing lines, printing text into
 * 			a defined section (text box), and changing the colors of all these things.
 * */

#ifndef _GRAFIKS
#define _GRAFIKS
#include "types.h"
#include "kolors.h"
#include "control.h"
#include <pthread.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum {
    CENTER, RIGHT, LEFT, ALIGNED
}align;

typedef struct pts pts;
typedef struct section section;
typedef struct kolor kolor;
typedef struct funktion funktion;

/** @brief      Gets the size of the whole terminal
 *  @return     Returns a pointer to an integer array with the following structure. [rows, columns] */
int* get_winsize();

/** @brief      Draws a layout in the terminal
 *  @param marco    Flag that creates borders layout.
 *              value 0: no borders
 *              value 1: outernborder
 *                       ╔══════════════╗
 *                       ║              ║
 *                       ║              ║
 *                       ║              ║
 *                       ╚══════════════╝
 *              value 2:
 *                       ╔════╦════╦════╗
 *                       ║    ║    ║    ║
 *                       ║    ║    ║    ║
 *                       ║    ║    ║    ║
 *                       ╚════╩════╩════╝
 *  @param k    Indicates the default color of the terminal, and also of the sections returned.
 *              If NULL, defaults will be applied
 *  @return     NULL in case of error. Otherwise, an array of pointers to section is returned.
 *              If macro= 0 or 1, the array of sections has only one element.
 *              If macro= 2, the array has three elements.
 *              These pointers to section correspond to the inner part of each legible box. 
 *              Any content which is meant to be displayed must be within these section.
 *              I.e. passing the hole section or a subsection inside these section.
 */
section **paint_layout(const short marco, kolor *k);

/** @brief      Initializes the terminal's param */
void init_terminal();

/** @brief      Resets the terminal's params */
void close_terminal();

void clear_stdin();

/** @brief      Creates a kolor
 *  @param k   Address of a pointer of type kolor
 *  @return     Returns ERROR in case of error and OK if everything goes well */
status create_kolor(kolor **k);

/** @brief      Gets the foreground color
 *  @param k   Pointer of kolor
 *  @return     Returns -1 in case of error. Otherwise, it returns the foreground color (See kolors.h) */
short get_kolor_fg(const kolor* k);

/** @brief      Gets the background color
 *  @param k   Pointer of kolor
 *  @return     Returns -1 in case of error. Otherwise, it returns the background color (See kolors.h) */
short get_kolor_bg(const kolor* k);

/** @brief      Sets a background and foreground color
 *  @param k    Pointer of kolor
 *  @param fg_color   Foreground color. Please use only colors in kolors.h
 *  @param bg_color   Background color. Please use only colors in kolors.h
 *  @return     Returns ERROR in case of error and OK if everything goes well */
status set_kolor(kolor *k, const int fg_color, const int bg_color);

/** @brief      Frees the kolor.
 *  @param k   Address of pointer to kolor **/
void free_kolor(kolor **k);

/** @brief      Applies a kolor.
 *  @param k   Address of pointer to kolor **/
void apply_kolor(kolor *k);

/** @brief      Resets the color scheme to the terminal's default */
void reset_kolor();

/** @brief      Creates a point
 *  @param p    Address of a pointer of type pts
 *  @return     Returns ERROR in case of error and OK if everything goes well */
status create_point(pts **p);

/** @brief      Gets the position of row of a point
 *  @param p   Pointer of point
 *  @return     Returns -1 in case of error, otherwise the appropiate row */
int get_point_row(const pts *p);

/** @brief      Gets the position of column of a point
 *  @param p   Pointer of point
 *  @return     Returns -1 in case of error, otherwise the appropiate col */
int get_point_col(const pts *p);

/** @brief      Sets a point to a position
 *  @param p    Pointer of point
 *  @param row  Row in which the point is located
 *  @param column   Column in which the point is located
 *  @return     Returns ERROR in case of error and OK if everything goes well */
status set_point(pts *p, const int row, const int column);

/** @brief      Frees the point.
 *  @param p   Address of pointer to pts **/
void free_point(pts **p);

/** @brief      Creates a section
 *  @param sec  Address of a pointer of type section
 *  @return     Returns ERROR in case of error and OK if everything goes well */
status create_section(section **sec);

/** @brief      Gets the upper left point of the section
 *  @param sec  Pointer of section
 *  @return     Returns NULL in case of error and a copy of a pointer pts if succeds */
pts* get_section_point(const section *sec);

/** @brief      Gets the number of rows which defines the section
 *  @param sec  Pointer of section
 *  @return     Returns -1 in case of error and the number of rows otherwise */
int get_section_row(const section *sec);

/** @brief      Gets the number of columns which defines the section
 *  @param sec  Pointer of section
 *  @return     Returns -1 in case of error and the number of columns otherwise */
int get_section_col(const section *sec);

/** @brief      Gets the default background and foreground color of the section
 *  @param sec  Pointer of section
 *  @return     Returns NULL in case of error and a copy of the section's default color */
kolor *get_section_kolor(const section *sec);

/** @brief      Sets a section to a size and place.
 *  @param sec  Pointer of a section
 *  @param p    Point situated up left
 *  @param rows   Number of rows
 *  @param cols   Number of columns
 *              f2  |-----|     cols = 5       
 *                  |     |
 *                  |     |     rows = 4
 *                  |-----|
 *              resulting in a secion of 4x5
 *  @param k    Default background and foreground color.    
 *  @return     Returns ERROR in case of error and OK if everything goes well
 *              If a section surpases any borders of the layout or the terminal's size, ERROR will be returned.  
 * */
status set_section(section *sec, const pts *p, const int rows, const int cols, kolor *k);

/** @brief      Frees the section.
 *  @param sec  Address of pointer to section **/
void free_section(section **sec);

/** @brief      Clears the section.
 *  @param sec  Pointer to section **/
void clear_section(section *sec);

/** @brief      Clears the section.
 *  @param f1   Pointer to section 
 *  @param f2   Pointer of a kolor **/
void fill_section(section *sec, kolor *k);

/** @brief      Checks wether a point is in a section or not
 *  @return     True if point is in section. False otherwise. */
bool in_section(const section *sec,const pts *p);

/**
 *  @brief      Creates a textbox in the uppermost part, with a margin of 1px and 
 *  @param f1   Text that has to be displayed
 *  @param f2   Specifies the section in which the text must be written
 *  @param f3   Indicates the alignment used to show the text.
 *              -1 for left
 *              0 for centered
 *              1 for right
 *  @param f4   Color for text. Pass NULL if the default value from section is wanted to be used
 *  @return     Returns ERROR in case of error and OK if everything goes well */
status textbox(const char *str, const section* sec, const int al, kolor *k);

/** @brief      Draws a line from point p1 to point p2 
 *  @param f1   Refers to the point where the graph must start in
 *  @param f2   Refers to the point where the graph must end in
 *  @param f3   If not null, creates the line with the string indicated. If NULL, it uses ╱, ╲ or ─. 
 *  @param f4   Specifies the section in which line must be plotted
 *  @param f5   Specifies a color to draw the line. If NULL, section's default color will be used.
 *  @param f6   Indicates whether the string thikk must be displayed horizontally (flag=0) or vertically (flag!=0). I.e.:
 *              horizontal
 *              v
 *              e
 *              r
 *              t
 *              i
 *              c
 *              a
 *              l
 *  @return     Returns ERROR in case of error and OK if everything goes well */
status draw_line(const pts *p1,const pts*p2, char* thikk, const section* sec, kolor* k, short flag);
#endif /*_GRAFIKS*/
