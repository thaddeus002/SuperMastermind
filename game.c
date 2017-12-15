/**
 * \file game.c
 * Defining the game's rules.
 */

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include "secret.h"
#include "board.h"



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

    gameState = ENDED;

    // show result;
    board_show_ending_message(screen, victory);

    // TODO Show code
    board_show_secret(screen, &secret);

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

    p = place_color(x, y, tryNumber);
    if(p>=0) {
        try[p]=selectedColor;
        board_draw_color(screen, p, selectedColor, tryNumber);
        if(is_completed(&try)) {
            board_show_button(screen, tryNumber);
        }
        return;
    }

    v = clic_verify(x, y, tryNumber);
    if(v && is_completed(&try)) {
        int *result = test(&try, &secret);
        fprintf(stdout, "%d good - %d misplaced\n", result[0], result[1]);
        board_show_result(screen, result, tryNumber);
        if(result[0] == CODE_LENGHT) {
            end_game(screen, 1);
        } else if(tryNumber < 10) {
            tryNumber++;
            init_attempt_state();
            next_try(screen, tryNumber);
        } else {
            end_game(screen, 0);
        }

        free(result);
        return;
    }
}




int new_game() {
    SDL_Surface *screen;
    SDL_Event event;
    int loop = 1;

    init_game_state();


    screen = create_board();

    if(screen == NULL) {
        return(EXIT_FAILURE);
    }

    next_try(screen, tryNumber);

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

