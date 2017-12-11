/**
 * \file secret.c
 * The super mind master's secret.
 */


#include <stdlib.h>
#include "secret.h"


/**
 * Put a new random code in a pointed location.
 * \param code a pre-allocated code location
 * \return 0 in case of success
 */
int change_code(code_t *code) {

    // TODO Randomize

    int i;

    for(i=0; i<CODE_LENGHT; i++) {
        (*code)[i]=i;
    }
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




