#ifndef INTERFACE_H
#define INTERFACE_H

#include "types.h"
#include "utils.h"

#define WHITE 7
#define YELLOW 103
#define ORANGE 45
#define GREEN 42
#define BLUE 44
#define RED 41
#define GREY 47

typedef struct _rect rect;


/**
 * @brief Allocates a rectangle and sets its data.
 */
rect *rect_init(int line,int column,int l,int h );


void rect_free(rect*r);


/**
 * @brief allocates a new rectangle with the same data of the argument
 * @param r rect to copy
 */ 
rect *rect_copy(rect*r);

/**
 * @brief Clears the part of the terminal delimited by the rectangle
 */
void rect_clear(rect *r);

/**
 * @brief Prints the border of a rectangle using unicode chars
 */
Status rect_border(rect *r);

/**
 * @brief inits a rectangle with the values of an existin rectangle expanded in x direction and y.
 * @param x changes column--> column-x && l+x
 * @param y changes line-->line-y && h+y
 * @return pointer to rect or NULL in case of error;
 */
rect *rect_expand(rect *r, int x, int y);


/**
 * @brief paint a rectangle of a specfic color
 * @param r pointer to rectangle already allocated
 * @param color code of the color for the ESC sequence
 * @return OK or ERROR in case of invalid inputs
 */ 
Status rect_color(rect*r, int color);

/********GETTERS AND SETTERS************/
Status rect_setline(rect*r,int line);
Status rect_setcolumn(rect *r, int column);
Status rect_setlength(rect *r, int l);
Status rect_setheight(rect*r,int h);
int rect_getline(rect*r);
int rect_getcolumn(rect*r);
int rect_getl(rect*r);
int rect_geth(rect*r);
/**************************************/


/**
 * @brief Prints the content of a file starting on a rect
 * @return OK or ERROR in case of error opening File
 */
Status print_element(char *filename,rect*r);

/**
 * @brief Prints the content of a buffer starting on a rect
 * @param size if size==-2, it is a terminanting \0 string, so size will be calculated with strlen
 */
Status print_buffer(char *buf, int size, rect *r);


 /**
 * @brief Prints a letter from file starting on a rect
 * @return OK or ERROR in case of error opening File
 */
Status print_letter(char *filename, rect *r);


/**
 * @brief Clears the screen using the C ANSI ESC code
 * */
void terminal_clear();


/**
 * @brief Resizes the terminal window, 
 * @param h height
 * @param w width
 */ 
Status terminal_resize(int h, int w);

/**
 * @brief positiones the cursor in a line and column (starting by 1) using C ANSI ESC codes
 * */
 Status positionate_cursor(int line, int column);


/**
 * @brief Modifies the string files (and also returns a pointer to it) 
 * so that it contains the name of the file with 'letter' as an ascii banner. 
 * */
char *file_of_letter(char*file, char letter);



/**
 * @brief Prints the solution of the cube in a rectangle
 * @param sol string with the solution
 * @param r pointer to rect where to print it (one letter is 9x9)
 * @param letters_per_line number of letters that are written per line
 */
Status print_solution(char*sol,rect*r,int letters_per_line);

/**
 * @brief Prints the solution of the cube in a rectangle using le letters_buffer
 * @param l_buffer array of buffers where the letters have been filled with fill_buffer_letter
 * @param sol string with the solution
 * @param r pointer to rect where to print it (one letter is 9x9)
 * @param letters_per_line number of letters that are written per line
 * @param print_from position of the string to start printing
 * @return -1 if all the sol has been printed or the number of chars printed
 */
int print_solution_2(char *sol, rect *r, char **l_buffer, int letters_per_line,int print_from);


/**
 * @brief displays confetti in a rectangle
 * @param r if r is NULL the confetti is display in the whole terminal
 * @param ndots number of dots to be displyed
 */
Status print_confeti(rect *r, int ndots);


/**
 * @brief returns the position of the array of buffers where the letter is stored
 * @param letter must be RUFLBDMESXYZ in capital and lower letters
 */ 
int hash_letter(char letter);



/**
 * @brief allocates memory for an array of buffers where the letters are stored. 
 * each letter is stores in buf[hash_letter(l)]. Should be called with 
 * RUFLBDMESXYZruflbdmesxyz
 * @param letters array of the letters to be filled.
 * @param buff has to be allocated outside. Size should be 34
 */ 
Status fill_buffer_letter(char *letters, char **buff);


/**
 * @brief allocates the array of buffers (only the char** not the 34 char*) that will be filled with the letters
 * @param size number of buffers of the array (it should be 34)
 * 
 */ 
char **allocate_array_lettersbuffer(int size);


/**
 * @brief frees the array of buffers previously alocated 
 * @param buff double array of chars to be freed
 * @param size size of the array (should be the same used in allocate_arra_lettersbuffer)
 */ 
void free_array_lettersbuffer(char **buff, int size);


/**
 * @brief Effect of fade to a color starting from the center of the screen
 * @param color code of the color in ESC sequences
 */

Status fade_to_color(int color);



#endif