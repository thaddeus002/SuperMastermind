/**
 * \file board.h
 * Drawing the game's board.
 */


#ifndef __BOARD_H_
#define __BOARD_H_


#include <SDL/SDL.h>

#include "secret.h"


/**
 * \return the index of color for pos (x, y) or -1
 */
int place_color(int x, int y, int tryNumber);


void board_draw_color(SDL_Surface *screen, int index, color_t color, int tryNumber);


/**
 * Draw a button to validate an attempt.
 */
void board_show_button(SDL_Surface *screen, int tryNumber);



/**
 * Clic on the "Verify" button. try must be complete.
 * \return 1 only if the try is completed
 */
int clic_verify(int x, int y, int tryNumber) ;

void board_show_result(SDL_Surface *screen, int *result, int tryNumber);



/**
 * Show the places for the colors of next try.
 * \param board main window screen
 */
void next_try(SDL_Surface *screen, int tryNumber);



SDL_Surface *create_board();


#endif

