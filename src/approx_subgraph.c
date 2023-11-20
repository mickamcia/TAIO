#include "approx_subgraph.h"
#include "subgraph.h"
#include "approx_clique.h"
#include <stdlib.h>

void approx_subgraph_run(matrix* a, matrix* b)
{
	matrix* mod_prod = modular_product(a, b);
	
	matrix* clique = approx_clique_run(mod_prod);

	extract_solution(clique, a, b);

	matrix_destroy(clique);
	matrix_destroy(mod_prod);
}