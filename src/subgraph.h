#ifndef SUBGRAPH_H
#define SUBGRAPH_H

#include "matrix.h"

void symmetrize_max(matrix* m);
matrix* edge_coding(matrix* a, matrix* b);
matrix* modular_product(matrix* a, matrix* b);
void extract_solution(matrix* mod_prod, matrix* clique, matrix* a, matrix* b);

#endif // !SUBGRAPH_H