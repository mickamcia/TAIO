#ifndef SUBGRAPH_H
#define SUBGRAPH_H

#include "matrix.h"

matrix* modular_product(matrix* a, matrix* b);
void extract_solution(matrix* clique, matrix* a, matrix* b);

#endif // !SUBGRAPH_H