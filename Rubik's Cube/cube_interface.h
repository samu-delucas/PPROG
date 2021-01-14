#ifndef _CUBE_INTERFACE_H_
#define _CUBE_INTERFACE_H_

/** @brief calls the cube interface with the option (2 or 3),
  new==1 iff we want a new game, else we charge a game
  from a file
*/
int c_interface(int option, int new, char *save_game_file);

#endif