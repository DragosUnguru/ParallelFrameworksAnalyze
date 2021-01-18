#include <mpi.h>
#include <math.h>
#include "encoder.h"

#define MASTER 0

typedef struct scope_t {
    int start_scope;
    int end_scope;
    int scope_len;
    int next_idx_to_write;
}scope_t;

char *execute(char *str) {
    char *encoded, *result;

    encoded = burrows_wheeler_transform(str);
    result = compute_rle(encoded);

    free(encoded);
    return result;
}

int main(int argc, char *argv[]) {
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == MASTER) {
        int communicating_id;
        char **data_out, **data_in;
        float step;
        FILE *in_fp = NULL;
        size_t no_lines, len = 0, read_line_len;
        ssize_t read_bytes;
        scope_t *scopes;

        // Open input file and validate file pointer
        in_fp = fopen(INPUT_FILE, "r");
        if (in_fp == NULL) {
            fprintf(stderr, "Unable to open file. Exiting...");
            exit(EXIT_FAILURE);
        }

        // Read number of lines and alloc structure accordingly
        fscanf(in_fp, "%lu", &no_lines);
        data_out = (char **) malloc(no_lines * sizeof(*data_out));
        data_in = (char **) malloc(no_lines * sizeof(*data_in));

        scopes = (scope_t *) malloc(size * sizeof(*scopes));
        step = (float) no_lines / (float) ((float) size - 1.f);

        for (int i = 1; i < size; ++i) {
            scopes[i].start_scope = ceil(((float) i - 1.f) * step);
            scopes[i].end_scope = MIN(no_lines, (int) ceil((float) ((float) i) * step));
            scopes[i].scope_len = scopes[i].end_scope - scopes[i].start_scope;
            scopes[i].next_idx_to_write = scopes[i].start_scope;

            // Signal every proc of the incoming number of lines each has to process
            MPI_Send(&scopes[i].scope_len, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        
        // Keep all the lines in memory for benchmarking fairness and
        // to send in a circular manner, not flooding one process at a time
        for (int i = 0; i < no_lines; ++i) {
            data_in[i] = NULL;
            len = 0;
            getline(&data_in[i], &len, in_fp);
        }

        // First worker proc has guaranteed the most work to do
        while (scopes[1].next_idx_to_write != scopes[1].end_scope) {

            // Send work to each and every worker proc
            for (int i = 1; i < size; ++i) {
                if (scopes[i].next_idx_to_write != scopes[i].end_scope) {
                    read_line_len = get_length(data_in[scopes[i].next_idx_to_write]);
                    MPI_Send(&read_line_len, 1, MPI_UNSIGNED_LONG, i, 0, MPI_COMM_WORLD);
                    MPI_Send(data_in[scopes[i].next_idx_to_write], read_line_len, MPI_BYTE, i, 0, MPI_COMM_WORLD);
                }
            }

            // Wait for procs to finish work and reconstruct file
            for (int i = 1; i < size; ++i) {
                if (scopes[i].next_idx_to_write != scopes[i].end_scope) {
                    MPI_Recv(&len, 1, MPI_UNSIGNED_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    data_out[scopes[i].next_idx_to_write] = (char *) malloc(len * sizeof(char));

                    MPI_Recv(data_out[scopes[i].next_idx_to_write], len, MPI_BYTE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    scopes[i].next_idx_to_write++;
                }
            }
        }

        // Print result
        for (int i = 0; i < no_lines; ++i) {
            printf("%s\n", data_out[i]);
            free(data_out[i]);
            free(data_in[i]);
        }
        free(data_out);
        free(data_in);
        fclose(in_fp);

    } else {
        int local_no_lines;
        size_t line_len, encoded_line_len;
        char *recv_line, *encoded;
        char **local_data;

        // Receive number of lines to process
        MPI_Recv(&local_no_lines, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        local_data = (char **) malloc(local_no_lines * sizeof(*local_data));
        
        for (int i = 0; i < local_no_lines; ++i) {
            // Receive line length to encode and alloc accordingly
            MPI_Recv(&line_len, 1, MPI_UNSIGNED_LONG, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // fprintf(stderr, "Proc [%d]: Received LINE_LEN: %lu\n", rank, line_len);
            local_data[i] = (char *) malloc((line_len + 1) * sizeof(char));
            local_data[i][line_len] = '\0';

            // Receive line
            MPI_Recv(local_data[i], line_len, MPI_BYTE, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Execute
            encoded = execute(local_data[i]);

            // Send result
            encoded_line_len = get_length(encoded) + 1;
            MPI_Send(&encoded_line_len, 1, MPI_UNSIGNED_LONG, MASTER, 0, MPI_COMM_WORLD);
            MPI_Send(encoded, get_length(encoded) + 1, MPI_BYTE, MASTER, 0, MPI_COMM_WORLD);

            free(encoded);
        }

        // Free memory
        for (int i = 0; i < local_no_lines; ++i) {
            free(local_data[i]);
        }
        free(local_data);
    }

    MPI_Finalize();
    return 0;
}
