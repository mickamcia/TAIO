#ifndef GRAPH_H
#define GRAPH_H

#include "matrix.h"

matrix* graph_load_from_file(char* path);
void graph_generate(matrix* g, int edge_weight_max, int edge_weight_min, float edge_occur_prob);
void graph_print(matrix* g);
void graph_save_to_file(matrix* g, char* path);

void graph_permute(matrix* g);
void graph_add_noise(matrix* g, float prob, int absolute, float relative);

#endif