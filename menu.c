/**
 * \file menu.c
 * Display and manage a menu as a line of buttons.
 */


#include "menu.h"

#define BUTTON_HEIGHT 38
#define BUTTON_WIDTH 100

static int menuHeight = 0;


/** the files for the buttons */
static char *buttonsImages[] = { "data/new_game_btn.bmp", "data/help_btn.bmp", "data/about_btn.bmp", "data/quit_btn.bmp"};


static int button_pos(SDL_Surface *screen, int index) {

    int interval;

    interval = (screen->w - BUTTON_WIDTH * BUTTONS_NB) / (BUTTONS_NB  + 1);

    return interval + index * (BUTTON_WIDTH + interval);
}


static void draw_button(SDL_Surface *screen, int index) {

    SDL_Surface *buttonImg = SDL_LoadBMP(buttonsImages[index]);
    SDL_Rect btnPosition;

    if(buttonImg==NULL) {
        fprintf(stderr, "Unable to open file %s\n", buttonsImages[index]);
        return;
    }

    btnPosition.x = button_pos(screen, index);
    btnPosition.y = menuHeight;

    SDL_BlitSurface(buttonImg, NULL, screen, &btnPosition);
    SDL_FreeSurface(buttonImg);
}



/**
 * Display the dialog box.
 * \param screen main program screen
 * \param height where to place the menu : nb of pixel from the top
 */
void menu_display(SDL_Surface *screen, int height){

    int i;

    menuHeight = height;

    for(i=0; i<BUTTONS_NB; i++) {
        draw_button(screen, i);
    }
}


/**
 * When a mouse clic is done at position (x,y), call this function to
 * know if and which button is clicked.
 * \param screen main program screen
 * \param x x coordinate of mouse pointer
 * \param y y coordinate of mouse pointer
 */
menu_action_t menu_clicked(SDL_Surface *screen, int x, int y){

    menu_action_t act = MENU_NONE;

    if (y<menuHeight || y>menuHeight+BUTTON_HEIGHT) {
        return MENU_NONE;
    }

    act++;
    while(act < BUTTONS_NB) {

        int x0 = button_pos(screen, act);
        if(x>x0 && x<x0+BUTTON_WIDTH) {
            return act;
        }

        act++;
    }

    return MENU_NONE;
}
