#ifndef BMA
#define BMA

#include "matrix.h"

int bma(matrix* g, int* cost, int* previous);
void bma_init(matrix* g, matrix* exclude_curr, matrix* exclude_next, int* cost, int* previous);
int bma_relaxation(matrix* g, matrix* exclude_curr, matrix* exclude_next, int* cost, int* previous);

#endif