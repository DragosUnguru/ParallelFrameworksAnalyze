
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
    if (low < high) {
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}