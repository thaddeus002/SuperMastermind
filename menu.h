/**
 * \file menu.h
 * Display and manage a menu as a line of buttons.
 */

#ifndef __MENU_H_
#define __MENU_H_

#include <SDL/SDL.h>


/** Type of menu's button clicked */
typedef enum {
    MENU_NONE=-1, NEW_GAME, HELP, ABOUT, QUIT, BUTTONS_NB
} menu_action_t;



/**
 * Display the dialog box.
 * \param screen main program screen
 * \param height where to place the menu : nb of pixel from the top
 */
void menu_display(SDL_Surface *screen, int height);


/**
 * When a mouse clic is done at position (x,y), call this function to
 * know if and which button is clicked.
 * \param screen main program screen
 * \param x x coordinate of mouse pointer
 * \param y y coordinate of mouse pointer
 */
menu_action_t menu_clicked(SDL_Surface *screen, int x, int y);




#endif

