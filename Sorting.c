#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define N_MAX 10000

typedef void (*SortFunc)(int arr[], int n, int* swap_count, FILE* fp);

void fillRandomArray(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void copyArray(int src[], int dst[], int size) {
    for (int i = 0; i < size; i++) {
        dst[i] = src[i];
    }
}

void saveIteration(int arr[], int size, FILE* fp) {
    for (int i = 0; i < size; i++) {
        fprintf(fp, "%d ", arr[i]);
    }
    fprintf(fp, "\n");
}

void bubbleSort(int arr[], int n, int* swap_count, FILE* fp) {
    bool flag_swap;
    do {
        flag_swap = false;
        for (int j = 0; j < n - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                flag_swap = true;
                (*swap_count)++;
                saveIteration(arr, n, fp);
            }
        }
    } while (flag_swap);
}

void shakerSort(int arr[], int n, int* swap_count, FILE* fp) {
    bool flag_swap;
    do {
        flag_swap = false;
        for (int j = 0; j < n - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                flag_swap = true;
                (*swap_count)++;
                saveIteration(arr, n, fp);
            }
        }
        if (!flag_swap) {
            break;
        }
        flag_swap = false;
        for (int j = n - 2; j >= 0; j--) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                flag_swap = true;
                (*swap_count)++;
                saveIteration(arr, n, fp);
            }
        }
    } while (flag_swap);
}

static int merge_buf[N_MAX];

void saveMergeState(int arr[], int n, int il, int filled, FILE* fp) {
    for (int i = 0; i < n; i++) {
        int value = (i >= il && i < il + filled) ? merge_buf[i - il] : arr[i];
        fprintf(fp, "%d ", value);
    }
    fprintf(fp, "\n");
}

void mergeRange(int arr[], int n, int il, int ir, int* swap_count, FILE* fp) {
    if (il >= ir) {
        return;
    }

    int im = il + (ir - il) / 2;
    mergeRange(arr, n, il, im, swap_count, fp);
    mergeRange(arr, n, im + 1, ir, swap_count, fp);

    int i1 = il;
    int i2 = im + 1;
    int j = 0;

    while (i1 <= im || i2 <= ir) {
        if (i2 > ir || (i1 <= im && arr[i1] <= arr[i2])) {
            merge_buf[j++] = arr[i1++];
        } else {
            merge_buf[j++] = arr[i2++];
        }
        (*swap_count)++;
        saveMergeState(arr, n, il, j, fp);
    }

    for (int i = 0; i < j; i++) {
        arr[il + i] = merge_buf[i];
    }
}

void mergeSort(int arr[], int n, int* swap_count, FILE* fp) {
    mergeRange(arr, n, 0, n - 1, swap_count, fp);
}

void insertionSort(int arr[], int n, int* swap_count, FILE* fp) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            (*swap_count)++;
            saveIteration(arr, n, fp);
            j--;
        }
        if (j + 1 != i) {
            arr[j + 1] = key;
            (*swap_count)++;
            saveIteration(arr, n, fp);
        }
    }
}

void shellSort(int arr[], int n, int* swap_count, FILE* fp) {
    for (int gap = n / 2; gap >= 1; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int key = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > key; j -= gap) {
                arr[j] = arr[j - gap];
                (*swap_count)++;
                saveIteration(arr, n, fp);
            }
            if (j != i) {
                arr[j] = key;
                saveIteration(arr, n, fp);
            }
        }
    }
}

int runSort(const char* name, SortFunc sort, const char* path, int source[], int size) {
    int arr[N_MAX];
    int swap_count = 0;

    FILE* fp = fopen(path, "w");
    if (fp == NULL) {
        printf("Cannot open file %s\n", path);
        return 1;
    }

    copyArray(source, arr, size);
    saveIteration(arr, size, fp);

    clock_t start = clock();
    sort(arr, size, &swap_count, fp);
    float t_sec = (float)(clock() - start) / CLOCKS_PER_SEC;
    fclose(fp);

    printf("%-10s swaps = %6d, time = %f s -> %s\n", name, swap_count, t_sec, path);
    return 0;
}

int main(int argc, char* argv[]) {
    const char* out_dir = (argc > 1) ? argv[1] : ".";
    int size = (argc > 2) ? atoi(argv[2]) : 100;

    if (size < 2 || size > N_MAX) {
        printf("Size must be between 2 and %d\n", N_MAX);
        return 1;
    }

    int arr[N_MAX];
    fillRandomArray(arr, size);

    const char* names[] = { "bubble", "shaker", "merge", "insertion", "shell" };
    SortFunc sorts[] = { bubbleSort, shakerSort, mergeSort, insertionSort, shellSort };
    int count = sizeof(sorts) / sizeof(sorts[0]);

    for (int k = 0; k < count; k++) {
        char path[512];
        snprintf(path, sizeof(path), "%s/sort%d.txt", out_dir, k + 1);
        if (runSort(names[k], sorts[k], path, arr, size) != 0) {
            return 1;
        }
    }

    return 0;
}
