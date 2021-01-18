#include "encoder.h"
#include "sorter.h"

size_t get_length(char *str) {
    size_t result;

    for (result = 0; str[result] != '\0'; ++result);
    return result;
}

void *rotate_right(char *str) {
    int i, tmp, length;

    length = get_length(str);

    // Retain the last character that will be moved to the beggining
    tmp = str[length - 1];

    // Shift all values to the right
    for (i = length - 2; i >= 0; --i) {
        str[i + 1] = str[i];
    }

    // Move last character to the beggining
    str[0] = tmp;
}

char **get_all_rotations(char *str, int str_len) {
    int i;
    char **rotations;

    // Initialize array of char arrays containing all the rotations
    rotations = (char **) malloc(str_len * sizeof(*rotations));

    #pragma omp parallel for shared(i,rotations,str_len,str) schedule(dynamic) num_threads(1)
    for (i = 0; i < str_len; ++i) {
        // Allocate, copy value and manage null terminator
        rotations[i] = malloc((str_len + 1) * sizeof(**rotations));
        strcpy(rotations[i], str);
        rotations[i][str_len] = '\0';

        rotate_right(str);
    }

    return rotations;
}

char *burrows_wheeler_transform(char *str) {
    char **rotations;
    char *encoded;
    size_t str_len;
    int i;

    // Compute all rotations of the string
    str_len = get_length(str);
    rotations = get_all_rotations(str, str_len);

    // Sort all rotations alphanumerically
    quick_sort(rotations, 0, str_len - 1);

    // Retreive and keep the last character of every rotation
    encoded = (char *) malloc((str_len + 1) * sizeof(*encoded));

    #pragma omp parallel for shared(i,str_len,rotations,encoded) schedule(static) num_threads(1)
    for (i = 0; i < str_len; ++i) {
        encoded[i] = rotations[i][str_len - 1];

        // We no longer need the rotations
        free(rotations[i]);
    }
    free(rotations);

    encoded[str_len] = '\0';
    return encoded;
}

char *compute_rle(char *str) {
    const char BUFLEN = 16;
    char buf[BUFLEN];
    size_t str_len = get_length(str);
    char *encoded = (char *) calloc(LINE_LEN, sizeof(*encoded));

    memset(buf, 0, BUFLEN * sizeof(*buf));
    for (int i = 0; i < str_len; ++i) {
 
        // Count occurrences of current character
        int count = 1;
        while (i < str_len - 1 && str[i] == str[i + 1]) {
            count++;
            i++;
        }
 
        sprintf(buf, "%c%d", str[i], count);
        strcat(encoded, buf);
    }

    return encoded;
}