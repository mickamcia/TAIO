#include "subgraph.h"
#include <string.h>
#include <stdlib.h>
#include "utils.h"

void symmetrize_max(matrix* m){
    for (int i = 0; i < m->size; i++) {
        for (int j = i + 1; j < m->size; j++) {
            const int u_v = m->mat[i * m->size + j];
            const int v_u = m->mat[i + m->size * j];
            const int max_edge = u_v + v_u;
            m->mat[i * m->size + j] = max_edge;
            m->mat[i + m->size * j] = max_edge;
        }
    }
}

matrix* edge_coding(matrix* a, matrix* b)
{
    matrix* c = matrix_init(a->size * b->size);
    for (int a_i = 0; a_i < a->size; a_i++) {
        for (int b_i = 0; b_i < b->size; b_i++) {
            for (int a_j = 0; a_j < a->size; a_j++) {
                if (a_i == a_j) continue;
                for (int b_j = 0; b_j < b->size; b_j++) {
                    if (b_i == b_j) continue;
                    const int a_edge = a->mat[a_i * a->size + a_j];
                    const int b_edge = b->mat[b_i * b->size + b_j];
                    if (a_edge != 0 && b_edge != 0) {
                        c->mat[(a_i * b->size + b_i) * c->size + (a_j * b->size + b_j)] = minimum(a_edge, b_edge);
                    }
                }
            }
        }
    }
    return c;
}

matrix* modular_product(matrix* a, matrix* b)
{
    matrix* c = matrix_init(a->size * b->size);
    for (int a_i = 0; a_i < a->size; a_i++) {
        for (int b_i = 0; b_i < b->size; b_i++) {
            for (int a_j = 0; a_j < a->size; a_j++) {
                if (a_i == a_j) continue;
                for (int b_j = 0; b_j < b->size; b_j++) {
                    if (b_i == b_j) continue;
                    const int a_edge = a->mat[a_i * a->size + a_j];
                    const int b_edge = b->mat[b_i * b->size + b_j];
                    if (a_edge != 0 && b_edge != 0) {
                        c->mat[(a_i * b->size + b_i) * c->size + (a_j * b->size + b_j)] = minimum(a_edge, b_edge) * c->size * c->size;
                    }
                    else if (a_edge == 0 && b_edge == 0) {
                        c->mat[(a_i * b->size + b_i) * c->size + (a_j * b->size + b_j)] = 1;
                    }
                }
            }
        }
    }
    return c;
}
void remove_isolated_vertices(matrix* g){
    for(int i = 0; i < g->size; i++){
        int does_have_an_edge = false;
        for(int j = 0; j < g->size; j++){
            if(g->mat[i * g->size + j] > 0 || g->mat[i + g->size * j] > 0){
                does_have_an_edge = true;
                break;
            }
        }
        if(false == does_have_an_edge){
            for(int j = 0; j < g->size; j++){
                g->mat[i * g->size + j] = -1;
                g->mat[i + g->size * j] = -1;
            }
        }
    }
}
void extract_solution(matrix* mod_prod, matrix* clique, matrix* a, matrix* b)
{
    int* a_clique_indices = (int*)malloc(sizeof(int) * a->size);
    int* b_clique_indices = (int*)malloc(sizeof(int) * b->size);
    memset(a_clique_indices, 0, sizeof(int) * a->size);
    memset(b_clique_indices, 0, sizeof(int) * b->size);

    for (int i = 0; i < clique->size; i++) {
        for (int j = 0; j < clique->size; j++) {
            if (clique->mat[i * clique->size + j] != -1) {
                a_clique_indices[i / b->size] = 1;
                b_clique_indices[i % b->size] = 1;
            }
        }
    }
    
    matrix* a_original = matrix_clone(a);
    matrix* b_original = matrix_clone(b);
    memset(a->mat, 0, sizeof(int) * a->size * a->size);
    memset(b->mat, 0, sizeof(int) * b->size * b->size);

    for (int a_i = 0; a_i < a->size; a_i++) {
        for (int a_j = 0; a_j < a->size; a_j++) {
            const int a_edge = a_original->mat[a_i * a->size + a_j];
            for (int b_i = 0; b_i < b->size; b_i++) {
                for (int b_j = 0; b_j < b->size; b_j++) {
                    const int b_edge = b_original->mat[b_i * b->size + b_j];
                    const int clique_val = clique->mat[(a_i * b->size + b_i) * clique->size + (a_j * b->size + b_j)];
                    const int mod_prod_val = mod_prod->mat[(a_i * b->size + b_i) * clique->size + (a_j * b->size + b_j)];
                    const int edge_val = mod_prod_val > clique_val ? clique_val : mod_prod_val;
                    if (a_clique_indices[a_i] == 0 || a_clique_indices[a_j] == 0) {
                        a->mat[a_i * a->size + a_j] = -1;
                    }
                    else if (a_i != a_j && clique_val > 0 && a_edge != 0 && b_edge != 0) {
                        a->mat[a_i * a->size + a_j] = edge_val / (mod_prod->size * mod_prod->size);
                    }


                    if (b_clique_indices[b_i] == 0 || b_clique_indices[b_j] == 0) {
                        b->mat[b_i * b->size + b_j] = -1;
                    }
                    else if (b_i != b_j && clique_val > 0 && a_edge != 0 && b_edge != 0) {
                        b->mat[b_i * b->size + b_j] = edge_val / (mod_prod->size * mod_prod->size);
                    }
                }
            }
        }
    }
    remove_isolated_vertices(a);
    remove_isolated_vertices(b);
    matrix_destroy(a_original);
    matrix_destroy(b_original);
    free(a_clique_indices);
    free(b_clique_indices);
}