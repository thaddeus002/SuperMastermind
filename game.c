/**
 * \file game.c
 * Defining the game's rules.
 */

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include "secret.h"
#include "board.h"
#include "dialog.h"
#include "menu.h"


/** Number of current try */
static int tryNumber;

/** Use this color to fill the code to try */
static color_t selectedColor;

/** current code try */
static code_t try;

/** The secret to find */
static code_t secret;


/** A game can be running or terminated or waiting */
typedef enum {
    RUNNING, ENDED, WAITING_CONFIRM_QUIT
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

    // show code
    board_show_secret(screen, &secret);

    SDL_Flip(screen);
}



static void confirm_quit(SDL_Surface *screen) {
    gameState = WAITING_CONFIRM_QUIT;
    dialog_display(screen, "data/confirm_quit.bmp", 2);
}



/**
 * A mouse clic has been made on position x,y.
 */
static void got_clic(SDL_Surface *screen, int x, int y) {

    int p;
    int v;
    int c;
    int m;

    if(gameState == RUNNING) {

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
            board_show_result(screen, result, tryNumber);
            if(result[0] == CODE_LENGHT) {
                end_game(screen, 1);
            } else if(tryNumber < NB_ATTEMPTS) {
                tryNumber++;
                init_attempt_state();
                next_try(screen, tryNumber);
            } else {
                end_game(screen, 0);
            }

            free(result);
            return;
        }

        c= is_color_selected(x, y);
        if(c>=0) {
            board_select(screen, selectedColor, c);
            selectedColor=c;
	        return;
        }
    }

    m = menu_clicked(screen, x, y);
    if(m>=0) {
        switch(m) {
            case QUIT:
                confirm_quit(screen);
                break;
            default:
                fprintf(stdout, "Not yet implemented\n");
        }
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

    board_select(screen, UNDEFINED, selectedColor);
    next_try(screen, tryNumber);

    SDL_Flip(screen); /* Update screen */

    while (loop) {
        SDL_WaitEvent(&event);
        switch(event.type) {
        case SDL_QUIT:
            confirm_quit(screen);
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            // press 'q' to quit
            case SDLK_q:
                confirm_quit(screen);
                break;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                if(gameState == WAITING_CONFIRM_QUIT) {
                    action_t action = is_clicked(screen, event.button.x, event.button.y);
                    if(action==OK) {
                        loop = 0;
                    }
                    if(action != NONE) {
                        gameState = RUNNING;
                    }
                } else {
                    got_clic(screen, event.button.x, event.button.y);
                }
            }
            break;
        }
    }

    SDL_Quit();

    return 0;
}

