#include "exact_clique.h"
#include <stdlib.h>
#include <string.h>

// internal functions
void simplify_graph(matrix* g);
void recursive_travelsal(matrix* g, int* curr_clique, int* curr_size, int curr_vertex, int* best_clique, int* best_size);
int can_add_to_clique(matrix* g, int* clique, int vertex_index);

void simplify_graph(matrix *g) // turns directed multigraph into a graph
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


void recursive_travelsal(matrix* g, int* curr_clique, int* curr_size, int curr_vetrex, int* best_clique, int* best_size)
{
    const int n = g->size;
    if(*curr_size > *best_size){
        *best_size = *curr_size;
        memcpy(best_clique, curr_clique, sizeof(int) * n);
    }
    
    for(int i = curr_vetrex; i < n; i++){
        if(can_add_to_clique(g, curr_clique, i)){
            curr_clique[i] = 1;
            *curr_size += 1;
            recursive_travelsal(g, curr_clique, curr_size, curr_vetrex + 1, best_clique, best_size);
            curr_clique[i] = 0;
            *curr_size -= 1;
        }
    }
}

int can_add_to_clique(matrix *g, int *clique, int vertex_index)
{
    const int n = g->size;
    if(clique[vertex_index] == 1){
        return 0;
    }
    for(int i = 0; i < n; i++){
        if(clique[i] == 1 && i != vertex_index && g->mat[i * n + vertex_index] == 0){
            return 0;
        }
    }
    return 1;
}

// driver function
void exact_clique_run(matrix *g)
{
    simplify_graph(g);

    const int n = g->size;
    int curr_size = 0;
    int best_size = 0;
    int* curr_clique = (int*)malloc(sizeof(int) * n);
    int* best_clique = (int*)malloc(sizeof(int) * n);
    memset(curr_clique, 0, sizeof(int) * n);
    memset(best_clique, 0, sizeof(int) * n);

    recursive_travelsal(g, curr_clique, &curr_size, 0, best_clique, &best_size);

    for(int i = 0; i < n; i++){
        if(best_clique[i] == 0){
            for(int j = 0; j < n; j++){
                g->mat[i * n + j] = 0;
                g->mat[i + n * j] = 0;
            }
        }
    }

    free(curr_clique);
    free(best_clique);
}
