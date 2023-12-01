#include "approx_subgraph.h"
#include "subgraph.h"
#include "approx_clique.h"
#include "graph.h"
#include <stdlib.h>

void approx_subgraph_run(matrix* a, matrix* b)
{
	matrix* mod_prod = modular_product(a, b);
	matrix* sym_mod_prod = matrix_clone(mod_prod);
    symmetrize_max(sym_mod_prod);
	
	matrix* clique = approx_clique_run(sym_mod_prod);

	extract_solution(mod_prod, clique, a, b);

	matrix_destroy(clique);
	matrix_destroy(mod_prod);
	matrix_destroy(sym_mod_prod);
}