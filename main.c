/**
 * \file main.c
 * \brief Main program of the SuperMastermind project.
 */

#include <stdio.h>
#include <stdlib.h>
#include "secret.h"
#include "game.h"

#define AUTHOR "Yannick Garcia"
#define VERSION "0.0.1"


/**
 * Program presentation.
 */
static void present(char *prog) {
    fprintf(stdout, "\n%s : SuperMastermind program - Version %s\n", prog, VERSION);
    fprintf(stdout, "Bring to you by the great mind master %s\n\n", AUTHOR);
}

/**
 * Show code to stdout.
 */
static void print_code(code_t code){

    int i;

    fprintf(stdout, "\nCode is : ");

    for(i=0; i<CODE_LENGHT; i++) {
        fprintf(stdout, "%d ", code[i]);
    }

    fprintf(stdout, "\n\n");
}


static void print_result(int *result) {
    fprintf(stdout, "\n%d good - %d misplaced\n\n", result[0], result[1]);
}


int main(int argc, char **argv) {

    present(argv[0]);

    // start game
    return new_game();
}

