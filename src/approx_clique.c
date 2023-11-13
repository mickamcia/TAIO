#include "approx_clique.h"
#include "bma.h"
#include "graph.h"
#include <stdlib.h>
#include "utils.h"
#include <string.h>

matrix* get_all_cliques(matrix* g, int* cost, int* previous, int lastRound) {
	// row - clique number, column - vertex
	matrix* all_cliques = matrix_init(g->size);

	for (int round = lastRound; round >= 0; round--) {
		for (int u = 0; u < g->size; u++) {
			if (cost[round * g->size + u] == g->size) {
				int prev = previous[round * g->size + u];
				int k = round - 1;
				all_cliques->mat[u * g->size + u] = 1;
				while (k >= 0 && prev != -1) {
					all_cliques->mat[u * g->size + prev] = 1;
					prev = previous[k * g->size + prev];
					k--;
				}
			}
		}
	}

	return all_cliques;
}

matrix* get_max_clique(matrix* all_cliques, matrix* g) {
	matrix* clique = matrix_init(g->size);
	matrix* max_clique = matrix_init(g->size);

	int maxSize = 0;

	for (int i = 0; i < all_cliques->size; i++) {
		memset(clique->mat, 0, sizeof(int) * g->size * g->size);
		for (int j = 0; j < all_cliques->size; j++) {
			for (int k = 0; k < all_cliques->size; k++) {
				clique->mat[j * clique->size + k] = -1;
				clique->mat[k * clique->size + j] = -1;
				if (all_cliques->mat[i * all_cliques->size + j] == 0) 
					continue;
				if (j != k && all_cliques->mat[i * all_cliques->size + k]) {
					if (g->mat[j * g->size + k] > 0)
						clique->mat[j * clique->size + k] = g->mat[j * g->size + k];
					if (g->mat[k * g->size + j] > 0)
						clique->mat[k * clique->size + j] = g->mat[k * g->size + j];
				}
			}
		}

		int size = graph_calc_clique_size(clique);
		// todo: rozwazyc przypadek ==
		if (maxSize < size) {
			maxSize = size;
			memcpy(max_clique->mat, clique->mat, sizeof(int) * max_clique->size * max_clique->size);
		}
	}

	matrix_destroy(clique);

	return max_clique;
}

void approx_clique_run(matrix *g)
{
	matrix* g_s = matrix_clone(g);
	graph_simplify_multidigraph_to_graph(g_s);

	matrix* g_c = graph_complement(g_s);

	int rows = g->size + 1;
	int columns = g->size;

	// vertices - columns, rounds - rows
	int* cost = (int*)malloc(sizeof(int) * columns * rows);
	int* previous = (int*)malloc(sizeof(int) * columns * rows);
	if (!cost || !previous)
		ERR("malloc");

	memset(cost, 0, sizeof(int) * columns * rows);
	memset(previous, 0, sizeof(int) * columns * rows);

	int lastRound = bma(g_c, cost, previous);

	matrix* all_cliques = get_all_cliques(g, cost, previous, lastRound);

	printf("\nAll cliques:");
	graph_print(all_cliques);

	matrix* max_clique = get_max_clique(all_cliques, g);

	printf("\n\nMax clique - size %d:", graph_calc_clique_size(max_clique));
	graph_print(max_clique);

	free(previous);
	free(cost);
	matrix_destroy(max_clique);
	matrix_destroy(all_cliques);
	matrix_destroy(g_c);
	matrix_destroy(g_s);
}