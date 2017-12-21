/**
 * \file board.c
 * Drawing the game's board.
 */


#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include "board.h"
#include "secret.h"
#include "menu.h"

#define WIDTH 541
#define HEIGHT 699

#define HEADER_HEIGHT 120
#define LINE_HEIGHT 54

/** the files for the colored balls */
static char *ballsImages[] = { "data/red.bmp", "data/green.bmp", "data/blue.bmp", "data/white.bmp", "data/orange.bmp", "data/cyan.bmp", "data/yellow.bmp", "data/purple.bmp"};


/**
 * \return the index of color for pos (x, y) or -1
 */
int place_color(int x, int y, int tryNumber) {

    int yCenter = HEADER_HEIGHT + LINE_HEIGHT * (tryNumber-1) + LINE_HEIGHT / 2;
    int i;

    if((y>yCenter+15) || (y<yCenter-15)) {
        return -1;
    }

    for(i=0; i<CODE_LENGHT; i++) {
        int xCenter = 70 + i * 50;

        if((x>xCenter-15) && (x<xCenter+15)) {
            return i;
        }
    }

    return -1;
}



/**
 * Draw a color ball at the specified position.
 * \param x x coordinate of the left-top corner
 * \param y y coordinate of the left-top corner
 */
static void board_draw_color_ball_to_pos(SDL_Surface *screen, color_t color, int x, int y) {

    SDL_Surface *colorBall;
    SDL_Rect ballPosition;

    ballPosition.x = x;
    ballPosition.y = y;

    colorBall = SDL_LoadBMP(ballsImages[color]);
    /* The white must be turned to transparent */
    SDL_SetColorKey(colorBall, SDL_SRCCOLORKEY, SDL_MapRGB(colorBall->format, 255, 255, 255));
    SDL_BlitSurface(colorBall, NULL, screen, &ballPosition);
    SDL_FreeSurface(colorBall);
}



/**
 * Draw a color ball at the specified height.
 * \param y the height of ball center (from top)
 */
static void board_draw_color_ball(SDL_Surface *screen, int index, color_t color, int y) {

    board_draw_color_ball_to_pos(screen, color, 70 + 50 * index - 17, y - 17);
}


void board_draw_color(SDL_Surface *screen, int index, color_t color, int tryNumber) {

    int y = HEADER_HEIGHT + LINE_HEIGHT * (tryNumber-1) + LINE_HEIGHT / 2;
    board_draw_color_ball(screen, index, color, y);

    /* Update screen */
    SDL_Flip(screen);
}



/**
 * Draw a button to validate an attempt.
 */
void board_show_button(SDL_Surface *screen, int tryNumber) {

    SDL_Surface *button;
    SDL_Rect position;

    button = SDL_LoadBMP("data/verify_btn.bmp");

    if(button == NULL) {
        fprintf(stderr, "Couldn't open %s\n", "data/verify_btn.bmp");
        return;
    }

    position.x = 340 + 200 / 2 - button->w / 2;
    position.y = HEADER_HEIGHT + LINE_HEIGHT * (tryNumber-1) + LINE_HEIGHT / 2 - button->h / 2;

    SDL_BlitSurface(button, NULL, screen, &position);
    SDL_FreeSurface(button);
    /* Update screen */
    SDL_Flip(screen);
}

/**
 * Draw a button to validate an attempt.
 */
static void board_mask_button(SDL_Surface *screen, int tryNumber) {

    SDL_Surface *mask;
    SDL_Rect position;

    mask = SDL_CreateRGBSurface(SDL_HWSURFACE, 100, 38, 32, 0, 0, 0, 0);
    SDL_FillRect(mask, NULL, SDL_MapRGB(mask->format, 79, 33, 17));

    position.x = 340 + 200 / 2 - mask->w / 2;
    position.y = HEADER_HEIGHT + LINE_HEIGHT * (tryNumber-1) + LINE_HEIGHT / 2 - mask->h / 2;

    SDL_BlitSurface(mask, NULL, screen, &position);
    SDL_FreeSurface(mask);
}


