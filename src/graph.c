#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void graph_generate(graph* g, int edge_weight_max, int edge_weight_min, float edge_occur_prob){
    for(int i = 0; i < g->vert_count; i++){
        for(int j = 0; j < g->vert_count; j++){
            if(edge_occur_prob < (float)rand() / RAND_MAX){
                g->mat[i * g->vert_count + j] = 0;
            }
            else{
                g->mat[i * g->vert_count + j] = rand() % (1 + edge_weight_max - edge_weight_min) + edge_weight_min;   
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
