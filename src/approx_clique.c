#include "approx_clique.h"
#include "bma.h"
#include "graph.h"
#include <stdlib.h>

void approx_clique_run(matrix *g)
{
	matrix* gc = graph_complement(g);

	bma(gc);

	matrix_destroy(gc);
}