#include <stdlib.h>
#include <string.h>
#include "metric.h"
#include "utils.h"

// internal
int* build_degree_vector(matrix* g);
int* vector_diff(int* v1, int* v2, int size);

int distance(matrix* g1, matrix* g2) {
	int* v1 = build_degree_vector(g1);
	int* v2 = build_degree_vector(g2);

	array_print(v1, g1->size, "v1");
	array_print(v2, g2->size, "v2");

	int d = vector_diff(v1, v2, g1->size);

	free(v1);
	free(v2);

	return d;
}

int* build_degree_vector(matrix* g) {
	int* vector = (int*)malloc(sizeof(int) * g->size);
	if (!vector)
		ERR("malloc");

	memset(vector, 0, sizeof(int) * g->size);
	
	for (int i = 0; i < g->size; i++) {
		for (int j = 0; j < g->size; j++) {
			if (g->mat[i * g->size + j] > 0) {
				vector[i] += g->mat[i * g->size + j];
			}
		}
	}

	bubble_sort(vector, g->size);
	return vector;
}

int* vector_diff(int* v1, int* v2, int size) {
	int diff = 0;

	for (int i = 0; i < size; i++) {
		diff += abs(v1[i] - v2[i]);
	}

	return diff;
}