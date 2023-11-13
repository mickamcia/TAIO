#include "bma.h"
#include "limits.h"
#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

#define DEBUG 0

void bma_init(matrix* g, matrix* exclude_curr, matrix* exclude_next, int* cost, int* previous)
{
	int n = g->size;
	for (int u = 0; u < n; u++) {
		for (int v = 0; v < n; v++) {
			if (u == v || g->mat[u * n + v])	// for each neighbor of u or u itself
			{
				exclude_curr->mat[u * n + v] = 1;
				cost[u]++;
			}
		}

		previous[u] = -1;
	}

	for (int round = 1; round <= n; round++) {
		for (int j = 0; j < n; j++) {
			previous[round * n + j] = -1;
			cost[round * n + j] = INT_MAX;
		}
	}
}

/// <summary>
/// Helper function - to be removed
/// </summary>
/// <param name="g"></param>
/// <param name="size"></param>
/// <param name="name"></param>
void array_print(int* g, int size, char name[])
{
	printf("\n%s\n", name);
	for (int i = 0; i < size; i++) {
		const int val = g[i];
		if (val == INT_MAX) printf("inf ");
		else if (val >= -1) printf("% 3d ", val); // prints from 0 to 99, should change to "%d" by the end
		else printf("    ");
	}
}

/// <summary>
/// Helper function - to be removed
/// </summary>
/// <param name="arr"></param>
/// <param name="columns"></param>
/// <param name="rows"></param>
/// <param name="name"></param>
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

int bma_relaxation(matrix* g, matrix* exclude_curr, matrix* exclude_next, int* cost, int* previous)
{
	int lastRound = -1;
	int n = g->size;
	int* vExclude = (int*)malloc(sizeof(int) * n);
	if (!vExclude)
		ERR("malloc");

	int vCost = -1;

	for (int round = 0; round < g->size; round++) {	// round in rows
		short changed = 0;

		for (int u = 0; u < n; u++) {		// u - rows in exclude, columns in cost
			for (int v = 0; v < n; v++) {	// v - columns in exclude
				if (exclude_curr->mat[u * n + v] == 1 || cost[round * n + u] == INT_MAX)
					continue;

				memcpy(vExclude, &(exclude_curr->mat[u * n]), sizeof(int) * n);
				vExclude[v] = 1;
				vCost = 1 + cost[round * n + u];

				for (int j = 0; j < n; j++) {	// for each neighbor of v
					if (j != v && g->mat[v * n + j] == 1) { 
						if (vExclude[j] == 0) {
							vExclude[j] = 1;
							vCost++;
						}
					}
				}

				if (cost[(round + 1) * n + v] > vCost) {
					previous[(round + 1) * n + v] = u;
					memcpy(&(exclude_next->mat[v * n]), vExclude, sizeof(int) * n);
					cost[(round + 1) * n + v] = vCost;
					
					changed = 1;
				}
			}
		}

		if (changed == 0) {
			lastRound = round;
			break;
		}

		memcpy(exclude_curr->mat, exclude_next->mat, sizeof(int) * n * n);
		memset(exclude_next->mat, 0, sizeof(int) * n * n);

		if (DEBUG) {
			printf("\nexclude_curr:");
			graph_print(exclude_curr);
			array2d_print(cost, g->size, g->size + 1, "cost");
			array2d_print(previous, g->size, g->size + 1, "previous");
		}
	}
	free(vExclude);

	return lastRound;
}

int bma(matrix* g, int* cost, int* previous)
{
	int rows = g->size + 1;
	int columns = g->size;

	matrix* exclude_curr = matrix_init(g->size);
	matrix* exclude_next = matrix_init(g->size);

	bma_init(g, exclude_curr, exclude_next, cost, previous);

	if (DEBUG) {
		printf("\ninit exclude_curr:");
		graph_print(exclude_curr);
		array2d_print(cost, columns, rows, "init cost");
		array2d_print(previous, columns, rows, "init previous");
	}

	int lastRound = bma_relaxation(g, exclude_curr, exclude_next, cost, previous);

	if (DEBUG) {
		printf("\nexclude_curr:");
		graph_print(exclude_curr);
		array2d_print(cost, columns, rows, "cost");
		array2d_print(previous, columns, rows, "previous");
	}

	matrix_destroy(exclude_next);
	matrix_destroy(exclude_curr);

	return lastRound;
}
