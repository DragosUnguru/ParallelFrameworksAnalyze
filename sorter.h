
/**
 * A utility function to swap two elements
 * **a -> address of the char array to be swapped
 * **b -> address of the char array to be swapped
 */
void swap(char **a, char **b);

/**
 * Custom comparator for comparing char arrays, assuming
 * that str1 and str2 are rotations of the same string, therefore
 * having the same length
 * *str1 -> string to compare
 * *str2 -> string to compare str1 against to
 * returns 0 if str1 is bigger than str2, 1 otherwise
 */
int compare_string(char *str1, char *str2);

/**
 * This function takes last element as pivot,
 * and manages the rest of the char arrays according
 * to this pivot
 * **arr -> array of char arrays to be sorted
 */
char partition(char **str, int low, int high);

/**
 * The main function that implements QuickSort
 * **arr -> array to be sorted,
 * low -> starting index,
 * high -> ending index
 */
void quick_sort(char **arr, int low, int high);