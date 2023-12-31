#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limits.h"

#define MAX_PRINT_SIZE 20

// internal functions
int graph_calc_clique_size_n(matrix* g);
int graph_calc_clique_size_m(matrix* g);

void graph_generate(matrix* g, int edge_weight_max, int edge_weight_min, float edge_occur_prob, int should_be_directed){
    for(int i = 0; i < g->size; i++){
        for(int j = 0; j < g->size; j++){
            if(i != j && edge_occur_prob > (float)rand() / RAND_MAX){
                g->mat[i * g->size + j] = rand() % (1 + edge_weight_max - edge_weight_min) + edge_weight_min;   
            }
            else{
                g->mat[i * g->size + j] = 0;
            }
        }
    }

    if(should_be_directed) return;

    for(int i = 0; i < g->size; i++){
        for(int j = 0; j < g->size; j++){
            g->mat[i * g->size + j] = g->mat[i + g->size * j];   
        }
    }
}

void graph_print(matrix *g, const char* name)
{
    if (g->size > MAX_PRINT_SIZE) {
        //printf("Graph is too large to print.\n");
        return;
    }

    printf("\n%s\n", name);


    for (int j = 0; j <= g->size; j++) {
        printf("----");
    }

    printf("-\n u\\v|");

    for (int j = 0; j < g->size; j++) {
        printf("% 3d ", j);
    }

    printf("\n----|");

    for (int j = 0; j < g->size; j++) {
        printf("----");
    }

    printf("\n");

    for (int i = 0; i < g->size; i++) {
        printf("%3d |", i);
        for (int j = 0; j < g->size; j++) {
            const int val = g->mat[i * g->size + j];
            if (val == INT_MAX) printf("inf ");
            else if (val >= 0) printf("%3d ", val); // prints from 0 to 99, should change to "%d" by the end
            else printf("    ");
        }
        printf("\n");
    }

    for (int j = 0; j <= g->size; j++) {
        printf("----");
    }

    printf("-\n");
}

matrix* graph_load_from_file(char *path)
{
    FILE *file = fopen(path, "r");

    int size;

    fscanf(file, "%d", &size); // scan first "1\n"
    fscanf(file, "%d", &size);
    matrix* g = matrix_init(size);
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (fscanf(file, "%d", &g->mat[i * size + j]) != 1) {
                printf("Error reading data from the file.\n");
                fclose(file);
                return NULL;
            }
        }
    }

    fclose(file);
    return g;
}

void graph_save_to_file(matrix *g, char *path)
{
    FILE *file = fopen(path, "w");
    fprintf(file, "1\n"); // save first "1\n"
    fprintf(file, "%d\n", g->size);
    for(int i = 0; i < g->size; i++){
        for(int j = 0; j < g->size; j++){
            const int val = g->mat[i * g->size + j] > 0 ? g->mat[i * g->size + j] : 0;
            fprintf(file, "%d ", val);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void graph_permute(matrix *g)
{
    matrix* permutation = matrix_init(g->size);
    matrix* temp = matrix_init(g->size);
    
    matrix_generate_permutation(permutation);
    matrix_multiply(g, permutation, temp);
    matrix_transpose(permutation);
    matrix_multiply(permutation, temp, g);

    matrix_destroy(permutation);
    matrix_destroy(temp);
}

void graph_add_noise(matrix* g, float prob, int absolute, float relative)
{
    for(int i = 0; i < g->size; i++){
        for(int j = 0; j < g->size; j++){
            if(prob > (float)rand() / RAND_MAX){
                int value = g->mat[i * g->size + j];
                if(value == 0) continue; // do not create new edges

                int max_error = (int)(relative * value) + absolute;
                int error = (rand() % (2 * max_error + 1)) - max_error; // error in < -max_error, max_error >
                value += error;
                g->mat[i * g->size + j] = value < 0 ? 0 : value; // might delete an edge
            }
        }
    }
}

void graph_simplify_multidigraph_to_multigraph(matrix *g) // turns directed multigraph into a multigraph
{
    const int n = g->size;
    const int min_edges = 1;
    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            const int a_to_b = g->mat[i * n + j];
            const int b_to_a = g->mat[i + n * j];
            if(a_to_b >= min_edges && b_to_a >= min_edges){
                g->mat[i * n + j] = a_to_b + b_to_a;
                g->mat[i + n * j] = a_to_b + b_to_a;
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

void graph_simplify_multidigraph_to_graph(matrix *g) // turns directed multigraph into a graph
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

matrix* graph_complement(matrix* g)
{
    matrix* gc = matrix_init(g->size);

    int n = g->size;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            int index = i * n + j;
            if (g->mat[index] == 0)
                gc->mat[index] = 1;
        }
    }

    return gc;
}

int graph_calc_clique_size(matrix* g) {
    return graph_calc_clique_size_m(g);
}

int graph_calc_clique_size_n(matrix* g) {
    return 0;
}

int graph_calc_clique_size_m(matrix* g) {
    int graph_size = 0;
    for (int i = 0; i < g->size; i++)
        for (int j = 0; j < g->size; j++)
            if (g->mat[i * g->size + j] > 0)
                graph_size += g->mat[i * g->size + j];

    return graph_size;
}

int graph_clique_equal(matrix* g1, matrix* g2) {
    return graph_calc_clique_size(g1) == graph_calc_clique_size(g2);
}