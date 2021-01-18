#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define PRINTING_ENABLED 1
#define ITERATIONS 10
#define LINE_LEN 30000

#ifndef INPUT_FILE
#define INPUT_FILE "input.txt"
#endif

/**
 * Retrurns the length of the string, assuming
 * that the string is NULL terminated
 * *str -> string to copute length
 * returns int -> length of the string
 */
size_t get_length(char *str);

/**
 * Rotates the characters of a char array one position
 * to the right
 * *str -> char array to rotate
 * returns -> 1-position-right-rotated char array
 */
void *rotate_right(char *str);

/**
 * Given a string, returns an array of char arrays
 * containing all the rotations of the given string.
 * *str -> char array to compute all rotations
 * strl_len -> length of the given null terminated char array
 * returns -> all the rotations of 'str' in a array of char arrays
 */
char **get_all_rotations(char *str, int str_len);

/**
 * Runs the Burrows-Wheeler transform on a given string
 * *str -> char array to transform
 * returns -> char array containing the encoded value
 */
char *burrows_wheeler_transform(char *str);

/**
 * Computes the Run Length Encoding on
 * the given string.
 * *str -> char array to encode
 * returns -> Run-length encoded char array of str
 */
char *compute_rle(char *str);