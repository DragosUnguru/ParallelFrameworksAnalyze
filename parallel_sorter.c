#include <omp.h>

void swap(char **a, char **b) {
    char *tmp = *a;
    *a = *b;
    *b = tmp;
}

int compare_string(char *str1, char *str2) {
    int i;

    for (i = 0; str1[i] != '\0' || str2[i] != '\0'; ++i) {
        if (str1[i] > str2[i]) {
            return 0;
        }
        else if (str1[i] < str2[i]) {
            return 1;
        }
    }

    // Equal, no need to swap
    return 0;
}


int partition(char **str, int low, int high) {
    char *pivot = str[high];
    int i = low - 1;  // Index of smaller element
    int j;

    for (j = low; j < high; ++j) {
        // If current element is smaller than the pivot
        if (compare_string(str[j], pivot)) {
            i++;
            swap(&str[i], &str[j]);
        }
    }

    swap(&str[i + 1], &str[high]);
    return i + 1;
}


void quick_sort(char **arr, int low, int high) {
    int i;
    int lows[2];
    int highs[2];

    if (low < high) {
        int pi = partition(arr, low, high);

        lows[0] = low;
        lows[1] = pi + 1;
        highs[0] = pi - 1;
        highs[1] = high;

        // Generate two different threads that will run
        // concurrently; each one will execute, create and
        // manage other two tasks, and so on.
        #pragma omp parallel num_threads(1)
        {
            #pragma omp for nowait
            for(i = 0; i <= 1; ++i) {
                quick_sort(arr, lows[i], highs[i]);
            }
        }
    }
}