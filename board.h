/**
 * \file board.h
 * Drawing the game's board.
 */


#ifndef __BOARD_H_
#define __BOARD_H_


#include <SDL/SDL.h>

#include "secret.h"

// Number of possible attempts
#define NB_ATTEMPTS 10


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
 * Show victory or defeat message.
 * \param screen the board surface
 * \param victory 1 if the secret was found, 0 otherwise
 */
void board_show_ending_message(SDL_Surface *screen, int victory);

/**
 * Show the secret after the game has ended.
 */
void board_show_secret(SDL_Surface *screen, code_t *secret);


/**
 * Show the places for the colors of next try.
 * \param board main window screen
 */
void next_try(SDL_Surface *screen, int tryNumber);


/**
 * Test if a clic is on a color on selecting board.
 * \return -1 if no color is selected, or the selected color
 */
color_t is_color_selected(int x, int y);


/**
 * Mark the newly selected color.
 */
void board_select(SDL_Surface *screen, color_t old, color_t new);

SDL_Surface *create_board();

/**
 * Clean the board for beginning a new game.
 */
void board_clean(SDL_Surface *screen);

#endif

