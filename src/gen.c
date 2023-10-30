#include "graph.h"
#include <time.h>
#include <stdlib.h>

int main(){
    srand(time(NULL));

    matrix* g1 = matrix_init(10);
    graph_generate(g1, 4, 3, 0.5);
    matrix* g2 = matrix_clone(g1);

    graph_add_noise(g2, 0.5, 1, 0.0);

    graph_print(g1);
    graph_print(g2);
    
    graph_save_to_file(g1, "res/G1.txt");
    graph_save_to_file(g2, "res/G2.txt");
    matrix_destroy(g1);
    matrix_destroy(g2);
}
