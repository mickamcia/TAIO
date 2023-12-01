#include "exact_subgraph.h"
#include "exact_clique_bb.h"
#include "graph.h"
#include "subgraph.h"


// driver function
void exact_subgraph_run(matrix* a, matrix* b)
{

	matrix* mod_prod = modular_product(a, b);
	matrix* sym_mod_prod = matrix_clone(mod_prod);
    symmetrize_max(sym_mod_prod);

    matrix* clique = matrix_clone(sym_mod_prod);

    exact_clique_bb_run(clique);
    extract_solution(mod_prod, clique, a, b);
    
    matrix_destroy(mod_prod);
    matrix_destroy(clique);
    matrix_destroy(sym_mod_prod);
}