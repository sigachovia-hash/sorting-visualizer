#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define N_MAX 10000

int randomInRange(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

void fillArray(int arr[], int size, int min, int max) {
    for (int i = 0; i < size; i++) {
        arr[i] = randomInRange(min, max);
    }
}

// Функция для заполнения массива от 1 до n в случайном порядке
void fillRandomArray(int* arr, int n) {
    // 1. Заполняем массив числами от 1 до n
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    // 2. Перемешиваем элементы (алгоритм Фишера — Йетса)
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1); // Случайный индекс от 0 до i

        // Меняем местами arr[i] и arr[j]
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void copyArray(int arr1[], int arr2[], int size) {
    // Поэлементное копирование
    for (int i = 0; i < size; i++) {
        arr2[i] = arr1[i];
    }
}

void printArray(int arr[], int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void saveIteration(int arr[], int size, FILE* fp) {
   
    for (int i = 0; i < size; i++) {
        fprintf(fp, "%d ", arr[i]);
    } 
    fprintf(fp, "\n");
}

void bubbleSort(int arr[], int n, int* swap_count, FILE* fp) {
    int j, temp;
    // Инициализируем счетчик перестановок
    *swap_count = 0;

    //printf("bubble\n");

    bool flag_swap;
    // Внешний цикл: для каждого прохода
    do {
        flag_swap = false;
        //printf("bubble do\n");
        // Внутренний цикл: сравнение соседних элементов
        for (j = 0; j < n - 1; j++) {
            //printf("j = %d\n", j);
            // Если текущий элемент больше следующего, меняем их местами
            if (arr[j] > arr[j + 1]) {
                // Перестановка
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                // Взводим флаг
                flag_swap = true;

                // Увеличиваем счетчик перестановок
                (*swap_count)++;
                saveIteration(arr, n, fp);
            }
        }
    } while (flag_swap);
}

void shakerSort(int arr[], int n, int* swap_count, FILE* fp) {
    int j, temp;
    // Инициализируем счетчик перестановок
    *swap_count = 0;

    //printf("bubble\n");

    bool flag_swap;
    // Внешний цикл: для каждого прохода
    do {
        flag_swap = false;
        //printf("bubble do\n");
        // Внутренний цикл: сравнение соседних элементов
        for (j = 0; j < n - 1; j++) {
            //printf("j = %d\n", j);
            // Если текущий элемент больше следующего, меняем их местами
            if (arr[j] > arr[j + 1]) {
                // Перестановка
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                // Взводим флаг
                flag_swap = true;

                // Увеличиваем счетчик перестановок
                (*swap_count)++;
                saveIteration(arr, n, fp);
            }
        }
        if (!flag_swap) {
            break;
        }
        flag_swap = false;
        for (j = n - 2; j >= 0; j--) {
            //printf("j = %d\n", j);
            // Если текущий элемент больше следующего, меняем их местами
            if (arr[j] > arr[j + 1]) {
                // Перестановка
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                // Взводим флаг
                flag_swap = true;

                // Увеличиваем счетчик перестановок
                (*swap_count)++;
                saveIteration(arr, n, fp);

            }
        }
    } while (flag_swap);
}

void mergeSort(int arr[], int il, int ir, int* swap_count, FILE* fp) {
    //il - index left, ir - index right, im - index middle

    //printf("mergeSort: il = %d, ir = %d\n", il, ir); // отладка

    //return;

    if (il > ir) { // ???
        return;
    }
    else if (il == ir) {
        return; // один элемент всегда отсортирован :)
    }

    // здесь il < ir

    // Находим середину
    int im = il + (ir - il) / 2;

    // Рекурсивно сортируем первую и вторую половины
    mergeSort(arr, il, im, swap_count, fp);
    mergeSort(arr, im + 1, ir, swap_count, fp);
    //mergeSort(arr, il, im);
    //mergeSort(arr, im + 1, ir);

    // Слияние отсортированных половин
    //merge(arr, il, im, ir);

    int size_new = ir - il + 1;
    int arr_new[N_MAX]; //размер был size_new

    for (int i = 0; i < N_MAX; i++) {
        arr_new[i] = 0; //Инициализация массива
    }


    int i1 = il;
    int i2 = im + 1;
    int j = 0;

    do {
        if ((i1 <= im) && (i2 <= ir)) {
            //сравниваем элементы и наименьший помещаем в промежуточный массив
            if (arr[i1] < arr[i2]) {
                arr_new[j] = arr[i1];
                i1++;
                j++;
                (*swap_count)++;
                saveIteration(arr_new, size_new, fp);


            }
            else if (arr[i1] > arr[i2]) {
                arr_new[j] = arr[i2];
                i2++;
                j++;
                (*swap_count)++;
                saveIteration(arr_new, size_new, fp);
            }
            else {
                arr_new[j] = arr[i1];
                i1++;
                j++;
                arr_new[j] = arr[i2];
                i2++;
                j++;
                // не считаем за перестановки, так как это 2 одинаковых числа
                //(*swap_count)++;
                //(*swap_count)++;
                saveIteration(arr_new, size_new, fp);
            }
        }
        else if (i1 > im) {
            // первый фрагмент закончился. 
            // (прим.) фрагменты не закончатся одновременно!
            // "хвост" второго подмассива помещаем в промежуточный массив

            for (int i = i2; i <= ir; i++) {
                arr_new[j] = arr[i];
                saveIteration(arr_new, size_new, fp);
                j++;
            }
            i2 = ir + 1; // сдвинем указатель за пределы части массива №2

        }
        else {
            // второй фрагмент закончился
            //"хвост" первого подмассива помещаем в промежуточный массив

            for (int i = i1; i <= im; i++) {
                //printf("mergeSort, for #3: i = %d, i1 = %d, im = %d\n", i, i1, im); // отладка                
                arr_new[j] = arr[i];
                saveIteration(arr_new, size_new, fp);
                j++;
            }
            i1 = im + 1; // сдвинем указатель за пределы части массива №1
        }

    } while ((i1 <= im) || (i2 <= ir));

    // перемещаем резервный массив в начальный
    for (int i = 0; i < size_new; i++) {
        arr[il + i] = arr_new[i];
    }
    saveIteration(arr_new, size_new, fp);
}

void insertionSort(int arr[], int n, int* swap_count, FILE* fp) {

    int i, key, j; // key - вводим для запоминания текущего сортируемого значения; 

    // Начинаем со второго элемента (индекс 1), так как первый (индекс 0) считается уже отсортированной частью.
    for (i = 1; i < n; i++) {
        // Выбираем элемент, который будем вставлять (Ключ)
        key = arr[i];

        // Инициализируем указатель на последний элемент уже отсортированной части
        j = i - 1;

        // Сдвиг: Перемещаем элементы arr[0...i-1], которые больше, чем ключ,
        // на одну позицию вправо от их текущей позиции.
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j]; // Сдвиг элемента вправо
            (*swap_count)++; // Счетчик перестановок
            saveIteration(arr, n, fp);
            j--; // Переход к следующему элементу влево
        }

        // Вставка: Вставляем ключ в найденное место
        // (позиция j+1, так как цикл while уменьшил j на 1 лишний раз)
        arr[j + 1] = key;
        (*swap_count)++;
        saveIteration(arr, n, fp);
    }
}

