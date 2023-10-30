#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void graph_generate(graph* g, int edge_weight_max, int edge_weight_min, float edge_occur_prob){
    for(int i = 0; i < g->vert_count; i++){
        for(int j = 0; j < g->vert_count; j++){
            if(edge_occur_prob > (float)rand() / RAND_MAX){
                g->mat[i * g->vert_count + j] = rand() % (1 + edge_weight_max - edge_weight_min) + edge_weight_min;   
            }
            else{
                g->mat[i * g->vert_count + j] = 0;
            }
        }
    }
}

void graph_print(graph *g)
{
    printf("%d\n", g->vert_count);
    for(int i = 0; i < g->vert_count; i++){
        for(int j = 0; j < g->vert_count; j++){
            printf("%d ", g->mat[i * g->vert_count + j]);
        }
        printf("\n");
    }
}

graph* graph_init(int vert_count){
    graph* g = (graph*)malloc(sizeof(graph));
    g->vert_count = vert_count;
    g->mat = (int*)malloc(sizeof(int) * vert_count * vert_count);
    return g;
}
graph* graph_load_from_file(char *path)
{
    FILE *file = fopen(path, "r");

    int vert_count;

    fscanf(file, "%d", &vert_count);
    graph* g = graph_init(vert_count);
    
    for (int i = 0; i < vert_count; i++) {
        for (int j = 0; j < vert_count; j++) {
            if (fscanf(file, "%d", &g->mat[i * vert_count + j]) != 1) {
                printf("Error reading data from the file.\n");
                fclose(file);
                return NULL;
            }
        }
    }

    fclose(file);
    return g;
}
void graph_destroy(graph *g)
{
    free(g->mat);
    free(g);
}

graph *graph_clone(graph *g)
{
    graph* copy = graph_init(g->vert_count);
    memcpy(copy->mat, g->mat, sizeof(int) * g->vert_count * g->vert_count);
    return copy;
}

void graph_permute(graph *g)
{
    
}

void graph_add_noise(graph* g, float prob, int absolute, float relative)
{
    for(int i = 0; i < g->vert_count; i++){
        for(int j = 0; j < g->vert_count; j++){
            if(prob > (float)rand() / RAND_MAX){
                int value = g->mat[i * g->vert_count + j];
                int max_error = (int)(relative * value) + absolute;
                int error = (rand() % (2 * max_error + 1)) - max_error; // error in < -max_error, max_error >
                value += error;
                g->mat[i * g->vert_count + j] = value < 0 ? 0 : value;
            }
        }
    }
}

void graph_save_to_file(graph *g, char *path)
{
    FILE *file = fopen(path, "w");
    
    fprintf(file, "%d\n", g->vert_count);
    for(int i = 0; i < g->vert_count; i++){
        for(int j = 0; j < g->vert_count; j++){
            fprintf(file, "%d ", g->mat[i * g->vert_count + j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}
