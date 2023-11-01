#ifndef MATRIX_H
#define MATRIX_H

typedef struct{
    int* mat;
    int size;
} matrix;


matrix* matrix_init(int size);
void matrix_destroy(matrix* m);
matrix* matrix_clone(matrix* m);
matrix* matrix_extend(matrix* m, int plus_size);

void matrix_generate_permutation(matrix* m);
void matrix_add(matrix* a, matrix* b, matrix* c);
void matrix_multiply(matrix* a, matrix* b, matrix* c);
void matrix_multiply_elementwise(matrix *a, matrix *b, matrix *c);
void matrix_overload(matrix *a, matrix *b, matrix *c);
void matrix_transpose(matrix* m);

#endif