#ifndef _SDL_INTERFACE_H_
#define _SDL_INTERFACE_H_




/*calls the SDL interface with the option (2 or 3), 
  new==1 iff we want a new game, else we charge a game 
  from a file
*/
int SDL_interface(int option, int use_saved_game, char *save_game_file);

#endif