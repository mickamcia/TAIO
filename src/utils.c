#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "limits.h"
#include "utils.h"

void utils_print_execution_time(clock_t exact, clock_t approx) {
	printf("\nExact: %f seconds\n", ((double)exact) / CLOCKS_PER_SEC);
	printf("Approx: %f seconds\n", ((double)approx) / CLOCKS_PER_SEC);
}

void bubble_sort(int* arr, int size) {
	int i, j;
	int swapped;
	for (i = 0; i < size - 1; i++) {
		swapped = 0;
		for (j = 0; j < size - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				int tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
				swapped = 1;
			}
		}

		if (swapped == 0)
			break;
	}
}

void array_print(int* g, int size, char name[])
{
	printf("\n%s\n", name);
	for (int i = 0; i < size; i++) {
		const int val = g[i];
		if (val == INT_MAX) printf("inf ");
		else if (val >= -1) printf("% 3d ", val); // prints from 0 to 99, should change to "%d" by the end
		else printf("    ");
	}
	printf("\n");
}

void array2d_print(int* arr, int columns, int rows, char name[])
{
	printf("\n%s\n", name);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			const int val = arr[i * columns + j];
			if (val == INT_MAX) printf("inf ");
			else if (val >= -1) printf("% 3d ", val); // prints from 0 to 99, should change to "%d" by the end
			else printf("    ");
		}
		printf("\n");
	}
}