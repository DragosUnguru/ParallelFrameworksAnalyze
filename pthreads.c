#include <pthread.h>
#include <math.h>
#include "encoder.h"

#define NUM_THREADS 8

char **data_in, **data_out;
size_t no_lines;

void *thread_func(void *args) {
    int thread_id = *((int *) args);
    int start_scope, end_scope, i;
    float step;
    char *bw_transformed;

    step = (float) no_lines / (float) NUM_THREADS;
    start_scope = thread_id * step;
    end_scope = MIN(no_lines, (int) ceil((float) ((float) thread_id + 1.f) * step));

    // Execute
    for (i = start_scope; i < end_scope; ++i) {
        bw_transformed = burrows_wheeler_transform(data_in[i]);
        data_out[i] = compute_rle(bw_transformed);

        free(bw_transformed);
    }
}

int main() {
    int i;
    char *encoded, *line;
    FILE *in_fp;
    size_t len = 0;
    ssize_t read_bytes;
    pthread_t threads[NUM_THREADS];
    int tids[NUM_THREADS];

    // Open input file and validate file pointer
    in_fp = fopen(INPUT_FILE, "r");

    if (in_fp == NULL) {
        fprintf(stderr, "Unable to open file. Exiting...");
        exit(EXIT_FAILURE);
    }

    // Read number of lines and alloc structures accordingly
    fscanf(in_fp, "%lu", &no_lines);
    data_in = (char **) malloc(no_lines * sizeof(*data_in));
    data_out = (char **) malloc(no_lines * sizeof(*data_out));

    // Keep all the lines in memory
    for (i = 0; i < no_lines; ++i) {
        data_in[i] = NULL;
        len = 0;
        getline(&data_in[i], &len, in_fp);
    }

    // Execute
    for (i = 0; i < NUM_THREADS; ++i) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, (void *) &tids[i]);
    }

    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Free memory and print output
    for (i = 0; i < no_lines; ++i) {
        printf("%s\n", data_out[i]);
        free(data_out[i]);
        free(data_in[i]);
    }
    free(data_in);
    free(data_out);
    fclose(in_fp);

    return 0;
}
