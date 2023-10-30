#include "graph.h"

int main(){
    graph* g = graph_init(10);
    graph_generate(g, 4, 3, 0.5);
    graph_save_to_file(g, "res/G.txt");
    graph_destroy(g);

    graph* g2 = graph_load_from_file("res/G.txt");
    graph_print(g2);
    graph_save_to_file(g2, "res/G2.txt");
}
