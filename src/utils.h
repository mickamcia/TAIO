#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

void utils_print_execution_time(clock_t exact, clock_t approx);
void bubble_sort(int* array, int size);
void array_print(int* g, int size, char name[]);
void array2d_print(int* arr, int columns, int rows, char name[]);

#endif