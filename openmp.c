#include <omp.h>
#include "encoder.h"

int main() {
    int i;
    char *encoded, *line;
    char **data_in, **data_out;
    FILE *in_fp;
    size_t len = 0, no_lines;
    ssize_t read_bytes;

    // Open input file and validate file pointer
    in_fp = fopen(INPUT_FILE, "r");

    if (in_fp == NULL) {
        fprintf(stderr, "Unable to open file. Exiting...");
        exit(EXIT_FAILURE);
    }

    // Read number of lines and alloc structure accordingly
    fscanf(in_fp, "%lu", &no_lines);

    data_in = (char **) malloc(no_lines * sizeof(*data_in));
    data_out = (char **) malloc(no_lines * sizeof(*data_out));

    for (i = 0; i < no_lines; ++i) {
        data_in[i] = NULL;
        len = 0;
        getline(&data_in[i], &len, in_fp);
    }

    #pragma omp parallel for shared(i,data_in,data_out,no_lines) private(encoded) schedule(dynamic) num_threads(1)
    for (i = 0; i < no_lines; ++i) {
        encoded = burrows_wheeler_transform(data_in[i]);
        data_out[i] = compute_rle(encoded);
        free(encoded);
    }

    // Print sequentially
    for (i = 0; i < no_lines; ++i) {
        printf("%s\n", data_out[i]);
    }

    #pragma omp parallel for shared(i,data_in,data_out,no_lines) schedule(static) num_threads(1)
    for (i = 0; i < no_lines; ++i) {
        free(data_in[i]);
        free(data_out[i]);
    }

    free(data_in);
    free(data_out);
    fclose(in_fp);

    return 0;
}
