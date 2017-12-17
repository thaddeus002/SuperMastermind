/**
 * \file dialog.h
 * Management of a dialog box on top of the main screen.
 * A dialog box will be displayed on screen, disabling all actions,
 * until user has clicked a button.
 * Only one dialog box can be active at the same time.
 */

#ifndef __DIALOG_H_
#define __DIALOG_H_

#include <SDL/SDL.h>


/** Type of action for a button. */
typedef enum {
    NONE=0, CANCEL, OK
} action_t;



/**
 * Display the dialog box.
 * \param screen main program screen
 * \param imageFilename filename of the image including the buttons.
 *        the buttons must be at the good place in the image.
 * \param nb the number of button in this dialog box
 */
void dialog_display(SDL_Surface *screen, char *imageFilename, int nb);


/**
 * When a mouse clic is done at position (x,y), call this function to
 * know if and which button is clicked.
 * If a button was clicked, the function will restore screen before
 * returning.
 * \param screen main program screen
 * \param x x coordinate of mouse pointer
 * \param y y coordinate of mouse pointer
 */
action_t is_clicked(SDL_Surface *screen, int x, int y);




#endif

