/**
 * \file secret.c
 * The super mind master's secret.
 */


#include <time.h>
#include <stdlib.h>
#include "secret.h"


/**
 * Put a new random code in a pointed location.
 * \param code a pre-allocated code location
 * \return 0 in case of success
 */
int change_code(code_t *code) {

    int i;

    srand(time(NULL));   // should only be called once

    for(i=0; i<CODE_LENGHT; i++) {
        (*code)[i]=rand() % COLORS_NB;
    }
}


/**
 * Verify that all the keys in a code are defined.
 * \return 1 is there a no UNDEFINED value in code, 0 otherwise
 */
int is_completed(code_t *code){

    int i;

    for(i=0; i<CODE_LENGHT; i++) {
        if((*code)[i]==UNDEFINED) {
            return 0;
        }
    }

    return 1;
}



/**
 * Compare a combinaison to the secret code.
 */
int *test(code_t *combinaison, code_t *secret){
    int *test = malloc(sizeof(int)*2);
    int i, j;
    int counted[CODE_LENGHT];


    test[0]=0;
    test[1]=0;

    for(i=0; i<CODE_LENGHT; i++) {
        if( (*combinaison)[i] == (*secret)[i] ) {
            test[0]++;
            counted[i]=1;
        } else {
            counted[i]=0;
        }
    }

    for(i=0; i<CODE_LENGHT; i++) {
        if( (*combinaison)[i] != (*secret)[i] ) {
            for(j=0; j<CODE_LENGHT; j++) {
                if(!counted[j] && (*combinaison)[i] == (*secret)[j] ) {
                    test[1]++;
                    counted[j]=1;
                }
            }
        }
    }

    return test;
}




