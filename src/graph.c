#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void graph_generate(matrix* g, int edge_weight_max, int edge_weight_min, float edge_occur_prob){
    for(int i = 0; i < g->size; i++){
        for(int j = 0; j < g->size; j++){
            if(edge_occur_prob > (float)rand() / RAND_MAX){
                g->mat[i * g->size + j] = rand() % (1 + edge_weight_max - edge_weight_min) + edge_weight_min;   
            }
            else{
                g->mat[i * g->size + j] = 0;
            }
        }
    }
}

void graph_print(matrix *g)
{
    printf("%d\n", g->size);
    for(int i = 0; i < g->size; i++){
        for(int j = 0; j < g->size; j++){
            printf("%d ", g->mat[i * g->size + j]);
        }
        printf("\n");
    }
}

matrix* graph_load_from_file(char *path)
{
    FILE *file = fopen(path, "r");

    int size;

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
    fprintf(file, "%d\n", g->size);
    for(int i = 0; i < g->size; i++){
        for(int j = 0; j < g->size; j++){
            fprintf(file, "%d ", g->mat[i * g->size + j]);
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
