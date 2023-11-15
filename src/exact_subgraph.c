#include "exact_subgraph.h"
#include "exact_clique_bb.h"
#include "graph.h"
#include "subgraph.h"


// driver function
void exact_subgraph_run(matrix* a, matrix* b)
{
    matrix* mod_prod = modular_product(a, b);
    matrix* clique = matrix_clone(mod_prod);

    exact_clique_bb_run(clique);
    extract_solution(clique, a, b);
    
    matrix_destroy(mod_prod);
    matrix_destroy(clique);
}