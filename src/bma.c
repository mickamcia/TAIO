#include "bma.h"
#include "limits.h"
#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void bma_init(matrix* g, matrix* exclude_curr, matrix* exclude_next, int* cost, int* previous)
{
	int n = g->size;
	for (int j = 0; j < n; j++) {

		for (int i = 0; i < n; i++) {
			if (j == i || g->mat[j * n + i])
			{
				exclude_curr->mat[i * n + j] = 1;
				cost[j]++;
			}
		}

		previous[j] = -1;
	}

	for (int round = 1; round <= n; round++) {
		for (int j = 0; j < n; j++) {
			previous[round * n + j] = -1;
			cost[round * n + j] = INT_MAX;
		}
	}
}

void array_print(int* g, int size, char name[])
{
	printf("\n%s\n", name);
	for (int i = 0; i < size; i++) {
		const int val = g[i];
		if (val == INT_MAX) printf("max ");
		else if (val >= -1) printf("% 3d ", val); // prints from 0 to 99, should change to "%d" by the end
		else printf("    ");
	}
}

void array2d_print(int* arr, int columns, int rows, char name[])
{
	printf("\n%s\n", name);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			const int val = arr[i * columns + j];
			if (val == INT_MAX) printf("max ");
			else if (val >= -1) printf("% 3d ", val); // prints from 0 to 99, should change to "%d" by the end
			else printf("    ");
		}
		printf("\n");
	}
}

void bma_run(matrix* g, matrix* exclude_curr, matrix* exclude_next, int* cost, int* previous)
{
	int lastRound = -1;
	int lastVertex = -1;
	int n = g->size;
	int* vExclude = (int*)malloc(sizeof(int) * n);
	int vCost = -1;

	for (int round = 0; round < g->size; round++) {	// round in rows
		short changed = 0;

		for (int u = 0; u < n; u++) {		// u in rows in exclude, in columns in cost
			for (int v = 0; v < n; v++) {	// v in columns in exclude
				if (exclude_curr->mat[u * n + v] == 1 || cost[round * n + u] == INT_MAX)
					continue;

				memcpy(vExclude, &(exclude_curr->mat[u * n]), sizeof(int) * n);
				//array_print(vExclude, n, "vExclude after memcpy");
				vExclude[v] = 1;
				//array_print(vExclude, n, "vExclude[u] = 1");
				vCost = 1 + cost[round * n + u];

				for (int j = 0; j < n; j++) {
					if (j != v && g->mat[v * n + j] == 1) { // for each neighbor
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

					//printf("\nexclude_next:");
					//graph_print(exclude_next);
					//array2d_print(cost, n, n+1, "cost");
					//array2d_print(previous, n, n + 1, "previous");

					changed = 1;
				}
			}
		}

		if (changed == 0) {
			lastRound = round;
			//lastVertex = 
			break;
		}

		printf("\nexclude_curr:");
		graph_print(exclude_curr);
		printf("\nexclude_next:");
		graph_print(exclude_next);

		memcpy(exclude_curr->mat, exclude_next->mat, sizeof(int) * n);
		memset(exclude_next->mat, 0, sizeof(int) * n);
	}
	free(vExclude);
}

void bma(matrix* g)
{
	// vertices - columns, rounds - rows
	matrix* exclude_curr = matrix_init(g->size);
	matrix* exclude_next = matrix_init(g->size);
	int rows = g->size + 1;
	int columns = g->size;
	int* cost = (int*)malloc(sizeof(int) * columns * rows);
	int* previous = (int*)malloc(sizeof(int) * columns * rows);
	memset(cost, 0, sizeof(int) * columns * rows);
	memset(previous, 0, sizeof(int) * columns * rows);

	bma_init(g, exclude_curr, exclude_next, cost, previous);

	printf("\nexclude_curr:");
	graph_print(exclude_curr);

	array2d_print(cost, columns, rows, "cost");

	array2d_print(previous, columns, rows, "previous");

	bma_run(g, exclude_curr, exclude_next, cost, previous);

	printf("\nexclude_curr:");
	graph_print(exclude_curr);

	array2d_print(cost, columns, rows, "cost");

	array2d_print(previous, columns, rows, "previous");

	free(previous);
	free(cost);
	matrix_destroy(exclude_next);
	matrix_destroy(exclude_curr);
}