/**
 * Clic on the "Verify" button. try must be complete.
 * \return 1 only if the try is completed
 */
int clic_verify(int x, int y, int tryNumber) {

    int xmin, xmax, ymin, ymax;
    int x0, y0;

    x0 = 340 + 200 / 2;
    y0 = HEADER_HEIGHT + LINE_HEIGHT * (tryNumber-1) + LINE_HEIGHT / 2;

    xmin = x0 - 100 / 2;
    xmax = x0 + 100 / 2;
    ymin = y0 - 38 / 2;
    ymax = y0 + 38 / 2;

    if(x>xmin && x<xmax && y>ymin && y<ymax) {
        return 1;
    }
    return 0;
}


void board_show_result(SDL_Surface *screen, int *result, int tryNumber) {

    int i; // counter
    int stars = result[0] + result[1];
    SDL_Rect position;
    SDL_Surface *good, *mis;

    good = NULL;
    mis = NULL;

    board_mask_button(screen, tryNumber);

    position.y = HEADER_HEIGHT + LINE_HEIGHT * (tryNumber-1) + LINE_HEIGHT / 2 - 7;

    for(i=1; i<=stars; i++) {
        position.x = 340 + 12 + 40 * (i-1);

        if(i<=result[0]) {
            if(good == NULL) {
                good = SDL_LoadBMP("data/good.bmp");
                SDL_SetColorKey(good, SDL_SRCCOLORKEY, SDL_MapRGB(good->format, 255, 255, 255));
            }
            SDL_BlitSurface(good, NULL, screen, &position);
        } else {
            if(mis == NULL) {
                mis = SDL_LoadBMP("data/misplaced.bmp");
                SDL_SetColorKey(mis, SDL_SRCCOLORKEY, SDL_MapRGB(mis->format, 255, 255, 255));
            }
            SDL_BlitSurface(mis, NULL, screen, &position);
        }
    }

    if(good!=NULL) SDL_FreeSurface(good);
    if(mis!=NULL) SDL_FreeSurface(mis);
    /* Update screen */
    SDL_Flip(screen);
}




/**
 * Show the places for the colors of next try.
 * \param board main window screen
 */
void next_try(SDL_Surface *screen, int tryNumber) {

    SDL_Surface *undefColor;
    SDL_Rect ballPosition;
    int i;

    undefColor = SDL_LoadBMP("data/undefined.bmp");
    /* The white must be turned to transparent */
    SDL_SetColorKey(undefColor, SDL_SRCCOLORKEY, SDL_MapRGB(undefColor->format, 255, 255, 255));


    ballPosition.y = HEADER_HEIGHT + LINE_HEIGHT * (tryNumber-1) + LINE_HEIGHT / 2 - undefColor->h/2;

    for(i=0; i<CODE_LENGHT; i++) {
        ballPosition.x = 70 + 50 * i - undefColor->w/2;
        SDL_BlitSurface(undefColor, NULL, screen, &ballPosition);
    }

    SDL_FreeSurface(undefColor);
    /* Update screen */
    SDL_Flip(screen);
}



/**
 * Separate two parts in a Surface with an horizontal line.
 * \param board the image to split
 * \param dist number of pixels from image top
 */
static void add_horizontal_separator(SDL_Surface *board, int dist){

    SDL_Surface *line;
    SDL_Rect position;

    position.x = 0;
    position.y = dist;

    line = SDL_CreateRGBSurface(SDL_HWSURFACE, board->w, 1, 32, 0, 0, 0, 0);
    SDL_FillRect(line, NULL, SDL_MapRGB(board->format, 178, 86, 8));

    SDL_BlitSurface(line, NULL, board, &position);
    SDL_FreeSurface(line);
}


