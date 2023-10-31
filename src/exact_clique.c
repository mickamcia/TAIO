#include "exact_clique.h"
#include <stdlib.h>

matrix* exact_clique_run(matrix *g)
{
    matrix* result = matrix_clone(g);
}

void exact_clique_simplify_graph(matrix *g)
{
    const int n = g->size;
    const int min_edges = 1;
    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            if(g->mat[i * n + j] >= min_edges && g->mat[i + n * j] >= min_edges){
                g->mat[i * n + j] = 1;
                g->mat[i + n * j] = 1;
            }
            else{
                g->mat[i * n + j] = 0;
                g->mat[i + n * j] = 0;
            }
        }
    }
    for(int i = 0; i < n; i++){
        g->mat[i * n + i] = 0;
    }
}
