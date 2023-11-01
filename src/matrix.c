#include "matrix.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

matrix* matrix_init(int size){
    matrix* m = (matrix*)malloc(sizeof(matrix));
    m->size = size;
    m->mat = (int*)malloc(sizeof(int) * size * size);
    memset(m->mat, 0, sizeof(int) * m->size * m->size);
    return m;
}

void matrix_destroy(matrix *m)
{
    free(m->mat);
    free(m);
}

matrix *matrix_clone(matrix *m)
{
    matrix* copy = matrix_init(m->size);
    memcpy(copy->mat, m->mat, sizeof(int) * m->size * m->size);
    return copy;
}

matrix *matrix_extend(matrix *m, int extend_by_size)
{
    if(extend_by_size < 0){
        printf("Error, extend_by_size < 0.\n");
        return NULL;
    }

    const int smaller = m->size;
    const int bigger = smaller + extend_by_size;
    matrix* result = matrix_init(bigger);

    for(int i = 0; i < smaller; i++){
        memcpy(&(result->mat[i * bigger]), &(m->mat[i * smaller]), smaller * sizeof(int));
    }

    return result;
}

void matrix_generate_permutation(matrix *m)
{
    int* permutation = (int*)malloc(sizeof(int) * m->size);
    for (int i = 0; i < m->size; i++) {
        permutation[i] = i;
    }
    for (int i = m->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int swap = permutation[i];
        permutation[i] = permutation[j];
        permutation[j] = swap;
    }

    memset(m->mat, 0, sizeof(int) * m->size * m->size);
    for (int i = 0; i < m->size; i++) {
        m->mat[i * m->size + permutation[i]] = 1;
    }
    free(permutation);
}


void matrix_add(matrix *a, matrix *b, matrix *c)
{
    if(a->size != b->size || a->size != c->size){
        printf("Error inequal sizes\n");
        return;
    }
    
    const int n = a->size;
    memset(c->mat, 0, sizeof(int) * n * n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c->mat[i * n + j] = a->mat[i * n + j] + b->mat[i * n + j];
        }
    }
}

void matrix_multiply_elementwise(matrix *a, matrix *b, matrix *c)
{
    if(a->size != b->size || a->size != c->size){
        printf("Error inequal sizes\n");
        return;
    }
    
    const int n = a->size;
    memset(c->mat, 0, sizeof(int) * n * n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c->mat[i * n + j] = a->mat[i * n + j] * b->mat[i * n + j];
        }
    }
}

void matrix_multiply(matrix *a, matrix *b, matrix *c)
{    
    if(a->size != b->size || a->size != c->size){
        printf("Error inequal sizes\n");
        return;
    }

    const int BLOCK_SIZE = 16;
    const int n = a->size;
    memset(c->mat, 0, sizeof(int) * n * n);

    for (int i = 0; i < n; i += BLOCK_SIZE) {
        for (int j = 0; j < n; j += BLOCK_SIZE) {
            for (int k = 0; k < n; k += BLOCK_SIZE) {
                for (int ii = i; ii < i + BLOCK_SIZE && ii < n; ii++) {
                    for (int jj = j; jj < j + BLOCK_SIZE && jj < n; jj++) {
                        for (int kk = k; kk < k + BLOCK_SIZE && kk < n; kk++) {
                            c->mat[ii * n + jj] += a->mat[ii * n + kk] * b->mat[kk * n + jj];
                        }
                    }
                }
            }
        }
    }
}

void matrix_transpose(matrix *m)
{
    const int BLOCK_SIZE = 16;
    const int n = m->size;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            int swap = m->mat[i * n + j];
            m->mat[i * n + j] = m->mat[i + n * j];
            m->mat[i + n * j] = swap;
        }
    }
}

/*
couldn't think of a proper name, that's how this function works:

    a  a     b  b  b     a  a  b
    a  a  +  b  b  b  =  a  a  b
             b  b  b     b  b  b    
*/
#include "graph.h"
void matrix_overload(matrix *a, matrix *b, matrix *c)
{
    if(a->size  > b->size || b->size != c->size){
        printf("Error bad sizes\n");
        return;
    }
    
    memset(c->mat, 0, sizeof(int) * c->size * c->size);

    for (int i = 0; i < c->size; i++) {
        for (int j = 0; j < c->size; j++) {
            if(i < a->size && j < a->size){
                c->mat[i * c->size + j] = a->mat[i * a->size + j];
            }
            else{
                c->mat[i * c->size + j] = b->mat[i * b->size + j];
            }
        }
    }
}
