//
// Created by Ivo Georgiev on 2019-03-05.
//

//
// Created by Ivo Georgiev on 2019-02-28.
//
#include <stddef.h>
#include <stdio.h>

void print_int_array(int *a, size_t size) {
    int i = 0;
    for (; i < size - 1; i ++)
        printf("%d, ", a[i]);
    printf("%d ", a[i]);
}

void run_arrays() {
    int int_array[] = { 1, 4, 6, 8, 10, 101, 3033 };

    print_int_array(int_array, sizeof(int_array)/ sizeof(int_array[0]));
}