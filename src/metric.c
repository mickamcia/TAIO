#include <stdlib.h>
#include <string.h>
#include "metric.h"
#include "utils.h"
#include "graph.h"
#include "limits.h"

// internal
int* build_degree_vector(matrix* g, int* newSize);
int vector_diff(int* v1, int* v2, int size);

int distance(matrix* g1, matrix* g2) {
	int v1_size, v2_size;
	int* v1 = build_degree_vector(g1, &v1_size);
	int* v2 = build_degree_vector(g2, &v2_size);

	if (v1_size != v2_size)
		return INT_MAX;

	int d = vector_diff(v1, v2, v1_size);

	free(v1);
	free(v2);

	return d;
}

int* build_degree_vector(matrix* g, int* newSize) {
	*newSize = 0;
	for (int i = 0; i < g->size; i++)
		if (g->mat[i * g->size + i] == 0)
			(*newSize)++;

	int* vector = (int*)malloc(sizeof(int) * *newSize);
	if (!vector)
		ERR("malloc");

	memset(vector, 0, sizeof(int) * *newSize);

	int v_i = 0, v_j = 0;
	for (int i = 0; i < g->size; i++) {
		if (g->mat[i * g->size + i] == 0) {
			for (int j = 0; j < g->size; j++) {
				if (g->mat[i * g->size + j] > 0) {
					vector[v_i] += g->mat[i * g->size + j];
				}
			}
			v_i++;
		}
	}

	bubble_sort(vector, *newSize);
	return vector;
}

int vector_diff(int* v1, int* v2, int size) {
	int diff = 0;

	for (int i = 0; i < size; i++) {
		diff += abs(v1[i] - v2[i]);
	}

	return diff;
}