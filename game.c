/**
 * \file game.c
 * Defining the game's rules.
 */

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include "secret.h"

#define WIDTH 541
#define HEIGHT 700

#define HEADER_HEIGHT 120
#define LINE_HEIGHT 58


/** the files for the colored balls */
static char *ballsImages[] = { "data/red.bmp", "data/green.bmp", "data/blue.bmp", "data/white.bmp", "data/orange.bmp", "data/cyan.bmp", "data/yellow.bmp", "data/purple.bmp"};


/** Number of current try */
static int tryNumber;

/** Use this color to fill the code to try */
static color_t selectedColor;

/** current code try */
static code_t try;

/** The secret to find */
static code_t secret;

/** A game can be running or terminated */
typedef enum {
    RUNNING, ENDED
} state_t;

static state_t gameState;

static void init_attempt_state() {
    int i;
    for(i=0; i<CODE_LENGHT; i++) {
        try[i]=UNDEFINED;
    }
}


static void init_game_state() {
    tryNumber = 1;
    selectedColor = RED;
    change_code(&secret);
    init_attempt_state();
    gameState = RUNNING;
}


/**
 * Terminate the game. Show code and résult (victory or defeat).
 * \param victory 1 is secret was found, 0 otherwise
 */
static void end_game(SDL_Surface *screen, int victory) {

    SDL_Surface *result;
    SDL_Rect position;

    gameState = ENDED;
    // show result;

    position.x = 340 + 1;
    position.y = 0;

    if(victory) {
        result = SDL_LoadBMP("data/victory.bmp");
    } else {
        result = SDL_LoadBMP("data/failled.bmp");
    }

    SDL_BlitSurface(result, NULL, screen, &position);
    SDL_FreeSurface(result);


    // TODO Show code


    SDL_Flip(screen);
}


/**
 * \return the index of color for pos (x, y) or -1
 */
static int place_color(int x, int y) {

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


static void board_draw_color(SDL_Surface *screen, int index, color_t color) {

    SDL_Surface *colorBall;
    SDL_Rect ballPosition;

    ballPosition.x = 70 + 50 * index - 17;
    ballPosition.y = HEADER_HEIGHT + LINE_HEIGHT * (tryNumber-1) + LINE_HEIGHT / 2 - 17;

    colorBall = SDL_LoadBMP(ballsImages[selectedColor]);
    /* The white must be turned to transparent */
    SDL_SetColorKey(colorBall, SDL_SRCCOLORKEY, SDL_MapRGB(colorBall->format, 255, 255, 255));
    SDL_BlitSurface(colorBall, NULL, screen, &ballPosition);
    SDL_FreeSurface(colorBall);
    /* Update screen */
    SDL_Flip(screen);
}


/**
 * Draw a button to validate an attempt.
 */
static void board_show_button(SDL_Surface *screen) {

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
static void board_mask_button(SDL_Surface *screen) {

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
static int clic_verify(int x, int y) {

    int xmin, xmax, ymin, ymax;
    int x0, y0;

    if(!is_completed(&try)) {
        return 0;
    }

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


static void board_show_result(SDL_Surface *screen, int *result) {

    int i; // counter
    int stars = result[0] + result[1];
    SDL_Rect position;
    SDL_Surface *good, *mis;

    good = NULL;
    mis = NULL;

    board_mask_button(screen);

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
static void next_try(SDL_Surface *screen) {

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
 * A mouse clic has been made on position x,y.
 */
static void got_clic(SDL_Surface *screen, int x, int y) {

    int p;
    int v;

    // Don't allow clic when game is ended
    if(gameState == ENDED) {
        return;
    }

    p = place_color(x, y);
    if(p>=0) {
        try[p]=selectedColor;
        board_draw_color(screen, p, selectedColor);
        if(is_completed(&try)) {
            board_show_button(screen);
        }
        return;
    }

    v = clic_verify(x, y);
    if(v) {
        int *result = test(&try, &secret);
        fprintf(stdout, "%d good - %d misplaced\n", result[0], result[1]);
        board_show_result(screen, result);
        if(result[0] == CODE_LENGHT) {
            end_game(screen, 1);
        } else if(tryNumber < 10) {
            tryNumber++;
            init_attempt_state();
            next_try(screen);
        } else {
            end_game(screen, 0);
        }

        free(result);
        return;
    }
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

    position.x = dist;
    position.y = 0;

    line = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, board->h, 32, 0, 0, 0, 0);
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
 * Create the colors board and add it the the screen.
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

        colorPosition.x = 20 + 3 + 40 * i;
        colorPosition.y = 35 + 3;

        if(i>=COLORS_NB/2) {
            colorPosition.x -= (COLORS_NB/2) * 40;
            colorPosition.y += 40;
        }

        colorBall = SDL_LoadBMP(ballsImages[i]);
        /* The white must be turned to transparent */
        SDL_SetColorKey(colorBall, SDL_SRCCOLORKEY, SDL_MapRGB(colorBall->format, 255, 255, 255));
        SDL_BlitSurface(colorBall, NULL, board, &colorPosition);
        SDL_FreeSurface(colorBall);
    }

    SDL_BlitSurface(board, NULL, plateau, &position);

    SDL_FreeSurface(board);
}


int new_game() {
    SDL_Surface *screen;
    SDL_Event event;
    int loop = 1;
    int i;

    init_game_state();

    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        fprintf(stderr, "Error initializing SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL) {
        fprintf(stderr, "Can't open video mode : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("SuperMastermind is dominating you", NULL);

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 79, 33, 17));
    add_vertical_separator(screen, 340);
    add_secret(screen);
    add_colors_board(screen);
    add_horizontal_separator(screen, HEADER_HEIGHT);
    // tries' zones
    for(i=1;i<=9;i++) {
        add_horizontal_separator(screen, HEADER_HEIGHT+LINE_HEIGHT*i);
    }
    next_try(screen);

    SDL_Flip(screen); /* Update screen */

    while (loop) {
        SDL_WaitEvent(&event);
        switch(event.type) {
        case SDL_QUIT:
            loop = 0;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            // press escape or 'q' to quit
            case SDLK_ESCAPE:
            case SDLK_q:
                loop = 0;
                break;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                got_clic(screen, event.button.x, event.button.y);
            }
            break;
        }
    }

    SDL_Quit();

    return EXIT_SUCCESS;
}

