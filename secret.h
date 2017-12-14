/**
 * \file secret.h
 * The super mind master's secret.
 */


#ifndef __SECRET_H_
#define __SECRET_H_

/** Number of colors */
#define COLORS_NB 8

/** Number of keys in the code */
#define CODE_LENGHT 5

/** The possible color for each code's key */
typedef enum color_ {
    UNDEFINED=-1, RED, GREEN, BLUE, WHITE, ORANGE, CYAN, YELLOW, PURPLE
} color_t;

/** A type to store the secret*/
typedef color_t code_t[CODE_LENGHT];

/**
 * Compare a combinaison to the secret code will result in two int.
 * The first for the number of good placements.
 * The second for good colors at the bad place.
 */
typedef int test_t[2];

/**
 * Put a new random code in a pointed location.
 * \param code a pre-allocated code location
 * \return 0 in case of success
 */
int change_code(code_t *code);


/**
 * Verify that all the keys in a code are defined.
 * \return 1 is there a no UNDEFINED value in code, 0 otherwise
 */
int is_completed(code_t *code);


/**
 * Compare a combinaison to the secret code.
 */
int *test(code_t *combinaison, code_t *secret);




#endif