/**
 * Separate two parts in a Surface with a vertical line.
 * \param board the image to split
 * \param dist number of pixels from image left
 */
static void add_vertical_separator(SDL_Surface *board, int dist){

    SDL_Surface *line;
    SDL_Rect position;
    int h;

    position.x = dist;
    position.y = 0;

    h = HEADER_HEIGHT + LINE_HEIGHT * NB_ATTEMPTS;

    line = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, h, 32, 0, 0, 0, 0);
    SDL_FillRect(line, NULL, SDL_MapRGB(board->format, 178, 86, 8));

    SDL_BlitSurface(line, NULL, board, &position);
    SDL_FreeSurface(line);
}


/**
 * Add the secret cod mask.
 */
static void add_secret(SDL_Surface *plateau) {

    SDL_Surface *secret;
    SDL_Rect position;

    position.x = 20;
    position.y = 30;

    secret = SDL_LoadBMP("data/secret.bmp");

    SDL_BlitSurface(secret, NULL, plateau, &position);
    SDL_FreeSurface(secret);
}


/**
 * Give the relative position of colors ball on the selection board.
 * Fill x and y with values for the blit method
 */
static void colors_position_on_board(color_t color, Sint16 *x, Sint16 *y){

    *x = 20 + 3 + 40 * color;
    *y = 35 + 3;

    if(color>=COLORS_NB/2) {
        *x -= (COLORS_NB/2) * 40;
        *y += 40;
    }
}


/**
 * Create the colors board and add it to the screen.
 */
static void add_colors_board(SDL_Surface *plateau) {
    SDL_Surface *board, *text;
    SDL_Rect position;
    SDL_Rect bannerPosition;
    int i;

    position.x = 340 + 1;
    position.y = 0;

    board = SDL_CreateRGBSurface(SDL_HWSURFACE, 200, HEADER_HEIGHT, 32, 0, 0, 0, 0);
    SDL_FillRect(board, NULL, SDL_MapRGB(board->format, 121, 69, 24));

    text = SDL_LoadBMP("data/selector.bmp");
    bannerPosition.x=0;
    bannerPosition.y=0;
    SDL_BlitSurface(text, NULL, board, &bannerPosition);
    SDL_FreeSurface(text);

    for(i = 0; i < COLORS_NB; i++) {
        SDL_Surface *colorBall;
        SDL_Rect colorPosition;

        colors_position_on_board(i, &(colorPosition.x), &(colorPosition.y));

        colorBall = SDL_LoadBMP(ballsImages[i]);
        /* The white must be turned to transparent */
        SDL_SetColorKey(colorBall, SDL_SRCCOLORKEY, SDL_MapRGB(colorBall->format, 255, 255, 255));
        SDL_BlitSurface(colorBall, NULL, board, &colorPosition);
        SDL_FreeSurface(colorBall);
    }

    SDL_BlitSurface(board, NULL, plateau, &position);

    SDL_FreeSurface(board);
}


/**
 * Test if a clic is on a color on selecting board.
 * \return -1 if no color is selected, or the selected color
 */
color_t is_color_selected(int x, int y) {

    int x0 = 340 + 1;
    int i;

    if(x <= x0 || y < 35+3 || y>35+40+40 ) {
        return UNDEFINED;
    }

    for(i = 0; i < COLORS_NB; i++) {

        Sint16 xc;
        Sint16 yc;

        colors_position_on_board(i, &xc, &yc);
        xc += x0 + 17;
        yc += 17;

        if((xc-x)*(xc-x)+(yc-y)*(yc-y) <= 17*17) {
            return i;
        }
    }

    return UNDEFINED;
}



/**
 * Mark the newly selected color.
 */
