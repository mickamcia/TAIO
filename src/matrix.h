#ifndef MATRIX_H
#define MATRIx_H

typedef struct{
    int* mat;
    int size;
} matrix;


matrix* matrix_init(int size);
void matrix_destroy(matrix* m);
matrix* matrix_clone(matrix* m);

void matrix_generate_permutation(matrix* m);
void matrix_multiply(matrix* a, matrix* b, matrix* c);
void matrix_transpose(matrix* m);
#endif