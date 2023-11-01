#include "exact_subgraph.h"
#include "exact_clique.h"
#include "graph.h"
#include <stdlib.h>

//internal functions
inline int min(const int a, const int b);
matrix* modular_product(matrix* a, matrix* b);


int min(const int a, const int b){
    return a > b ? b : a;
}

matrix* modular_product(matrix* a, matrix* b)
{
    matrix* c = matrix_init(a->size * b->size);
    for(int a_i = 0; a_i < a->size; a_i++){
        for(int b_i = 0; b_i < b->size; b_i++){
            for(int a_j = 0; a_j < a->size; a_j++){
                for(int b_j = 0; b_j < b->size; b_j++){
                    if(a_i == a_j) continue;
                    if(b_i == b_j) continue;
                    const int a_edge = a->mat[a_i * a->size + a_j];
                    const int b_edge = b->mat[b_i * b->size + b_j];
                    if(a_edge != 0 && b_edge != 0){
                        c->mat[(a_i * b->size + b_i) * c->size + (a_j * b->size + b_j)] = 1;//min(a_edge, b_edge);
                    }
                    else if(a_edge == 0 && b_edge == 0){
                        c->mat[(a_i * b->size + b_i) * c->size + (a_j * b->size + b_j)] = 1;
                    }
                }
            }
        }
    }
    return c;
}

// driver function
void exact_subgraph_run(matrix* a, matrix* b)
{
    matrix* mod_prod = modular_product(a, b);
    matrix* result = matrix_clone(mod_prod);
    exact_clique_run(result);
    graph_print(a);
    graph_print(b);
    graph_print(mod_prod);
    graph_print(result);
    matrix_destroy(mod_prod);
}