void board_select(SDL_Surface *screen, color_t old, color_t new) {

    Sint16 x, y;
    SDL_Surface *star;
    SDL_Rect position;

    // deselect old color
    if(old != UNDEFINED) {
        colors_position_on_board(old, &x, &y);
        board_draw_color_ball_to_pos(screen, old, x+340+1, y);
    }

    // select new color
    if(new != UNDEFINED) {
        colors_position_on_board(new, &(position.x), &(position.y));
        position.x += 340+1 + 9;
        position.y += 9;
        star = SDL_LoadBMP("data/selected.bmp");
        SDL_SetColorKey(star, SDL_SRCCOLORKEY, SDL_MapRGB(star->format, 255, 255, 255));
        SDL_BlitSurface(star, NULL, screen, &position);
        SDL_FreeSurface(star);
    }

    SDL_Flip(screen);
}


/**
 * Clean the board for beginning a new game.
 */
void board_clean(SDL_Surface *screen) {

    int h = LINE_HEIGHT * NB_ATTEMPTS;
    int w = screen->w;
    SDL_Surface *mask;
    SDL_Rect position;
    int i;

    mask = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, 0, 0, 0, 0);
    SDL_FillRect(mask, NULL, SDL_MapRGB(mask->format, 79, 33, 17));

    position.x=0;
    position.y=HEADER_HEIGHT+1;
    SDL_BlitSurface(mask, NULL, screen, &position);
    SDL_FreeSurface(mask);

    add_vertical_separator(screen, 340);
    for(i=0;i<=NB_ATTEMPTS;i++) {
        add_horizontal_separator(screen, HEADER_HEIGHT+LINE_HEIGHT*i);
    }

    SDL_Flip(screen);
}



SDL_Surface *create_board() {
    SDL_Surface *screen;
    int i;

    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        fprintf(stderr, "Error initializing SDL : %s\n", SDL_GetError());
        return NULL;
    }

    screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL) {
        fprintf(stderr, "Can't open video mode : %s\n", SDL_GetError());
        return NULL;
    }

    SDL_WM_SetCaption("SuperMastermind is dominating you", NULL);

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 79, 33, 17));
    add_vertical_separator(screen, 340);
    add_secret(screen);
    add_colors_board(screen);
    add_horizontal_separator(screen, HEADER_HEIGHT);
    // tries' zones
    for(i=1;i<=NB_ATTEMPTS;i++) {
        add_horizontal_separator(screen, HEADER_HEIGHT+LINE_HEIGHT*i);
    }

    menu_display(screen, HEADER_HEIGHT+LINE_HEIGHT*NB_ATTEMPTS + 1);

    return screen;
}


/**
 * Show victory or defeat message.
 * \param screen the board surface
 * \param victory 1 if the secret was found, 0 otherwise
 */
void board_show_ending_message(SDL_Surface *screen, int victory) {
    SDL_Surface *result;
    SDL_Rect position;

    position.x = 340 + 1;
    position.y = 0;

    if(victory) {
        result = SDL_LoadBMP("data/victory.bmp");
    } else {
        result = SDL_LoadBMP("data/failled.bmp");
    }

    SDL_BlitSurface(result, NULL, screen, &position);
    SDL_FreeSurface(result);
}


/**
 * Show the secret after the game has ended.
 */
void board_show_secret(SDL_Surface *screen, code_t *secret) {

    SDL_Surface *mask;
    SDL_Rect position;
    int y;
    int i;

    // clean secret zone

    position.x = 20 + 2;
    position.y = 30 + 2;

    mask = SDL_CreateRGBSurface(SDL_HWSURFACE, 300 - 4, 60 - 4, 32, 0, 0, 0, 0);
    SDL_FillRect(mask, NULL, SDL_MapRGB(mask->format, 121, 69, 24));
    SDL_BlitSurface(mask, NULL, screen, &position);
    SDL_FreeSurface(mask);

    // draw secret
    y = 30 + 60 / 2;
    for(i=0; i<CODE_LENGHT; i++) {
        board_draw_color_ball(screen, i, (*secret)[i], y);
    }
}
