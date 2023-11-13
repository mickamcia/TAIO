#ifndef BMA
#define BMA

#include "matrix.h"

void bma(matrix* g);
void bma_init(matrix* g, matrix* exclude_curr, matrix* exclude_next, int* cost, int* previous);
void bma_relaxation(matrix* g, matrix* exclude_curr, matrix* exclude_next, int* cost, int* previous);

#endif