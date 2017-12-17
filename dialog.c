/**
 * \file dialog.c
 * Management of a dialog box on top of the main screen.
 * A dialog box will be displayed on screen, disabling all actions,
 * until user has clicked a button.
 * Only one dialog box can be active at the same time.
 */

#include "dialog.h"


typedef struct button {
    // relative pos of the button
    int xmin, xmax, ymin, ymax;
    // button type
    action_t action;
} button_t;


typedef struct dialog {
    SDL_Surface *savedScreen;
    // box position on screen
    int x, y;
    /** A NULL terminated array of buttons */
    button_t *buttons[3];
} dialog_t;


static dialog_t *dialogBox = NULL;




static button_t *create_button(SDL_Surface *box, int buttonNumber, int nb) {

    button_t *newButton;
    int l = box->w / nb;

    fprintf(stdout, "Box de %dx%d\n", box->w, box->h);


    newButton = malloc(sizeof(button_t));

    // button width is 100
    newButton->xmin = (l - 100) / 2 + l*(buttonNumber- 1);
    // button height is 38
    newButton->ymin = box->h - 7 - 38;

    newButton->xmax = newButton->xmin + 100;
    newButton->ymax = newButton->ymin + 38;


    fprintf(stdout, "Pos bouton = %d,%d - %d,%d\n", newButton->xmin, newButton->ymin, newButton->xmax, newButton->ymax);


    switch(buttonNumber) {
    case 1:
        newButton->action = OK;
        break;
    default:
        newButton->action = CANCEL;
    }

    return newButton;
}



/**
 * Display the dialog box.
 * \param screen main program screen
 * \param imageFilename filename of the image including the buttons.
 *        the buttons must be at the good place in the image.
 * \param nb the number of button in this dialog box
 */
void dialog_display(SDL_Surface *screen, char *imageFilename, int nb){

    SDL_Rect position;
    SDL_Surface *box;
    int i;

    if(dialogBox != NULL) {
        fprintf(stderr, "Can open a dialogBox while the current is not closed\n");
        return;
    }

    box = SDL_LoadBMP(imageFilename);

    if(box == NULL) {
        fprintf(stderr, "Unable to open file %s\n", imageFilename);
        return;
    }

    dialogBox = malloc(sizeof(dialog_t));

    // save screen
    dialogBox->savedScreen = SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w, screen->h, 32, 0, 0, 0, 0);
    position.x = 0;
    position.y = 0;
    SDL_BlitSurface(screen, NULL, dialogBox->savedScreen, &position);

    // draw box
    position.x = (screen->w - box->w) / 2;
    position.y = (screen->h - box->h) / 2;
    dialogBox->x = position.x;
    dialogBox->y = position.y;
    SDL_BlitSurface(box, NULL, screen, &position);

    // buttons
    i=0;
    while(i<nb && i<2) {
        dialogBox->buttons[i] = create_button(box, i+1, nb);
        i++;
    }
    SDL_FreeSurface(box);
    dialogBox->buttons[i] = NULL;
    SDL_Flip(screen);
}



static void free_dialog() {

    int i=0;

    if(dialogBox == NULL) {
        return;
    }

    SDL_FreeSurface(dialogBox->savedScreen);

    while(dialogBox->buttons[i] != NULL) {
        free(dialogBox->buttons[i]);
        i++;
    }

    free(dialogBox);
    dialogBox =NULL;
}


static int in_button(button_t *but, int x, int y){

    fprintf(stdout, "%d,%d entre %d-%d et %d-%d\n", x, y, but->xmin, but->xmax, but->ymin, but->ymax);

    if(x>but->xmin && x<but->xmax && y>but->ymin && y<but->ymax) {
        return 1;
    }

    return 0;
}




/**
 * When a mouse clic is done at position (x,y), call this function to
 * know if and which button is clicked.
 * If a button was clicked, the function will restore screen before
 * returning.
 * \param screen main program screen
 * \param x x coordinate of mouse pointer
 * \param y y coordinate of mouse pointer
 */
action_t is_clicked(SDL_Surface *screen, int x, int y) {

    action_t act = NONE;
    SDL_Rect position;
    button_t **but;

    if(dialogBox == NULL) {
        return NONE;
    }

    but = dialogBox->buttons;
    while(*but != NULL) {
        if(in_button(*but, x - dialogBox->x, y - dialogBox->y)){
            act = (*but)->action;
            break;
        }
        but++;
    }

    if(act != NONE) {
        // restore screen
        position.x = 0;
        position.y = 0;
        SDL_BlitSurface(dialogBox->savedScreen, NULL, screen, &position);
        SDL_Flip(screen);
        // free memory
        free_dialog();
    }

    return act;
}

