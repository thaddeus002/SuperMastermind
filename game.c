/**
 * \file game.c
 * Defining the game's rules.
 */

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include "secret.h"

#define WIDTH 640
#define HEIGHT 700


/** the files for the colored balls */
static char *ballsImages[] = { "data/red.bmp", "data/green.bmp", "data/blue.bmp", "data/white.bmp", "data/orange.bmp", "data/cyan.bmp", "data/yellow.bmp", "data/purple.bmp"};




/**
 * Create the colors board and add it the the screen.
 */
static void add_colors_board(SDL_Surface *plateau) {
    SDL_Surface *board;
    SDL_Rect position;
    int i;

    position.x = WIDTH / 2 + 20;
    position.y = 40;

    board = SDL_CreateRGBSurface(SDL_HWSURFACE, WIDTH / 2 - 40, 80, 32, 0, 0, 0, 0);
    SDL_FillRect(board, NULL, SDL_MapRGB(board->format, 79, 33, 17));

    for(i = 0; i < COLORS_NB; i++) {
        SDL_Surface *colorBall;
        SDL_Rect colorPosition;

        colorPosition.x = 3 + 40 * i;
        colorPosition.y = 3;

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
    add_colors_board(screen);
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
        }
    }

    SDL_Quit();

    return EXIT_SUCCESS;
}

