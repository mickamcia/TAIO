typedef struct{
    int* mat;
    int vert_count;
} graph;


graph* graph_init(int vert_count);
graph* graph_load_from_file(char* path);
void graph_generate(graph* g, int edge_weight_max, int edge_weight_min, float edge_occur_prob);
void graph_destroy(graph* g);
void graph_save_to_file(graph* g, char* path);