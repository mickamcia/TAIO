#include "subgraph.h"
#include <string.h>
#include <stdlib.h>


inline int minimum(const int a, const int b);

int minimum(const int a, const int b) {
    return a > b ? b : a;
}

matrix* modular_product(matrix* a, matrix* b)
{
    matrix* c = matrix_init(a->size * b->size);
    for (int a_i = 0; a_i < a->size; a_i++) {
        for (int b_i = 0; b_i < b->size; b_i++) {
            for (int a_j = 0; a_j < a->size; a_j++) {
                for (int b_j = 0; b_j < b->size; b_j++) {
                    if (a_i == a_j) continue;
                    if (b_i == b_j) continue;
                    const int a_edge = a->mat[a_i * a->size + a_j];
                    const int b_edge = b->mat[b_i * b->size + b_j];
                    if (a_edge != 0 && b_edge != 0) {
                        c->mat[(a_i * b->size + b_i) * c->size + (a_j * b->size + b_j)] = minimum(a_edge, b_edge);
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

void extract_solution(matrix* clique, matrix* a, matrix* b)
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
    for (int i = 0; i < a->size; i++) {
        for (int j = 0; j < a->size; j++) {
            if (a_clique_indices[i] == 0 || a_clique_indices[j] == 0) {
                a->mat[i * a->size + j] = -1;
            }
        }
    }
    for (int i = 0; i < b->size; i++) {
        for (int j = 0; j < b->size; j++) {
            if (b_clique_indices[i] == 0 || b_clique_indices[j] == 0) {
                b->mat[i * b->size + j] = -1;
            }
        }
    }

    free(a_clique_indices);
    free(b_clique_indices);
}