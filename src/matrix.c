#include "matrix.h"
#include <string.h>
#include <stdlib.h>

matrix* matrix_init(int size){
    matrix* m = (matrix*)malloc(sizeof(matrix));
    m->size = size;
    m->mat = (int*)malloc(sizeof(int) * size * size);
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


void matrix_generate_permutation(matrix *m)
{
}

void matrix_multiply(matrix *a, matrix *b, matrix *c)
{
}