void shellSort(int arr[], int n, int* swap_count, FILE* fp) {

    // Начинаем с большого интервала (шага) и постепенно уменьшаем его
    for (int gap = n / 2; gap >= 1; gap /= 2) {
        // При gap = 1 алгоритм сводится к сортировке методом insert 

        //printf("shellSort: %d\n", gap);


        // Выполняем сортировку вставками для текущего интервала.
        // Элементы, отстоящие друг от друга на `gap`, формируют подмассив.
        for (int i = gap; i < n; i++) {

            // Сохраняем элемент, который нужно вставить (Ключ)
            int key = arr[i];

            // Сдвигаем ранее отсортированные элементы направо до тех пор, пока не найдем правильное место для key.
            int j;

            // По сути сортировка методом insert по подмассиву элементов с "периодом" gap
            for (j = i; j >= gap && arr[j - gap] > key; j -= gap) {
                arr[j] = arr[j - gap]; // Сдвиг
                (*swap_count)++; // Счетчик перестановок
                saveIteration(arr, n, fp);
            }

            // Вставляем key (исходный arr[i]) на правильное место
            arr[j] = key;
            //(*swap_count)++;
            saveIteration(arr, n, fp);
        }
    }

}


int main() {

    // Установка кодировки UTF-8 для консоли
    //SetConsoleOutputCP(65001);
    //SetConsoleCP(65001);

    int size = 100;
    int min = 1;
    int max = 1000;
    int arr[N_MAX], arrSort[N_MAX]; //размер был size
    int swap_count;
    clock_t time1, time2;
    float t_sec;
    FILE* fp;

    char sort1[] = "C:\\Users\\julia\\source\\repos\\Sorting_C\\Output\\sort1.txt";
    char sort2[] = "C:\\Users\\julia\\source\\repos\\Sorting_C\\Output\\sort2.txt";
    char sort3[] = "C:\\Users\\julia\\source\\repos\\Sorting_C\\Output\\sort3.txt";
    char sort4[] = "C:\\Users\\julia\\source\\repos\\Sorting_C\\Output\\sort4.txt";
    char sort5[] = "C:\\Users\\julia\\source\\repos\\Sorting_C\\Output\\sort5.txt";

    fp = fopen(sort1, "w");

    if (fp == NULL) {
        printf("File not open! %s", sort1);
        return 1;
    }

    //fillArray(arr, size, min, max);
    fillRandomArray(arr, size);
    copyArray(arr, arrSort, size);

    if (size < 30) {
        printf("Исходный массив: \n");
        printArray(arr, size);
    }

    //////////////////////////////////////////////////
    // сортировка пузырьком
    
    copyArray(arr, arrSort, size);

    time1 = clock();

    // swap_count - исходящий параметр (то есть получим его назад из функции)
    bubbleSort(arrSort, size, &swap_count, fp);
    fclose(fp);

    time2 = clock();
    t_sec = ((float)(time2 - time1)) / CLOCKS_PER_SEC;
    //float t_sec = ((float) (time2-time1))/1000;

    printf("Отсортированный массив (bubbleSort, количество перестановок = %d, время = %f сек): \n", swap_count, t_sec);
    if (size < 30) {
        printArray(arrSort, size);
    }

    //////////////////////////////////////////////////
    // шейкерная сортировка 
    
    fp = fopen(sort2, "w");

    if (fp == NULL) {
        printf("File not open! %s", sort2);
        return 1;
    }
    
    copyArray(arr, arrSort, size);
    

    time1 = clock();

    swap_count = 0;
    // swap_count - исходящий параметр (то есть получим его назад из функции)
    shakerSort(arrSort, size, &swap_count, fp);
    fclose(fp);

    time2 = clock();
    t_sec = ((float)(time2 - time1)) / CLOCKS_PER_SEC;

    printf("Отсортированный массив (shakerSort, количество перестановок = %d, время = %f сек): \n", swap_count, t_sec);
    if (size < 30) {
        printArray(arrSort, size);
    }

    //////////////////////////////////////////////////
    // сортировка слиянием 

    fp = fopen(sort3, "w");

    if (fp == NULL) {
        printf("File not open! %s", sort3);
        return 1;
    }

    copyArray(arr, arrSort, size);

    time1 = clock();

    swap_count = 0;
    // swap_count - исходящий параметр (то есть получим его назад из функции)
    mergeSort(arrSort, 0, size - 1, &swap_count, fp);
    //mergeSort(arrSort, 0, size-1);
    fclose(fp);

    time2 = clock();
    t_sec = ((float)(time2 - time1)) / CLOCKS_PER_SEC;

    printf("Отсортированный массив (mergeSort, количество перестановок = %d, время = %f сек): \n", swap_count, t_sec);
    if (size < 30) {
        printArray(arrSort, size);
    }

    //////////////////////////////////////////////////
    // сортировка простыми вставками
    fp = fopen(sort4, "w");

    if (fp == NULL) {
        printf("File not open! %s", sort4);
        return 1;
    }

    copyArray(arr, arrSort, size);

    time1 = clock();

    swap_count = 0;
    // swap_count - исходящий параметр (то есть получим его назад из функции)
    insertionSort(arrSort, size, &swap_count, fp);
    fclose(fp);

    time2 = clock();
    t_sec = ((float)(time2 - time1)) / CLOCKS_PER_SEC;
    //float t_sec = ((float) (time2-time1))/1000;

    printf("Отсортированный массив (insertionSort, количество перестановок = %d, время = %f сек): \n", swap_count, t_sec);
    if (size < 30) {
        printArray(arrSort, size);
    }

    //////////////////////////////////////////////////
    // сортировка Шелла
    fp = fopen(sort5, "w");

    if (fp == NULL) {
        printf("File not open! %s", sort5);
        return 1;
    }

    copyArray(arr, arrSort, size);

    time1 = clock();

    swap_count = 0;
    // swap_count - исходящий параметр (то есть получим его назад из функции)
    shellSort(arrSort, size, &swap_count, fp);
    fclose(fp);

    time2 = clock();
    t_sec = ((float)(time2 - time1)) / CLOCKS_PER_SEC;
    //float t_sec = ((float) (time2-time1))/1000;

    printf("Отсортированный массив (shellSort, количество перестановок = %d, время = %f сек): \n", swap_count, t_sec);
    if (size < 30) {
        printArray(arrSort, size);
    }

    return 0;
}
